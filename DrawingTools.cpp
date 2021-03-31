#include "DrawingTools.h"


void DrawingToolUtility::drawingLine(WCharCP unparsed)
{
	DrawingCreateTool::InstallNewInstance(unparsed);
}

void DrawingToolUtility::drawingMline(WCharCP unparsed)
{
	//DrawingCreateTool::InstallNewInstance(unparsed);
}

void createDimensionsLinear(WCharCP)
{

}

static MdlCommandNumber s_commandNumbers[] =
{
	{DrawingToolUtility::drawingLine,  CMD_DRAWING_LINE},
	{DrawingToolUtility::drawingMline, CMD_DRAWING_MLINE},
	{DrawingToolUtility::drawingMline, CMD_DRAWING_MLINE},
};


extern "C" void MdlMain(int argc, WCharCP argv[])
{
	RscFileHandle rfHandle;

	mdlResource_openFile(&rfHandle, NULL, RSC_READONLY);
	mdlState_registerStringIds(STRINGLISTID_Commands, STRINGLISTID_Prompts);
	mdlSystem_registerCommandNumbers(s_commandNumbers);
	mdlParse_loadCommandTable(NULL);
}
