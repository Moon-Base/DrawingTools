#include "DrawingTools.h"

USING_NAMESPACE_BENTLEY_DGNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM;
USING_NAMESPACE_BENTLEY_MSTNPLATFORM_ELEMENT;


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
