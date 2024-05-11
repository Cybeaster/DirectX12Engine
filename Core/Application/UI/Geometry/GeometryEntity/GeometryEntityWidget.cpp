//
// Created by Cybea on 02/02/2024.
//

#include "GeometryEntityWidget.h"

#include "Engine/Engine.h"
#include "EngineHelper.h"
#include "UI/Geometry/GeometryManager.h"
#include "imgui.h"

void OGeometryEntityWidget::DrawSubmeshParameters()
{
	if (ImGui::CollapsingHeader("Submeshes"))
	{
		if (ImGui::BeginListBox("##Submeshes"))
		{
			for (auto& Submesh : GetGeometry()->DrawArgs)
			{
				auto name = Submesh.first.c_str();

				if (ImGui::Selectable(name, SelectedSubmesh == name))
				{
					if (SelectedSubmesh != name)
					{
						SelectedSubmesh = name;
					}
					else
					{
						SelectedSubmesh = "";
					}
				}
			}
			ImGui::EndListBox();
		}

		if (SelectedSubmesh != "" && GetGeometry()->DrawArgs.contains(SelectedSubmesh))
		{
			const auto& Submesh = GetGeometry()->DrawArgs[SelectedSubmesh];
			const auto formattedName = "Submesh " + SelectedSubmesh;
			ImGui::SeparatorText(formattedName.c_str());
			if (Submesh->Vertices)
			{
				ImGui::SetNextItemWidth(SliderWidth - 50);
				ImGui::Text("Vertices & Indices");
				if (ImGui::BeginListBox("##Vertices & Indices"))
				{
					int32_t counter = 0;
					for (auto& vertex : *Submesh->Vertices)
					{
						string vertexName = "##Vertex: " + std::to_string(counter);
						DirectX::XMFLOAT3 newVertex = vertex;
						if (ImGui::SliderFloat3(vertexName.c_str(), &newVertex.x, -100.0f, 100.0f))
						{
							DirectX::XMStoreFloat3(&vertex, DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&vertex), DirectX::XMLoadFloat3(&newVertex), 0.1f));
							RebuildRequest();
						}

						ImGui::SameLine();

						string idxName = "##Index: " + std::to_string(counter);
						int32_t idx = counter;
						if (ImGui::InputInt(idxName.c_str(), &idx))
						{
							Submesh->Indices->at(counter) = idx;
							RebuildRequest();
						}

						counter++;
					}
					ImGui::EndListBox();
				}
			}
		}
	}
}

void OGeometryEntityWidget::DrawInstanceParameters()
{
	if (ImGui::CollapsingHeader("Object Instances"))
	{
		if (ImGui::BeginListBox("##Object Instances"))
		{
			size_t counter = 1;
			for (auto& Instance : RenderItem.lock()->Instances)
			{
				auto name = "Instance: " + std::to_string(counter);

				if (ImGui::Selectable(name.c_str(), SelectedInstance == name))
				{
					if (SelectedInstance != name)
					{
						SelectedInstanceData = &Instance;
						SelectedInstance = name;
					}
					else
					{
						SelectedInstanceData = nullptr;
						SelectedInstance = "";
					}
				}
				counter++;
			}

			ImGui::EndListBox();
			if (!SelectedInstance.empty())
			{
				MaterialPickerWidget->SetCurrentMaterial(FindMaterial(SelectedInstanceData->HlslData.MaterialIndex));
				OHierarchicalWidgetBase::Draw();
			}
		}
	}
}

void OGeometryEntityWidget::Draw()
{
	if (RenderItem.lock()->Instances.size() > 1)
	{
		auto setNextWindowPos = []() {
			ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, ImGui::GetWindowPos().y));
		};
		setNextWindowPos();
		ImGui::SetNextWindowSize(ImVec2(SliderWidth, 500));

		if (ImGui::Begin("Submesh parameters")) // Begin new window for Submesh parameters
		{
			DrawSubmeshParameters();
			DrawInstanceParameters();
		}
		ImGui::End();
	}
	else
	{
		SelectedInstanceData = RenderItem.lock()->GetDefaultInstance();
		MaterialPickerWidget->SetCurrentMaterial(FindMaterial(SelectedInstanceData->HlslData.MaterialIndex));
		OHierarchicalWidgetBase::Draw();
	}
}

void OGeometryEntityWidget::InitWidget()
{
	IWidget::InitWidget();
	using namespace DirectX;

	// If the decompose fails, set the default values
	Position = RenderItem.lock()->GetDefaultInstance()->HlslData.Position;
	QuaternionToEulerAngles(RenderItem.lock()->GetDefaultInstance()->HlslData.Rotation, Rotation);

	Scale = RenderItem.lock()->GetDefaultInstance()->HlslData.Scale;

	TransformWidget = MakeWidget<OGeometryTransformWidget>(&Position, &Rotation, &Scale);
	MaterialPickerWidget = MakeWidget<OMaterialPickerWidget>(Engine->GetMaterialManager());

	TransformWidget->GetOnTransformUpdate().Add([this]() {
		// Convert to XMVECTOR for operations
		const XMVECTOR vPosition = XMLoadFloat3(&Position);
		const XMVECTOR vRotation = XMLoadFloat3(&Rotation);
		const XMVECTOR vScale = XMLoadFloat3(&Scale);

		// Create individual transform matrices
		const XMMATRIX matScale = XMMatrixScalingFromVector(vScale);
		const XMMATRIX matRotation = XMMatrixRotationRollPitchYawFromVector(vRotation);
		const XMMATRIX matTranslation = XMMatrixTranslationFromVector(vPosition);

		// Combine into a single transform matrix
		const XMMATRIX transformMatrix = matScale * matRotation * matTranslation;
		XMStoreFloat4x4(&SelectedInstanceData->HlslData.World, transformMatrix);
		SelectedInstanceData->HlslData.Position = Position;
		Put(SelectedInstanceData->HlslData.Rotation, XMQuaternionRotationRollPitchYaw(Rotation.x, Rotation.y, Rotation.z));
		SelectedInstanceData->HlslData.Scale = Scale;
	});

	MaterialPickerWidget->GetOnMaterialUpdateDelegate().Add([this](const weak_ptr<SMaterial>& Material) {
		SelectedInstanceData->HlslData.MaterialIndex = Material.lock()->MaterialCBIndex;
	});
}

void OGeometryEntityWidget::RebuildRequest() const
{
	Manager->RebuildRequest();
}
