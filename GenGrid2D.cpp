#include "Header.hpp"

GenGrid2D::GenGrid2D()
{
    rotor_grid_par.n_level_wedge    =1;
    rotor_grid_par.n_level_arm      =1;
    rotor_grid_par.n_level_body     =1;

    stator_grid_par.n_level_wedge   =1;
    stator_grid_par.n_level_arm     =1;
    stator_grid_par.n_level_body    =1;
    stator_grid_par.n_level_air     =1;
    stator_grid_par.n_level_ext     =1;

    grid_mark_size                  =7;

    bool grid_size_test = true;

    if (grid_size_test)
    {
        rotor_grid_par.Nmax = 50;
        rotor_grid_par.Np_s = 4;
        rotor_grid_par.Np_p = 4;

        rotor_grid_par.M_w   = 4;
        rotor_grid_par.M_a   = 4;
        rotor_grid_par.M_b   = 32;
        rotor_grid_par.M_air = 0;

        stator_grid_par.Nmax = 50;
        stator_grid_par.Np_s = 4;
        stator_grid_par.Np_p = 8;

        stator_grid_par.M_w   = 2;
        stator_grid_par.M_a   = 8;
        stator_grid_par.M_b   = 16;
        stator_grid_par.M_air = 6;
        stator_grid_par.M_ext = 6;

    }
    else
        {
            rotor_grid_par.Nmax = 50;
            rotor_grid_par.Np_s = 10;
            rotor_grid_par.Np_p = 10;

            rotor_grid_par.M_w   = 6;
            rotor_grid_par.M_a   = 10;
            rotor_grid_par.M_b   = 40;
            rotor_grid_par.M_air = 0;

            stator_grid_par.Nmax = 50;
            stator_grid_par.Np_s = 10;
            stator_grid_par.Np_p = 10;

            stator_grid_par.M_w   = 6;
            stator_grid_par.M_a   = 20;
            stator_grid_par.M_b   = 40;
            stator_grid_par.M_air = 20;
            stator_grid_par.M_ext = 6;
        }

}

grid_data set_grid_data(double x_in,    double y_in,    int sector_in, int point_num_in, int EH_in, int material_in, bool source_in, double EHnumX_in, double EHnumY_in)
{
    grid_data val;
    val.x           = x_in;
    val.y           = y_in;
    val.sector      = sector_in;
    val.point_num   = point_num_in;

    val.EH          = EH_in;

    val.material    = material_in;

    val.source      = source_in;

    val.EHnumX      = EHnumX_in;
    val.EHnumY      = EHnumY_in;

    return val;
}

join_grid_data set_join_grid_data(double x_in,    double y_in,    double Fi_j_in, double arg_in, double arg1_in, double arg2_in, double dx_in, double dy_in, double val_in, int i1_in, int i2_in )
{
    join_grid_data valG;

    valG.x           = x_in;
    valG.y           = y_in;
    valG.Fi_j        = Fi_j_in;

    valG.arg         = arg_in;
    valG.arg1        = arg1_in;
    valG.arg2        = arg2_in;

    valG.dx           = dx_in;
    valG.dy           = dy_in;
    valG.val          = val_in;

    valG.i1           = i1_in;
    valG.i2           = i2_in;
    return valG;
}

void GenGrid2D ::Gen_Grid_Vector_init    (GenGeom2D *G)
{

    rotor_grid_par.num_arm_layer    = G->rot_par.n_layer;
    stator_grid_par.num_arm_layer   = G->stat_par.n_layer;

    rotor_grid_par.n_level          = rotor_grid_par.n_level_wedge
                                    + rotor_grid_par.n_level_arm * rotor_grid_par.num_arm_layer
                                    + rotor_grid_par.n_level_body;

    stator_grid_par.n_level         = stator_grid_par.n_level_wedge
                                    + stator_grid_par.n_level_arm * stator_grid_par.num_arm_layer
                                    + stator_grid_par.n_level_body + stator_grid_par.n_level_air + stator_grid_par.n_level_ext;

    rotor_grid_par.Row              =(rotor_grid_par.M_w * rotor_grid_par.n_level_wedge
                                    + rotor_grid_par.M_a * rotor_grid_par.n_level_arm * rotor_grid_par.num_arm_layer
                                    + rotor_grid_par.M_b * rotor_grid_par.n_level_body);

    rotor_grid_par.Num_slot         = rotor_grid_par.Np_s * rotor_grid_par.Row;
    rotor_grid_par.Num_pin          = rotor_grid_par.Np_p * rotor_grid_par.Row;

    rotor_grid_par.Sec_num          = rotor_grid_par.Num_slot + rotor_grid_par.Num_pin;
    rotor_grid_par.Num              = rotor_grid_par.Sec_num * G->rot_par.n_slot_pin;

    rotor_grid_par.Col              = (rotor_grid_par.Np_s + rotor_grid_par.Np_p) * G->rot_par.n_slot_pin;

    stator_grid_par.Row             =(stator_grid_par.M_w   * stator_grid_par.n_level_wedge
                                    + stator_grid_par.M_a   * stator_grid_par.n_level_arm * stator_grid_par.num_arm_layer
                                    + stator_grid_par.M_b   * stator_grid_par.n_level_body
                                    + stator_grid_par.M_air * stator_grid_par.n_level_air
                                    + stator_grid_par.M_ext * stator_grid_par.n_level_ext);

    stator_grid_par.Num_slot        = stator_grid_par.Np_s * stator_grid_par.Row;
    stator_grid_par.Num_pin         = stator_grid_par.Np_p * stator_grid_par.Row;

    stator_grid_par.Sec_num         = stator_grid_par.Num_slot + stator_grid_par.Num_pin;
    stator_grid_par.Num             = stator_grid_par.Sec_num * G->stat_par.n_slot_pin;
    stator_grid_par.Col             = (stator_grid_par.Np_s + stator_grid_par.Np_p) * G->stat_par.n_slot_pin;

    rot_mater_pin.resize(rotor_grid_par.n_level);
    rot_mater_slot.resize(rotor_grid_par.n_level);
    stat_mater_slot.resize(stator_grid_par.n_level);
    stat_mater_pin.resize(stator_grid_par.n_level);

    rot_dy.resize(rotor_grid_par.Num);
    rot_dx.resize(rotor_grid_par.Num);
    stat_dy.resize(stator_grid_par.Num);
    stat_dx.resize(stator_grid_par.Num);
}

