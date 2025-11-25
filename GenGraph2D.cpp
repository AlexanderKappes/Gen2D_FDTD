#include "Header.hpp"

GenGraph2D::GenGraph2D()
{
}

coil_data set_coil_data(int sector_in, char phase_in, int coil_in, int turn_in, int slot_in, int layer_in)
{
    coil_data val;

    val.sector  = sector_in;
    val.turn    = turn_in;
    val.slot    = slot_in;
    val.layer   = layer_in;

    val.coil    = coil_in;
    val.phase   = phase_in;
    return val;
}

//Формирование кривой стали статора
void GenGraph2D::stator_steel_curve_Create      (   GenGeom2D *G)
{
//Заполнение массива стали статора
        for(int pole=0;pole<G->stat_par.n_pole;pole++)//pole circle
    {
        for(int ph=0;ph<3;ph++)//phase circle
        {
            for(int slot=0;slot<G->stat_par.n_slot_pin_pole_ph;slot++)//pin circle
            {
                int slot_idx = pole*G->stat_par.n_slot_pin_pole_ph*3
                              +ph  *G->stat_par.n_slot_pin_pole_ph
                              +slot;
                double val_abs_pin   = G->stat_par.R_in;
                double val_abs_slot  = G->stat_par.R_in+G->stat_par.h_slot+G->stat_par.h_wedge;
                double slot_start_arg = slot_idx*(G->stat_par.slot_arg+G->stat_par.pin_arg);
                double x[4];
                double y[4];

                x[0]=val_abs_slot*qCos(slot_start_arg );
                y[0]=val_abs_slot*qSin(slot_start_arg );
                x[1]=val_abs_slot*qCos(slot_start_arg +G->stat_par.slot_arg );
                y[1]=val_abs_slot*qSin(slot_start_arg +G->stat_par.slot_arg );
                x[2]=val_abs_pin *qCos(slot_start_arg +G->stat_par.slot_arg );
                y[2]=val_abs_pin *qSin(slot_start_arg +G->stat_par.slot_arg );
                x[3]=val_abs_pin *qCos(slot_start_arg +G->stat_par.slot_arg +G->stat_par.pin_arg );
                y[3]=val_abs_pin *qSin(slot_start_arg +G->stat_par.slot_arg +G->stat_par.pin_arg );

                stator_steel[slot_idx*4+0] = QCPCurveData(slot_idx*4+0,x[0],y[0]);
                stator_steel[slot_idx*4+1] = QCPCurveData(slot_idx*4+1,x[1],y[1]);
                stator_steel[slot_idx*4+2] = QCPCurveData(slot_idx*4+2,x[2],y[2]);
                stator_steel[slot_idx*4+3] = QCPCurveData(slot_idx*4+3,x[3],y[3]);
                //----------------------------------------------------------------
                double v_abs;
                x[0]=qCos(slot_start_arg );
                y[0]=qSin(slot_start_arg );

                x[1]=qCos(slot_start_arg + G->stat_par.slot_arg);
                y[1]=qSin(slot_start_arg + G->stat_par.slot_arg);

                v_abs = G->stat_par.R_out; //stator border
                gener_total[slot_idx*2]     = QCPCurveData(slot_idx*2,          x[0]*v_abs, y[0]*v_abs);
                gener_total[slot_idx*2+1]   = QCPCurveData(slot_idx*2+1,      x[1]*v_abs, y[1]*v_abs);

                v_abs = G->stat_par.R_in;
                stator_R [slot_idx*2]       = QCPCurveData(slot_idx*2,          x[0]*v_abs, y[0]*v_abs);
                stator_R [slot_idx*2+1]     = QCPCurveData(slot_idx*2+1,      x[1]*v_abs, y[1]*v_abs);

                v_abs = G->stat_par.R_in+G->stat_par.h_wedge;
                stator_w [slot_idx*2]       = QCPCurveData(slot_idx*2,          x[0]*v_abs, y[0]*v_abs);
                stator_w [slot_idx*2+1]     = QCPCurveData(slot_idx*2+1,      x[1]*v_abs, y[1]*v_abs);

                v_abs = G->stat_par.R_in+G->stat_par.h_wedge+G->stat_par.h_slot/2;
                stator_L1[slot_idx*2]       = QCPCurveData(slot_idx*2,          x[0]*v_abs, y[0]*v_abs);
                stator_L1[slot_idx*2+1]     = QCPCurveData(slot_idx*2+1,        x[1]*v_abs, y[1]*v_abs);

                v_abs = G->stat_par.R_in+G->stat_par.h_wedge+G->stat_par.h_slot;
                stator_L2[slot_idx*2]       = QCPCurveData(slot_idx*2,          x[0]*v_abs, y[0]*v_abs);
                stator_L2[slot_idx*2+1]     = QCPCurveData(slot_idx*2+1,        x[1]*v_abs, y[1]*v_abs);
            }
        }
    }
       stator_R [G->stat_par.n_slot_pin*2]          = stator_R [0];
       stator_w [G->stat_par.n_slot_pin*2]          = stator_w [0];
       stator_L1[G->stat_par.n_slot_pin*2]          = stator_L1[0];
       stator_L2[G->stat_par.n_slot_pin*2]          = stator_L2[0];
       stator_steel[G->stat_par.n_slot_pin*4]       = stator_steel[0];
       gener_total[G->stat_par.n_slot_pin*2]          = gener_total[0];

}

