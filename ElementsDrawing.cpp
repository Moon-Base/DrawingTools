#include "ElementsDrawing.h"


#define ptStack statedata.dPointStack

enum ElementPropsOptions
{
	ElementPropsOptions_None,
	ElementPropsOptions_Custom,
	ElementPropsOptions_Active
};


void DimensionsCreateTool::InstallNewInstance(WCharCP unparsed)
{
	mdlState_startPrimitive(line_dataPt1, InstallNewInstance, CMDNAME_Line, PROMPT_EnterPoint);
}

void DimensionsCreateTool::line_dataPt1(DPoint3dCP ptP, int view)
{
	ptStack[0] = *ptP;
	StateCallback::SetDataPointFunction(line_dataPt2);
	StateCallback::SetComplexDynamicsFunction(line_drawLine);
	mdlOutput_rscPrintf(MSG_PROMPT, NULL, STRINGLISTID_Prompts, PROMPT_EnterPoint);

}

void DimensionsCreateTool::line_dataPt2(DPoint3dCP ptP, int view)
{
	double length;
	DPoint3d xAxisVec;
	line_drawLine(ptP, view, DgnPlatform::DRAW_MODE_Normal);
	length = mdlVec_computeNormal(&xAxisVec, ptP, &ptStack[0]);

	mdlState_setAccudrawContext(
		DgnPlatform::ACCUDRAW_SetDistance |
		DgnPlatform::ACCUDRAW_SetXAxis,
		NULL,
		NULL,
		&length,
		NULL,
		&xAxisVec
	);
	ptStack[0] = *ptP;
	StateCallback::SetComplexDynamicsFunction(line_drawLine);
	mdlOutput_rscPrintf(MSG_PROMPT, NULL, STRINGLISTID_Prompts, PROMPT_EnterPoint);

}

void DimensionsCreateTool::line_drawLine(DPoint3dCP ptP, int view, DgnPlatform::DgnDrawMode drawMode)
{
	DgnPlatform::MSElement elem;
	DPoint3d points[2];

	points[0] = ptStack[0];
	points[1] = *ptP;
	mdlLine_create(&elem, NULL, points);
	mdlElement_display(&elem, drawMode);
	if (drawMode == DgnPlatform::DRAW_MODE_Normal)
	{
		mdlElement_add(&elem);
	}
}