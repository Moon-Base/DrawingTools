#include "DrawingTools.h"



void ToolUtility::DrawingLine(WCharCP unparsed)
{
	DimensionsCreateTool::InstallNewInstance(unparsed);
}

void ToolUtility::Boundingbox(WCharCP unparsed)
{
	BoundingboxDrawing::InstallNewInstance();
}

void ToolUtility::RectangleDynamicDrawing(WCharCP unparsed)
{
	RectangleDrawing::InstallNewInstance(CMDNAME_RectangleDynamic, PROMPT_DrawRectangle);
}

static MdlCommandNumber s_commandNumbers[] =
{
	{ToolUtility::DrawingLine,				CMD_DRAWING_LINE},
	{ToolUtility::Boundingbox,				CMD_DRAWING_BOUNDINGBOX},
	{ToolUtility::RectangleDynamicDrawing,  CMD_DRAWING_RECTANGLEDYNAMIC},
};


extern "C" void MdlMain(int argc, WCharCP argv[])
{
	RscFileHandle rfHandle;

	mdlResource_openFile(&rfHandle, NULL, RSC_READONLY);
	mdlState_registerStringIds(STRINGLISTID_Commands, STRINGLISTID_Prompts);
	mdlSystem_registerCommandNumbers(s_commandNumbers);
	mdlParse_loadCommandTable(NULL);
}