//Формирование кривой стали ротора
void GenGraph2D::rotor_steel_curve_Create  ( GenGeom2D *G, double arg_beg)
{
   bool pole_pair = false;

   for(int pole=0;pole<G->rot_par.n_pole;pole++)//Цикл по колличеству полюсов
       {
           int pole_coil_slot_cnt=G->rot_par.n_coil_slot_pole;
           pole_pair = !(pole_pair); //first pole of pole pair
           //Цикл по числу рабочих пазов на один полюс (26 шагов)
           for(int slot=0;slot<G->rot_par.n_slot_pin_pole;slot++,pole_coil_slot_cnt--)//slot circle
           {
               double x[4];
               double y[4];
               double v_abs;
               int slot_idx = pole*G->rot_par.n_slot_pin_pole+slot;
               double slot_start_arg = slot_idx*(G->rot_par.slot_arg+G->rot_par.pin_arg)+arg_beg;// Номер паза * на угол Паза + Угол Зубца

               double val_abs_pin   = G->rot_par.R;//внешний радиус ротора
               double val_abs_slot  = G->rot_par.R-G->rot_par.h_slot-G->rot_par.h_wedge;//Расстояние от центра до дна паза

               if(pole_coil_slot_cnt<=0)     {val_abs_slot= G->rot_par.R-G->rot_par.h_wedge;}//Если сегмент принадлежит наконечнику полюса, то пазов нет

               x[0]=val_abs_slot*qCos(slot_start_arg );//Косинус с углом паза расстояния от центра до дна паза ротора
               y[0]=val_abs_slot*qSin(slot_start_arg );//Синус с угом паза расстояния от центра до дна паза ротора
               x[1]=val_abs_slot*qCos(slot_start_arg +G->rot_par.slot_arg );//Косинус с текщим угом + угол одного паза   расстояния от центра до дна паза ротора
               y[1]=val_abs_slot*qSin(slot_start_arg +G->rot_par.slot_arg );//Синус с текщим угом + угол одного паза   расстояния от центра до дна паза ротора
               x[2]=val_abs_pin *qCos(slot_start_arg +G->rot_par.slot_arg );//Косинус с текщим угом + угол одного паза   расстояния от центра до внешнего радиуса
               y[2]=val_abs_pin *qSin(slot_start_arg +G->rot_par.slot_arg );//Синус с текщим угом + угол одного паза   расстояния от центра до внешнего радиуса
               x[3]=val_abs_pin *qCos(slot_start_arg +G->rot_par.slot_arg +G->rot_par.pin_arg );//Косинус с текщим угом + угол одного паза   расстояния от центра до внешнего радиуса
               y[3]=val_abs_pin *qSin(slot_start_arg +G->rot_par.slot_arg +G->rot_par.pin_arg );//Синус с текщим угом + угол одного паза   расстояния от центра до внешнего радиуса
               //Вектор стали ротора
               rot_steel[slot_idx*4+0] = QCPCurveData(slot_idx*4+0,x[0],y[0]); //key - это массив x[], value - это массив y[]
               rot_steel[slot_idx*4+1] = QCPCurveData(slot_idx*4+1,x[1],y[1]);
               rot_steel[slot_idx*4+2] = QCPCurveData(slot_idx*4+2,x[2],y[2]);
               rot_steel[slot_idx*4+3] = QCPCurveData(slot_idx*4+3,x[3],y[3]);
               //----------------------------------------------------------------
               //присовоение нулевым элементам х и у косинуса и синуса текущего у
               x[0]=qCos(slot_start_arg );                                  y[0]=qSin(slot_start_arg );
               x[1]=qCos(slot_start_arg +G->rot_par.slot_arg);              y[1]=qSin(slot_start_arg +G->rot_par.slot_arg);

               v_abs = G->rot_par.R;//Вектор окружности ротора
               rotor_R [slot_idx*2]     = QCPCurveData(slot_idx*2,      x[0]*v_abs, y[0]*v_abs);
               rotor_R [slot_idx*2+1]   = QCPCurveData(slot_idx*2+1,    x[1]*v_abs, y[1]*v_abs);

                v_abs = G->rot_par.R - G->rot_par.h_wedge;//Вектор окружности клина
               rotor_w [slot_idx*2]     = QCPCurveData(slot_idx*2,      x[0]*v_abs, y[0]*v_abs);
               rotor_w [slot_idx*2+1]   = QCPCurveData(slot_idx*2+1,    x[1]*v_abs, y[1]*v_abs);

                //Вектор окружности слоев
               if (G->rot_par.n_layer >= 1)
               {
                   v_abs = G->rot_par.R - G->rot_par.h_wedge - G->rot_par.h_slot*1/G->rot_par.n_layer;
                   rotor_L1[slot_idx*2]     = QCPCurveData(slot_idx*2,    x[0]*v_abs, y[0]*v_abs);
                   rotor_L1[slot_idx*2+1]   = QCPCurveData(slot_idx*2+1,  x[1]*v_abs, y[1]*v_abs);
               }
               if (G->rot_par.n_layer >= 2)
               {
                   v_abs = G->rot_par.R - G->rot_par.h_wedge - G->rot_par.h_slot*2/G->rot_par.n_layer;
                   rotor_L2[slot_idx*2]     = QCPCurveData(slot_idx*2,    x[0]*v_abs, y[0]*v_abs);
                   rotor_L2[slot_idx*2+1]   = QCPCurveData(slot_idx*2+1,  x[1]*v_abs, y[1]*v_abs);
               }
               if (G->rot_par.n_layer >= 3)
               {
                   v_abs = G->rot_par.R - G->rot_par.h_wedge - G->rot_par.h_slot*3/G->rot_par.n_layer;
                   rotor_L3[slot_idx*2]     = QCPCurveData(slot_idx*2,    x[0]*v_abs, y[0]*v_abs);
                   rotor_L3[slot_idx*2+1]   = QCPCurveData(slot_idx*2+1,  x[1]*v_abs, y[1]*v_abs);
               }
               if (G->rot_par.n_layer >= 4)
               {
                   v_abs = G->rot_par.R - G->rot_par.h_wedge - G->rot_par.h_slot*4/G->rot_par.n_layer;
                   rotor_L4[slot_idx*2]     = QCPCurveData(slot_idx*2,    x[0]*v_abs, y[0]*v_abs);
                   rotor_L4[slot_idx*2+1]   = QCPCurveData(slot_idx*2+1,  x[1]*v_abs, y[1]*v_abs);
               }
               if (G->rot_par.n_layer >= 5)
               {
                   v_abs = G->rot_par.R - G->rot_par.h_wedge - G->rot_par.h_slot*5/G->rot_par.n_layer;
                   rotor_L5[slot_idx*2]     = QCPCurveData(slot_idx*2,    x[0]*v_abs, y[0]*v_abs);
                   rotor_L5[slot_idx*2+1]   = QCPCurveData(slot_idx*2+1,  x[1]*v_abs, y[1]*v_abs);
               }
               if (G->rot_par.n_layer >= 6)
               {
                   v_abs = G->rot_par.R - G->rot_par.h_wedge - G->rot_par.h_slot*6/G->rot_par.n_layer;
                   rotor_L6[slot_idx*2]     = QCPCurveData(slot_idx*2,    x[0]*v_abs, y[0]*v_abs);
                   rotor_L6[slot_idx*2+1]   = QCPCurveData(slot_idx*2+1,  x[1]*v_abs, y[1]*v_abs);
               }
               if (G->rot_par.n_layer >= 7)
               {
                   v_abs = G->rot_par.R - G->rot_par.h_wedge - G->rot_par.h_slot*7/G->rot_par.n_layer;
                   rotor_L7[slot_idx*2]     = QCPCurveData(slot_idx*2,    x[0]*v_abs, y[0]*v_abs);
                   rotor_L7[slot_idx*2+1]   = QCPCurveData(slot_idx*2+1,  x[1]*v_abs, y[1]*v_abs);
               }
               if (G->rot_par.n_layer >= 8)
               {
                   v_abs = G->rot_par.R - G->rot_par.h_wedge - G->rot_par.h_slot*8/G->rot_par.n_layer;
                   rotor_L8[slot_idx*2]     = QCPCurveData(slot_idx*2,    x[0]*v_abs, y[0]*v_abs);
                   rotor_L8[slot_idx*2+1]   = QCPCurveData(slot_idx*2+1,  x[1]*v_abs, y[1]*v_abs);
               }
               if (G->rot_par.n_layer == 9)
               {
                   v_abs = G->rot_par.R - G->rot_par.h_wedge - G->rot_par.h_slot*9/G->rot_par.n_layer;
                   rotor_L9[slot_idx*2]     = QCPCurveData(slot_idx*2,    x[0]*v_abs, y[0]*v_abs);
                   rotor_L9[slot_idx*2+1]   = QCPCurveData(slot_idx*2+1,  x[1]*v_abs, y[1]*v_abs);
               }
          }
   }
   GraphCircleEnd(G);

}

