#include "OCPNDrawCanvasMenuHandler.h"

OCPNDrawCanvasMenuHandler::OCPNDrawCanvasMenuHandler(ChartCanvas *parentCanvas,
                  Path *selectedPath,
                  OCPNPoint *selectedOCPNPoint) : CanvasMenuHandler( parentCanvas, (Route *) NULL, (Route *) NULL, (RoutePoint *) NULL, (int) 0, NULL )
{
    //ctor
    parent = parentCanvas;
    m_pSelectedPath = selectedPath;
    m_pFoundOCPNPoint = selectedOCPNPoint;
}

OCPNDrawCanvasMenuHandler::~OCPNDrawCanvasMenuHandler()
{
    //dtor
}
