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
#include	<DgnPlatform/TextHandlers.h>


#include	<cmath>
#include	<vector>

#include	"DrawingToolsIds.h"

using namespace std;
USING_NAMESPACE_BENTLEY_DGNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT

class CWaterLevelSymbolDrawing : public DgnPrimitiveTool
{
public:
	CWaterLevelSymbolDrawing(int toolName, int toolPrompt) : DgnPrimitiveTool(toolName, toolPrompt) {};
	~CWaterLevelSymbolDrawing();
	static void InstallNewInstance(int toolId, int toolPrompt);

protected:
	//右键复位
	virtual bool			_OnResetButton(DgnButtonEventCR ev) override { _OnRestartTool(); return true; }
	virtual void			_OnRestartTool() override { InstallNewInstance(GetToolId(), GetToolPrompt()); };

	virtual bool			_OnDataButton(DgnButtonEventCR ev) override;
	virtual void			_OnPostInstall() override;
	virtual void			_OnDynamicFrame(DgnButtonEventCR ev) override;


private:
#pragma region 新方法
	//levelDatumPt水平基准面上的点
	//vtDistance 离基准点的y轴距离
	bool CreateWaterLevelSymbol(EditElementHandleR eeh, DPoint3d levelDatumPt, vector<double> vtDistance, vector<WCharCP> vtText);
	bool CreateSymbol(EditElementHandleR eeh, DPoint3d levelDatumPt, vector<double> vtDistance);
	bool CreateElement(EditElementHandleR eeh, vector<DPoint3d> points);
	void CalcAllPt(DPoint3d levelDatumPt, vector<double> vtDistance);
	void calcSymbolPoints(DPoint3d point, vector<double> waterLevelHeight);
	vector<DPoint3d> calcTrianglePt(DPoint3d symbolLocatePt, double triSideDis);
	vector<DPoint3d> calcLinePt(DPoint3d symbolLocatePt, double lineDis, double intervalDis);
	bool CreateText(EditElementHandleR eeh, DPoint3d point, WCharCP text);
	void CalcTextPt(DPoint3d point, vector<double> vtText);
	void CalcVerticalLinePt(DPoint3d levelDatumPt, vector<double> vtDistance);
	
	
#pragma endregion

	
	bool pickupDamDatum(); //拾取大坝基准点  TODO

	
private:
	vector<DPoint3d> m_DataButtonPickNum;
	double m_waterLevelDatum;  //水位基准 TODO

	vector<vector<DPoint3d>> m_sumSymbolPt;
	vector<vector<DPoint3d>> m_sumTextPt;
	
	//vector<double> vtDistance = { -100000, -500000, -900000};
	//vector<WCharCP> vtText = { L"设计高水位", L"设计常水位", L"设计低水位" };
	vector<double> m_vtDistance = { -100000.0 };
	vector<WCharCP> m_vtText = { L"设计高水位" };
};