//Формирование кривой обмотки статора
void GenGraph2D::stator_coil_curve_Create       (GenGeom2D *G)
{

    int num_pole_stator_slots    =   G->stat_par.n_slot_pin/G->stat_par.n_pole; //n_slot_stator/num_poles;

    double slot_arg_stator = G->stat_par.slot_arg;//*3.1415926/180;
    double  pin_arg_stator = G->stat_par.pin_arg ;//*3.1415926/180;
    //======================================================================================
    int n_pole_pair = G->stat_par.n_pole/2;
    //Заполнение массивов обмотки A1, A2, B1, B2, C1, C2
    for(int pole=0;pole<n_pole_pair;pole++)//pole circle (для двухполюсного не имеет значения)
    {
         for(int slot=0;slot<G->stat_par.n_phase_turn;slot++)//pin circle
        {

            int slot_idx1_in  = CircleBuff(pole*n_pole_pair+slot, G->stat_par.n_slot_pin);
            int slot_idx1_out = CircleBuff(pole*n_pole_pair+slot+num_pole_stator_slots-G->stat_par.shift_coil, G->stat_par.n_slot_pin);

            int slot_idx2_in  = CircleBuff(pole*n_pole_pair+slot+G->stat_par.n_slot_pin-G->stat_par.shift_coil, G->stat_par.n_slot_pin);
            int slot_idx2_out = CircleBuff(pole*n_pole_pair+slot+num_pole_stator_slots, G->stat_par.n_slot_pin);

            double x[2];
            double y[2];
            double R_L1_stator = G->stat_par.R_in+G->stat_par.h_wedge+G->stat_par.h_slot*1/4.0;//середина верхнего слоя
            double R_L2_stator = G->stat_par.R_in+G->stat_par.h_wedge+G->stat_par.h_slot*3/4.0;//середина нижнего слоя
            //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            int slot_A1 = slot*2;
            int slot_X1 = slot*2+1;
            int slot_A1_idx = CircleBuff(G->stat_par.n_phase_turn*0+slot_idx1_in, G->stat_par.n_slot_pin);
            int slot_X1_idx = CircleBuff(G->stat_par.n_phase_turn*0+slot_idx1_out,G->stat_par.n_slot_pin);
            double      slot_start_arg_A1 = (slot_A1_idx)*(slot_arg_stator+pin_arg_stator);
            double      slot_start_arg_X1 = (slot_X1_idx)*(slot_arg_stator+pin_arg_stator);
            x[0]= qCos( slot_start_arg_A1 + slot_arg_stator/2);
            y[0]= qSin( slot_start_arg_A1 + slot_arg_stator/2);
            x[1]= qCos( slot_start_arg_X1 + slot_arg_stator/2);
            y[1]= qSin( slot_start_arg_X1 + slot_arg_stator/2);
            stator_coil_A1[slot_A1] = QCPCurveData(slot_A1,R_L1_stator*x[0],R_L1_stator*y[0]);
            stator_coil_A1[slot_X1] = QCPCurveData(slot_X1,R_L2_stator*x[1],R_L2_stator*y[1]);

            int slot_A2 = slot*2;
            int slot_X2 = slot*2+1;
            int slot_A2_idx = CircleBuff(G->stat_par.n_phase_turn*0+slot_idx2_in, G->stat_par.n_slot_pin);
            int slot_X2_idx = CircleBuff(G->stat_par.n_phase_turn*0+slot_idx2_out,G->stat_par.n_slot_pin);
            double      slot_start_arg_A2 = (slot_A2_idx)*(slot_arg_stator+pin_arg_stator);
            double      slot_start_arg_X2 = (slot_X2_idx)*(slot_arg_stator+pin_arg_stator);
            x[0]= qCos( slot_start_arg_A2 + slot_arg_stator/2);
            y[0]= qSin( slot_start_arg_A2 + slot_arg_stator/2);
            x[1]= qCos( slot_start_arg_X2 + slot_arg_stator/2);
            y[1]= qSin( slot_start_arg_X2 + slot_arg_stator/2);
            stator_coil_A2[slot_A2] = QCPCurveData(slot_A2,R_L2_stator*x[0],R_L2_stator*y[0]);
            stator_coil_A2[slot_X2] = QCPCurveData(slot_X2,R_L1_stator*x[1],R_L1_stator*y[1]);
            //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            int slot_B1 = slot*2;
            int slot_Y1 = slot*2+1;
            int slot_B1_idx = CircleBuff(G->stat_par.n_phase_turn*2+slot_idx1_in, G->stat_par.n_slot_pin);
            int slot_Y1_idx = CircleBuff(G->stat_par.n_phase_turn*2+slot_idx1_out,G->stat_par.n_slot_pin);
            double      slot_start_arg_B1 = (slot_B1_idx)*(slot_arg_stator+pin_arg_stator);
            double      slot_start_arg_Y1 = (slot_Y1_idx)*(slot_arg_stator+pin_arg_stator);
            x[0]= qCos( slot_start_arg_B1 + slot_arg_stator/2);
            y[0]= qSin( slot_start_arg_B1 + slot_arg_stator/2);
            x[1]= qCos( slot_start_arg_Y1 + slot_arg_stator/2);
            y[1]= qSin( slot_start_arg_Y1 + slot_arg_stator/2);
            stator_coil_B1[slot_B1] = QCPCurveData(slot_B1,R_L1_stator*x[0],R_L1_stator*y[0]);
            stator_coil_B1[slot_Y1] = QCPCurveData(slot_Y1,R_L2_stator*x[1],R_L2_stator*y[1]);

            int slot_B2 = slot*2;
            int slot_Y2 = slot*2+1;
            int slot_B2_idx = CircleBuff(G->stat_par.n_phase_turn*2+slot_idx2_in, G->stat_par.n_slot_pin);
            int slot_Y2_idx = CircleBuff(G->stat_par.n_phase_turn*2+slot_idx2_out,G->stat_par.n_slot_pin);
            double      slot_start_arg_B2 = (slot_B2_idx)*(slot_arg_stator+pin_arg_stator);
            double      slot_start_arg_Y2 = (slot_Y2_idx)*(slot_arg_stator+pin_arg_stator);
            x[0]= qCos( slot_start_arg_B2 + slot_arg_stator/2);
            y[0]= qSin( slot_start_arg_B2 + slot_arg_stator/2);
            x[1]= qCos( slot_start_arg_Y2 + slot_arg_stator/2);
            y[1]= qSin( slot_start_arg_Y2 + slot_arg_stator/2);
            stator_coil_B2[slot_B2] = QCPCurveData(slot_B2,R_L2_stator*x[0],R_L2_stator*y[0]);
            stator_coil_B2[slot_Y2] = QCPCurveData(slot_Y2,R_L1_stator*x[1],R_L1_stator*y[1]);
            //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            int slot_C1 = slot*2;
            int slot_Z1 = slot*2+1;
            int slot_C1_idx = CircleBuff(G->stat_par.n_phase_turn*4+slot_idx1_in, G->stat_par.n_slot_pin);
            int slot_Z1_idx = CircleBuff(G->stat_par.n_phase_turn*4+slot_idx1_out, G->stat_par.n_slot_pin);
            double      slot_start_arg_C1 = (slot_C1_idx)*(slot_arg_stator+pin_arg_stator);
            double      slot_start_arg_Z1 = (slot_Z1_idx)*(slot_arg_stator+pin_arg_stator);
            x[0]= qCos( slot_start_arg_C1 + slot_arg_stator/2);
            y[0]= qSin( slot_start_arg_C1 + slot_arg_stator/2);
            x[1]= qCos( slot_start_arg_Z1 + slot_arg_stator/2);
            y[1]= qSin( slot_start_arg_Z1 + slot_arg_stator/2);
            stator_coil_C1[slot_C1] = QCPCurveData(slot_C1,R_L1_stator*x[0],R_L1_stator*y[0]);
            stator_coil_C1[slot_Z1] = QCPCurveData(slot_Z1,R_L2_stator*x[1],R_L2_stator*y[1]);

            int slot_C2 = slot*2;
            int slot_Z2 = slot*2+1;
            int slot_C2_idx = CircleBuff(G->stat_par.n_phase_turn*4+slot_idx2_in, G->stat_par.n_slot_pin);
            int slot_Z2_idx = CircleBuff(G->stat_par.n_phase_turn*4+slot_idx2_out, G->stat_par.n_slot_pin);
            double      slot_start_arg_C2 = (slot_C2_idx)*(slot_arg_stator+pin_arg_stator);
            double      slot_start_arg_Z2 = (slot_Z2_idx)*(slot_arg_stator+pin_arg_stator);
            x[0]= qCos( slot_start_arg_C2 + slot_arg_stator/2);
            y[0]= qSin( slot_start_arg_C2 + slot_arg_stator/2);
            x[1]= qCos( slot_start_arg_Z2 + slot_arg_stator/2);
            y[1]= qSin( slot_start_arg_Z2 + slot_arg_stator/2);
            stator_coil_C2[slot_C2] = QCPCurveData(slot_C2,R_L2_stator*x[0],R_L2_stator*y[0]);
            stator_coil_C2[slot_Z2] = QCPCurveData(slot_Z2,R_L1_stator*x[1],R_L1_stator*y[1]);
            //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            stat_coil_data [slot_A1_idx]                          = set_coil_data(slot_A1_idx, 'A', 1, slot + 1, 0, 1);
            stat_coil_data [G->stat_par.n_slot_pin + slot_A2_idx]  = set_coil_data(slot_A2_idx, 'A', 2, slot + 1, 0, 2);
            stat_coil_data [slot_B1_idx]                          = set_coil_data(slot_B1_idx, 'B', 1, slot + 1, 0, 1);
            stat_coil_data [G->stat_par.n_slot_pin + slot_B2_idx]  = set_coil_data(slot_B2_idx, 'B', 2, slot + 1, 0, 2);
            stat_coil_data [slot_C1_idx]                          = set_coil_data(slot_C1_idx, 'C', 1, slot + 1, 0, 1);
            stat_coil_data [G->stat_par.n_slot_pin + slot_C2_idx]  = set_coil_data(slot_C2_idx, 'C', 2, slot + 1, 0, 2);

            stat_coil_data [G->stat_par.n_slot_pin + slot_X1_idx]  = set_coil_data(slot_X1_idx, 'X', 1, slot + 1, 0, 2);
            stat_coil_data [+ slot_X2_idx]                        = set_coil_data(slot_X2_idx, 'X', 2, slot + 1, 0, 1);
            stat_coil_data [G->stat_par.n_slot_pin + slot_Y1_idx]  = set_coil_data(slot_Y1_idx, 'Y', 1, slot + 1, 0, 2);
            stat_coil_data [+ slot_Y2_idx]                        = set_coil_data(slot_Y2_idx, 'Y', 2, slot + 1, 0, 1);
            stat_coil_data [G->stat_par.n_slot_pin + slot_Z1_idx]  = set_coil_data(slot_Z1_idx, 'Z', 1, slot + 1, 0, 2);
            stat_coil_data [slot_Z2_idx]                          = set_coil_data(slot_Z2_idx, 'Z', 2, slot + 1, 0, 1);
        }
    }
}