void GenGrid2D ::Gen_Grid_Calc(GenGeom2D *G)
{

    rotor_grid_par.R_start           =G->rot_par.R/3.0;
    rotor_grid_par.R_stop            =G->rot_par.R;
    //1 - клин, 2 - обмотка, 3 - сталь

    rotor_grid_par.dh_level = new double[rotor_grid_par.n_level];


    dh_rot_wedge =     G->rot_par.h_wedge/rotor_grid_par.n_level_wedge;
    dh_rot_arm   =     G->rot_par.h_arm  /rotor_grid_par.n_level_arm;
    dh_rot_body  =    (rotor_grid_par.R_start - G->rot_par.h_wedge - G->rot_par.h_slot
                       -rotor_grid_par.R_stop)/rotor_grid_par.n_level_body;

    dh_rot_body  =     (rotor_grid_par.R_stop - G->rot_par.h_wedge - G->rot_par.h_slot
                        -rotor_grid_par.R_start)/rotor_grid_par.n_level_body;
    dh_rot_arm   =     G->rot_par.h_arm  /rotor_grid_par.n_level_arm;
    dh_rot_wedge  =    G->rot_par.h_wedge/rotor_grid_par.n_level_wedge;
    //n_level = n_level_wedge + n_level_arm * num_arm_layer + n_level_body = 1 + 1*7 + 2 = 10
    // Массив отрезков между внешней окружностью ротора и дном сетки
    for (int i=0;i<rotor_grid_par.n_level;i++)
    {
        if(i    < rotor_grid_par.n_level_body)
            {
                rotor_grid_par.dh_level[i] = dh_rot_body;
                rot_mater_slot[i] = 3;
                rot_mater_pin[i] = 3;
            }
        else if(i    <rotor_grid_par.n_level_body +rotor_grid_par.n_level_arm*rotor_grid_par.num_arm_layer)
            {
                rotor_grid_par.dh_level[i] = dh_rot_arm;
                rot_mater_slot[i] = 2;
                rot_mater_pin[i] = 3;
            }
        else
            {
                rotor_grid_par.dh_level[i] = dh_rot_wedge;
                rot_mater_slot[i] = 1;
                rot_mater_pin[i] = 3;
            }
    }

    //===========================================================================================
    G->stat_par.R_ext               = G->stat_par.R_out + stator_grid_par.M_ext*(G->stat_par.R_out - G->stat_par.R_in - G->stat_par.h_wedge - G->stat_par.h_slot)/(stator_grid_par.n_level_body * stator_grid_par.M_b);
    stator_grid_par.R_start         = G->rot_par.R;
    stator_grid_par.R_stop          = G->stat_par.R_ext;

    //1 - воздух, 2 - клин, 3 - обмотка, 4 - сталь поперек
    //5 - сталь вдоль, 4 - сталь поперек

    stator_grid_par.dh_level        = new double[stator_grid_par.n_level];

    dh_stat_wedge                   = G->stat_par.h_wedge/stator_grid_par.n_level_wedge;
    dh_stat_arm                     = G->stat_par.h_arm  /stator_grid_par.n_level_arm;
    dh_stat_body                    = (G->stat_par.R_out - G->stat_par.R_in - G->stat_par.h_wedge - G->stat_par.h_slot)/stator_grid_par.n_level_body;
    dh_stat_air                     = G->air_gap/stator_grid_par.n_level_air;
    dh_stat_ext                     = (stator_grid_par.R_stop - G->stat_par.R_out)/stator_grid_par.n_level_ext;

    //n_level = n_level_wedge + n_level_arm * num_arm_layer + n_level_body = 1 + 1*2 + 3 = 6
    // Массив отрезков между внутренней окружностью статора и дном сетки
    for (int i=0;i<stator_grid_par.n_level;i++)
    {
        if      (i    < stator_grid_par.n_level_air)
            {
                stator_grid_par.dh_level[i]= dh_stat_air;
                stat_mater_slot[i]  = 1;
                stat_mater_pin[i]   = 1;
            }
        else if(i    < (stator_grid_par.n_level_wedge + stator_grid_par.n_level_air))
            {
                stator_grid_par.dh_level[i]= dh_stat_wedge;
                stat_mater_slot[i]  = 2;
                stat_mater_pin[i]   = 5;
            }
        else if(i    < (stator_grid_par.n_level_air + stator_grid_par.n_level_wedge + stator_grid_par.n_level_arm*stator_grid_par.num_arm_layer))
            {
                stator_grid_par.dh_level[i]= dh_stat_arm;
                stat_mater_slot[i]  = 3;
                stat_mater_pin[i]   = 5;
            }
        else if(i    < (stator_grid_par.n_level_air + stator_grid_par.n_level_wedge + stator_grid_par.n_level_arm*stator_grid_par.num_arm_layer+stator_grid_par.n_level_body))
            {
                stator_grid_par.dh_level[i]= dh_stat_body;
                stat_mater_slot[i]  = 4;
                stat_mater_pin[i]   = 4;
            }
        else
            {
                stator_grid_par.dh_level[i]= dh_stat_ext;
                stat_mater_slot[i]  = 1;
                stat_mater_pin[i]   = 1;
            }
    }
}

