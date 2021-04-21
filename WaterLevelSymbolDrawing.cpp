#include "WaterLevelSymbolDrawing.h"


const double INTERVALDIS = 50000.0;
const double MAXLINEDIS = 400000.0;
const double MIDLINEDIS = 300000.0;
const double MINLINEDIS = 100000.0;
const double TRIANGLESIDEDIS = 200000.0;

const double TEXTHEIGHT = 100000;
const double TEXTLINELENGTH = 700000.0;



void CWaterLevelSymbolDrawing::InstallNewInstance(int toolId, int toolPrompt)
{
	CWaterLevelSymbolDrawing* tool = new CWaterLevelSymbolDrawing(toolId, toolPrompt);
	tool->InstallTool();
}

CWaterLevelSymbolDrawing::~CWaterLevelSymbolDrawing()
{

}

bool CWaterLevelSymbolDrawing::_OnDataButton(DgnButtonEventCR ev)
{
	/*if (0 == m_trianglePoints.size())
		_BeginDynamics();*/
	DPoint3d pickPt = *ev.GetPoint();
	m_DataButtonPickNum.push_back(pickPt);
	if (m_DataButtonPickNum.size() == 1)
	{
		m_waterLevelDatum = m_DataButtonPickNum[0].y;
		_BeginDynamics();
		return true;
	}
	
	DPoint3d levelDatumPt = DPoint3d::From(pickPt.x, m_waterLevelDatum, 0.0);
	m_waterLevelDatum = 0.0;
	m_DataButtonPickNum.clear();
	
	CalcAllPt(levelDatumPt, m_vtDistance);

	
	for (vector<DPoint3d> vtPt : m_sumSymbolPt)
	{
		EditElementHandle eeh;
		if (CreateElement(eeh, vtPt))
			eeh.AddToModel();
	}
	
	for (int i = 0; i < m_sumTextPt.size(); i++)
	{
		EditElementHandle eehText;
		if (CreateText(eehText, m_sumTextPt[i][0], m_vtText[i]))
			eehText.AddToModel();
	}
	

	//calcSymbolPoints(symbolLocatePt, vtWaterLevelHeight);

	//if (m_sumSymbolPt.size()==0)
	//	return false;
	//
	//for (vector<DPoint3d> vtPt : m_sumSymbolPt)
	//{
	//	if (!CreateElement(vtPt))
	//		return false;
	//}
	////放置文本和绘制文本底线
	//DPoint3d textLocatePt = symbolLocatePt;
	//textLocatePt.y += vtWaterLevelHeight[0];
	//double offsetX = MAXLINEDIS / 2; //默认情况：一个符号时，文本定位点x偏移MAXLINEDIS/2
	//double offsetY = 0;  //默认情况：一个符号时，定位点y没有起始偏移
	//if (vtText.size() != 1)
	//{
	//	//不止一个水位符号的情况
	//	offsetX = 0;
	//	double ylen = sqrt(pow(TRIANGLESIDEDIS, 2) - pow(TRIANGLESIDEDIS / 2, 2));
	//	offsetY = ylen * 3;
	//}

	//for (int i = 0; i < vtText.size(); i++)
	//{
	//	vector<DPoint3d> vtTextLinePt = CalcTextPt(textLocatePt, offsetX, offsetY + TEXTHEIGHT * i);
	//	CreateElement(vtTextLinePt);
	//	vtTextLinePt[0].y += TEXTHEIGHT * 0.75;
	//	CreateText(vtTextLinePt[0], vtText[i]);
	//}

	////绘制垂直线
	//if (vtText.size() != 1)
	//{
	//	vector<DPoint3d> vtVerticalLinePt;
	//	DPoint3d verticalLineBottomPt = symbolLocatePt;
	//	verticalLineBottomPt.y += vtWaterLevelHeight[vtWaterLevelHeight.size() - 1] - INTERVALDIS * 2;
	//	vtVerticalLinePt.push_back(verticalLineBottomPt);
	//	DPoint3d verticalLineTopPt = symbolLocatePt;
	//	verticalLineTopPt.y += vtWaterLevelHeight[0] + offsetY + TEXTHEIGHT * vtText.size();
	//	vtVerticalLinePt.push_back(verticalLineTopPt);
	//	CreateElement(vtVerticalLinePt);
	//}
	
	return true;
}

void CWaterLevelSymbolDrawing::_OnPostInstall()
{
	AccuSnap::GetInstance().EnableSnap(true); // Enable snapping for create tools.
	__super::_OnPostInstall();
}

