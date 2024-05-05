
#include "BilateralFilterWidget.h"

#include "Engine/RenderTarget/Filters/BilateralBlur/BilateralBlurFilter.h"
#include "Logger.h"
#include "imgui.h"
void OBilateralBlurFilterWidget::Draw()
{
	bEnabled = ImGui::TreeNode("Bilateral Blur");
	if (bEnabled)
	{
		ImGui::SliderFloat("Bilateral Spatial Sigma", &SpatialSigma, 0.0f, 50.0f);
		ImGui::SliderFloat("Bilateral Intensity Sigma", &IntensitySigma, 0.0f, 25.0f);
		ImGui::SliderInt("Bilateral Blur Count", &BlurCount, 0, 15);
		ImGui::TreePop();
	}
}

void OBilateralBlurFilterWidget::Update()
{
	if (bEnabled)
	{
		if (Filter == nullptr)
		{
			LOG(Widget, Error, "Filter is nullptr");
			return;
		}

		Filter->SetBlurCount(BlurCount);
		Filter->SetIntensitySigma(IntensitySigma);
		Filter->SetSpatialSigma(SpatialSigma);
	}
	else
	{
		Filter->SetBlurCount(0);
	}
}