//Формирование вектора точек сетки ротора
void GenGrid2D ::Gen_Grid_Pos_rot(GenGeom2D *G, double arg_beg)
{
    bool pole_pair = false;
    int point_num = 0;
    int EH = 0;

    int material = 0;
    int id = 0;
    bool slot_work = false;


    for(int pole=0;pole<G->rot_par.n_pole;pole++)//Цикл по колличеству полюсов
        {
            int pole_coil_slot_cnt=G->rot_par.n_coil_slot_pole;
            pole_pair = !(pole_pair); //first pole of pole pair
            //Цикл по числу рабочих пазов на один полюс (26 шагов)
            for(int slot=0;slot<G->rot_par.n_slot_pin_pole;slot++,pole_coil_slot_cnt--)//slot circle
            {
                if(slot < G->rot_par.n_coil_slot/2) slot_work = true;
                else slot_work = false;

                double x[rotor_grid_par.Nmax];
                double y[rotor_grid_par.Nmax];
                //double arg[rotor_grid_par.Nmax];

                double v_abs;
                int slot_idx = pole*G->rot_par.n_slot_pin_pole+slot;
                double slot_start_arg = slot_idx*(G->rot_par.slot_arg+G->rot_par.pin_arg) + arg_beg;// Номер паза * на угол Паза + Угол Зубца

                //slot ray. Координаты середины паза pin ray. Координаты середины зубца
                for (int i = 0; i < (rotor_grid_par.Np_s + rotor_grid_par.Np_p);i++)
                {
                    if (i == rotor_grid_par.Np_s)
                        id = 0;
                    if (i < rotor_grid_par.Np_s)
                    {
                        //arg[i] = slot_start_arg +G->rot_par.slot_arg*(double)(id)/(double)(rotor_grid_par.Np_s);
                        x[i]=qCos(slot_start_arg +G->rot_par.slot_arg*(double) id/(double) rotor_grid_par.Np_s);
                        y[i]=qSin(slot_start_arg +G->rot_par.slot_arg*(double) id/(double) rotor_grid_par.Np_s);
                        id ++;
                    }
                    else
                    {
                        //arg[i] = slot_start_arg +G->rot_par.slot_arg + G->rot_par.pin_arg*(double)(id)/(double)(rotor_grid_par.Np_p);
                        x[i]=qCos(slot_start_arg +G->rot_par.slot_arg + G->rot_par.pin_arg*(double) id/(double) rotor_grid_par.Np_p);
                        y[i]=qSin(slot_start_arg +G->rot_par.slot_arg + G->rot_par.pin_arg*(double) id/(double) rotor_grid_par.Np_p);
                        id ++;
                    }
                }
                id = 0;

                for (int ray=0;ray < (rotor_grid_par.Np_s + rotor_grid_par.Np_p);ray++) //ray = 0 - заполнение точками паза, ray = 1 - заполнение точками зубца
                {
                    v_abs = rotor_grid_par.R_start;

                    for (int lev=0;lev<rotor_grid_par.n_level;lev++)    //ray level circle
                    {
                        if(ray < rotor_grid_par.Np_s && slot_work)
                            material = rot_mater_slot[lev];
                        else
                            material = rot_mater_pin[lev];

                        if (lev < rotor_grid_par.n_level_body)
                        {
                            for (int i = 0; i < rotor_grid_par.M_b;i++)
                            {
                                if      (((i+1)%2   != 0)   && ((ray+1)%2 != 0))    EH = 1;
                                else if (((i+1)%2   == 0)   && ((ray+1)%2 != 0))    EH = 3;
                                else if (((i+1)%2   != 0)   && (ray+1)%2  == 0)     EH = 2;
                                else if (((i+1)%2   == 0)   && (ray+1)%2  == 0)     EH = 0;

                                double val = v_abs + rotor_grid_par.dh_level[lev]   *   (double)(i)/(double)(rotor_grid_par.M_b);
                                rot_grid_pos.push_back(set_grid_data(x[ray]*val, y[ray]*val, slot_idx, point_num, EH, material, false, 0.0, 0.0));
                                point_num ++;
                            }
                        }
                        else if (lev < (rotor_grid_par.n_level_body + rotor_grid_par.n_level_arm * rotor_grid_par.num_arm_layer) &&  lev >= rotor_grid_par.n_level_body)
                        {
                            for (int i = 0; i < rotor_grid_par.M_a;i++)
                            {
                                if      (((i+1)%2   != 0)   && ((ray+1)%2 != 0))    EH = 1;
                                else if (((i+1)%2   == 0)   && ((ray+1)%2 != 0))    EH = 3;
                                else if (((i+1)%2   != 0)   && (ray+1)%2  == 0)     EH = 2;
                                else if (((i+1)%2   == 0)   && (ray+1)%2  == 0)     EH = 0;

                                bool s = false;
                                if (i  % 2 == 0 && material == 2 && ray  % 2 == 0 && i!=0 && ray!=0)
                                    s = true;

                                double val = v_abs + rotor_grid_par.dh_level[lev]   *   (double)(i)/(double)(rotor_grid_par.M_a);
                                rot_grid_pos.push_back(set_grid_data(x[ray]*val, y[ray]*val, slot_idx, point_num, EH, material, s, 0.0, 0.0));
                                point_num ++;
                            }
                        }
                        else
                        {
                            for (int i = 0; i < rotor_grid_par.M_w;i++)
                            {
                                if      (((i+1)%2   != 0)   && ((ray+1)%2 != 0))    EH = 1;
                                else if (((i+1)%2   == 0)   && ((ray+1)%2 != 0))    EH = 3;
                                else if (((i+1)%2   != 0)   && (ray+1)%2  == 0)     EH = 2;
                                else if (((i+1)%2   == 0)   && (ray+1)%2  == 0)     EH = 0;

                                double val = v_abs + rotor_grid_par.dh_level[lev]   *   (double)(i)/(double)(rotor_grid_par.M_w);
                                rot_grid_pos.push_back(set_grid_data(x[ray]*val, y[ray]*val, slot_idx, point_num, EH, material, false, 0.0, 0.0));
                                point_num ++;
                                //Определение точек соединения сеток по Ez
                                if (ray % 2 == 0 && (i + 1) == rotor_grid_par.M_w)
                                    {
                                        double Fi_j =0.0;
                                        if (i < rotor_grid_par.Np_s)
                                        {
                                            Fi_j = slot_start_arg + G->rot_par.slot_arg*(double) ray/(double) rotor_grid_par.Np_s;
                                        }
                                        else
                                        {
                                            Fi_j = slot_start_arg +G->rot_par.slot_arg + G->rot_par.pin_arg*(double) ray/(double) rotor_grid_par.Np_p;
                                        }
                                        double val_j = v_abs + rotor_grid_par.dh_level[lev]   *   (double)(i+1)/(double)(rotor_grid_par.M_w);
                                        join_Ez_grid_pos.push_back(set_join_grid_data(x[ray]*val_j, y[ray]*val_j,    Fi_j*180/M_PI, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0, 0));
                                    }
                            }
                        }
                        v_abs += rotor_grid_par.dh_level[lev];
                    }
                }
           }
    }
}

