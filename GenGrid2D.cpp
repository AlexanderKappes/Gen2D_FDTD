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

    grid_mark_size                  =7;

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

}

grid_data set_grid_data(double x_in,    double y_in,    int sector_in, int point_num_in, int EH_in, int material_in, bool source_in, double EHnumX_in, double EHnumY_in)
{
    grid_data val;
    val.x           = x_in;
    val.y           = y_in;
    val.sector      = sector_in;
    val.point_num   = point_num_in;

    val.EH           = EH_in;

    val.material    = material_in;

    val.source      = source_in;

    val.EHnumX           = EHnumX_in;
    val.EHnumY           = EHnumY_in;

    return val;
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
                                    + stator_grid_par.n_level_body + stator_grid_par.n_level_air;

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
                                    + stator_grid_par.M_air * stator_grid_par.n_level_air);

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
    stator_grid_par.R_start         = G->rot_par.R;
    stator_grid_par.R_stop          = G->stat_par.R_out;

    //1 - воздух, 2 - клин, 3 - обмотка, 4 - сталь поперек
    //5 - сталь вдоль, 4 - сталь поперек

    stator_grid_par.dh_level        = new double[stator_grid_par.n_level];

    dh_stat_wedge                   = G->stat_par.h_wedge/stator_grid_par.n_level_wedge;
    dh_stat_arm                     = G->stat_par.h_arm  /stator_grid_par.n_level_arm;
    dh_stat_body                    = (stator_grid_par.R_stop - G->stat_par.R_in - G->stat_par.h_wedge - G->stat_par.h_slot)/stator_grid_par.n_level_body;
    dh_stat_air                     = G->air_gap/stator_grid_par.n_level_air;

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
        else if(i    < stator_grid_par.n_level_wedge + stator_grid_par.n_level_air)
            {
                stator_grid_par.dh_level[i]= dh_stat_wedge;
                stat_mater_slot[i]  = 2;
                stat_mater_pin[i]   = 5;
            }
        else if(i    < stator_grid_par.n_level_air + stator_grid_par.n_level_wedge + stator_grid_par.n_level_arm*stator_grid_par.num_arm_layer)
            {
                stator_grid_par.dh_level[i]= dh_stat_arm;
                stat_mater_slot[i]  = 3;
                stat_mater_pin[i]   = 5;
            }
        else
            {
                stator_grid_par.dh_level[i]= dh_stat_body;
                stat_mater_slot[i]  = 4;
                stat_mater_pin[i]   = 4;
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
                double arg[rotor_grid_par.Nmax];
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
                        //arg[i] = slot_start_arg +G->rot_par.slot_arg*(double)(id)/(double)(Np_r_s);
                        x[i]=qCos(slot_start_arg +G->rot_par.slot_arg*(double) id/(double) rotor_grid_par.Np_s);
                        y[i]=qSin(slot_start_arg +G->rot_par.slot_arg*(double) id/(double) rotor_grid_par.Np_s);
                        id ++;
                    }
                    else
                    {
                        //arg[i] = slot_start_arg +G->rot_par.slot_arg + G->rot_par.pin_arg*(double)(id)/(double)(Np_r_p);
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
                                if (i == (rotor_grid_par.M_a + 1)/2 && material == 2 && ray == (rotor_grid_par.Np_s + 1)/2) s = true;

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
                            }
                        }
                        v_abs += rotor_grid_par.dh_level[lev];
                    }
                }
           }
    }



    int M = rotor_grid_par.Row;
    int N = rotor_grid_par.Col;
    double y_dy = 0, x_dy = 0;
    double y_dx = 0, x_dx = 0;
    double EZnumX = 0.0;
    double EZnumY = 0.0;
    double HXnumX = 0.0;
    double HXnumY = 0.5;
    double HYnumX = 0.5;
    double HYnumY = 0.0;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
            {
                if (j < (M - 2))//Расчет расстояний между точками
                {
                    x_dx = rot_grid_pos[j + i*M].x - rot_grid_pos[j + 2 + i*M].x;
                    y_dx = rot_grid_pos[j + i*M].y - rot_grid_pos[j + 2 + i*M].y;

                    if (i  < (N - 2))
                      {
                        x_dy = rot_grid_pos[j + i*M].x - rot_grid_pos[j + i*M + 2*M].x;
                        y_dy = rot_grid_pos[j + i*M].y - rot_grid_pos[j + i*M + 2*M].y;
                      }
                    else
                    {
                        x_dy = rot_grid_pos[j + i*M].x - rot_grid_pos[j + (i - N + 2)*M].x;
                        y_dy = rot_grid_pos[j + i*M].y - rot_grid_pos[j + (i - N + 2)*M].y;
                    }

                    rot_dy[j + i*M] = sqrt (x_dy * x_dy + y_dy * y_dy);
                    rot_dx[j + i*M] = sqrt (x_dx * x_dx + y_dx * y_dx);
                }

                if (rot_grid_pos[j + i*M].EH == 1)// EH = 1 - Ez // EH = 2 - Hx // EH = 3 - Hy
                {
                    rot_grid_pos[j + i*M].EHnumX = EZnumX;
                    rot_grid_pos[j + i*M].EHnumY = EZnumY;
                    EZnumY++;
                }
                if (rot_grid_pos[j + i*M].EH == 2)// EH = 1 - Ez // EH = 2 - Hx // EH = 3 - Hy
                {
                    rot_grid_pos[j + i*M].EHnumX = HXnumX;
                    rot_grid_pos[j + i*M].EHnumY = HXnumY;
                    HXnumY++;
                }
                if (rot_grid_pos[j + i*M].EH == 3)// EH = 1 - Ez // EH = 2 - Hx // EH = 3 - Hy
                {
                    rot_grid_pos[j + i*M].EHnumX = HYnumX;
                    rot_grid_pos[j + i*M].EHnumY = HYnumY;
                    HYnumY++;
                }

            }
        EZnumY = 0;
        HXnumY = 0.5;
        HYnumY = 0;
        if ((i+1)%2 == 0)
        {
            EZnumX++;
            HXnumX++;
            HYnumX++;
        }
    }

    std::string strPath = "D:\\work\\Gen2D_FDTD\\TextFiles\\";
    ArrOutText (strPath + "\\GenGrid\\Rotor\\", "rot_mat", N, M, rotor_grid_par.Np_s,  rotor_grid_par.Np_p, rot_grid_pos, 1);
    ArrOutText (strPath + "\\GenGrid\\Rotor\\", "rot_mat", N, M, rotor_grid_par.Np_s,  rotor_grid_par.Np_p, rot_grid_pos, 2);
    ArrOutText (strPath + "\\GenGrid\\Rotor\\", "rot_mat", N, M, rotor_grid_par.Np_s,  rotor_grid_par.Np_p, rot_grid_pos, 3);
    ArrOutText (strPath + "\\GenGrid\\Rotor\\", "rot_mat", N, M, rotor_grid_par.Np_s,  rotor_grid_par.Np_p, rot_grid_pos, 4);
    ArrOutText (strPath + "\\GenGrid\\Rotor\\", "rot_mat", N, M, rotor_grid_par.Np_s,  rotor_grid_par.Np_p, rot_grid_pos, 5);
    ArrOutText (strPath + "\\GenGrid\\Rotor\\", "rot_mat", N, M, rotor_grid_par.Np_s,  rotor_grid_par.Np_p, rot_grid_pos, 6);
    ArrOutText (strPath + "\\GenGrid\\Rotor\\", "rot_mat", N, M, rotor_grid_par.Np_s,  rotor_grid_par.Np_p, rot_grid_pos, 7);
    ArrOutText (strPath + "\\GenGrid\\Rotor\\", "rot_mat", N, M, rotor_grid_par.Np_s,  rotor_grid_par.Np_p, rot_grid_pos, 8);
    ArrOutText (strPath + "\\GenGrid\\Rotor\\", "rot_mat", N, M, rotor_grid_par.Np_s,  rotor_grid_par.Np_p, rot_grid_pos, 9);
}

