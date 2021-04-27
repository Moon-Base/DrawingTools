#include "BoundingboxDrawing.h"
#define _CRT_SECURE_NO_WARNINGS

BoundingboxDrawing::BoundingboxDrawing()
{
}

BoundingboxDrawing::~BoundingboxDrawing()
{
}

void BoundingboxDrawing::InstallNewInstance()
{
	BoundingboxDrawing* tool = new BoundingboxDrawing();

	tool->InstallTool();
}

void BoundingboxDrawing::_OnRestartTool()
{
	InstallNewInstance();
}

bool BoundingboxDrawing::_OnDataButton(DgnButtonEventCR ev)
{
	__super::_OnDataButton(ev);	
	return true;
}

void BoundingboxDrawing::_SetupAndPromptForNextAction()
{
	__super::_SetupAndPromptForNextAction();
}

BENTLEY_NAMESPACE_NAME::StatusInt BoundingboxDrawing::_OnElementModify(EditElementHandleR eeh)
{
	return ERROR;
}

BENTLEY_NAMESPACE_NAME::StatusInt BoundingboxDrawing::_ProcessAgenda(DgnButtonEventCR ev)
{
	__super::_ProcessAgenda(ev);
	CalcLineBounding();
	return SUCCESS;
}

bool BoundingboxDrawing::_OnModifyComplete(DgnButtonEventCR ev)
{
	__super::_OnModifyComplete(ev);

	CreateBoundingLine();
	//CreateBounding3D();
	//CreateBounding();
	return true;
}

void BoundingboxDrawing::CalcLineBounding()
{
	auto& agenda = GetElementAgenda();
	int size = (int)agenda.size();
	if (size == 0)
		return;
	
	for (auto& eh : agenda)
	{
		DRange3d rangeCur;

		eh.GetDisplayHandler()->CalcElementRange(eh, rangeCur, nullptr);
		//status = SUCCESS;

		m_rangeSum.UnionOf(m_rangeSum, rangeCur);

		
		//针对多段线的包围盒计算绘制
		/*int type = eh.GetElementType();
		if (type == 3)
		{
			line_3d line3d = eh.GetElementCP()->line_3d;
			DPoint3d sp = line3d.start;
			DPoint3d ep = line3d.end;

			std::vector<DPoint3d> pts;
			pts.emplace_back(sp); pts.emplace_back(ep);
			for (DPoint3d& pt : pts)
			{
				m_minPoint.x > pt.x ? m_minPoint.x = pt.x : m_minPoint.x = m_minPoint.x;
				m_minPoint.y > pt.y ? m_minPoint.y = pt.y : m_minPoint.y = m_minPoint.y;
				m_minPoint.z > pt.z ? m_minPoint.z = pt.z : m_minPoint.z = m_minPoint.z;

				m_maxPoint.x < pt.x ? m_maxPoint.x = pt.x : m_maxPoint.x = m_maxPoint.x;
				m_maxPoint.y < pt.y ? m_maxPoint.y = pt.y : m_maxPoint.y = m_maxPoint.y;
				m_maxPoint.z < pt.z ? m_maxPoint.z = pt.z : m_maxPoint.z = m_maxPoint.z;
			}
		}*/
	}
}


void BoundingboxDrawing::CreateBoundingLine()
{
	EditElementHandle eeh;
	bvector<DPoint3d> points(5);
	if (m_rangeSum.IsNull())
		return;
	points[0] = Dpoint3d::From(m_rangeSum.low.x,  m_rangeSum.low.y,  0);
	points[1] = Dpoint3d::From(m_rangeSum.high.x, m_rangeSum.low.y,  0);
	points[2] = Dpoint3d::From(m_rangeSum.high.x, m_rangeSum.high.y, 0);
	points[3] = Dpoint3d::From(m_rangeSum.low.x,  m_rangeSum.high.y, 0);
	points[4] = Dpoint3d::From(m_rangeSum.low.x,  m_rangeSum.low.y,  0);
	/*if (points.size() < 2)
		return;*/

	DgnModelP model = ISessionMgr::GetActiveDgnModelP();

	MultilineStylePtr activeStyle = MultilineStyle::GetSettings(*(ISessionMgr::GetActiveDgnFile()));
	if (SUCCESS != LineStringHandler::CreateLineStringElement(eeh, nullptr, points.data(), points.size(), model->Is3d(), *model))
		return;
	eeh.AddToModel();
}