void CWaterLevelSymbolDrawing::_OnDynamicFrame(DgnButtonEventCR ev)
{
	/*DPoint3d point;
	__super::_OnDynamicFrame(ev);
	point = *ev.GetPoint();
	
	m_dynamicTrgPoints = calcTrianglePoint(m_dynamicTrgPoints);
	
	EditElementHandle eeh;
	if (!CreateTriangle(eeh, m_dynamicTrgPoints))
		return;*/
	//__super::_OnDynamicFrame(ev);
	if (m_waterLevelDatum == 0)
		return;
	DPoint3d pickPt = *ev.GetPoint();
	DPoint3d levelDatumPt = DPoint3d::From(pickPt.x, m_waterLevelDatum, 0.0);

	CalcAllPt(levelDatumPt, m_vtDistance);
	
	for (vector<DPoint3d> vtPt : m_sumSymbolPt)
	{
		EditElementHandle eeh;
		if (!CreateElement(eeh, vtPt))
			continue;
		RedrawElems redrawElems;
		redrawElems.SetDynamicsViews(IViewManager::GetActiveViewSet(), ev.GetViewport()); // Display in all views, draws to cursor view first...
		redrawElems.SetDrawMode(DRAW_MODE_TempDraw);
		redrawElems.SetDrawPurpose(DrawPurpose::Dynamics);
		redrawElems.DoRedraw(eeh);
	}
	

	for (int i = 0; i < m_sumTextPt.size(); i++)
	{
		EditElementHandle eehText;
		if (!CreateText(eehText, m_sumTextPt[i][0], m_vtText[i]))
			continue;
		RedrawElems redrawElems;
		redrawElems.SetDynamicsViews(IViewManager::GetActiveViewSet(), ev.GetViewport()); // Display in all views, draws to cursor view first...
		redrawElems.SetDrawMode(DRAW_MODE_TempDraw);
		redrawElems.SetDrawPurpose(DrawPurpose::Dynamics);
		redrawElems.DoRedraw(eehText);
	}
	
	
}


bool CWaterLevelSymbolDrawing::pickupDamDatum()
{
	return true;
}


#pragma region 新方法
bool CWaterLevelSymbolDrawing::CreateWaterLevelSymbol
(EditElementHandleR eeh, DPoint3d levelDatumPt, vector<double> vtDistance, vector<WCharCP> vtText)
{
	if (!CreateSymbol(eeh, levelDatumPt, vtDistance))
		return false;
	/*if (!CreateText(eeh, m_sumTextPt, vtText))
		return false;*/
	return true;
}

void CWaterLevelSymbolDrawing::CalcAllPt(DPoint3d levelDatumPt, vector<double> vtDistance)
{
	m_sumSymbolPt.clear();
	calcSymbolPoints(levelDatumPt, vtDistance);
	CalcVerticalLinePt(levelDatumPt, vtDistance);
	CalcTextPt(levelDatumPt, vtDistance);
}

bool CWaterLevelSymbolDrawing::CreateSymbol
(EditElementHandleR eeh, DPoint3d levelDatumPt, vector<double> vtDistance)
{
	//获取符号的坐标点
	if (m_sumSymbolPt.size() == 0)
		return false;

	for (vector<DPoint3d> vtPt : m_sumSymbolPt)
	{
		if (!CreateElement(eeh, vtPt))
			return false;
	}
	return true;
}

//计算水位符号
void CWaterLevelSymbolDrawing::calcSymbolPoints
(DPoint3d point, vector<double> waterLevelHeight)
{
	DPoint3d symbolLocatePt = point;
	for (double wh : waterLevelHeight)
	{
		//通过水位基准和各水位线差求间距
		//TODO
		//求每个符号的定点坐标（三角形下方点坐标）
		symbolLocatePt.x = point.x; symbolLocatePt.y = point.y + wh; symbolLocatePt.z = point.z;
		m_sumSymbolPt.push_back(calcTrianglePt(symbolLocatePt, TRIANGLESIDEDIS));
		m_sumSymbolPt.push_back(calcLinePt(symbolLocatePt, MAXLINEDIS, 0));
		m_sumSymbolPt.push_back(calcLinePt(symbolLocatePt, MIDLINEDIS, INTERVALDIS));
	}
	//求minLine坐标点（2个）
	m_sumSymbolPt.push_back(calcLinePt(symbolLocatePt, MINLINEDIS, INTERVALDIS * 2));
}

//计算三角形
vector<DPoint3d> CWaterLevelSymbolDrawing::calcTrianglePt
(DPoint3d symbolLocatePt, double triSideDis)
{
	//求三角形点坐标（4个）
	vector<DPoint3d> vecTriP;
	DPoint3d p2, p3;

	vecTriP.push_back(symbolLocatePt);
	double xLen = triSideDis / 2;
	double ylen = sqrt(pow(triSideDis, 2) - pow(xLen, 2));

	p2.z = p3.z = symbolLocatePt.z;
	p2.x = symbolLocatePt.x - xLen;
	p2.y = symbolLocatePt.y + ylen;
	p3.x = symbolLocatePt.x + xLen;
	p3.y = symbolLocatePt.y + ylen;
	vecTriP.push_back(p2);
	vecTriP.push_back(p3);
	vecTriP.push_back(symbolLocatePt);
	return vecTriP;
}

