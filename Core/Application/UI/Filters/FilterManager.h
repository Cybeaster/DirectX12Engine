#pragma once
#include "BilateralFilterWidget.h"
#include "Engine/Engine.h"
#include "GaussianBlurWidget.h"
#include "SobelFilterWidget.h"
#include "UI/Widget.h"

class OFilterManagerWidget : public OHierarchicalWidgetBase
{
public:
	OFilterManagerWidget(OEngine* Other)
	    : Engine(Other){};
	~OFilterManagerWidget() = default;

	void Draw() override;
	void InitWidget() override;

private:
	OEngine* Engine = nullptr;
};

inline void OFilterManagerWidget::Draw()
{
	if (ImGui::CollapsingHeader("Filters"))
	{
		OHierarchicalWidgetBase::Draw();
	}
}
inline void OFilterManagerWidget::InitWidget()
{
	OHierarchicalWidgetBase::InitWidget();
	MakeWidget<OBilateralBlurFilterWidget>(Engine->GetBilateralBlurFilter());
	MakeWidget<OGaussianBlurWidget>(Engine->GetBlurFilter());
	MakeWidget<OSobelFilterWidget>(Engine->GetSobelFilter());
}