void BoundingboxDrawing::CreateBounding3D()
{
	EditElementHandle eeh;
	bvector<DPoint3d> points(16);
	if (m_rangeSum.IsNull())
		return;
	points[0] = Dpoint3d::From(m_rangeSum.low.x,  m_rangeSum.low.y,  m_rangeSum.low.z);
	points[1] = Dpoint3d::From(m_rangeSum.high.x, m_rangeSum.low.y,  m_rangeSum.low.z);
	points[2] = Dpoint3d::From(m_rangeSum.high.x, m_rangeSum.high.y, m_rangeSum.low.z);
	points[3] = Dpoint3d::From(m_rangeSum.low.x,  m_rangeSum.high.y, m_rangeSum.low.z);
	points[4] = Dpoint3d::From(m_rangeSum.low.x,  m_rangeSum.low.y,  m_rangeSum.low.z);

	points[5] = Dpoint3d::From(m_rangeSum.low.x, m_rangeSum.low.y, m_rangeSum.high.z);
	points[6] = Dpoint3d::From(m_rangeSum.high.x, m_rangeSum.low.y, m_rangeSum.high.z);
	points[7] = Dpoint3d::From(m_rangeSum.high.x, m_rangeSum.high.y, m_rangeSum.high.z);
	points[8] = Dpoint3d::From(m_rangeSum.low.x, m_rangeSum.high.y, m_rangeSum.high.z);
	points[9] = Dpoint3d::From(m_rangeSum.low.x, m_rangeSum.low.y, m_rangeSum.high.z);

	points[10] = Dpoint3d::From(m_rangeSum.high.x, m_rangeSum.low.y, m_rangeSum.high.z);
	points[11] = Dpoint3d::From(m_rangeSum.high.x, m_rangeSum.low.y, m_rangeSum.low.z);
	points[12] = Dpoint3d::From(m_rangeSum.high.x, m_rangeSum.high.y, m_rangeSum.low.z);
	points[13] = Dpoint3d::From(m_rangeSum.high.x, m_rangeSum.high.y, m_rangeSum.high.z);
	points[14] = Dpoint3d::From(m_rangeSum.low.x, m_rangeSum.high.y, m_rangeSum.high.z);
	points[15] = Dpoint3d::From(m_rangeSum.low.x, m_rangeSum.high.y, m_rangeSum.low.z);


	DgnModelP model = ISessionMgr::GetActiveDgnModelP();

	MultilineStylePtr activeStyle = MultilineStyle::GetSettings(*(ISessionMgr::GetActiveDgnFile()));
	if (SUCCESS != LineStringHandler::CreateLineStringElement(eeh, nullptr, points.data(), points.size(), model->Is3d(), *model))
		return;
	eeh.AddToModel();
}

void BoundingboxDrawing::CreateBoundingPoly(bvector<DPoint3d> points)
{
	EditElementHandle eeh;
	PolyfaceHeaderPtr polyface = PolyfaceHeader::CreateVariableSizeIndexed();
	polyface->AddPolygon(points);

	if (SUCCESS != MeshHeaderHandler::CreateMeshElement(eeh, NULL, *polyface, true, *ACTIVEMODEL))
		return;
	ElementPropertyUtils::ApplyActiveSettings(eeh);
	eeh.AddToModel();
}

void BoundingboxDrawing::CreateBounding()
{
	bvector<DPoint3d> pointsSum(8);
	pointsSum[0] = Dpoint3d::From(m_rangeSum.low.x, m_rangeSum.low.y, m_rangeSum.low.z);
	pointsSum[1] = Dpoint3d::From(m_rangeSum.high.x, m_rangeSum.low.y, m_rangeSum.low.z);
	pointsSum[2] = Dpoint3d::From(m_rangeSum.high.x, m_rangeSum.high.y, m_rangeSum.low.z);
	pointsSum[3] = Dpoint3d::From(m_rangeSum.low.x, m_rangeSum.high.y, m_rangeSum.low.z);

	pointsSum[4] = Dpoint3d::From(m_rangeSum.low.x, m_rangeSum.low.y, m_rangeSum.high.z);
	pointsSum[5] = Dpoint3d::From(m_rangeSum.high.x, m_rangeSum.low.y, m_rangeSum.high.z);
	pointsSum[6] = Dpoint3d::From(m_rangeSum.high.x, m_rangeSum.high.y, m_rangeSum.high.z);
	pointsSum[7] = Dpoint3d::From(m_rangeSum.low.x, m_rangeSum.high.y, m_rangeSum.high.z);
	if (m_rangeSum.IsNull())
		return;
	
	bvector<DPoint3d> pointsBottom(4);
	pointsBottom[0] = pointsSum[0];
	pointsBottom[1] = pointsSum[1];
	pointsBottom[2] = pointsSum[2];
	pointsBottom[3] = pointsSum[3];
	CreateBoundingPoly(pointsBottom);

	bvector<DPoint3d> pointsTop(4);
	pointsTop[0] = pointsSum[4];
	pointsTop[1] = pointsSum[5];
	pointsTop[2] = pointsSum[6];
	pointsTop[3] = pointsSum[7];
	CreateBoundingPoly(pointsTop);

	bvector<DPoint3d> pointsFront(4);
	pointsFront[0] = pointsSum[0];
	pointsFront[1] = pointsSum[1];
	pointsFront[2] = pointsSum[5];
	pointsFront[3] = pointsSum[4];
	CreateBoundingPoly(pointsFront);

	bvector<DPoint3d> pointsRear(4);
	pointsRear[0] = pointsSum[2];
	pointsRear[1] = pointsSum[3];
	pointsRear[2] = pointsSum[7];
	pointsRear[3] = pointsSum[6];
	CreateBoundingPoly(pointsRear);

	bvector<DPoint3d> pointsLeft(4);
	pointsLeft[0] = pointsSum[3];
	pointsLeft[1] = pointsSum[0];
	pointsLeft[2] = pointsSum[4];
	pointsLeft[3] = pointsSum[7];
	CreateBoundingPoly(pointsLeft);

	bvector<DPoint3d> pointsRight(4);
	pointsRight[0] = pointsSum[1];
	pointsRight[1] = pointsSum[2];
	pointsRight[2] = pointsSum[6];
	pointsRight[3] = pointsSum[5];
	CreateBoundingPoly(pointsRight);
}
