#pragma once

#include "DrawingTools.h"

USING_NAMESPACE_BENTLEY_DGNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT;

struct DrawingCreateTool
{
private:
	static void line_dataPt1(DPoint3dCP ptP, int view);
	static void line_dataPt2(DPoint3dCP ptP, int view);
	static void line_drawLine(DPoint3dCP ptP, int view, DgnPlatform::DgnDrawMode drawMode);
public:
	static void InstallNewInstance(WCharCP unparsed);
};