//Формирование вектора точек сетки статора
void GenGrid2D ::Gen_Grid_Pos_stat( GenGeom2D *G)
{
    int point_num = 0;
    int EH = 0;
    int material = 0;
    int id = 0;

    int id_j = 0;

    for(int pole=0;pole<G->stat_par.n_pole;pole++)//pole circle
    {
        for(int ph=0;ph<3;ph++)//phase circle
        {
            for(int slot=0;slot<G->stat_par.n_slot_pin_pole_ph;slot++)//pin circle
            {
                int slot_idx = pole*G->stat_par.n_slot_pin_pole_ph*3
                              +ph  *G->stat_par.n_slot_pin_pole_ph
                              +slot;
                double slot_start_arg = slot_idx*(G->stat_par.slot_arg+G->stat_par.pin_arg);
                double x[stator_grid_par.Nmax];
                double y[stator_grid_par.Nmax];
                double v_abs;


                for (int i = 0; i < (stator_grid_par.Np_s + stator_grid_par.Np_p);i++)
                {
                    if (i == stator_grid_par.Np_s)
                        id = 0;
                    if (i < stator_grid_par.Np_s)
                    {
                        x[i]=qCos(slot_start_arg +G->stat_par.slot_arg*(double) id/(double) stator_grid_par.Np_s);
                        y[i]=qSin(slot_start_arg +G->stat_par.slot_arg*(double) id/(double) stator_grid_par.Np_s);
                        id ++;
                    }
                    else
                    {
                        x[i]=qCos(slot_start_arg +G->stat_par.slot_arg + G->stat_par.pin_arg*(double) id/(double) stator_grid_par.Np_p);
                        y[i]=qSin(slot_start_arg +G->stat_par.slot_arg + G->stat_par.pin_arg*(double) id/(double) stator_grid_par.Np_p);
                        id ++;
                    }
                }
                id = 0;

                for (int ray=0;ray < (stator_grid_par.Np_s + stator_grid_par.Np_p);ray++) //ray = 0 - заполнение точками паза, ray = 1 - заполнение точками зубца
                {
                    v_abs = stator_grid_par.R_start;

                    for (int lev=0;lev<stator_grid_par.n_level;lev++)    //ray level circle
                    {
                        if(ray < stator_grid_par.Np_s)
                            material = stat_mater_slot[lev];
                        else
                            material = stat_mater_pin[lev];

                        if (lev < stator_grid_par.n_level_air)
                        {
                            for (int i = 0; i < stator_grid_par.M_air;i++)
                            {
                                if      (((i+1)%2   != 0)   && ((ray+1)%2 != 0))    EH = 1;
                                else if (((i+1)%2   == 0)   && ((ray+1)%2 != 0))    EH = 3;
                                else if (((i+1)%2   != 0)   && (ray+1)%2  == 0)     EH = 2;
                                else if (((i+1)%2   == 0)   && (ray+1)%2  == 0)     EH = 0;

                                double val = v_abs + stator_grid_par.dh_level[lev]   *   (double)(i)/(double)(stator_grid_par.M_air);
                                stat_grid_pos.push_back(set_grid_data(x[ray]*val, y[ray]*val, slot_idx, point_num, EH, material, false, 0.0, 0.0));
                                point_num ++;

                                //Определение точек соединения сеток по Hy
                                if (ray % 2 == 0 && lev == 0 && i == 0)
                                    {
                                        double Fi_j =0.0;

                                        if (ray == stator_grid_par.Np_s)
                                            id_j = 0;

                                        if (ray < stator_grid_par.Np_s)
                                        {
                                            Fi_j = slot_start_arg + G->stat_par.slot_arg*(double) id_j/(double) stator_grid_par.Np_s;
                                            id_j +=2;
                                        }
                                        else
                                        {
                                            Fi_j = slot_start_arg +G->stat_par.slot_arg + G->stat_par.pin_arg*(double) id_j/(double) stator_grid_par.Np_p;
                                            id_j +=2;
                                        }

                                        double val_j = v_abs - dh_rot_wedge * 1.0/(double)(rotor_grid_par.M_w);
                                        join_Hy_grid_pos.push_back(set_join_grid_data(x[ray]*val_j, y[ray]*val_j,    Fi_j*180/M_PI, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0, 0));

                                    }
                            }
                        }

                        else if (lev < (stator_grid_par.n_level_air + stator_grid_par.n_level_wedge) &&  lev >= stator_grid_par.n_level_air)
                        {
                            for (int i = 0; i < stator_grid_par.M_w;i++)
                            {
                                if      (((i+1)%2   != 0)   && ((ray+1)%2 != 0))    EH = 1;
                                else if (((i+1)%2   == 0)   && ((ray+1)%2 != 0))    EH = 3;
                                else if (((i+1)%2   != 0)   && (ray+1)%2  == 0)     EH = 2;
                                else if (((i+1)%2   == 0)   && (ray+1)%2  == 0)     EH = 0;

                                double val = v_abs + stator_grid_par.dh_level[lev]   *   (double)(i)/(double)(stator_grid_par.M_w);
                                stat_grid_pos.push_back(set_grid_data(x[ray]*val, y[ray]*val, slot_idx, point_num, EH, material, false, 0.0, 0.0));
                                point_num ++;
                            }
                        }
                        else if (lev < (stator_grid_par.n_level_air + stator_grid_par.n_level_wedge + stator_grid_par.n_level_arm * stator_grid_par.num_arm_layer)
                                 &&  lev >= (stator_grid_par.n_level_air + stator_grid_par.n_level_wedge))
                        {
                            for (int i = 0; i < stator_grid_par.M_a;i++)
                            {
                                if      (((i+1)%2   != 0)   && ((ray+1)%2 != 0))    EH = 1;
                                else if (((i+1)%2   == 0)   && ((ray+1)%2 != 0))    EH = 3;
                                else if (((i+1)%2   != 0)   && (ray+1)%2  == 0)     EH = 2;
                                else if (((i+1)%2   == 0)   && (ray+1)%2  == 0)     EH = 0;

                                bool s = false;
                                if (i % 2 == 0 && material == 3 && ray % 2 == 0 && i!=0 && ray!=0)
                                    s = true;

                                double val = v_abs + stator_grid_par.dh_level[lev]   *   (double)(i)/(double)(stator_grid_par.M_a);
                                stat_grid_pos.push_back(set_grid_data(x[ray]*val, y[ray]*val, slot_idx, point_num, EH, material, s, 0.0, 0.0));
                                point_num ++;
                            }
                        }
                        else if(lev    < (stator_grid_par.n_level_air + stator_grid_par.n_level_wedge + stator_grid_par.n_level_arm*stator_grid_par.num_arm_layer+stator_grid_par.n_level_body))
                        {
                            for (int i = 0; i < stator_grid_par.M_b;i++)
                            {
                                if      (((i+1)%2   != 0)   && ((ray+1)%2 != 0))    EH = 1;
                                else if (((i+1)%2   == 0)   && ((ray+1)%2 != 0))    EH = 3;
                                else if (((i+1)%2   != 0)   && (ray+1)%2  == 0)     EH = 2;
                                else if (((i+1)%2   == 0)   && (ray+1)%2  == 0)     EH = 0;

                                double val = v_abs + stator_grid_par.dh_level[lev]   *   (double)(i)/(double)(stator_grid_par.M_b);
                                stat_grid_pos.push_back(set_grid_data(x[ray]*val, y[ray]*val, slot_idx, point_num, EH, material, false, 0.0, 0.0));
                                point_num ++;
                            }
                        }
                        else
                        {
                            for (int i = 0; i < stator_grid_par.M_ext;i++)
                            {
                                if      (((i+1)%2   != 0)   && ((ray+1)%2 != 0))    EH = 1;
                                else if (((i+1)%2   == 0)   && ((ray+1)%2 != 0))    EH = 3;
                                else if (((i+1)%2   != 0)   && (ray+1)%2  == 0)     EH = 2;
                                else if (((i+1)%2   == 0)   && (ray+1)%2  == 0)     EH = 0;

                                double val = v_abs + stator_grid_par.dh_level[lev]   *   (double)(i)/(double)(stator_grid_par.M_ext);
                                stat_grid_pos.push_back(set_grid_data(x[ray]*val, y[ray]*val, slot_idx, point_num, EH, material, false, 0.0, 0.0));
                                point_num ++;
                            }
                        }
                        v_abs += stator_grid_par.dh_level[lev];
                    }
                }
                id_j = 0;
            }
        }
    }
}

