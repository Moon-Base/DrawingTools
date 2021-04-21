#pragma once

#include "DrawingToolsIds.h"
#include "DrawingToolsCmd.h"
#include "ElementsDrawing.h"
#include "BoundingboxDrawing.h"
#include "RectangleDrawing.h"
#include "Compass.h"
#include "WaterLevelSymbolDrawing.h"

struct ToolUtility
{
	static void DrawingLine(WCharCP);
	static void Boundingbox(WCharCP);
	static void RectangleDynamicDrawing(WCharCP);
	static void CompassPlace(WCharCP);
	static void WaterLevelSymbol(WCharCP);
};


