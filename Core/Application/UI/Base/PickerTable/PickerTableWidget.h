
#pragma once
#include "UI/Widget.h"
class OPickerTableWidget : public OHierarchicalWidgetBase
{
public:
	void Draw() override;

	virtual void DrawTable() = 0;
	virtual void DrawProperty() = 0;

protected:
	string HeadderName = "Table";
	string PropertyName = "Property";
	string ListName = "List";

private:
	float SplitterPercent = 0.25f;
	bool ResizingSplitter = false;
	const float SplitterWidth = 8.0f; // Width of the splitter
};
