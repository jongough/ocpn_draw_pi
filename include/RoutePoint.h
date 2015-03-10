#ifndef ROUTEPOINT_H
#define ROUTEPOINT_H

#include <OCPNPoint.h>


class RoutePoint : public OCPNPoint
{
    public:
        RoutePoint();
        virtual ~RoutePoint();
    protected:
    private:
};

WX_DECLARE_LIST(RoutePoint, RoutePointList);// establish class as list member


#endif // ROUTEPOINT_H