void GenGrid2D ::Gen_Grid_Pos_join( GenGeom2D *G, double arg_beg)
{
    bool pole_pair = false;
    //bool slot_work = false;

    //Ротор Hy
    int id_r = 0;
    int i_r = 0;
    int ij_r = 0;
    int EHrot = 0;

    double Fi_1_r = 0.0;
    double Fi_2_r = 0.0;

    double arg1_r = 0.0;
    double arg2_r = 0.0;
    double arg_r  = 0.0;

    for(int pole=0;pole<G->rot_par.n_pole;pole++)//Цикл по колличеству полюсов
        {
            int pole_coil_slot_cnt=G->rot_par.n_coil_slot_pole;
            pole_pair = !(pole_pair); //first pole of pole pair
            //Цикл по числу рабочих пазов на один полюс (26 шагов)
            for(int slot=0;slot<G->rot_par.n_slot_pin_pole;slot++,pole_coil_slot_cnt--)//slot circle
            {
                //if(slot < G->rot_par.n_coil_slot/2) slot_work = true;

                int slot_idx = pole*G->rot_par.n_slot_pin_pole+slot;
                double slot_start_arg = slot_idx*(G->rot_par.slot_arg+G->rot_par.pin_arg) + arg_beg;// Номер паза * на угол Паза + Угол Зубца

                //slot ray. Координаты середины паза pin ray. Координаты середины зубца
                while (id_r < rotor_grid_par.Np_s && (ij_r * 2) != stator_grid_par.Col)
                {
                    EHrot = rot_grid_pos[1+id_r*rotor_grid_par.Row].EH;

                    Fi_1_r = (slot_start_arg +G->rot_par.slot_arg*(double) id_r/(double) rotor_grid_par.Np_s)*180/M_PI;
                    id_r += 2;
                    Fi_2_r = (slot_start_arg +G->rot_par.slot_arg*(double) (id_r) /(double) rotor_grid_par.Np_s)*180/M_PI;


                    while (join_Hy_grid_pos[ij_r].Fi_j >= Fi_1_r && join_Hy_grid_pos[ij_r].Fi_j <= Fi_2_r && EHrot == 3 )
                        {

                            arg1_r = join_Hy_grid_pos[ij_r].Fi_j - Fi_1_r;
                            arg2_r = Fi_2_r - join_Hy_grid_pos[ij_r].Fi_j;
                            arg_r = Fi_2_r - Fi_1_r;

                            join_Hy_grid_pos[ij_r].arg1 = arg1_r;
                            join_Hy_grid_pos[ij_r].arg2 = arg2_r;
                            join_Hy_grid_pos[ij_r].arg  = arg_r;

                            join_Hy_grid_pos[ij_r].i1  = i_r;
                            join_Hy_grid_pos[ij_r].i2  = i_r + 2;

                            ij_r++;
                            if ((ij_r * 2) == stator_grid_par.Col)
                                break;
                        }
                    i_r += 2;
                }
                id_r = 0;


                while (id_r < rotor_grid_par.Np_p && (ij_r * 2) != stator_grid_par.Col)
                {
                    EHrot = rot_grid_pos[1+id_r*rotor_grid_par.Row].EH;

                    Fi_1_r = (slot_start_arg +G->rot_par.slot_arg+G->rot_par.pin_arg*(double) id_r/(double) rotor_grid_par.Np_p)*180/M_PI;
                    id_r += 2;
                    Fi_2_r = (slot_start_arg +G->rot_par.slot_arg+G->rot_par.pin_arg*(double) (id_r) /(double) rotor_grid_par.Np_p)*180/M_PI;


                    while (join_Hy_grid_pos[ij_r].Fi_j >= Fi_1_r && join_Hy_grid_pos[ij_r].Fi_j <= Fi_2_r && EHrot == 3 )
                        {

                            arg1_r = join_Hy_grid_pos[ij_r].Fi_j - Fi_1_r;
                            arg2_r = Fi_2_r - join_Hy_grid_pos[ij_r].Fi_j;
                            arg_r = Fi_2_r - Fi_1_r;

                            join_Hy_grid_pos[ij_r].arg1 = arg1_r;
                            join_Hy_grid_pos[ij_r].arg2 = arg2_r;
                            join_Hy_grid_pos[ij_r].arg  = arg_r;

                            join_Hy_grid_pos[ij_r].i1  = i_r;
                            join_Hy_grid_pos[ij_r].i2  = i_r + 2;

                            ij_r++;
                            if ((ij_r * 2) == stator_grid_par.Col)
                                break;
                        }
                    i_r += 2;
                }
                id_r = 0;
            }
        }


    //Статор Ez
    int id_s = 0;
    int i_s = 0;
    int ij_s = 0;
    int EHstat = 0;

    double Fi_1_s = 0.0;
    double Fi_2_s = 0.0;

    double arg1_s = 0.0;
    double arg2_s = 0.0;
    double arg_s  = 0.0;

    for(int pole=0;pole<G->stat_par.n_pole;pole++)//pole circle
    {
        for(int ph=0;ph<3;ph++)//phase circle
        {
            for(int slot=0; slot < G->stat_par.n_slot_pin_pole_ph; slot++)//pin circle
            {
                int     slot_idx        = pole * G->stat_par.n_slot_pin_pole_ph*3 + ph*G->stat_par.n_slot_pin_pole_ph + slot;
                double  slot_start_arg  = slot_idx*(G->stat_par.slot_arg+G->stat_par.pin_arg);
                //slot ray. Координаты середины паза pin ray. Координаты середины зубца
                while (id_s < rotor_grid_par.Np_s && (ij_s * 2) != rotor_grid_par.Col)
                {
                    EHstat = stat_grid_pos[id_s*stator_grid_par.Row].EH;

                    Fi_1_s = (slot_start_arg +G->stat_par.slot_arg*(double) id_s/(double) stator_grid_par.Np_s)*180/M_PI;
                    id_s += 2;
                    Fi_2_s = (slot_start_arg +G->stat_par.slot_arg*(double) (id_s) /(double) stator_grid_par.Np_s)*180/M_PI;

                    while (join_Ez_grid_pos[ij_s].Fi_j >= Fi_1_s && join_Ez_grid_pos[ij_s].Fi_j <= Fi_2_s && EHstat == 1 )
                        {

                            arg1_s = join_Ez_grid_pos[ij_s].Fi_j - Fi_1_s;
                            arg2_s = Fi_2_s - join_Ez_grid_pos[ij_s].Fi_j;
                            arg_s = Fi_2_s - Fi_1_s;

                            join_Ez_grid_pos[ij_s].arg1 = arg1_s;
                            join_Ez_grid_pos[ij_s].arg2 = arg2_s;
                            join_Ez_grid_pos[ij_s].arg  = arg_s;

                            join_Ez_grid_pos[ij_s].i1  = i_s;
                            join_Ez_grid_pos[ij_s].i2  = i_s + 2;

                            ij_s++;
                            if ((ij_s * 2) == rotor_grid_par.Col)//Число точек сетки Joint в два раза меньше, чем строк сетки ротора
                                break;
                        }
                    i_s +=2;
                }
                id_s = 0;

                while (id_s < stator_grid_par.Np_p && (ij_s * 2) != rotor_grid_par.Col)
                {
                    EHrot = stat_grid_pos[id_s*stator_grid_par.Row].EH;

                    Fi_1_s = (slot_start_arg +G->stat_par.slot_arg+G->stat_par.pin_arg*(double) id_s/(double) stator_grid_par.Np_p)*180/M_PI;
                    id_s += 2;
                    Fi_2_s = (slot_start_arg +G->stat_par.slot_arg+G->stat_par.pin_arg*(double) (id_s) /(double) stator_grid_par.Np_p)*180/M_PI;

                    while (join_Ez_grid_pos[ij_s].Fi_j >= Fi_1_s && join_Ez_grid_pos[ij_s].Fi_j <= Fi_2_s && EHstat == 1 )
                        {

                            arg1_s = join_Ez_grid_pos[ij_s].Fi_j - Fi_1_s;
                            arg2_s = Fi_2_s - join_Ez_grid_pos[ij_s].Fi_j;
                            arg_s = Fi_2_s - Fi_1_s;

                            join_Ez_grid_pos[ij_s].arg1 = arg1_s;
                            join_Ez_grid_pos[ij_s].arg2 = arg2_s;
                            join_Ez_grid_pos[ij_s].arg  = arg_s;

                            join_Ez_grid_pos[ij_s].i1  = i_s;
                            join_Ez_grid_pos[ij_s].i2  = i_s + 2;

                            ij_s++;
                            if ((ij_s * 2) == rotor_grid_par.Col)
                                break;
                        }
                    i_s +=2;
                }
                id_s = 0;
            }
        }
    }

}

