#ifndef GENGEOM2D_H
#define GENGEOM2D_H

struct rotor_parameters
{
    int n_pole;
    int n_coil_slot_pole;
    int n_slot_pin_pole;
    int n_turn_pole;
    int n_layer;
    int n_coil_slot;
    int n_slot_pin;

    double R;         //rotor external radius
    double h_wedge;   //wedge size
    double h_arm;     //armature size
    double slot_arg;  //slot angle
    double pin_arg;   //pin  angle

    double b_slot;
    double h_slot;

    double slot_pin_angle;
    double sum_all_slot_b;
    double pi_D;
    double slot_part;
    double pin_angle;
    double slot_angle;
    double wedge_pole_num;
};

struct stator_parameters
{
    int n_pole;
    int n_phase;
    int n_slot_pin_pole_ph;
    int n_a_coil_turn_pole_ph;
    int n_ph_coil;
    int shift_coil;
    int n_layer;

    int n_slot_pin;

    double R_out;
    double R_in;
    double h_wedge;
    double h_arm;
    double slot_arg;
    double pin_arg;

    double b_slot;
    double h_slot;

    double butt_slot_R;
    double slot_pin_angle;
    double sum_all_slot_b;
    double pi_D_buttom_slot;
    double slot_part;
    double pin_angle;
    double slot_angle;

    int n_phase_slot;
    int n_phase_turn;
};

class GenGeom2D
{
    //Q_OBJECT
public:

    GenGeom2D();
    ~GenGeom2D();
    void init_rotor ();
    void init_stator ();

    rotor_parameters rot_par;
    stator_parameters stat_par;
    double air_gap;
    double L;

private:

//signals:

//private:

//public slots:
};



#endif // GENGEOM2D_H