//Формирование кривой обмотки ротора
void GenGraph2D::rotor_coil_curve_Create       (GenGeom2D *G, GenGrid2D *GenGr, double arg_beg)
{
    double x[4];
    double y[4];
    double v_abs;
    SlotRotPos           (G);

    int turn_num_start;
    int slot_num_start;

    int turn_num_end;
    int slot_num_end;
    int coil_layer=0;
    double val;

    for (int idx=0; idx < G->rot_par.n_turn_pole*2; idx += G->rot_par.n_layer)
    {
        turn_num_start        = rot_coil_data[idx].turn;
        slot_num_start        = rot_coil_data[idx].sector;

        for(int i=0; i < G->rot_par.n_turn_pole*2; i++)
        {
            if (rot_coil_data[i].turn == turn_num_start +1)
            {
                turn_num_end        = rot_coil_data[i].turn;
                slot_num_end        = rot_coil_data[i].sector;
                break;
            }
        }

        double slot_start_arg = slot_num_start*(G->rot_par.slot_arg+G->rot_par.pin_arg);
        double slot_stop_arg  = slot_num_end  *(G->rot_par.slot_arg+G->rot_par.pin_arg);
        //start slot ray
        x[0]=qCos(slot_start_arg +G->rot_par.slot_arg/2 + arg_beg);
        y[0]=qSin(slot_start_arg +G->rot_par.slot_arg/2 + arg_beg);
        //stop  slot ray
        x[1]=qCos(slot_stop_arg  +G->rot_par.slot_arg/2 + arg_beg);
        y[1]=qSin(slot_stop_arg  +G->rot_par.slot_arg/2 + arg_beg);
        v_abs = GenGr->rotor_grid_par.R_start+GenGr->rotor_grid_par.dh_level[0];
        if (turn_num_start == 0|| ((turn_num_end+1) % G->rot_par.n_coil_slot == 0 ))
            {   coil_layer++;
                v_abs += GenGr->rotor_grid_par.dh_level[coil_layer+1];
                val = v_abs - GenGr->rotor_grid_par.dh_level[coil_layer+1]/2;
            }
        rotor_coil[turn_num_start]     = QCPCurveData(turn_num_start,   x[0]*val, y[0]*val);
        rotor_coil[turn_num_end]       = QCPCurveData(turn_num_end,     x[1]*val, y[1]*val);
    }
}

