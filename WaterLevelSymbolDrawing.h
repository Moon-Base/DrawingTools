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

class CWaterLevelSymbolDrawing : public DgnPrimitiveTool
{
public:
	CWaterLevelSymbolDrawing(int toolName, int toolPrompt) : DgnPrimitiveTool(toolName, toolPrompt) {};
	~CWaterLevelSymbolDrawing();
	static void InstallNewInstance(int toolId, int toolPrompt);

protected:
	//�Ҽ���λ
	virtual bool _OnResetButton(DgnButtonEventCR ev) override { _OnRestartTool(); return true; }
	virtual void _OnRestartTool() override { InstallNewInstance(GetToolId(), GetToolPrompt()); };

	virtual bool _OnDataButton(DgnButtonEventCR ev) override;
	virtual void _OnPostInstall() override;
	virtual void _OnDynamicFrame(DgnButtonEventCR ev) override;


private:
#pragma region �·���
	//levelDatumPtˮƽ��׼���ϵĵ�
	//vtDistance ���׼���y�����
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
	double                   m_waterLevelDatum = 0.0;  //ˮλ��׼ TODO

	vector<vector<DPoint3d>> m_sumSymbolPt;
	vector<vector<DPoint3d>> m_sumTextPt;
	
	//vector<double>           m_vtDistance = { -1000000, -2000000, -3000000}; //���1��
	vector<double>           m_waterLevels = { -100000.0, -200000.0, -300000.0 }; //���1��
	vector<wstring>          m_vtText = { L"��Ƹ�ˮλ", L"��Ƴ�ˮλ", L"��Ƶ�ˮλ" };
	/*vector<double>         m_vtDistance = { -100000.0 };
	vector<std::wstring>          m_vtText = { L"��Ƹ�ˮλ" };*/
};
//��Ƹ�ˮλ����Ƴ�ˮλ����Ƶ�ˮλ
//-100000��-500000��-900000