//Формирование вектора точек сетки статора
void GenGrid2D ::Gen_Grid_Pos_stat( GenGeom2D *G)
{
    int point_num = 0;
    int EH = 0;
    int material = 0;
    int id = 0;


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
                                if (i == (stator_grid_par.M_a + 1)/2 && material == 3 && ray == (stator_grid_par.Np_s + 1)/2) s = true;

                                double val = v_abs + stator_grid_par.dh_level[lev]   *   (double)(i)/(double)(stator_grid_par.M_a);
                                stat_grid_pos.push_back(set_grid_data(x[ray]*val, y[ray]*val, slot_idx, point_num, EH, material, s, 0.0, 0.0));
                                point_num ++;
                            }
                        }
                        else
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
                        v_abs += stator_grid_par.dh_level[lev];
                    }
                }
            }
        }
    }

    int M = stator_grid_par.Row;
    int N = stator_grid_par.Col;
    double y_dy = 0, x_dy = 0;
    double y_dx = 0, x_dx = 0;
    double EZnumX = 0.0;
    double EZnumY = 0.0;
    double HXnumX = 0.0;
    double HXnumY = 0.5;
    double HYnumX = 0.5;
    double HYnumY = 0.0;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
            {
                if (j < (M - 2))//Расчет расстояний между точками
                {
                    x_dx = stat_grid_pos[j + i*M].x - stat_grid_pos[j + 2 + i*M].x;
                    y_dx = stat_grid_pos[j + i*M].y - stat_grid_pos[j + 2 + i*M].y;

                    if (i  < (N - 2))
                      {
                        x_dy = stat_grid_pos[j + i*M].x - stat_grid_pos[j + i*M + 2*M].x;
                        y_dy = stat_grid_pos[j + i*M].y - stat_grid_pos[j + i*M + 2*M].y;
                      }
                    else
                    {
                        x_dy = stat_grid_pos[j + i*M].x - stat_grid_pos[j + (i - N + 2)*M].x;
                        y_dy = stat_grid_pos[j + i*M].y - stat_grid_pos[j + (i - N + 2)*M].y;
                    }

                    stat_dy[j + i*M] = sqrt (x_dy * x_dy + y_dy * y_dy);
                    stat_dx[j + i*M] = sqrt (x_dx * x_dx + y_dx * y_dx);
                }

                if (stat_grid_pos[j + i*M].EH == 1)// EH = 1 - Ez // EH = 2 - Hx // EH = 3 - Hy
                {
                    stat_grid_pos[j + i*M].EHnumX = EZnumX;
                    stat_grid_pos[j + i*M].EHnumY = EZnumY;
                    EZnumY++;
                }
                if (stat_grid_pos[j + i*M].EH == 2)// EH = 1 - Ez // EH = 2 - Hx // EH = 3 - Hy
                {
                    stat_grid_pos[j + i*M].EHnumX = HXnumX;
                    stat_grid_pos[j + i*M].EHnumY = HXnumY;
                    HXnumY++;
                }
                if (stat_grid_pos[j + i*M].EH == 3)// EH = 1 - Ez // EH = 2 - Hx // EH = 3 - Hy
                {
                    stat_grid_pos[j + i*M].EHnumX = HYnumX;
                    stat_grid_pos[j + i*M].EHnumY = HYnumY;
                    HYnumY++;
                }

            }
        EZnumY = 0;
        HXnumY = 0.5;
        HYnumY = 0;
        if ((i+1)%2 == 0)
        {
            EZnumX++;
            HXnumX++;
            HYnumX++;
        }
    }

    std::string strPath = "D:\\work\\Gen2D_FDTD\\TextFiles\\";
    ArrOutText (strPath + "\\GenGrid\\Stator\\", "stat_mat", N, M, stator_grid_par.Np_s,  stator_grid_par.Np_p, stat_grid_pos, 1);
    ArrOutText (strPath + "\\GenGrid\\Stator\\", "stat_mat", N, M, stator_grid_par.Np_s,  stator_grid_par.Np_p, stat_grid_pos, 2);
    ArrOutText (strPath + "\\GenGrid\\Stator\\", "stat_mat", N, M, stator_grid_par.Np_s,  stator_grid_par.Np_p, stat_grid_pos, 3);
    ArrOutText (strPath + "\\GenGrid\\Stator\\", "stat_mat", N, M, stator_grid_par.Np_s,  stator_grid_par.Np_p, stat_grid_pos, 4);
    ArrOutText (strPath + "\\GenGrid\\Stator\\", "stat_mat", N, M, stator_grid_par.Np_s,  stator_grid_par.Np_p, stat_grid_pos, 5);
    ArrOutText (strPath + "\\GenGrid\\Stator\\", "stat_mat", N, M, stator_grid_par.Np_s,  stator_grid_par.Np_p, stat_grid_pos, 6);
    ArrOutText (strPath + "\\GenGrid\\Stator\\", "stat_mat", N, M, stator_grid_par.Np_s,  stator_grid_par.Np_p, stat_grid_pos, 7);
    ArrOutText (strPath + "\\GenGrid\\Stator\\", "stat_mat", N, M, stator_grid_par.Np_s,  stator_grid_par.Np_p, stat_grid_pos, 8);
    ArrOutText (strPath + "\\GenGrid\\Stator\\", "stat_mat", N, M, stator_grid_par.Np_s,  stator_grid_par.Np_p, stat_grid_pos, 9);
}

GenGrid2D::~GenGrid2D()
{

}