//Формирование вектора точек сетки статора
void GenGraph2D::GridPoint_stator(GenGrid2D *GenGr)
{
    for(int i=0;i<GenGr->stat_grid_pos.size();i++)//Цикл по колличеству полюсов
        {
            if      (GenGr->stat_grid_pos[i].EH == 1)
                QV_stator_grid_Ez.push_back(QCPCurveData(GenGr->stat_grid_pos[i].sector, GenGr->stat_grid_pos[i].x, GenGr->stat_grid_pos[i].y));
            else if (GenGr->stat_grid_pos[i].EH == 2)
                QV_stator_grid_Hx.push_back(QCPCurveData(GenGr->stat_grid_pos[i].sector, GenGr->stat_grid_pos[i].x, GenGr->stat_grid_pos[i].y));
            else if (GenGr->stat_grid_pos[i].EH == 3)
                QV_stator_grid_Hy.push_back(QCPCurveData(GenGr->stat_grid_pos[i].sector, GenGr->stat_grid_pos[i].x, GenGr->stat_grid_pos[i].y));
        }
}

//Формирование вектора точек сетки ротора
void GenGraph2D::GridPoint_rotor( GenGrid2D *GenGr)
{
    for(int i=0;i<GenGr->rot_grid_pos.size();i++)//Цикл по колличеству полюсов
        {
            if      (GenGr->rot_grid_pos[i].EH == 1)
                QV_rotor_grid_Ez.push_back(QCPCurveData(GenGr->rot_grid_pos[i].sector, GenGr->rot_grid_pos[i].x, GenGr->rot_grid_pos[i].y));
            else if (GenGr->rot_grid_pos[i].EH == 2)
                QV_rotor_grid_Hx.push_back(QCPCurveData(GenGr->rot_grid_pos[i].sector, GenGr->rot_grid_pos[i].x, GenGr->rot_grid_pos[i].y));
            else if (GenGr->rot_grid_pos[i].EH == 3)
                QV_rotor_grid_Hy.push_back(QCPCurveData(GenGr->rot_grid_pos[i].sector, GenGr->rot_grid_pos[i].x, GenGr->rot_grid_pos[i].y));
        }
}

