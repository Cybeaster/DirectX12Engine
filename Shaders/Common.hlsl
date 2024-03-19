

#ifndef MAX_DIFFUSE_MAPS
#define MAX_DIFFUSE_MAPS 3
#endif

#ifndef MAX_NORMAL_MAPS
#define MAX_NORMAL_MAPS 3
#endif

#ifndef MAX_HEIGHT_MAPS
#define MAX_HEIGHT_MAPS 3
#endif
// Include structures and functions for lighting.
#include "LightingUtils.hlsl"

struct InstanceData
{
	float4x4 World;
	float4x4 TexTransform;
	uint MaterialIndex;
	float2 DisplacementMapTexelSize;
	float GridSpatialStep;
};

struct MaterialData
{
	float4 DiffuseAlbedo;
	float3 FresnelR0;
	float Roughness;
	float4x4 MatTransform;
	uint DiffuseMapCount;
	uint NormalMapCount;
	uint HeightMapCount;
	uint DiffuseMapIndex[MAX_DIFFUSE_MAPS];
	uint NormalMapIndex[MAX_NORMAL_MAPS];
	uint HeightMapIndex[MAX_HEIGHT_MAPS];
	float pad1;
	float pad2;
};

Texture2D gTextureMaps[41] : register(t0);
Texture2D gDisplacementMap : register(t0,space3);
TextureCube gCubeMap : register(t1,space3);

StructuredBuffer<InstanceData> gInstanceData : register(t0, space1);
StructuredBuffer<MaterialData> gMaterialData : register(t1, space1);

StructuredBuffer<SpotLight> gSpotLights : register(t0, space2);
StructuredBuffer<PointLight> gPointLights : register(t1, space2);
StructuredBuffer<DirectionalLight> gDirectionalLights : register(t2, space2);

cbuffer cbPass : register(b0)
{
	float4x4 gView;
	float4x4 gInvView;
	float4x4 gProj;
	float4x4 gInvProj;
	float4x4 gViewProj;
	float4x4 gInvViewProj;
	float3 gEyePosW;

	float cbPerPassPad1; // Use this to pad gEyePosW to 16 bytes

	float2 gRenderTargetSize;
	float2 gInvRenderTargetSize;
	float gNearZ;
	float gFarZ;
	float gTotalTime;
	float gDeltaTime;
	float4 gAmbientLight;
	float4 gFogColor;
	float gFogStart;
	float gFogRange;

	uint gNumDirLights;

	float cbPerPassPad2; // Padding to align following uints
	uint gNumPointLights;
	uint gNumSpotLights;
	float cbPerPassPad3; // Padding to ensure the cbuffer ends on a 16-byte boundary
	float cbPerPassPad4; // Padding to ensure the cbuffer ends on a 16-byte boundary

};


SamplerState gsamPointWrap : register(s0);
SamplerState gsamPointClamp : register(s1);
SamplerState gsamLinearWrap : register(s2);
SamplerState gsamLinearClamp : register(s3);
SamplerState gsamAnisotropicWrap : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);




bool IsTangentValid(float3 TangentW)
{
    float epsilon = 0.0001f;
    float sum = abs(TangentW.x) + abs(TangentW.y) + abs(TangentW.z);
	return sum > epsilon;
}

float3 NormalSampleToWorldSpace(float3 NormalMapSample, float3 UnitNormalW, float3 TangentW)
{
	float3 normalT = NormalMapSample * 2.0f - 1.0f;
	float3 N = UnitNormalW;
	/*
	*  This code makes sure T is orthonormal to N by subtracting off any
	    component of T along the direction N
	*/
	float3 T = normalize(TangentW - dot(TangentW, N) * N);
	float3 B = cross(N, T);
	float3x3 TBN = float3x3(T, B, N);
	float3 bumpedNormal = mul(normalT, TBN);
	return bumpedNormal;
}

float3 ComputeNormalMaps(float3 NormalW, float3 TangentW, MaterialData matData, float2 TexC, out float NormalMapSampleA)
{
	float3 bumpedNormalW = NormalW;
	float4 normalMapSample = float4(0.f, 0.f, 1.0f, 1.0f);
	NormalMapSampleA = 1.0f;
    if(IsTangentValid(TangentW))
    {
      uint numNormalMaps = matData.NormalMapCount;
    	for (uint i = 0; i < numNormalMaps; ++i)
    	{
    		int normalMapIndex = matData.NormalMapIndex[i];
    		normalMapSample = gTextureMaps[normalMapIndex].Sample(gsamAnisotropicWrap, TexC);
    		bumpedNormalW += NormalSampleToWorldSpace(normalMapSample.rgb, normalize(NormalW), TangentW);
    		NormalMapSampleA += normalMapSample.a;
    	}
        NormalMapSampleA = NormalMapSampleA / numNormalMaps;
    }


	return normalize(bumpedNormalW);
}

float3 ComputeNormalMap(uint Index, float3 NormalW, float3 TangentW, MaterialData matData, float2 TexC, out float NormalMapSampleA)
{
	float3 bumpedNormalW = NormalW;
	float4 normalMapSample = float4(0.f, 0.f, 1.0f, 1.0f);
	NormalMapSampleA = 1.0f;
	if (IsTangentValid(TangentW))
	{
		int normalMapIndex = matData.NormalMapIndex[Index];
		normalMapSample = gTextureMaps[normalMapIndex].Sample(gsamAnisotropicWrap, TexC);
		bumpedNormalW = NormalSampleToWorldSpace(normalMapSample.rgb, NormalW, TangentW);
		NormalMapSampleA = normalMapSample.a;
	}
	return bumpedNormalW;
}

float3 ComputeHeightMaps(MaterialData matData, float3 NormalW, float3 TangentW, float2 TexC,float Offset )
{
	uint numHeightMaps = matData.HeightMapCount;
	float3 pos = float3(0.0f, 0.0f, 0.0f);
	for (uint j = 0; j < numHeightMaps; ++j)
	{
		int heightMapIndex = matData.HeightMapIndex[j];
		float heightMapSample = gTextureMaps[heightMapIndex].SampleLevel(gsamAnisotropicWrap, TexC,Offset).r;
		pos +=  heightMapSample;
	}
	return pos;
}

float4 ComputeDiffuseMaps(MaterialData MatData, float4 DiffuseAlbedo, float2 TexC)
{
	uint numDiffuseMaps = MatData.DiffuseMapCount;
	for (uint k = 0; k < numDiffuseMaps; ++k)
	{
		int diffuseMapIndex = MatData.DiffuseMapIndex[k];
		DiffuseAlbedo *= gTextureMaps[diffuseMapIndex].Sample(gsamLinearWrap, TexC);
	}
	return DiffuseAlbedo;
}