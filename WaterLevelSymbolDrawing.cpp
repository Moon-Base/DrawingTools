#include "WaterLevelSymbolDrawing.h"

//��λ΢�� 1mm = 1000um
const double TRIANGLEHEIGHT =      10000.0; //10mm �����θ� 
const double MAXLINEDIS =          30000.0; //40mm  ���߳���
const double MIDLINEDIS =          20000.0; //30mm  ���߳���
const double MINLINEDIS =          10000.0; //25mm ���߳���
const double LINEINTERVALDIS =     3000.0; //3mm �����ߵļ�� �����θߵ�1/3

//const double TEXTLINEDIS =         50000.0; //50m  �ı��·��ߵĳ���
//const double TEXTLINEINTERVALDIS = 10000.0; //10m  �ı��·��ߵļ��


void CWaterLevelSymbolDrawing::InstallNewInstance()
{
	CWaterLevelSymbolDrawing* tool = new CWaterLevelSymbolDrawing();
	tool->InstallTool();
}

CWaterLevelSymbolDrawing::~CWaterLevelSymbolDrawing()
{

}

bool CWaterLevelSymbolDrawing::_OnDataButton(DgnButtonEventCR ev)
{
	if (!CheckWinFormValue())
		return false;

	DgnFileP  pActiveDgnFile = mdlDgnFileObj_getMasterFile();
	DgnTextStylePtr m_textStyle = DgnTextStyle::GetSettings(*pActiveDgnFile);
	/*int ht;   //TextStyle_Height������double��������int��
	m_textStyle->GetProperty(TextStyle_Height, ht);*/
	double dt;
	m_textStyle->GetProperty(TextStyle_Height, dt);
	m_textLineIntervalDis = dt * 1.5; //�ı��߼����Ϊ�ı��߶ȵ�1.5

	DPoint3d pickPt = *ev.GetPoint();
	m_pickPts.emplace_back(*ev.GetPoint());
	if (m_pickPts.size() == 1)
	{
		if (m_waterLevelDatum == 0)
		{
			m_waterLevelDatum = m_pickPts[0].y;
		}
		_BeginDynamics();
		return true;
	}
	//��������
	DPoint3d levelDatumPt = DPoint3d::From(pickPt.x, m_waterLevelDatum, 0.0);
	m_waterLevelDatum = 0.0;
	m_pickPts.clear();
	
	CalcAllPt(levelDatumPt);

	for (vector<DPoint3d> vtPt : m_sumSymbolPt)
	{
		EditElementHandle eeh;
		if (CreateElement(eeh, vtPt))
			eeh.AddToModel();
	}
	
	for (int i = 0; i < m_sumTextPt.size(); i++)
	{
		EditElementHandle eeh;
		if (CreateText(eeh, m_sumTextPt[i][0], m_vtText[i]))
			eeh.AddToModel();
	}

	return true;
}

void CWaterLevelSymbolDrawing::_OnPostInstall()
{
	AccuSnap::GetInstance().EnableSnap(true); // Enable snapping for create tools.
	__super::_OnPostInstall();
}

void CWaterLevelSymbolDrawing::_OnDynamicFrame(DgnButtonEventCR ev)
{
	if (m_waterLevelDatum == 0)
		return;
	DPoint3d dynamicPt = *ev.GetPoint();
	DPoint3d levelDatumPt = DPoint3d::From(dynamicPt.x, m_waterLevelDatum, 0.0);

	CalcAllPt(levelDatumPt);
	
	for (vector<DPoint3d> vtPt : m_sumSymbolPt)
	{
		EditElementHandle eeh;
		if (!CreateElement(eeh, vtPt))
			continue;
		RedrawElems redrawElems;
		redrawElems.SetDynamicsViews(IViewManager::GetActiveViewSet(), ev.GetViewport());
		redrawElems.SetDrawMode(DRAW_MODE_TempDraw);
		redrawElems.SetDrawPurpose(DrawPurpose::Dynamics);
		redrawElems.DoRedraw(eeh);
	}

	for (int i = 0; i < m_sumTextPt.size(); i++)
	{
		EditElementHandle eeh;
		if (!CreateText(eeh, m_sumTextPt[i][0], m_vtText[i]))
			continue;
		RedrawElems redrawElems;
		// Display in all views, draws to cursor view first...
		redrawElems.SetDynamicsViews(IViewManager::GetActiveViewSet(), ev.GetViewport());
		redrawElems.SetDrawMode(DRAW_MODE_TempDraw);
		redrawElems.SetDrawPurpose(DrawPurpose::Dynamics);
		redrawElems.DoRedraw(eeh);
	}
}


