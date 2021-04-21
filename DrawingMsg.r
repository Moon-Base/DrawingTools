#include <Mstn\MdlApi\rscdefs.r.h>
#include <Mstn\MdlApi\cmdclass.r.h>
#include "DrawingToolsIds.h"

MessageList STRINGLISTID_Commands =
{
    {
    {CMDNAME_Line,				"Line"                      },
    {CMDNAME_Boundingbox,		"Boundingbox"               },
    {CMDNAME_RectangleDynamic,  "RectangleDynamic"          },
    {CMDNAME_PlaceCompass,		"Compass"					},
    {CMDNAME_WaterLevelSymbol,  "WaterLevelSymbol"          },
    }
};

MessageList STRINGLISTID_Prompts =
{
    {
    {PROMPT_Drawline,			"Drawing Line"              },
    {PROMPT_DrawMline,			"Drawing Mline"             },
    {PROMPT_DrawRectangle,      "Drawing Rectangle"         },
    {PROMPT_PlaceCompass,		"Place Compass"		        },
    {PROMPT_WaterLevelSymbol,	"Drawing WaterLevelSymbol"  },
    }
};
