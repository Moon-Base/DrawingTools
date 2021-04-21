#include "Compass.h"

CCompass::~CCompass()
{

}

void CCompass::InstallNewInstance(int toolName, int toolPrompt)
{
	CCompass *tool = new CCompass(toolName, toolPrompt);
	tool->InstallTool();
}

bool CCompass::_OnInstall()
{
	if (!CopyCellFromLibrary(m_pTempCell))
		return false;
	return true;
}

void CCompass::_OnPostInstall()
{
	_BeginDynamics();
	AccuSnap::GetInstance().EnableSnap(true);
	__super::_OnPostInstall();
}

void CCompass::_OnRestartTool()
{
	if (m_clickPoint != nullptr)
		mdlElmdscr_freeAll(&m_pTempCell);
	InstallNewInstance(0,0);
}

bool CCompass::_OnDataButton(DgnButtonEventCR ev)
{
	MSElementDescrP pDescr = nullptr;
	CopyCellFromLibrary(pDescr);
	m_clickPoint = ev.GetPoint();
	Transform tran;
	GetTransform(tran, ev);
	if (DrawCell(m_eeh, *m_clickPoint, tran, pDescr, false))
		m_eeh.AddToModel();

	mdlElmdscr_freeAll(&pDescr); 
	return true;
}

void CCompass::_OnDynamicFrame(DgnButtonEventCR ev)
{
	m_clickPoint = ev.GetPoint();
	Transform tran;
	GetTransform(tran, ev);

	if (!DrawCell(m_eeh, *m_clickPoint, tran, m_pTempCell, true))
		return;

	RedrawElems redrawElems;

	redrawElems.SetDynamicsViews(IViewManager::GetActiveViewSet(), ev.GetViewport());
	redrawElems.SetDrawMode(DRAW_MODE_TempDraw);
	redrawElems.SetDrawPurpose(DrawPurpose::Dynamics);

	redrawElems.DoRedraw(m_eeh);

	tran.InverseOf(tran);
	TransformInfo transformInfo(tran);
	m_eeh.GetHandler().ApplyTransform(m_eeh, transformInfo);
}

bool CCompass::_OnResetButton(DgnButtonEventCR ev)
{
	_OnRestartTool();
	return true;
}

void CCompass::_ExitTool()
{
	if (m_clickPoint != nullptr)
		mdlElmdscr_freeAll(&m_pTempCell);

	__super::_ExitTool();
}

bool CCompass::CopyCellFromLibrary(MSElementDescrP &pDescr)
{
	int status = mdlCell_getElmDscr(&pDescr, NULL, NULL, NULL, false,
		NULL, 0, 0, false, false, L"BATEN", NULL);

	if (status != SUCCESS)
		return false;

	return true;
}

void CCompass::GetTransform(Transform &tran, DgnButtonEventCR ev)
{
	tran.InitIdentity();
	DPoint3dCP pPt = ev.GetPoint();
	//RotMatrix matrix = RotMatrix::FromIdentity();
	RotMatrix rotMat;
	rotMat.InitFromAxisAndRotationAngle(2, -0.5);
	tran.SetMatrix(rotMat);
	tran.SetTranslation(*pPt);
	TransformInfo transformInfo(tran);
}

bool CCompass::DrawCell(EditElementHandleR eeh, DPoint3dCR point,
	const Transform& tran, MSElementDescrP pDescr, bool isDynamic)
{
	m_eeh.Invalidate();

	DgnModelP pActiveModel = ISessionMgr::GetActiveDgnModelP();

	NormalCellHeaderHandler::CreateOrphanCellElement(eeh, NULL, true, *pActiveModel);
	eeh.SetElementDescr(pDescr, !isDynamic, false);

	TransformInfo transformInfo(tran);

	eeh.GetHandler().ApplyTransform(eeh, transformInfo);
	eeh.ReplaceInModel(eeh.GetElementRef());
	return true;
}