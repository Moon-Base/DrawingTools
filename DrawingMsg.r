#include <Mstn\MdlApi\rscdefs.r.h>
#include <Mstn\MdlApi\cmdclass.r.h>
#include "DrawingToolsIds.h"

MessageList STRINGLISTID_Commands =
{
    {
    {CMDNAME_Line,           "Line"                      },
    {CMDNAME_Boundingbox,    "Boundingbox"               },
    }
};

MessageList STRINGLISTID_Prompts =
{
    {
    {PROMPT_EnterPoint,      "Drawing Line"           },
    {PROMPT_AcceptReject,    "Drawing Mline"      },
    }
};
