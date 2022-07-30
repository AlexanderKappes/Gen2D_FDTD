#ifndef BOUNDCOND_HPP
#define BOUNDCOND_HPP
#include "Header.hpp"

class BoundCond
{
public:
    BoundCond();
    int      initDone = 0;
    double   coef0, coef1, coef2;
    double   *ezBottom;
    void abcInit(grid_fdtd *g);
    void abc(grid_fdtd *g);
    ~BoundCond();

};

#endif // BOUNDCOND_HPP
