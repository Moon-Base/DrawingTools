#pragma once

#include <Mstn/MdlApi/MdlApi.h>
#include <DgnView/DgnElementSetTool.h>
#include <Bentley/stdcxx/bvector.h>
#include <Mstn/isessionmgr.h>
#include <DgnPlatform/LinearHandlers.h>
#include <DgnPlatform/DisplayHandler.h>
#include <DgnPlatform/MeshHeaderHandler.h>
#include <Mstn/ElementPropertyUtils.h>
#include <DgnPlatform/DropGraphics.h>
#include <DgnPlatform/TextHandlers.h>
#include <cmath>
#include <vector>
#include "DrawingToolsIds.h"


using namespace std;
USING_NAMESPACE_BENTLEY_DGNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT

class CWaterLevelSymbolDrawing : public DgnElementSetTool
{
public:
	CWaterLevelSymbolDrawing() : DgnElementSetTool() {};
	~CWaterLevelSymbolDrawing();
	static void InstallNewInstance();

protected:
	//右键复位
	virtual bool _OnResetButton(DgnButtonEventCR ev) override { _OnRestartTool(); return true; }
	virtual void _OnRestartTool() override { InstallNewInstance(); };
	virtual StatusInt _OnElementModify(EditElementHandleR el) override { return 0; };

	virtual bool _OnDataButton(DgnButtonEventCR ev) override;
	virtual void _OnPostInstall() override;
	virtual void _OnDynamicFrame(DgnButtonEventCR ev) override;


private:
#pragma region 新方法
	//levelDatumPt水平基准面上的点
	//vtDistance 离基准点的y轴距离
	bool             CreateElement(EditElementHandleR eeh, vector<DPoint3d> points);
	bool             CreateText(EditElementHandleR eeh, DPoint3d point, std::wstring text);
	void             CalcAllPt(DPoint3d levelDatumPt);
	void             calcSymbolPoints(DPoint3d point);
	vector<DPoint3d> calcTrianglePt(DPoint3d symbolLocatePt);
	vector<DPoint3d> calcLinePt(DPoint3d symbolLocatePt, double lineDis, double intervalDis);
	void             CalcVerticalLinePt(DPoint3d levelDatumPt);
	void             CalcTextPt(DPoint3d point);
	
#pragma endregion

	
private:
	vector<DPoint3d>         m_pickPts;
	//点击水位基准线获取值，则输入0；否则输入正常基准值
	double                   m_waterLevelDatum = 24018;  //水位基准
	vector<vector<DPoint3d>> m_sumSymbolPt;
	vector<vector<DPoint3d>> m_sumTextPt;
	double                   m_textLineIntervalDis;  //文本下方线的间距
	double                   m_textLineDis;          //文本下方线的长度
	vector<double>           m_waterLevels = { -10000.0, -30000.0, -60000.0 }; //间隔7mm
	vector<wstring>          m_vtText = { L"设计高水位", L"设计常水位", L"设计低水位" };
};