//Формирование массивов связи обмотки ротора
void GenGraph2D::SlotRotPos           ( GenGeom2D *G)
{
    int s_idx=0;

    for (int idx=0; idx < G->rot_par.n_turn_pole*2; idx += G->rot_par.n_layer)
    {
        for (int layer_idx=0; layer_idx < G->rot_par.n_layer; layer_idx++)
        {
         rot_coil_data[idx+layer_idx].slot = s_idx;
         rot_coil_data[idx+layer_idx].layer= layer_idx+1;
         if (s_idx < G->rot_par.n_coil_slot_pole)
            rot_coil_data[idx+layer_idx].sector = s_idx;
         if (s_idx >= G->rot_par.n_coil_slot_pole)
             rot_coil_data[idx+layer_idx].sector = s_idx + G->rot_par.wedge_pole_num;
        }
        s_idx ++;
    }
    //Заполнение витков
    s_idx=0;
    for (int idx=0;idx < G->rot_par.n_turn_pole*2;idx += G->rot_par.n_layer)
    {
        if (idx < G->rot_par.n_coil_slot_pole * G->rot_par.n_layer)
        {
            int gain_slot = 1;
            for (int layer_idx=0; layer_idx < G->rot_par.n_layer; layer_idx++)
            {
             if ( layer_idx == 0 )
                rot_coil_data[idx+layer_idx].turn = s_idx * 2;
                     else if (layer_idx % 2 == 1 )
                         rot_coil_data[idx+layer_idx].turn = gain_slot*2*G->rot_par.n_coil_slot - 2 - s_idx*2;
                     else if (layer_idx % 2 == 0 )
                         {
                             rot_coil_data[idx+layer_idx].turn = gain_slot*2*G->rot_par.n_coil_slot + s_idx*2 ;
                             gain_slot++;
                         }
             }
         }
        else
        {
            for (int layer_idx=0; layer_idx < G->rot_par.n_layer; layer_idx++)
            {
                 rot_coil_data[s_idx*G->rot_par.n_layer+layer_idx].turn = rot_coil_data[idx - G->rot_par.n_turn_pole + layer_idx].turn +1;
             }
        }
        s_idx ++;
    }
}