#pragma region ˮλ����
bool CWaterLevelSymbolDrawing::CreateElement(EditElementHandleR eeh, vector<DPoint3d> points)
{
	bvector<DPoint3d> bPoints;
	for (auto& p : points)
	{
		bPoints.push_back(p);
	}
	DgnModelP model = ISessionMgr::GetActiveDgnModelP();
	MultilineStylePtr activeStyle = MultilineStyle::GetSettings(*(ISessionMgr::GetActiveDgnFile()));
	if (SUCCESS != LineStringHandler::CreateLineStringElement(eeh, nullptr, points.data(), 
		points.size(), model->Is3d(), *model))
		return false;
	return true;
}

bool CWaterLevelSymbolDrawing::CreateText(EditElementHandleR eeh, DPoint3d point, wstring text)
{
	DgnFileP  pActiveDgnFile = mdlDgnFileObj_getMasterFile();
	DgnTextStylePtr m_textStyle = DgnTextStyle::GetSettings(*pActiveDgnFile);
	/*int ht;   //TextStyle_Height������double��������int��
	m_textStyle->GetProperty(TextStyle_Height, ht);*/
	double dt;
	m_textStyle->GetProperty(TextStyle_Height, dt);
	DgnTextStylePtr pTextStyle = DgnTextStyle::GetSettings(*(ISessionMgr::GetActiveDgnFile()));
	pTextStyle->SetProperty(TextStyle_Width, dt);
	pTextStyle->SetProperty(TextStyle_Height, dt);

	DgnModelP pActiveModel = ISessionMgr::GetActiveDgnModelP();
	TextBlockPtr pTextBlock = TextBlock::Create(*pTextStyle, *pActiveModel);
	pTextBlock->AppendText(text.data());
	//�ı�����������С��ʵ�ʴ�С�İ�Χ��
	point.y += 0.145 * dt;  //���ı������»���֮��
	pTextBlock->SetUserOrigin(point);

	if (TEXTBLOCK_TO_ELEMENT_RESULT_Success != TextHandlerBase::CreateElement(eeh, nullptr, *pTextBlock))
		return false;
	ElementPropertiesSetterPtr setter = ElementPropertiesSetter::Create();
	setter->SetColor(3); // 3 -- Red
	setter->Apply(eeh);

	DRange3d dr = pTextBlock->GetNominalRange();
	double xLen = dr.XLength();
	m_textLineDis = xLen;

	return true;
}

void CWaterLevelSymbolDrawing::CalcAllPt(DPoint3d levelDatumPt)
{
	m_sumSymbolPt.clear();
	m_sumTextPt.clear();
	calcSymbolPoints(levelDatumPt);
	CalcVerticalLinePt(levelDatumPt);
	CalcTextPt(levelDatumPt);
}

//����ˮλ����
void CWaterLevelSymbolDrawing::calcSymbolPoints(DPoint3d point)
{
	DPoint3d symbolLocatePt = point;
	for (double wh : m_waterLevels)
	{
		//ͨ��ˮλ��׼�͸�ˮλ�߲�����
		//TODO
		//��ÿ�����ŵĶ������꣨�������·������꣩
		symbolLocatePt.x = point.x; symbolLocatePt.y = point.y + wh; symbolLocatePt.z = point.z;
		m_sumSymbolPt.emplace_back(calcTrianglePt(symbolLocatePt));
		m_sumSymbolPt.emplace_back(calcLinePt(symbolLocatePt, MAXLINEDIS, 0));
		m_sumSymbolPt.emplace_back(calcLinePt(symbolLocatePt, MIDLINEDIS, LINEINTERVALDIS));
	}
	//��minLine����㣨2����
	m_sumSymbolPt.emplace_back(calcLinePt(symbolLocatePt, MINLINEDIS, LINEINTERVALDIS * 2));
}

