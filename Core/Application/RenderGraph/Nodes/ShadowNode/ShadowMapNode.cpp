#include "ShadowMapNode.h"

#include "Engine/Engine.h"
#include "EngineHelper.h"
#include "Profiler.h"

ORenderTargetBase* OShadowMapNode::Execute(ORenderTargetBase* RenderTarget)
{
	PROFILE_SCOPE();
	auto pso = FindPSOInfo(PSO);
	for (auto weak : OEngine::Get()->GetShadowMaps())
	{
		auto map = weak.lock();
		if (map->ConsumeUpdate() && map->IsValid())
		{
			CommandQueue->SetRenderTarget(map.get());
			CommandQueue->ResourceBarrier(map.get(), D3D12_RESOURCE_STATE_DEPTH_WRITE);
			CommandQueue->SetResource("cbPass", map->GetPassConstantAddresss(), pso);
			OEngine::Get()->DrawRenderItems(pso, SRenderLayers::Opaque, map->GetCulledInstancesInfo());
			CommandQueue->ResourceBarrier(map.get(), D3D12_RESOURCE_STATE_GENERIC_READ);
		}
	}
	OEngine::Get()->SetWindowViewport(); // TODO remove this to other place
	CommandQueue->SetRenderTarget(RenderTarget);
	return RenderTarget;
}

void OShadowMapNode::SetupCommonResources()
{
	PROFILE_SCOPE();
	auto resource = OEngine::Get()->CurrentFrameResource;
	auto pso = FindPSOInfo(PSO);
	CommandQueue->SetPipelineState(pso);
	CommandQueue->SetResource("cbPass", resource->PassCB->GetGPUAddress(), pso);
	CommandQueue->SetResource("gMaterialData", resource->MaterialBuffer->GetGPUAddress(), pso);
}