//Функция замыкания всех кривых генератора
void GenGraph2D::GraphCircleEnd(GenGeom2D *G)
{

    rotor_R [G->rot_par.n_slot_pin*2]       = rotor_R [0];//QCPCurveData(G->rot_par.n_slot_pin, G->rot_par.R, 0);
    rotor_w [G->rot_par.n_slot_pin*2]         = rotor_w [0]; //QCPCurveData(G->rot_par.n_slot_pin, G->rot_par.R - G->rot_par.h_wedge, 0);
    rot_steel[G->rot_par.n_slot_pin*4]      = rot_steel[0];

    if (G->rot_par.n_layer >= 1)
    {
     rotor_L1 [G->rot_par.n_slot_pin*2]   = rotor_L1 [0];
    }
    if (G->rot_par.n_layer >= 2)
    {
     rotor_L2 [G->rot_par.n_slot_pin*2]   = rotor_L2 [0];
    }
    if (G->rot_par.n_layer >= 3)
    {
     rotor_L3 [G->rot_par.n_slot_pin*2]   = rotor_L3 [0];
    }
    if (G->rot_par.n_layer >= 4)
    {
     rotor_L4 [G->rot_par.n_slot_pin*2]   = rotor_L4 [0];
    }
    if (G->rot_par.n_layer >= 5)
    {
     rotor_L5 [G->rot_par.n_slot_pin*2]   = rotor_L5 [0];
    }
    if (G->rot_par.n_layer >= 6)
    {
     rotor_L6 [G->rot_par.n_slot_pin*2]   = rotor_L6 [0];
    }
    if (G->rot_par.n_layer >= 7)
    {
     rotor_L7 [G->rot_par.n_slot_pin*2]   = rotor_L7 [0];
    }
    if (G->rot_par.n_layer >= 8)
    {
     rotor_L8 [G->rot_par.n_slot_pin*2]   = rotor_L8 [0];
    }
    if (G->rot_par.n_layer >= 9)
    {
     rotor_L9 [G->rot_par.n_slot_pin*2]   = rotor_L9 [0];
    }
}

