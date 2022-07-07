#include "Header.hpp"

GenGeom2D::GenGeom2D()
{
// /*
    //ТВВ-320
    rot_par.n_pole      = 2;
    rot_par.n_coil_slot =36;
    rot_par.n_slot_pin  =52;
    rot_par.n_layer     =7;

    rot_par.R           =1.075/2;
    rot_par.h_wedge     =0.034;
    rot_par.b_slot      =0.0325;
    rot_par.h_slot      =0.1145;

    L                   =4.45;
    air_gap             = 0.11;

    stat_par.n_phase    =3;
    stat_par.n_slot_pin =60;
    stat_par.n_layer    =2;
    stat_par.shift_coil =5;
    stat_par.R_out      =2.735/2;
    stat_par.h_wedge    =0.025;
    stat_par.b_slot     =0.031;
    stat_par.h_slot     =0.195;
    //stat_par.num_pole_stator_slots = 25;
// */
/*
    //ТГВ-200
    rot_par.n_pole = 2;
    rot_par.n_coil_slot =36;
    rot_par.n_slot_pin  =52;
    rot_par.n_layer     =3;
    rot_par.L           =4.55;
    rot_par.R           =1.075/2;
    rot_par.h_wedge     =0.03;
    rot_par.b_slot      =0.03;
    rot_par.h_slot      =0.11;

    air_gap             = 0.1;

    stat_par.n_phase    =3;
    stat_par.n_slot_pin =60;
    stat_par.n_layer    =2;
    stat_par.shift_coil =2;
    stat_par.R_out      =2.515/2;
    stat_par.h_wedge    =0.03;
    stat_par.b_slot     =0.04;
    stat_par.h_slot     =0.22;
// */

}

void GenGeom2D::init_rotor ()
{
    rot_par.n_coil_slot_pole   =rot_par.n_coil_slot/rot_par.n_pole;
    rot_par.n_slot_pin_pole    =rot_par.n_slot_pin /rot_par.n_pole;
    rot_par.n_turn_pole        =rot_par.n_coil_slot/rot_par.n_pole*rot_par.n_layer;

    rot_par.h_arm      =rot_par.h_slot/rot_par.n_layer;  //armature size
    rot_par.wedge_pole_num = (rot_par.n_slot_pin - rot_par.n_coil_slot)/2;
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //calc slot and pin angles of rotor
    rot_par.slot_pin_angle=2*3.1415926/rot_par.n_slot_pin;//rotor slot&pin sum angle
    rot_par.sum_all_slot_b =  rot_par.b_slot*rot_par.n_slot_pin;
    rot_par.pi_D = 2*rot_par.R*3.1415926;//air gap rotor circle length

    rot_par.slot_part = rot_par.sum_all_slot_b/rot_par.pi_D;

    rot_par.pin_angle     = (1-rot_par.slot_part)*rot_par.slot_pin_angle;
    rot_par.slot_angle    =    rot_par.slot_part *rot_par.slot_pin_angle;

    rot_par.slot_arg   =rot_par.slot_angle;
    rot_par.pin_arg    =rot_par.pin_angle;   //pin  angle
}

void GenGeom2D::init_stator()
{
    stat_par.n_pole       =rot_par.n_pole;
    stat_par.n_ph_coil    =  stat_par.n_layer;

    stat_par.n_slot_pin_pole_ph    = stat_par.n_slot_pin/stat_par.n_phase/stat_par.n_pole;
    stat_par.n_a_coil_turn_pole_ph = stat_par.n_slot_pin_pole_ph;

    stat_par.R_in      =rot_par.R+air_gap;

    stat_par.h_arm     =stat_par.h_slot/stat_par.n_layer;         //armature size

    stat_par.butt_slot_R = stat_par.R_in + stat_par.h_wedge + stat_par.h_slot;
    stat_par.slot_pin_angle=2*3.1415926/stat_par.n_slot_pin;//stator slot&pin sum angle
    stat_par.sum_all_slot_b = stat_par.b_slot*stat_par.n_slot_pin;
    stat_par.pi_D_buttom_slot = 2*stat_par.butt_slot_R*3.1415926;//buttom stator slot circle length

    stat_par.slot_part = stat_par.sum_all_slot_b/stat_par.pi_D_buttom_slot;

    stat_par.pin_angle     = (1-stat_par.slot_part)*stat_par.slot_pin_angle;
    stat_par.slot_angle    =    stat_par.slot_part *stat_par.slot_pin_angle;

    stat_par.slot_arg  =stat_par.slot_angle;
    stat_par.pin_arg   =stat_par.pin_angle;

    stat_par.n_phase_slot = (stat_par.n_slot_pin/3)/(stat_par.n_pole/2);   //num slots per phase
    stat_par.n_phase_turn = (stat_par.n_phase_slot)/2;   //num coil turns per phase

}

GenGeom2D::~GenGeom2D()
{
}