void GenGrid2D ::IntervalsCalc()
{
    int M_r = rotor_grid_par.Row;
    int N_r = rotor_grid_par.Col;
    double y_dy_r = 0, x_dy_r = 0;
    double y_dx_r = 0, x_dx_r = 0;
    double EZnumX_r = 0.0;
    double EZnumY_r = 0.0;
    double HXnumX_r = 0.0;
    double HXnumY_r = 0.5;
    double HYnumX_r = 0.5;
    double HYnumY_r = 0.0;

    for (int i = 0; i < N_r; i++)
    {
        for (int j = 0; j < M_r; j++)
            {
                //Расчет расстояний между точками
            if (j < (M_r - 2)) //dX
                {
                    x_dx_r = rot_grid_pos[j + i*M_r].x - rot_grid_pos[j + 2 + i*M_r].x;
                    y_dx_r = rot_grid_pos[j + i*M_r].y - rot_grid_pos[j + 2 + i*M_r].y;

                    rot_dx[j + i*M_r] = sqrt (x_dx_r * x_dx_r + y_dx_r * y_dx_r);
                }

                //dY
                if (i  < (N_r - 2))
                  {
                    x_dy_r = rot_grid_pos[j + i*M_r].x - rot_grid_pos[j + i*M_r + 2*M_r].x;
                    y_dy_r = rot_grid_pos[j + i*M_r].y - rot_grid_pos[j + i*M_r + 2*M_r].y;
                  }
                else
                {
                    x_dy_r = rot_grid_pos[j + i*M_r].x - rot_grid_pos[j + (i - N_r + 2)*M_r].x;
                    y_dy_r = rot_grid_pos[j + i*M_r].y - rot_grid_pos[j + (i - N_r + 2)*M_r].y;
                }

                rot_dy[j + i*M_r] = sqrt (x_dy_r * x_dy_r + y_dy_r * y_dy_r);

                //Определение типа точки
                if (rot_grid_pos[j + i*M_r].EH == 1)// EH = 1 - Ez // EH = 2 - Hx // EH = 3 - Hy
                {
                    rot_grid_pos[j + i*M_r].EHnumX = EZnumX_r;
                    rot_grid_pos[j + i*M_r].EHnumY = EZnumY_r;
                    EZnumY_r++;
                }
                if (rot_grid_pos[j + i*M_r].EH == 2)// EH = 1 - Ez // EH = 2 - Hx // EH = 3 - Hy
                {
                    rot_grid_pos[j + i*M_r].EHnumX = HXnumX_r;
                    rot_grid_pos[j + i*M_r].EHnumY = HXnumY_r;
                    HXnumY_r++;
                }
                if (rot_grid_pos[j + i*M_r].EH == 3)// EH = 1 - Ez // EH = 2 - Hx // EH = 3 - Hy
                {
                    rot_grid_pos[j + i*M_r].EHnumX = HYnumX_r;
                    rot_grid_pos[j + i*M_r].EHnumY = HYnumY_r;
                    HYnumY_r++;
                }

            }
        EZnumY_r = 0;
        HXnumY_r = 0.5;
        HYnumY_r = 0;
        if ((i+1)%2 == 0)
        {
            EZnumX_r++;
            HXnumX_r++;
            HYnumX_r++;
        }
    }

    int M_s = stator_grid_par.Row;
    int N_s = stator_grid_par.Col;
    double y_dy_s = 0, x_dy_s = 0;
    double y_dx_s = 0, x_dx_s = 0;
    double EZnumX_s = 0.0;
    double EZnumY_s = 0.0;
    double HXnumX_s = 0.0;
    double HXnumY_s = 0.5;
    double HYnumX_s = 0.5;
    double HYnumY_s = 0.0;

    for (int i = 0; i < N_s; i++)
    {
        for (int j = 0; j < M_s; j++)
            {
                //Расчет расстояний между точками
                if (j < (M_s - 2))//dX
                {
                    x_dx_s = stat_grid_pos[j + i*M_s].x - stat_grid_pos[j + 2 + i*M_s].x;
                    y_dx_s = stat_grid_pos[j + i*M_s].y - stat_grid_pos[j + 2 + i*M_s].y;

                    stat_dx[j + i*M_s] = sqrt (x_dx_s * x_dx_s + y_dx_s * y_dx_s);
                }

                if (i  < (N_s - 2))//dY
                  {
                    x_dy_s = stat_grid_pos[j + i*M_s].x - stat_grid_pos[j + i*M_s + 2*M_s].x;
                    y_dy_s = stat_grid_pos[j + i*M_s].y - stat_grid_pos[j + i*M_s + 2*M_s].y;
                  }
                else
                {
                    x_dy_s = stat_grid_pos[j + i*M_s].x - stat_grid_pos[j + (i - N_s + 2)*M_s].x;
                    y_dy_s = stat_grid_pos[j + i*M_s].y - stat_grid_pos[j + (i - N_s + 2)*M_s].y;
                }

                stat_dy[j + i*M_s] = sqrt (x_dy_s * x_dy_s + y_dy_s * y_dy_s);
                //Определение типа точки
                if (stat_grid_pos[j + i*M_s].EH == 1)// EH = 1 - Ez // EH = 2 - Hx // EH = 3 - Hy
                {
                    stat_grid_pos[j + i*M_s].EHnumX = EZnumX_s;
                    stat_grid_pos[j + i*M_s].EHnumY = EZnumY_s;
                    EZnumY_s++;
                }
                if (stat_grid_pos[j + i*M_s].EH == 2)// EH = 1 - Ez // EH = 2 - Hx // EH = 3 - Hy
                {
                    stat_grid_pos[j + i*M_s].EHnumX = HXnumX_s;
                    stat_grid_pos[j + i*M_s].EHnumY = HXnumY_s;
                    HXnumY_s++;
                }
                if (stat_grid_pos[j + i*M_s].EH == 3)// EH = 1 - Ez // EH = 2 - Hx // EH = 3 - Hy
                {
                    stat_grid_pos[j + i*M_s].EHnumX = HYnumX_s;
                    stat_grid_pos[j + i*M_s].EHnumY = HYnumY_s;
                    HYnumY_s++;
                }
            }

        EZnumY_s = 0;
        HXnumY_s = 0.5;
        HYnumY_s = 0;
        if ((i+1)%2 == 0)
        {
            EZnumX_s++;
            HXnumX_s++;
            HYnumX_s++;
        }
    }

    int M_Ej = rotor_grid_par.Row;
    int N_Ej = rotor_grid_par.Col/2;
    //double y_dy_Ej = 0, x_dy_Ej = 0;
    double y_dx_Ej = 0, x_dx_Ej = 0;

    for (int i = 0; i < N_Ej; i++)
    {
                x_dx_Ej = rot_grid_pos[M_Ej*(2*i+1) - 2].x - join_Ez_grid_pos   [i].x ;
                y_dx_Ej = rot_grid_pos[M_Ej*(2*i+1) - 2].y - join_Ez_grid_pos   [i].y;
                join_Ez_grid_pos      [i].dx = sqrt (x_dx_Ej * x_dx_Ej + y_dx_Ej * y_dx_Ej);
    }

    int M_Hj = stator_grid_par.Row;
    int N_Hj = stator_grid_par.Col/2;
    //double y_dy_Hj = 0, x_dy_Hj = 0;
    double y_dx_Hj = 0, x_dx_Hj = 0;

    for (int i = 0; i < N_Hj; i++)
    {
                x_dx_Hj = stat_grid_pos[1 + i*M_Hj*2].x - join_Hy_grid_pos   [i].x;
                y_dx_Hj = stat_grid_pos[1 + i*M_Hj*2].y - join_Hy_grid_pos   [i].y;
                join_Hy_grid_pos             [i].dx = sqrt (x_dx_Hj * x_dx_Hj + y_dx_Hj * y_dx_Hj);
    }
}


