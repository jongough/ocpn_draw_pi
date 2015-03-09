#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <Path.h>


class Boundary : public Path
{
    public:
        Boundary();
        virtual ~Boundary();

    int m_itest;
    
    protected:
    private:
};

WX_DECLARE_LIST(Boundary, BoundaryList); // establish class Path as list member

#endif // BOUNDARY_H
