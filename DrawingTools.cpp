#include "DrawingTools.h"


static MdlCommandNumber s_commandNumbers[] =
{
	{drawingLine,  CMD_DRAWING_LINE},
	{drawingMline, CMD_DRAWING_MLINE},
};


extern "C" void MdlMain(int argc, WCharCP argv[])
{
	RscFileHandle rfHandle;

	mdlResource_openFile(&rfHandle, NULL, RSC_READONLY);
	mdlState_registerStringIds(STRINGLISTID_Commands, STRINGLISTID_Prompts);
	mdlSystem_registerCommandNumbers(s_commandNumbers);
	mdlParse_loadCommandTable(NULL);
}
