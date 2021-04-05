#include "BoundingboxDrawing.h"


#pragma region 重写虚函数
void BoundingboxTool::_OnPostInstall()
{
	m_hitPath = NULL;
	__super::_OnPostInstall();
}

//bool BoundingboxTool::_OnDataButton(DgnButtonEventCR ev)
//{
//
//	return false;
//}

void BoundingboxTool::_OnRestartTool()
{
	//双击命令进入1
	__super::_OnRestartTool();
}

//bool BoundingboxTool::_OnPostLocate(HitPathCP path, WStringR cantAcceptReason)
//{
//	return true;
//}

bool BoundingboxTool::ValidateSelection(HitPathCP hitPath)
{
	//Gets the ElementHandle from the hitPath
	ElementHandle eleHandle(hitPath->GetHeadElem(), nullptr);
	//Checks whether selected element is line element or not
	return (LINE_ELM == eleHandle.GetElementType()) ? true : false;
}

void BoundingboxTool::_SetupAndPromptForNextAction()
{
	//双击命令进入2
	__super::_SetupAndPromptForNextAction();
}

//void BoundingboxTool::_OnDynamicFrame(DgnButtonEventCR ev)
//{
//	//第二次框选进入
//	__super::_OnDynamicFrame(ev);
//}

//bool BoundingboxTool::_OnResetButton(DgnButtonEventCR ev)
//{
//	_OnRestartTool();
//	return true;
//}


//StatusInt BoundingboxTool::_OnElementModify(EditElementHandleR el)
//{
//	return 0;
//}

//bool EleAgendaEvents::_OnRedrawGroupEvent(ElementAgendaCP, AgendaOperation, AgendaModify, RedrawGroupInfo const*)
//{
//	return false;
//}
//////////////////////////////////////  ////////////////////////////////////
void BoundingboxTool::_LocateOneElement(DgnButtonEventCR ev, bool newSearch)
{
	__super::_LocateOneElement(ev, newSearch);
}

HitPathCP BoundingboxTool::_DoLocate(DgnButtonEventCR ev, bool newSearch, ComponentMode complexComponent)
{
	__super::_DoLocate(ev, newSearch, complexComponent);
	return NULL;
}

//EditElementHandleP BoundingboxTool::_BuildLocateAgenda(HitPathCP path, DgnButtonEventCP ev)
//{
//	__super::_BuildLocateAgenda(path, ev);
//	return NULL;
//}

void BoundingboxTool::_ModifyAgendaEntries()
{
	__super::_ModifyAgendaEntries();
}

bool BoundingboxTool::_FilterAgendaEntries()
{
	__super::_FilterAgendaEntries();
	return true;
}

void BoundingboxTool::_HiliteAgendaEntries(bool changed)
{
	__super::_HiliteAgendaEntries(changed);
}

//bool BoundingboxTool::_SetupForModify(DgnButtonEventCR ev, bool isDynamics)
//{
//	__super::_SetupForModify(ev, isDynamics);
//	return true;
//}

StatusInt BoundingboxTool::_ProcessAgenda(DgnButtonEventCR ev)
{
	__super::_ProcessAgenda(ev);
	return 0;
}

bool BoundingboxTool::_OnModifyComplete(DgnButtonEventCR ev)
{
	__super::_OnModifyComplete(ev);
	return false;
}

void BoundingboxTool::_OnReinitialize()
{
	__super::_OnReinitialize();
}

#pragma endregion


#pragma region 自定义函数
void BoundingboxTool::InstallNewInstance(int toolId)
{
	BoundingboxTool* newToolP = new BoundingboxTool(toolId);
	newToolP->InstallTool();
}

bool BoundingboxTool::CreateBoundingbox(EditElementHandleR eeh, bvector<DPoint3d> const& points)
{
	

	return true;
}

bool BoundingboxTool::CreateElement(EditElementHandleR eeh, bvector<DPoint3d> points)
{
	
	return true;
}
#pragma endregion