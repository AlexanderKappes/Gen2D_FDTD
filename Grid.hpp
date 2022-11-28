#ifndef GRID_HPP
#define GRID_HPP

struct geometry_point{double abs; double arg;};
struct field_polar2d {double ray; double lv; };

//Wedge - Клин, Slot - Паз, Pin - Зубец, Arm - Один слой Паза
struct grid_parameters
{
    //------------------
    int n_slot;
    int n_ray_pin;
    int n_ray_slot;
    //lv - уровень (расстояние от центра), ray - угол
    field_polar2d u_pin;
    field_polar2d u_slot;
    //------------------
    int n_level;//Число слоев сетки. СТАТОР: 6, РОТОР: 6
    double* dh_level;

    int n_level_wedge;//Число слоев сетки на клин. СТАТОР: 1, РОТОР: 1
    int n_level_arm;//Число слоев сетки на один слой паза. СТАТОР: 1, РОТОР: 1
    int n_level_body;//Число слоев на тело. СТАТОР: 3, РОТОР: 2
    int n_level_ext;//Число слоев на тело. СТАТОР: 3, РОТОР: 2
    int num_arm_layer; //СТАТОР: Число слоев паза. СТАТОР: 2, РОТОР: 3
    int n_level_air;//Число слоев на тело. СТАТОР: 3, РОТОР: 2

    double R_start; //СТАТОР: Внутренний диаметр стстора
    double R_stop;//СТАТОР: На 80% обмотки статора

    int Nmax;

    int Np_s;//Число точек по ширине паза, зубца
    int Np_p;

    int M_w;//Число точек по высоте клина, обмотки, тела ротора
    int M_a;
    int M_b;
    int M_air;
    int M_ext;

    int Row;
    int Col;

    int Num_slot;
    int Num_pin;

    int Sec_num;
    int Num;
};

enum GRIDTYPE {oneDGrid, teZGrid, tmZGrid, threeDGrid};

struct grid_fdtd {
    double *hx, *hy, *ez;

    double *chxh, *chxe;
    double *chyh, *chye;
    double *ceze, *cezhx, *cezhy;

    int     type;
    double  cdtds;
};

#endif // GRID_HPP
