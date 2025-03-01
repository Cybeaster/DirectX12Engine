#pragma once
#include "Color.h"
#include "DirectX/DXHelper.h"
#include "Engine/RenderTarget/RenderTarget.h"
#include "Types.h"

struct SPSODescriptionBase;
struct SShaderPipelineDesc;
class OCommandQueue
{
public:
	OCommandQueue(ComPtr<ID3D12Device2> Device, D3D12_COMMAND_LIST_TYPE Type);
	virtual ~OCommandQueue();

	ComPtr<ID3D12GraphicsCommandList> GetCommandList();
	ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
	ComPtr<ID3D12CommandQueue> GetCommandQueue();

	uint64_t ExecuteCommandList();
	void ExecuteCommandListAndWait();

	uint64_t Signal();

	void WaitForFenceValue(uint64_t FenceValue);
	void Flush();
	void TryResetCommandList();
	ComPtr<ID3D12Fence> GetFence() const;
	void SetPipelineState(SPSODescriptionBase* PSOInfo);
	D3D12_RESOURCE_STATES ResourceBarrier(ORenderTargetBase* Resource, D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter) const;
	D3D12_RESOURCE_STATES ResourceBarrier(ORenderTargetBase* Resource, D3D12_RESOURCE_STATES StateAfter) const;
	D3D12_RESOURCE_STATES ResourceBarrier(SResourceInfo* Resource, D3D12_RESOURCE_STATES StateAfter) const;

	void CopyResourceTo(ORenderTargetBase* Dest, ORenderTargetBase* Src) const;
	void CopyResourceTo(SResourceInfo* Dest, SResourceInfo* Src) const;

	ORenderTargetBase* SetRenderTarget(ORenderTargetBase* RenderTarget, uint32_t Subtarget = 0);
	ORenderTargetBase* SetAndClearRenderTarget(ORenderTargetBase* RenderTarget, uint32_t Subtarget = 0);

	void ClearRenderTarget(const SDescriptorPair& RTV, SColor) const;
	void ClearDepthStencil(const SDescriptorPair& DSV) const;
	void SetRenderTargets(const SDescriptorPair& RTV, const SDescriptorPair& DSV) const;
	void SetRenderToRTVOnly(const SDescriptorPair& RTV) const;
	void SetRenderToDSVOnly(const SDescriptorPair& DSV) const;
	void ResetQueueState();
	void SetViewportScissors(const D3D12_VIEWPORT& Viewport, const D3D12_RECT& Scissors) const;
	void SetResource(const string& Name, D3D12_GPU_VIRTUAL_ADDRESS Resource, SPSODescriptionBase* PSO);
	void SetResource(const string& Name, D3D12_GPU_DESCRIPTOR_HANDLE Resource, SPSODescriptionBase* PSO);
	void SetHeap(SRenderObjectHeap* Heap);

	template<typename T>
	T* GetCommandListAs();

protected:
	ComPtr<ID3D12CommandAllocator> CreateCommandAllocator();
	ComPtr<ID3D12GraphicsCommandList> CreateCommandList(ComPtr<ID3D12CommandAllocator> Allocator);

private:
	ORenderTargetBase* SetRenderTargetImpl(ORenderTargetBase* RenderTarget, bool ClearDepth, bool ClearRenderTarget, uint32_t Subtarget = 0);

	struct CommandAllocatorEntry
	{
		uint64_t FenceValue;
		ComPtr<ID3D12CommandAllocator> CommandAllocator;
	};

	using TCommandAllocatorQueue = queue<CommandAllocatorEntry>;
	using TCommandListQueue = queue<ComPtr<ID3D12GraphicsCommandList>>;

	D3D12_COMMAND_LIST_TYPE CommandListType;
	ComPtr<ID3D12Device2> Device = nullptr;
	ComPtr<ID3D12CommandQueue> CommandQueue = nullptr;
	ComPtr<ID3D12Fence> Fence = nullptr;
	ComPtr<ID3D12CommandAllocator> CommandAllocator;
	ComPtr<ID3D12GraphicsCommandList> CommandList;

	ORenderTargetBase* CurrentRenderTarget = nullptr;

	HANDLE FenceEvent;
	uint64_t FenceValue;

	TCommandAllocatorQueue CommandAllocatorQueue;
	TCommandListQueue CommandListQueue;
	SRenderObjectHeap* CurrentObjectHeap = nullptr;
	bool IsReset = false;
	SPSODescriptionBase* CurrentPSO = nullptr;
	unordered_map<string, UINT64> SetResources;
};

template<typename T>
T* OCommandQueue::GetCommandListAs()
{
	T* result;
	if (CommandList->QueryInterface(IID_PPV_ARGS(&result)) != S_OK)
	{
		LOG(Engine, Error, "Failed to get command list as type!");
		return nullptr;
	}
	return Cast<T>(CommandList.Get());
}