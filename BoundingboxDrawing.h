#pragma once

#include    <Mstn/MdlApi/MdlApi.h>
//#include    <Mstn/ISessionMgr.h>
//#include    <Mstn/ElementPropertyUtils.h>
#include    <DgnView/DgnElementSetTool.h>
//#include    <DgnPlatform/DimensionHandler.h>
//#include    <DgnPlatform/ISettings.h>

//#include    "DrawingToolsIds.h"

USING_NAMESPACE_BENTLEY_DGNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT


struct BoundingboxTool : DgnElementSetTool
{
private:
	int                             m_iView;
	HitPathCP                       m_hitPath;
	bvector<DPoint3d>               m_dimPoints;
	int                             m_alignment;
	
	BoundingboxTool(int toolName, int alignment) : DgnElementSetTool(toolName), m_alignment(alignment) { }

	virtual     bool                _OnDataButton(DgnButtonEventCR ev) override;
	virtual     void                _OnRestartTool() override;
	virtual     UsesDragSelect      _AllowDragSelect() override { return USES_DRAGSELECT_Box; };
	StatusInt                       _OnElementModify(EditElementHandleR eeh) override { return SUCCESS; }
public:
	static      void                InstallNewInstance(int toolId, int alignment);
	
};