void GenGrid2D ::DataOut()
{
    int M_r = rotor_grid_par.Row;
    int N_r = rotor_grid_par.Col;

    std::string strPath_r = "D:\\work\\Gen2D_FDTD\\TextFiles\\";
    ArrOutText (strPath_r + "GenGrid\\Rotor\\", "rot_mat", N_r, M_r, rotor_grid_par.Np_s,  rotor_grid_par.Np_p, rot_grid_pos, 1);
    ArrOutText (strPath_r + "GenGrid\\Rotor\\", "rot_mat", N_r, M_r, rotor_grid_par.Np_s,  rotor_grid_par.Np_p, rot_grid_pos, 2);
    ArrOutText (strPath_r + "GenGrid\\Rotor\\", "rot_mat", N_r, M_r, rotor_grid_par.Np_s,  rotor_grid_par.Np_p, rot_grid_pos, 3);
    ArrOutText (strPath_r + "GenGrid\\Rotor\\", "rot_mat", N_r, M_r, rotor_grid_par.Np_s,  rotor_grid_par.Np_p, rot_grid_pos, 4);
    ArrOutText (strPath_r + "GenGrid\\Rotor\\", "rot_mat", N_r, M_r, rotor_grid_par.Np_s,  rotor_grid_par.Np_p, rot_grid_pos, 5);
    ArrOutText (strPath_r + "GenGrid\\Rotor\\", "rot_mat", N_r, M_r, rotor_grid_par.Np_s,  rotor_grid_par.Np_p, rot_grid_pos, 6);
    ArrOutText (strPath_r + "GenGrid\\Rotor\\", "rot_mat", N_r, M_r, rotor_grid_par.Np_s,  rotor_grid_par.Np_p, rot_grid_pos, 7);
    ArrOutText (strPath_r + "GenGrid\\Rotor\\", "rot_mat", N_r, M_r, rotor_grid_par.Np_s,  rotor_grid_par.Np_p, rot_grid_pos, 8);
    ArrOutText (strPath_r + "GenGrid\\Rotor\\", "rot_mat", N_r, M_r, rotor_grid_par.Np_s,  rotor_grid_par.Np_p, rot_grid_pos, 9);

    int M_s = stator_grid_par.Row;
    int N_s = stator_grid_par.Col;

    std::string strPath_s = "D:\\work\\Gen2D_FDTD\\TextFiles\\";
    ArrOutText (strPath_s + "GenGrid\\Stator\\", "stat_mat", N_s, M_s, stator_grid_par.Np_s,  stator_grid_par.Np_p, stat_grid_pos, 1);
    ArrOutText (strPath_s + "GenGrid\\Stator\\", "stat_mat", N_s, M_s, stator_grid_par.Np_s,  stator_grid_par.Np_p, stat_grid_pos, 2);
    ArrOutText (strPath_s + "GenGrid\\Stator\\", "stat_mat", N_s, M_s, stator_grid_par.Np_s,  stator_grid_par.Np_p, stat_grid_pos, 3);
    ArrOutText (strPath_s + "GenGrid\\Stator\\", "stat_mat", N_s, M_s, stator_grid_par.Np_s,  stator_grid_par.Np_p, stat_grid_pos, 4);
    ArrOutText (strPath_s + "GenGrid\\Stator\\", "stat_mat", N_s, M_s, stator_grid_par.Np_s,  stator_grid_par.Np_p, stat_grid_pos, 5);
    ArrOutText (strPath_s + "GenGrid\\Stator\\", "stat_mat", N_s, M_s, stator_grid_par.Np_s,  stator_grid_par.Np_p, stat_grid_pos, 6);
    ArrOutText (strPath_s + "GenGrid\\Stator\\", "stat_mat", N_s, M_s, stator_grid_par.Np_s,  stator_grid_par.Np_p, stat_grid_pos, 7);
    ArrOutText (strPath_s + "GenGrid\\Stator\\", "stat_mat", N_s, M_s, stator_grid_par.Np_s,  stator_grid_par.Np_p, stat_grid_pos, 8);
    ArrOutText (strPath_s + "GenGrid\\Stator\\", "stat_mat", N_s, M_s, stator_grid_par.Np_s,  stator_grid_par.Np_p, stat_grid_pos, 9);

    //int N_Ej = rotor_grid_par.Col/2;
    //int N_Hj = stator_grid_par.Col/2;
    //std::string strPath_j = "D:\\work\\Gen2D_FDTD\\TextFiles\\";
    //ArrOutText ( strPath_j + "JointGrid\\", "Ez_rotor_joint" , N_Ej, join_Ez_grid_pos);
    //ArrOutText ( strPath_j + "JointGrid\\", "Hy_stator_joint", N_Hj, join_Hy_grid_pos);
}

GenGrid2D::~GenGrid2D()
{

}