//Функция кругового буффера
int GenGraph2D::CircleBuff(int Val, int maxCircle_num)
{
    int Val_new = Val;

    if (Val >= maxCircle_num ) Val_new = Val -  maxCircle_num;

    return Val_new;
}

//Задание размерности массивов кривых
void GenGraph2D::GenGraphCurveSize(GenGeom2D *G)
{
    rot_steel.resize(G->rot_par.n_slot_pin*4+1);
    stator_steel.resize(G->stat_par.n_slot_pin*4+1);
    gener_total.resize(G->stat_par.n_slot_pin*2+1);

    //окружности +1, чтобы замкнуть
    rotor_R.resize (G->rot_par.n_slot_pin*2+1);
    rotor_w.resize (G->rot_par.n_slot_pin*2+1);
    rotor_L1.resize(G->rot_par.n_slot_pin*2+1);
    rotor_L2.resize(G->rot_par.n_slot_pin*2+1);
    rotor_L3.resize(G->rot_par.n_slot_pin*2+1);
    rotor_L4.resize(G->rot_par.n_slot_pin*2+1);
    rotor_L5.resize(G->rot_par.n_slot_pin*2+1);
    rotor_L6.resize(G->rot_par.n_slot_pin*2+1);
    rotor_L7.resize(G->rot_par.n_slot_pin*2+1);
    rotor_L8.resize(G->rot_par.n_slot_pin*2+1);
    rotor_L9.resize(G->rot_par.n_slot_pin*2+1);

    //окружности +1, чтобы замкнуть
    stator_R.resize (G->stat_par.n_slot_pin*2+1);
    stator_w.resize (G->stat_par.n_slot_pin*2+1);
    stator_L1.resize(G->stat_par.n_slot_pin*2+1);
    stator_L2.resize(G->stat_par.n_slot_pin*2+1);

    rotor_coil.resize(G->rot_par.n_turn_pole*2);

    stator_coil_A1.resize(G->stat_par.n_phase_turn*2);
    stator_coil_A2.resize(G->stat_par.n_phase_turn*2);

    stator_coil_B1.resize(G->stat_par.n_phase_turn*2);
    stator_coil_B2.resize(G->stat_par.n_phase_turn*2);

    stator_coil_C1.resize(G->stat_par.n_phase_turn*2);
    stator_coil_C2.resize(G->stat_par.n_phase_turn*2);

    stat_coil_data.resize(G->stat_par.n_slot_pin*2);
    rot_coil_data.resize (G->rot_par.n_turn_pole*2);
}

GenGraph2D::~GenGraph2D()
{
  /*
    delete []     gener_total_curve;
    delete []     stator_steel_curve;
    delete []     rotor_steel_curve;

    delete []     stator_coil_A1_curve;
    delete []     stator_coil_B1_curve;
    delete []     stator_coil_C1_curve;

    delete []     stator_coil_A2_curve;
    delete []     stator_coil_B2_curve;
    delete []     stator_coil_C2_curve;

    delete []     rotor_coil_curve;

    delete []     stator_R_curve;
    delete []     stator_w_curve;
    delete []     stator_L1_curve;
    delete []     stator_L2_curve;

    delete []     rotor_R_curve;
    delete []     rotor_w_curve;
    delete []     stator_grid_curve;
    delete []     rotor_grid_curve;
    delete []     rotor_L1_curve;
    delete []     rotor_L2_curve;
    delete []     rotor_L3_curve;
    delete []     rotor_L4_curve;
    delete []     rotor_L5_curve;
    delete []     rotor_L6_curve;
    delete []     rotor_L7_curve;
    delete []     rotor_L8_curve;
    delete []     rotor_L9_curve;
*/
    /*
    //Пример выполнение двумерного вектора
    int Ni = 20;
    int Nj = 50;

    QVector<double> E(Ni*Nj);

    for (int i=0;i<Ni;i++) //ray = 0 - заполнение точками паза, ray = 1 - заполнение точками зубца
    {
        for (int j=0;j<Nj;j++)    //ray level circle
        {
            E[j+i*Nj] = i*j ;
        }
    }
    int check = 0;
    */
}

//-------------------------------------------------------------------------------------------
