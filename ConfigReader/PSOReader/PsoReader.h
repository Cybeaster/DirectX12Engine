#pragma once
#include "ConfigReader.h"
#include "DirectX/ShaderTypes.h"

class OPSOReader : public OConfigReader
{
public:
	OPSOReader(const string& FileName)
	    : OConfigReader(FileName) {}

	vector<unique_ptr<SPSODescriptionBase>> LoadPSOs() const;
	unique_ptr<SPSOGraphicsDescription> LoadGraphicsPSO(const boost::property_tree::ptree& Node) const;
	unique_ptr<SPSOComputeDescription> LoadComputePSO(const boost::property_tree::ptree& Node) const;

private:
	static SShaderArrayText GetShaderArray(const boost::property_tree::ptree& Node);
	static D3D12_PIPELINE_STATE_FLAGS GetFlags(const boost::property_tree::ptree& Node);
	static D3D12_PRIMITIVE_TOPOLOGY_TYPE GetTopologyType(const boost::property_tree::ptree& Node);
	static void SetRenderTargetFormats(DXGI_FORMAT* Formats, const boost::property_tree::ptree& Node);
	static DXGI_FORMAT GetFormat(const boost::property_tree::ptree& Node);
	static DXGI_FORMAT GetFormat(const string& FormatString);
	static DXGI_SAMPLE_DESC GetSampleDescription(const boost::property_tree::ptree& Node);
	static CD3DX12_BLEND_DESC GetBlendDesc(const boost::property_tree::ptree& Node);
	static D3D12_LOGIC_OP GetLogicOp(const string& LogicOpString);
	static D3D12_BLEND_OP GetBlendOp(const string& BlendOpString);
	static D3D12_BLEND GetBlend(const string& BlendString);
	static CD3DX12_RASTERIZER_DESC GetRasterizerDesc(const boost::property_tree::ptree& Node);
	static D3D12_DEPTH_STENCILOP_DESC GetDepthStencilOp(const boost::optional<const boost::property_tree::ptree&>& Node);
	static D3D12_CULL_MODE GetCullMode(const string& CullModeString);
	static D3D12_CONSERVATIVE_RASTERIZATION_MODE GetConservativeRasterizationMode(const string& ConservativeRasterizationModeString);
	static D3D12_FILL_MODE GetFillMode(const string& FillModeString);
	static CD3DX12_DEPTH_STENCIL_DESC GetDepthStencilDesc(const boost::property_tree::ptree& Node);
	static D3D12_DEPTH_WRITE_MASK GetDepthWriteMask(const string& DepthWriteMaskString);
	static D3D12_COMPARISON_FUNC GetComparisonFunc(const string& ComparisonFuncString);
	static D3D12_STENCIL_OP GetStencilOp(const string& StencilOpString);
	static D3D12_COLOR_WRITE_ENABLE GetColorWriteEnable(const string& ColorWriteEnableString);
};
