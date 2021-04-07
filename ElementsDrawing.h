#pragma once

#include <Mstn/MdlApi/MdlApi.h>
#include <DgnView/DgnElementSetTool.h>
#include <DgnView/AccuDraw.h>

#include "DrawingToolsIds.h"


USING_NAMESPACE_BENTLEY_DGNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT;

struct DimensionsCreateTool : DgnElementSetTool
{
private:
	int                             m_iView;


	static void line_dataPt1(DPoint3dCP ptP, int view);
	static void line_dataPt2(DPoint3dCP ptP, int view);
	static void line_drawLine(DPoint3dCP ptP, int view, DgnPlatform::DgnDrawMode drawMode);
public:
	static void InstallNewInstance(WCharCP unparsed);
};


//struct DrawingCreateTool : DgnElementSetTool
//{
//private:
//	static void line_dataPt1(DPoint3dCP ptP, int view);
//	static void line_dataPt2(DPoint3dCP ptP, int view);
//	static void line_drawLine(DPoint3dCP ptP, int view, DgnPlatform::DgnDrawMode drawMode);
//public:
//	static void InstallNewInstance(WCharCP unparsed);
//};