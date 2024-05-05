#include "PickedRenderItem.h"

#include "Engine/Engine.h"
void OPickedRenderItemWidget::Draw()
{
	auto engine = OEngine::Get();
	auto picked = engine->GetPickedItem();
	ImGui::SeparatorText("Picked Item");
	if (picked)
	{
		ImGui::Text("Geometry Name: %s", picked->Geometry ? picked->Geometry->Name.c_str() : "None");
		ImGui::Text("Material: %s", picked->DefaultMaterial ? picked->DefaultMaterial->Name.c_str() : "None");
		ImGui::Text("Picked triangle %s", std::to_string(picked->ChosenSubmesh->StartIndexLocation).c_str());
	}
	else
	{
		ImGui::Text("Item not picked");
	}
}