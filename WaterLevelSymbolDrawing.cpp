#include "WaterLevelSymbolDrawing.h"

//单位微米 1mm = 1000um
const double TRIANGLEHEIGHT =      10000.0; //10mm 三角形高 
const double MAXLINEDIS =          30000.0; //40mm  长线长度
const double MIDLINEDIS =          20000.0; //30mm  中线长度
const double MINLINEDIS =          10000.0; //25mm 短线长度
const double LINEINTERVALDIS =     3000.0; //3mm 三条线的间距 三角形高的1/3

//const double TEXTLINEDIS =         50000.0; //50m  文本下方线的长度
//const double TEXTLINEINTERVALDIS = 10000.0; //10m  文本下方线的间距


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
	/*int ht;   //TextStyle_Height返回是double，不能用int接
	m_textStyle->GetProperty(TextStyle_Height, ht);*/
	double dt;
	m_textStyle->GetProperty(TextStyle_Height, dt);
	m_textLineIntervalDis = dt * 1.5; //文本线间距设为文本高度的1.5

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
	//基点坐标
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


#pragma region 水位符号
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
	/*int ht;   //TextStyle_Height返回是double，不能用int接
	m_textStyle->GetProperty(TextStyle_Height, ht);*/
	double dt;
	m_textStyle->GetProperty(TextStyle_Height, dt);
	DgnTextStylePtr pTextStyle = DgnTextStyle::GetSettings(*(ISessionMgr::GetActiveDgnFile()));
	pTextStyle->SetProperty(TextStyle_Width, dt);
	pTextStyle->SetProperty(TextStyle_Height, dt);

	DgnModelP pActiveModel = ISessionMgr::GetActiveDgnModelP();
	TextBlockPtr pTextBlock = TextBlock::Create(*pTextStyle, *pActiveModel);
	pTextBlock->AppendText(text.data());
	//文本的四至坐标小于实际大小的包围盒
	point.y += 0.145 * dt;  //将文本置于下划线之上
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

//计算水位符号
void CWaterLevelSymbolDrawing::calcSymbolPoints(DPoint3d point)
{
	DPoint3d symbolLocatePt = point;
	for (double wh : m_waterLevels)
	{
		//通过水位基准和各水位线差求间距
		//TODO
		//求每个符号的定点坐标（三角形下方点坐标）
		symbolLocatePt.x = point.x; symbolLocatePt.y = point.y + wh; symbolLocatePt.z = point.z;
		m_sumSymbolPt.emplace_back(calcTrianglePt(symbolLocatePt));
		m_sumSymbolPt.emplace_back(calcLinePt(symbolLocatePt, MAXLINEDIS, 0));
		m_sumSymbolPt.emplace_back(calcLinePt(symbolLocatePt, MIDLINEDIS, LINEINTERVALDIS));
	}
	//求minLine坐标点（2个）
	m_sumSymbolPt.emplace_back(calcLinePt(symbolLocatePt, MINLINEDIS, LINEINTERVALDIS * 2));
}

//计算水位符号-三角形
vector<DPoint3d> CWaterLevelSymbolDrawing::calcTrianglePt(DPoint3d symbolLocatePt)
{
	//求三角形点坐标（4个）
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

//计算水位符号-线条
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

//计算文本及下方线条的坐标
void CWaterLevelSymbolDrawing::CalcTextPt(DPoint3d point)
{
	double offsetX = MAXLINEDIS / 2; //默认情况：一个符号时，文本定位点x偏移MAXLINEDIS/2
	double offsetY = m_waterLevels[0];  //默认情况：一个符号时，定位点y没有起始偏移
	if (m_waterLevels.size() != 1)
	{
		//多个水位符号的情况
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

//获取垂直线坐标点
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
		mdlDialog_dmsgsPrint(L"请输入水位值！");
		return false;
	}
	if (m_vtText.size() == 0)
	{
		mdlDialog_dmsgsPrint(L"请输入水位文本！");
		return false;
	}
	if (m_waterLevels.size() != m_vtText.size())
	{
		mdlDialog_dmsgsPrint(L"请输入水位值与文本个数不匹配！");
		return false;
	}
	return true;
}
#pragma endregion
