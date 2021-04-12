#pragma once

#include    <Mstn/MdlApi/MdlApi.h>
#include    <DgnView/DgnElementSetTool.h>
#include    <Bentley/stdcxx/bvector.h>
#include	<Mstn/isessionmgr.h>
#include	<DgnPlatform/LinearHandlers.h>
#include	<DgnPlatform/DisplayHandler.h>
#include	<DgnPlatform/MeshHeaderHandler.h>
#include	<Mstn/ElementPropertyUtils.h>
#include	<DgnPlatform/DropGraphics.h>

#include	<cmath>
#include	<vector>

#include	"DrawingToolsIds.h"

using namespace std;
USING_NAMESPACE_BENTLEY_DGNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT

class RectangleDrawing : public DgnPrimitiveTool
{
public:
	RectangleDrawing(int toolName, int toolPrompt) : DgnPrimitiveTool(toolName, toolPrompt) {}
	static void InstallNewInstance(int toolId, int toolPrompt);
	
protected:
	//ÓÒ¼ü¸´Î»
	virtual bool			_OnResetButton(DgnButtonEventCR ev) override { _OnRestartTool(); return true; }
	virtual void			_OnRestartTool() override { InstallNewInstance(GetToolId(), GetToolPrompt()); };

	virtual bool			_OnDataButton(DgnButtonEventCR ev) override;
	virtual void			_OnPostInstall() override;
	virtual void			_OnDynamicFrame(DgnButtonEventCR ev) override;

private:
	bool CreateRectangle(EditElementHandleR eeh, vector<DPoint3d> points);
	bool CreateDynamicRectangle(ElementAgenda agenda, vector<DPoint3d> points);
	vector<DPoint3d> calcPoint(vector<DPoint3d> points);
private:
	vector<DPoint3d>   m_points;
	vector<DPoint3d>   m_dynamicPoints;
};

