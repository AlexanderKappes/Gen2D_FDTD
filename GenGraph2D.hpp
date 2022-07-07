#ifndef GENGRAPH2D_H
#define GENGRAPH2D_H
#include "Header.hpp"

struct coil_data
{
    int  sector;
    char phase;
    int  coil;

    int  turn;
    int  slot;
    int  layer;
};


class GenGraph2D
{
public:

    QVector<QCPCurveData> QV_rotor_grid_Ez;    QCPCurve  *rotor_grid_curve_Ez;
    QVector<QCPCurveData> QV_rotor_grid_Hx;    QCPCurve  *rotor_grid_curve_Hx;
    QVector<QCPCurveData> QV_rotor_grid_Hy;    QCPCurve  *rotor_grid_curve_Hy;

    QVector<QCPCurveData> QV_stator_grid_Ez;   QCPCurve  *stator_grid_curve_Ez;
    QVector<QCPCurveData> QV_stator_grid_Hx;   QCPCurve  *stator_grid_curve_Hx;
    QVector<QCPCurveData> QV_stator_grid_Hy;   QCPCurve  *stator_grid_curve_Hy;

    QVector<QCPCurveData> rot_steel;        QCPCurve  *rotor_steel_curve;
    QVector<QCPCurveData> rotor_R;          QCPCurve  *rotor_R_curve;
    QVector<QCPCurveData> rotor_w;          QCPCurve  *rotor_w_curve;
    QVector<QCPCurveData> rotor_L1;         QCPCurve  *rotor_L1_curve;
    QVector<QCPCurveData> rotor_L2;         QCPCurve  *rotor_L2_curve;
    QVector<QCPCurveData> rotor_L3;         QCPCurve  *rotor_L3_curve;
    QVector<QCPCurveData> rotor_L4;         QCPCurve  *rotor_L4_curve;
    QVector<QCPCurveData> rotor_L5;         QCPCurve  *rotor_L5_curve;
    QVector<QCPCurveData> rotor_L6;         QCPCurve  *rotor_L6_curve;
    QVector<QCPCurveData> rotor_L7;         QCPCurve  *rotor_L7_curve;
    QVector<QCPCurveData> rotor_L8;         QCPCurve  *rotor_L8_curve;
    QVector<QCPCurveData> rotor_L9;         QCPCurve  *rotor_L9_curve;

    QVector<QCPCurveData> stator_steel;     QCPCurve  *stator_steel_curve;
    QVector<QCPCurveData> gener_total;      QCPCurve  *gener_total_curve;

    QVector<QCPCurveData> stator_R;         QCPCurve  *stator_R_curve;
    QVector<QCPCurveData> stator_w;         QCPCurve  *stator_w_curve;
    QVector<QCPCurveData> stator_L1;        QCPCurve  *stator_L1_curve;
    QVector<QCPCurveData> stator_L2;        QCPCurve  *stator_L2_curve;

    QVector<QCPCurveData> stator_coil_A1;   QCPCurve  *stator_coil_A1_curve;
    QVector<QCPCurveData> stator_coil_A2;   QCPCurve  *stator_coil_A2_curve;

    QVector<QCPCurveData> stator_coil_B1;   QCPCurve  *stator_coil_B1_curve;
    QVector<QCPCurveData> stator_coil_B2;   QCPCurve  *stator_coil_B2_curve;

    QVector<QCPCurveData> stator_coil_C1;   QCPCurve  *stator_coil_C1_curve;
    QVector<QCPCurveData> stator_coil_C2;   QCPCurve  *stator_coil_C2_curve;
    QVector<QCPCurveData> rotor_coil;       QCPCurve  *rotor_coil_curve;

    QCPColorMap *colorMap;

    QVector<coil_data> stat_coil_data;
    QVector<coil_data> rot_coil_data;

    GenGraph2D();
    ~GenGraph2D();

    void GenGraphCurveSize              (GenGeom2D *G);

    void rotor_steel_curve_Create       (GenGeom2D *G,      double arg_beg);
    void rotor_coil_curve_Create        (GenGeom2D *G,      GenGrid2D *GenGr, double arg_beg);
    void GridPoint_rotor                (GenGrid2D *GenGr);

    void stator_coil_curve_Create       (GenGeom2D *G);
    void stator_steel_curve_Create      (GenGeom2D *G);
    void GridPoint_stator               (GenGrid2D *GenGr);

    void GraphCircleEnd(GenGeom2D *G);
    int CircleBuff(int Val, int maxCircle_num);

    void SlotRotPos (GenGeom2D *G);
};


#endif // GENGRAPH2D_H