//����ˮλ����-������
vector<DPoint3d> CWaterLevelSymbolDrawing::calcTrianglePt(DPoint3d symbolLocatePt)
{
	//�������ε����꣨4����
	DPoint3d p2, p3;
	vector<DPoint3d> trianglePts;
	trianglePts.emplace_back(symbolLocatePt);
	p2.z = p3.z = symbolLocatePt.z;
	p2.x = symbolLocatePt.x - TRIANGLEHEIGHT;
	p3.x = symbolLocatePt.x + TRIANGLEHEIGHT;
	p2.y = p3.y = symbolLocatePt.y + TRIANGLEHEIGHT;
	trianglePts.emplace_back(p2);
	trianglePts.emplace_back(p3);
	trianglePts.emplace_back(symbolLocatePt);
	return trianglePts;
}

//����ˮλ����-����
vector<DPoint3d> CWaterLevelSymbolDrawing::calcLinePt(DPoint3d symbolLocatePt, double lineDis, double lineIntervalDis)
{
	vector<DPoint3d> linePts;
	DPoint3d LinePt;
	//maxLine
	LinePt.z = symbolLocatePt.z;
	LinePt.x = symbolLocatePt.x - lineDis / 2;
	LinePt.y = symbolLocatePt.y - lineIntervalDis;
	linePts.emplace_back(LinePt);
	LinePt.x = symbolLocatePt.x + lineDis / 2;
	LinePt.y = symbolLocatePt.y - lineIntervalDis;
	linePts.emplace_back(LinePt);
	return linePts;
}

//�����ı����·�����������
void CWaterLevelSymbolDrawing::CalcTextPt(DPoint3d point)
{
	double offsetX = MAXLINEDIS / 2; //Ĭ�������һ������ʱ���ı���λ��xƫ��MAXLINEDIS/2
	double offsetY = m_waterLevels[0];  //Ĭ�������һ������ʱ����λ��yû����ʼƫ��
	if (m_waterLevels.size() != 1)
	{
		//���ˮλ���ŵ����
		offsetX = 0;
		offsetY = TRIANGLEHEIGHT * 2;
	}
	for (int i = 0; i < m_waterLevels.size(); i++) 
	{
		vector<DPoint3d> TextPts;
		DPoint3d textStartPt, textEndPt;
		textStartPt.x = point.x + offsetX;
		textStartPt.y = point.y + offsetY + m_textLineIntervalDis * i;
		textStartPt.z = textEndPt.z = point.z;
		TextPts.emplace_back(textStartPt);

		textEndPt.x = textStartPt.x + m_textLineDis;
		textEndPt.y = textStartPt.y;
		TextPts.emplace_back(textEndPt);
		m_sumTextPt.emplace_back(TextPts);
		m_sumSymbolPt.emplace_back(TextPts);
	}
}

//��ȡ��ֱ�������
void CWaterLevelSymbolDrawing::CalcVerticalLinePt(DPoint3d levelDatumPt)
{
	if (m_waterLevels.size() != 1)
	{
		//double TriHeight = sqrt(pow(TRIANGLEHEIGHT*2, 2) - pow(TRIANGLEHEIGHT*2 / 2, 2));
		double offsetY = TRIANGLEHEIGHT * 2;

		vector<DPoint3d> verticalLinePt;
		DPoint3d bottomPt = levelDatumPt;
		bottomPt.y += m_waterLevels[m_waterLevels.size() - 1] - LINEINTERVALDIS * 2;
		verticalLinePt.emplace_back(bottomPt);

		DPoint3d topPt = levelDatumPt;
		topPt.y += offsetY + m_textLineIntervalDis * (m_waterLevels.size());
		//topPt.y += m_waterLevels[0] + offsetY + TEXTLINEINTERVALDIS * (m_waterLevels.size() + 1);
		verticalLinePt.emplace_back(topPt);
		m_sumSymbolPt.emplace_back(verticalLinePt);
	}
}

bool CWaterLevelSymbolDrawing::CheckWinFormValue()
{
	if (m_waterLevels.size() == 0)
	{
		mdlDialog_dmsgsPrint(L"������ˮλֵ��");
		return false;
	}
	if (m_vtText.size() == 0)
	{
		mdlDialog_dmsgsPrint(L"������ˮλ�ı���");
		return false;
	}
	if (m_waterLevels.size() != m_vtText.size())
	{
		mdlDialog_dmsgsPrint(L"������ˮλֵ���ı�������ƥ�䣡");
		return false;
	}
	return true;
}
#pragma endregion
