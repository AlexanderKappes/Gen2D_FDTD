#include "Header.hpp"

BoundCond::BoundCond()
{
    initDone = 0;
}

void BoundCond::abcInit(grid_fdtd *g) {
    /*
    double temp1, temp2;

    initDone = 1;// ЭТО ЗНАЧЕНИЕ НИГДЕ НЕ ИСПОЛЬЗУЕТСЯ


    ALLOC_1D(ezBottom,  g->sizeX * 6, double);
    double Cezh_0 = g->cezh [(0) * g->sizeY + (0)];
    double Chye_0 = g->chye [(0) * g->sizeY + (0)];

    temp1 = sqrt(Cezh_0 * Chye_0);
    temp2 = 1.0 / temp1 + 2.0 + temp1;
    coef0 = -(1.0 / temp1 - 2.0 + temp1) / temp2;
    coef1 = -2.0 * (temp1 - 1.0 / temp1) / temp2;
    coef2 = 4.0 * (temp1 + 1.0 / temp1) / temp2;
    */
    return;
}

void BoundCond::abc(grid_fdtd *g)
{
    /*
    int mm, nn;

        double EzBottom_0_0, EzBottom_0_1, EzBottom_2_0, EzBottom_1_1, EzBottom_1_0, EzBottom_2_1;
        double Ez_mm_1, Ez_mm_2;

        for (mm = 0; mm < g->sizeX; mm++) {
            //EzBottom(N, Q, M)   ezBottom[(N) + (Q) * 3 + (M) * 6]
            EzBottom_0_1      = ezBottom    [(0) + (1) * 3 + (mm) * 6];
            EzBottom_0_0      = ezBottom    [(0) + (0) * 3 + (mm) * 6];
            EzBottom_2_0      = ezBottom    [(2) + (0) * 3 + (mm) * 6];
            EzBottom_1_1      = ezBottom    [(1) + (1) * 3 + (mm) * 6];
            EzBottom_1_0      = ezBottom    [(1) + (0) * 3 + (mm) * 6];
            EzBottom_2_1      = ezBottom    [(2) + (1) * 3 + (mm) * 6];
            Ez_mm_1           = g->ez   [(mm) * g->sizeY + (1)];
            Ez_mm_2           = g->ez   [(mm) * g->sizeY + (2)];
            g->ez   [(mm) * g->sizeY + (0)] = coef0 * (Ez_mm_2 + EzBottom_0_1) + coef1 * (EzBottom_0_0 + EzBottom_2_0 - Ez_mm_1 - EzBottom_1_1) + coef2 * EzBottom_1_0 - EzBottom_2_1;

        for (nn = 0; nn < 3; nn++) {
                ezBottom    [(nn) + (1) * 3 + (mm) * 6] = ezBottom    [(nn) + (0) * 3 + (mm) * 6];
                ezBottom    [(nn) + (0) * 3 + (mm) * 6] = g->ez   [(mm) * g->sizeY + (nn)];
            }
        }
    */
    return;
}

BoundCond::~BoundCond()
{

}
