#include "DrawingTools.h"

USING_NAMESPACE_BENTLEY_DGNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT;

#define ptStack statedata.dPointStack /* for readability */

enum ElementPropsOptions
{
	ElementPropsOptions_None,
	ElementPropsOptions_Custom,
	ElementPropsOptions_Active
};

struct ElementsCreate : DgnPrimitiveTool
{
public:
	static void InstallNewInstance();


protected:
	

};


void ElementsCreate::InstallNewInstance()
{
	// »æÖÆ
}

void drawingLine(WCharCP unparsed)
{
	ElementsCreate::InstallNewInstance();
}
void drawingMline(WCharCP unparsed)
{
	ElementsCreate::InstallNewInstance();
}

void    line_dataPt1
(
	DPoint3dCP  ptP,
	int         view
)
{
	ptStack[0] = *ptP;
	StateCallback::SetDataPointFunction(line_dataPt2);
	StateCallback::SetComplexDynamicsFunction(line_drawLine);
	//mdlState_setFunction (STATE_DATAPOINT, line_dataPt2);
	//mdlState_setFunction (STATE_COMPLEX_DYNAMICS, line_drawLine);
	mdlOutput_rscPrintf(MSG_PROMPT, NULL, MSGLIST_prompts,
		MSG_promptEnterPoint);
}

void    line_drawLine
(
	DPoint3dCP  ptP,
	int         view,
	DgnPlatform::DgnDrawMode        drawMode
)
{
	DgnPlatform::MSElement elem;
	Dpoint3d points[2];

	points[0] = ptStack[0];
	points[1] = *ptP;
	mdlLine_create(&elem, NULL, points);
	mdlElement_display(&elem, drawMode);

	if (drawMode == DgnPlatform::DRAW_MODE_Normal)
		mdlElement_add(&elem);
}

void line_dataPt2
(
	DPoint3dCP  ptP,
	int         view
)
{
	double      length;         /* length of line placed               */
	Dpoint3d    xAxisVec;       /* unit vector -- direction of line    */

	line_drawLine(ptP, view, DgnPlatform::DRAW_MODE_Normal);

	/* calculate length and unit vector */
	length = mdlVec_computeNormal(&xAxisVec, ptP, &ptStack[0]);

	mdlState_setAccudrawContext(
		DgnPlatform::ACCUDRAW_SetDistance |
		DgnPlatform::ACCUDRAW_SetXAxis,
		NULL,                   /* origin: last point OK  */
		NULL,                   /* delta (not used here)  */
		&length,                /* distance               */
		NULL,                   /* angle (not used here)  */
		&xAxisVec               /* x axis: last line      */
	);

	ptStack[0] = *ptP;
	StateCallback::SetComplexDynamicsFunction(line_drawLine);
	//mdlState_setFunction (STATE_COMPLEX_DYNAMICS, line_drawLine);
	mdlOutput_rscPrintf(MSG_PROMPT, NULL, MSGLIST_prompts,
		MSG_promptEnterPoint);
}

