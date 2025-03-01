#pragma once
#include "DXHelper.h"

struct SRenderConstants
{
	struct SSphere
	{
		const uint32_t SliceCount = 20;
		const uint32_t StackCount = 20;
		const float Radius = 0.5f;
	};

	struct SCylinder
	{
		const float BottomRadius = 1.f;
		const float TopRadius = 0.5f;
		const float Height = 3.0f;
		const uint32_t SliceCount = 20;
		const uint32_t StackCount = 20;
	};

	struct SQuad
	{
		const float X = -1.0f;
		const float Y = 1.0f;
		const float Width = 2.0f;
		const float Height = 2.0f;
		const float Depth = 0.0f;
	};

	struct SCube
	{
		const float Width = 1.0f;
		const float Height = 1.0f;
		const float Depth = 1.0f;
		const uint32_t NumSubdivisions = 1;
	};

	struct SBox
	{
		const float Width = 1.0f;
		const float Height = 1.0f;
		const float Depth = 1.0f;
		const uint32_t NumSubdivisions = 5;
	};

	inline static constexpr D3D12_HEAP_PROPERTIES DefaultHeapProperties = {
		D3D12_HEAP_TYPE_DEFAULT, D3D12_CPU_PAGE_PROPERTY_UNKNOWN, D3D12_MEMORY_POOL_UNKNOWN, 0, 0
	};

	inline static constexpr D3D12_HEAP_PROPERTIES UploadHeapProps = {
		D3D12_HEAP_TYPE_UPLOAD, D3D12_CPU_PAGE_PROPERTY_UNKNOWN, D3D12_MEMORY_POOL_UNKNOWN, 0, 0
	};

	static constexpr UINT ShadowMapSize = 2048;

	static const DXGI_FORMAT AmbientMapFormat = DXGI_FORMAT_R16_UNORM;
	static const DXGI_FORMAT NormalMapFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;

	inline static constexpr D3D_DRIVER_TYPE DriverType = D3D_DRIVER_TYPE_HARDWARE;
	inline static constexpr DXGI_FORMAT BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	inline static constexpr DXGI_FORMAT DepthBufferResourceFormat = DXGI_FORMAT_R32_TYPELESS;
	inline static constexpr DXGI_FORMAT DepthBufferDSVFormat = DXGI_FORMAT_D32_FLOAT;
	inline static constexpr DXGI_FORMAT DepthBufferSRVFormat = DXGI_FORMAT_R32_FLOAT;
	inline static constexpr DXGI_FORMAT DepthClearValueFormat = DXGI_FORMAT_D32_FLOAT;

	inline static constexpr uint32_t NumFrameResources = 3;
	inline static constexpr uint32_t MaxLights = 16;
	inline static constexpr uint32_t RenderBuffersCount = 2;
	inline static constexpr DirectX::XMUINT2 CubeMapDefaultResolution = { 1024, 1024 };
	inline static constexpr float CameraNearZ = 0.1f;
	inline static constexpr float CameraFarZ = 5000.f;

	inline static constexpr uint32_t MaxDiffuseMapsPerMaterial = 3;
	inline static constexpr uint32_t MaxNormalMapsPerMaterial = 3;
	inline static constexpr uint32_t MaxHeightMapsPerMaterial = 3;

	inline static constexpr SSphere Sphere;
	inline static constexpr SCylinder Cylinder;
	inline static constexpr SBox Box;
	inline static constexpr SQuad Quad;
	inline static const string DefaultSkyTexture = "grasscube1024";

	static D3D12_SHADER_RESOURCE_VIEW_DESC GetNullSRV();
};

inline D3D12_SHADER_RESOURCE_VIEW_DESC SRenderConstants::GetNullSRV()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	return srvDesc;
}

struct SRenderLayers
{
	RENDER_TYPE(None);
	RENDER_TYPE(Opaque);
	RENDER_TYPE(OpaqueDynamicReflections);
	RENDER_TYPE(Transparent);
	RENDER_TYPE(AlphaTested);
	RENDER_TYPE(Shadow);
	RENDER_TYPE(Reflected);
	RENDER_TYPE(Mirror);
	RENDER_TYPE(AlphaTestedTreeSprites);
	RENDER_TYPE(IcosahedronLODs);
	RENDER_TYPE(Waves);
	RENDER_TYPE(Tesselation);
	RENDER_TYPE(Highlight);
	RENDER_TYPE(Sky);
	RENDER_TYPE(PostProcess);
	RENDER_TYPE(Water);
	RENDER_TYPE(LightObjects);
	RENDER_TYPE(Debug);
	RENDER_TYPE(DebugBox);
	RENDER_TYPE(FrustumDebug);
	RENDER_TYPE(ShadowDebug);
	RENDER_TYPE(OneFullscreenQuad);
};

struct SPSOTypes
{
	RENDER_TYPE(Opaque);
	RENDER_TYPE(Transparent);
	RENDER_TYPE(AlphaTested);
	RENDER_TYPE(Shadow);
	RENDER_TYPE(Debug);
	RENDER_TYPE(StencilReflection);
	RENDER_TYPE(StencilMirrors);
	RENDER_TYPE(TreeSprites);
	RENDER_TYPE(Icosahedron);
	RENDER_TYPE(HorizontalBlur);
	RENDER_TYPE(VerticalBlur);
	RENDER_TYPE(SobelFilter);
	RENDER_TYPE(Composite);

	RENDER_TYPE(WavesRender);
	RENDER_TYPE(WavesDisturb);
	RENDER_TYPE(WavesUpdate);

	RENDER_TYPE(BilateralBlur);
	RENDER_TYPE(Tesselation);
	RENDER_TYPE(Highlight);
	RENDER_TYPE(Sky);
	RENDER_TYPE(ShadowDebug);
	RENDER_TYPE(DrawNormals);
	RENDER_TYPE(SSAO);
	RENDER_TYPE(SSAOBlur);
};
struct SShaderTypes
{
	RENDER_TYPE(VSBaseShader);

	RENDER_TYPE(PSAlphaTested);
	RENDER_TYPE(PSOpaque);
	RENDER_TYPE(PSBaseShader);

	RENDER_TYPE(VSTreeSprite);
	RENDER_TYPE(GSTreeSprite);
	RENDER_TYPE(PSTreeSprite);

	RENDER_TYPE(GSIcosahedron);
	RENDER_TYPE(PSIcosahedron);
	RENDER_TYPE(VSIcosahedron);

	RENDER_TYPE(CSHorizontalBlur);
	RENDER_TYPE(CSVerticalBlur);

	RENDER_TYPE(CSBilateralBlur);

	RENDER_TYPE(PSComposite);
	RENDER_TYPE(VSComposite);

	RENDER_TYPE(CSWavesDisturb);
	RENDER_TYPE(CSWavesUpdate);
	RENDER_TYPE(VSWaves);

	RENDER_TYPE(CSSobelFilter);
	RENDER_TYPE(VSTesselation);
	RENDER_TYPE(HSTesselation);
	RENDER_TYPE(DSTesselation);
	RENDER_TYPE(PSTesselation);
	RENDER_TYPE(VSSky);
	RENDER_TYPE(PSSky);
};

using SRenderLayer = string;
using SPSOType = string;