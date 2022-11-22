#ifndef ComGrid_H
#define ComGrid_H
#include "Header.hpp"

struct grid_data{
    double x;
    double y;
    int sector;
    int point_num;
    int EH; // EH = 1 - Ez
            // EH = 2 - Hx
            // EH = 3 - Hy
    int material;
            //Ротор
            //1 - клин, 2 - обмотка, 3 - сталь
            //Статор
            //1 - воздух, 2 - клин, 3 - обмотка, 4 - сталь поперек
            //5 - сталь вдоль
    bool source;
    double EHnumX;
    double EHnumY;

};

class GenGrid2D
{

    public:
        GenGrid2D();
        ~GenGrid2D();

        QVector<grid_data> rot_grid_pos;
        QVector<grid_data> stat_grid_pos;

        grid_parameters   stator_grid_par;
        grid_parameters   rotor_grid_par;
        //Wedge - Клин, Slot - Паз, Pin - Зубец, Arm - Один слой Паза
        double dh_rot_wedge;//длина отрезка сетки на клин Ротора
        double dh_rot_arm;//длина отрезка сетки на один слой паза Ротора
        double dh_rot_body;//расстояния от дна паза Ротора до окружности rot_R_stop / на число отрезков сетки

        double dh_stat_wedge;//длина отрезка сетки на клин Статора
        double dh_stat_arm;//длина отрезка сетки на один слой паза Статора
        double dh_stat_body;//расстояния от дна паза Статора до окружности stat_R_stop / на число отрезков сетки
        double dh_stat_air;

        int grid_mark_size;

        QVector<int> rot_mater_pin;
        QVector<int> rot_mater_slot;
        QVector<int> stat_mater_pin;
        QVector<int> stat_mater_slot;
        QVector<double> stat_dy;   QVector<double> stat_dx;
        QVector<double> rot_dy;    QVector<double> rot_dx;

        void Gen_Grid_Calc             (GenGeom2D *G);
        void Gen_Grid_Pos_rot          (GenGeom2D *G, double arg_beg);
        void Gen_Grid_Pos_stat         (GenGeom2D *G);
        void Gen_Grid_Vector_init      (GenGeom2D *G);
};

#endif // ComGrid_H