//计算线条
vector<DPoint3d> CWaterLevelSymbolDrawing::calcLinePt
(DPoint3d symbolLocatePt, double lineDis, double intervalDis)
{
	vector<DPoint3d> vecLinePt;
	DPoint3d LinePt;
	//maxLine
	LinePt.z = symbolLocatePt.z;
	LinePt.x = symbolLocatePt.x - lineDis / 2;
	LinePt.y = symbolLocatePt.y - intervalDis;
	vecLinePt.push_back(LinePt);
	LinePt.x = symbolLocatePt.x + lineDis / 2;
	LinePt.y = symbolLocatePt.y - intervalDis;
	vecLinePt.push_back(LinePt);
	return vecLinePt;
}

//绘制symbol
bool CWaterLevelSymbolDrawing::CreateElement(EditElementHandleR eeh, vector<DPoint3d> points)
{
	bvector<DPoint3d> bPoints;
	for (auto& p : points)
	{
		bPoints.push_back(p);
	}
	DgnModelP model = ISessionMgr::GetActiveDgnModelP();
	MultilineStylePtr activeStyle = MultilineStyle::GetSettings(*(ISessionMgr::GetActiveDgnFile()));
	if (SUCCESS != LineStringHandler::CreateLineStringElement(eeh, nullptr, points.data(), points.size(), model->Is3d(), *model))
		return false;
	return true;
}


bool CWaterLevelSymbolDrawing::CreateText(EditElementHandleR eeh, DPoint3d point, WCharCP text)
{
	DgnFileP  pActiveDgnFile = mdlDgnFileObj_getMasterFile();
	DgnTextStylePtr m_textStyle = DgnTextStyle::GetSettings(*pActiveDgnFile);
	/*int ht;   //TextStyle_Height返回是double，不能用int接
	m_textStyle->GetProperty(TextStyle_Height, ht);*/
	double dt;
	m_textStyle->GetProperty(TextStyle_Height, dt);
	DgnTextStylePtr pTextStyle = DgnTextStyle::GetSettings(*(ISessionMgr::GetActiveDgnFile()));
	pTextStyle->SetProperty(TextStyle_Width, dt * 2);
	pTextStyle->SetProperty(TextStyle_Height, dt * 2);

	DgnModelP pActiveModel = ISessionMgr::GetActiveDgnModelP();
	TextBlockPtr pTextBlock = TextBlock::Create(*pTextStyle, *pActiveModel);
	pTextBlock->AppendText(text);
	point.y += TEXTHEIGHT * 0.75;
	pTextBlock->SetUserOrigin(point);

	if (TEXTBLOCK_TO_ELEMENT_RESULT_Success != TextHandlerBase::CreateElement(eeh, nullptr, *pTextBlock))
		return false;
	ElementPropertiesSetterPtr setter = ElementPropertiesSetter::Create();
	setter->SetColor(3); // 3 -- Red
	setter->Apply(eeh);

	return true;
}

void CWaterLevelSymbolDrawing::CalcTextPt(DPoint3d point, vector<double> vtDistance)
{
	m_sumTextPt.clear();
	double offsetX = MAXLINEDIS / 2; //默认情况：一个符号时，文本定位点x偏移MAXLINEDIS/2
	double offsetY = vtDistance[0];  //默认情况：一个符号时，定位点y没有起始偏移
	if (vtDistance.size() != 1)
	{
		//多个水位符号的情况
		offsetX = 0;
		double ylen = sqrt(pow(TRIANGLESIDEDIS, 2) - pow(TRIANGLESIDEDIS / 2, 2));
		offsetY = ylen * 3;
	}
	for (int i = 0; i < vtDistance.size(); i++)
	{
		vector<DPoint3d> vtTextPt;
		DPoint3d textPt;
		textPt.x = point.x + offsetX;
		textPt.y = point.y + offsetY + TEXTHEIGHT * i;
		textPt.z = point.z;
		vtTextPt.push_back(textPt);
		textPt.x = textPt.x + TEXTLINELENGTH;
		vtTextPt.push_back(textPt);
		m_sumTextPt.push_back(vtTextPt);
		m_sumSymbolPt.push_back(vtTextPt);
	}
}

void CWaterLevelSymbolDrawing::CalcVerticalLinePt(DPoint3d levelDatumPt, vector<double> vtDistance)
{
	//获取垂直线坐标点
	if (vtDistance.size() != 1)
	{
		double TriHeight = sqrt(pow(TRIANGLESIDEDIS, 2) - pow(TRIANGLESIDEDIS / 2, 2));
		double offsetY = TriHeight * 3;

		vector<DPoint3d> verticalLinePt;
		DPoint3d bottomPt = levelDatumPt;
		bottomPt.y += vtDistance[vtDistance.size() - 1] - INTERVALDIS * 2;
		verticalLinePt.push_back(bottomPt);

		DPoint3d topPt = levelDatumPt;
		topPt.y += vtDistance[0] + offsetY + TEXTHEIGHT * (vtDistance.size() + 1);
		verticalLinePt.push_back(topPt);
		m_sumSymbolPt.push_back(verticalLinePt);
	}
}
#pragma endregion
