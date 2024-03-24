#pragma once
#include "../../../Components/RenderItemComponentBase.h"
#include "../../Materials/Material.h"
#include "DirectX/InstanceData.h"
#include "LightComponent/LightComponent.h"
#include "Logger.h"

struct SFrameResource;


struct SRenderItemGeometry
{

};

/**
 * @brief Object placeable in the world, having geomentry, material and variable number of instances
 */
struct ORenderItem
{
	DECLARE_DELEGATE(SPositionChanged, void);

	ORenderItem() = default;
	ORenderItem(ORenderItem&&) = default;
	ORenderItem(const ORenderItem&) = default;

	void BindResources(ID3D12GraphicsCommandList* CmdList, SFrameResource* Frame) const;
	void Update(const UpdateEventArgs& Arg) const;
	bool IsValid() const;
	bool IsValidChecked() const;
	string RenderLayer = "NONE";
	string Name;

	bool bTraceable = true;
	bool bFrustrumCoolingEnabled = true;
	template<typename T, typename... Args>
	T* AddComponent(Args&&... Arg);
	SMaterial* DefaultMaterial = nullptr;
	SMeshGeometry* Geometry = nullptr;
	SSubmeshGeometry* ChosenSubmesh = nullptr;

	DirectX::BoundingBox Bounds;
	vector<SInstanceData> Instances;

	UINT VisibleInstanceCount = 0;
	int32_t StartInstanceLocation = 0;

	SInstanceData* GetDefaultInstance();
	const vector<unique_ptr<OComponentBase>>& GetComponents() const;

private:
	vector<unique_ptr<OComponentBase>> Components;
};

template<typename T, typename... Args>
T* ORenderItem::AddComponent(Args&&... Arg)
{
	auto component = make_unique<T>(std::forward<Args>(Arg)...);
	auto res = component.get();
	Components.push_back(move(component));
	Components.back()->Init(this);
	return res;
}

struct SRenderItemParams
{
	SRenderItemParams(SMaterial* Material)
	    : MaterialParams(Material) {}
	SRenderItemParams() = default;

	SRenderItemParams(SMaterial* Material, size_t Instances)
	    : MaterialParams(Material), NumberOfInstances(Instances) {}

	string Submesh;
	SMaterialParams MaterialParams;
	size_t NumberOfInstances = 1;
	bool bFrustrumCoolingEnabled = false;
	bool Pickable = false;
};