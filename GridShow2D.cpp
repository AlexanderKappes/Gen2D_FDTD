#include "Header.hpp"

GridShow2D::GridShow2D()
{ 
    air_u_ray = 1.0;
    air_u_lv  = 1.0;

    stator_grid_param.n_ray_pin         = 1;
    stator_grid_param.n_ray_slot        = 1;
    stator_grid_param.n_level_arm       = 1;
    stator_grid_param.n_level_wedge     = 1;
    stator_grid_param.n_level_body      = 3;

    stator_grid_param.u_pin.ray         = 40000.0;
    stator_grid_param.u_pin.lv          =  4000.0;
    stator_grid_param.u_slot.ray        = air_u_ray;
    stator_grid_param.u_slot.lv         = air_u_lv;

    rotor_grid_param.n_ray_pin          = 1;
    rotor_grid_param.n_ray_slot         = 1;
    rotor_grid_param.n_level_arm        = 1;
    rotor_grid_param.n_level_wedge      = 1;
    rotor_grid_param.n_level_body       = 3;

    rotor_grid_param.u_pin.ray          = 40000.0;
    rotor_grid_param.u_pin.lv           =  4000.0;
    rotor_grid_param.u_slot.ray         = air_u_ray;
    rotor_grid_param.u_slot.lv          = air_u_lv;
}

void GridShow2D ::Gen_Grid_Calc(double G_stat_par_n_slot_pin, double G_stat_par_n_layer,
                                double G_rot_par_n_slot_pin, double G_rot_par_n_layer)
{
    stator_grid_param.n_slot            = G_stat_par_n_slot_pin;
    stator_grid_param.num_arm_layer     = G_stat_par_n_layer;
    rotor_grid_param.n_slot             = G_rot_par_n_slot_pin;
    rotor_grid_param.num_arm_layer      = G_rot_par_n_layer;
}

void GridShow2D ::init_rotor (){  }
void GridShow2D ::init_stator(){  }

GridShow2D::~GridShow2D()
{

}
