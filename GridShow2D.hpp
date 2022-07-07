#ifndef GENGRID2D_H
#define GENGRID2D_H

#include "Header.hpp"

class GridShow2D
{
    //variables
    public:
    grid_parameters  rotor_grid_param;
    grid_parameters  stator_grid_param;

    double air_u_ray;
    double air_u_lv;

    public:
         GridShow2D();
        ~GridShow2D();

         void init_rotor ();
         void init_stator();

    void Gen_Grid_Calc(double G_stat_par_n_slot_pin, double G_stat_par_n_layer,
                       double G_rot_par_n_slot_pin, double G_rot_par_n_layer);

};

#endif // GENGRID2D_H
