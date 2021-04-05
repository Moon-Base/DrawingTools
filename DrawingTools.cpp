#include "DrawingTools.h"



void ToolUtility::drawingLine(WCharCP unparsed)
{
	DimensionsCreateTool::InstallNewInstance(unparsed);
}

void ToolUtility::boundingbox(WCharCP unparsed)
{
	BoundingboxTool::InstallNewInstance(CMDNAME_Boundingbox);
}


static MdlCommandNumber s_commandNumbers[] =
{
	{ToolUtility::drawingLine,  CMD_DRAWING_LINE},
	{ToolUtility::boundingbox, CMD_DRAWING_BOUNDINGBOX},
};


extern "C" void MdlMain(int argc, WCharCP argv[])
{
	RscFileHandle rfHandle;

	mdlResource_openFile(&rfHandle, NULL, RSC_READONLY);
	mdlState_registerStringIds(STRINGLISTID_Commands, STRINGLISTID_Prompts);
	mdlSystem_registerCommandNumbers(s_commandNumbers);
	mdlParse_loadCommandTable(NULL);
}

