#include "RectangleDrawing.h"




void RectangleDrawing::InstallNewInstance(int toolId, int toolPrompt)
{
	RectangleDrawing* tool = new RectangleDrawing(toolId, toolPrompt);
	tool->InstallTool();
}


bool RectangleDrawing::_OnDataButton(DgnButtonEventCR ev)
{
	if (0 == m_points.size())
		_BeginDynamics();
	
	m_points.emplace_back(*ev.GetPoint());
	//第二次点击时，绘制rectangle
	if (m_points.size() == 2)
	{
		m_points = calcPoint(m_points);
	}
	
	if (m_points.size() < 4)
		return false;

	EditElementHandle eeh;

	if (CreateRectangle(eeh, m_points))
	{
		eeh.AddToModel();
		m_points.clear();
	}
	
	return true;
}

void RectangleDrawing::_OnPostInstall()
{
	AccuSnap::GetInstance().EnableSnap(true); // Enable snapping for create tools.
	__super::_OnPostInstall();
}

void RectangleDrawing::_OnDynamicFrame(DgnButtonEventCR ev)
{
	m_dynamicPoints = m_points;
	ElementAgenda agenda;
	//添加矩形的第二个点（动态点）
	m_dynamicPoints.emplace_back(*ev.GetPoint());

	m_dynamicPoints = calcPoint(m_dynamicPoints);

	EditElementHandle eeh;
	if (!CreateRectangle(eeh, m_dynamicPoints))
		return;

	RedrawElems redrawElems;

	redrawElems.SetDynamicsViews(IViewManager::GetActiveViewSet(), ev.GetViewport()); // Display in all views, draws to cursor view first...
	redrawElems.SetDrawMode(DRAW_MODE_TempDraw);
	redrawElems.SetDrawPurpose(DrawPurpose::Dynamics);
	redrawElems.DoRedraw(eeh);
}

bool RectangleDrawing::CreateRectangle(EditElementHandleR eeh, vector<DPoint3d> points)
{
	if (4 != points.size())
		return false;
	bvector<DPoint3d> bPoints;
	for (auto& p : points)
	{
		bPoints.push_back(p);
	}
	PolyfaceHeaderPtr polyface = PolyfaceHeader::CreateVariableSizeIndexed();
	polyface->AddPolygon(bPoints);
	if (SUCCESS != MeshHeaderHandler::CreateMeshElement(eeh, NULL, *polyface, true, *ACTIVEMODEL))
		return false;
	ElementPropertyUtils::ApplyActiveSettings(eeh);

	return true;
}

vector<DPoint3d> RectangleDrawing::calcPoint(vector<DPoint3d> points)
{
	//p1第一次点击的点，p2鼠标移动捕捉点
	vector<DPoint3d> rectanglePoint;
	double width = 100000;
	DPoint3d p1 = points[0];
	DPoint3d p2 = points[1];
	rectanglePoint = points;
	DPoint3d p3;
	DPoint3d p4;
	p1.Distance(p2);
	//获取动态线（矩形边）与x轴形成的三角形
	double xLenP12 = abs(p1.x - p2.x);
	double yLenp12 = abs(p1.y - p2.y);
	double cLenP12 = sqrt(pow(xLenP12, 2) + pow(yLenp12, 2));
	//动态边长与x轴的夹角
	double coos = xLenP12 / cLenP12;
	double siin = yLenp12 / cLenP12;
	//point3相对与其他一点的x，y绝对值
	double xLenP23 = width * siin;
	double yLenP23 = width * coos;

 	if (p1.x<p2.x && p1.y<p2.y || p1.x>p2.x && p1.y>p2.y)
	{
		p3.y = p2.y + yLenP23;
		p3.x = p2.x - xLenP23;
		p4.y = p1.y + yLenP23;
		p4.x = p1.x - xLenP23;
		rectanglePoint.emplace_back(p3);
		rectanglePoint.emplace_back(p4);
	}
	else if (p1.x<p2.x && p1.y>p2.y || p1.x>p2.x && p1.y<p2.y)
	{
		p3.y = p2.y + yLenP23;
		p3.x = p2.x + xLenP23;
		p4.y = p1.y + yLenP23;
		p4.x = p1.x + xLenP23;
		rectanglePoint.emplace_back(p3);
		rectanglePoint.emplace_back(p4);
	}
	else if (p1.y == p2.y)
	{
		p3.x = p2.x;
		p3.y = p2.y + width;
		p4.x = p1.x;
		p4.y = p1.y + width;
		rectanglePoint.emplace_back(p3);
		rectanglePoint.emplace_back(p4);
	}
	else if (p1.x == p2.x)
	{
		p3.x = p1.x + width;
		p3.y = p2.y;
		p4.x = p1.x + width;
		p4.y = p1.y;
		rectanglePoint.emplace_back(p3);
		rectanglePoint.emplace_back(p4);
	}
	return rectanglePoint;
}


bool RectangleDrawing::CreateCubiod(EditElementHandleR eeh, vector<DPoint3d> bottomPoints, vector<DPoint3d> topPoints)
{
	bvector<DPoint3d> bPoints;
	for (auto& b : bottomPoints)
	{
		bPoints.push_back(b);
	}
	PolyfaceHeaderPtr bPolyface = PolyfaceHeader::CreateVariableSizeIndexed();
	bPolyface->AddPolygon(bPoints);
	if (SUCCESS != MeshHeaderHandler::CreateMeshElement(eeh, NULL, *bPolyface, true, *ACTIVEMODEL))
		return false;
	ElementPropertyUtils::ApplyActiveSettings(eeh);

	bvector<DPoint3d> tPoints;
	for (auto& t : topPoints)
	{
		tPoints.push_back(t);
	}
	PolyfaceHeaderPtr tPolyface = PolyfaceHeader::CreateVariableSizeIndexed();
	tPolyface->AddPolygon(tPoints);
	if (SUCCESS != MeshHeaderHandler::CreateMeshElement(eeh, NULL, *tPolyface, true, *ACTIVEMODEL))
		return false;
	ElementPropertyUtils::ApplyActiveSettings(eeh);

	return true;
}

void RectangleDrawing::calcCubiodPoint(vector<DPoint3d> points)
{

}