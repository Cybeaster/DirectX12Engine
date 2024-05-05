#pragma once
#include "DirectX/ShaderTypes.h"
#include "Logger.h"

#include <dxcapi.h>

class OShader
{
public:
	void Init(const SShaderDefinition& Info, const ComPtr<IDxcBlob>& Blob);
	D3D12_SHADER_BYTECODE GetShaderByteCode() const;
	EShaderLevel GetShaderType() const { return ShaderInfo.ShaderType; }

private:
	SShaderDefinition ShaderInfo = {};
	ComPtr<IDxcBlob> ShaderBlob;
};
