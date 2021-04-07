#include "BoundingboxDrawing.h"
#define _CRT_SECURE_NO_WARNINGS

BoundingboxDrawing::BoundingboxDrawing()
{
	m_minPoint.x = m_minPoint.y = m_minPoint.z = INT_MAX;
	m_maxPoint.x = m_maxPoint.y = m_maxPoint.z = INT_MIN;
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

	auto& agenda = GetElementAgenda();
	int size = (int)agenda.size();
	if (size < 0)
		return false;

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
		//todo 用这种方法
		DRange3d range;
		BentleyStatus status = eh.GetDisplayHandler()->CalcElementRange(eh, range, nullptr);
		bool status = false;
		status = eh.GetDisplayHandler()->GetBasisRange(eh, range);
		

		int type = eh.GetElementType();
		if (type == 3)
		{
			m_maxPoint.x = range.high.x;
			m_maxPoint.y = range.high.y;
			m_maxPoint.z = range.high.z;

			m_minPoint.x = range.low.x;
			m_minPoint.y = range.low.y;
			m_minPoint.z = range.low.z;

			/*line_3d line3d = eh.GetElementCP()->line_3d;
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
			}*/
		}
	}
}


void BoundingboxDrawing::CreateBoundingLine()
{
	EditElementHandle eeh;
	bvector<DPoint3d> points(5);

	points[0] = Dpoint3d::From(m_minPoint.x, m_minPoint.y, 0);
	points[1] = Dpoint3d::From(m_maxPoint.x, m_minPoint.y, 0);
	points[2] = Dpoint3d::From(m_maxPoint.x, m_maxPoint.y, 0);
	points[3] = Dpoint3d::From(m_minPoint.x, m_maxPoint.y, 0);
	points[4] = Dpoint3d::From(m_minPoint.x, m_minPoint.y, 0);
	if (points.size() < 2)
		return;

	DgnModelP model = ISessionMgr::GetActiveDgnModelP();

	MultilineStylePtr activeStyle = MultilineStyle::GetSettings(*(ISessionMgr::GetActiveDgnFile()));
	if (SUCCESS != LineStringHandler::CreateLineStringElement(eeh, nullptr, points.data(), points.size(), model->Is3d(), *model))
		return;
	eeh.AddToModel();
}

