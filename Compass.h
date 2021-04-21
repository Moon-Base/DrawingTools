#pragma once

#include <Mstn/MdlApi/MdlApi.h>
#include <DgnView/IRedraw.h>
#include <Mstn/ISessionMgr.h>
#include <DgnPlatform/CellHeaderHandler.h>
//#include <>

USING_NAMESPACE_BENTLEY_DGNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT

class CCompass : public DgnPrimitiveTool
{
public:
	static void InstallNewInstance(int toolName, int toolPrompt);
public:
	CCompass(int toolName, int toolPrompt) :DgnPrimitiveTool(toolName, toolPrompt) {};
	~CCompass();
protected:
	virtual bool _OnInstall() override;
	virtual void _OnPostInstall() override;
	virtual void _OnRestartTool() override;
	virtual bool _OnDataButton(DgnButtonEventCR ev) override;
	virtual void _OnDynamicFrame(DgnButtonEventCR ev) override;
	virtual bool _OnResetButton(DgnButtonEventCR ev) override;
	virtual void _ExitTool() override;

private:
	bool CopyCellFromLibrary(MSElementDescrP& pDescr);
	void GetTransform(Transform& tran, DgnButtonEventCR ev);
	bool DrawCell(EditElementHandleR eeh, DPoint3dCR point,
		const Transform& tran, MSElementDescrP pDescr, bool isDynamic);
private:
	EditElementHandle m_eeh;
	DPoint3dCP      m_clickPoint;
	MSElementDescrP m_pTempCell;
};