#include "BoundingboxDrawing.h"

UsesDragSelect BoundingboxTool::_AllowDragSelect()
{

}

void BoundingboxTool::_OnRestartTool()
{
	InstallNewInstance(GetToolId(), m_alignment);
}

bool BoundingboxTool::_OnDataButton(DgnButtonEventCR ev)
{
	m_iView = ev.GetViewNum();
	return true;
}

void BoundingboxTool::InstallNewInstance(int toolId, int alignment)
{
	BoundingboxTool*   tool = new BoundingboxTool(toolId, alignment);
	tool->InstallTool();
}


