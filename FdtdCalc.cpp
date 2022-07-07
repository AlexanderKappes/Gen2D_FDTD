#include "Header.hpp"

Fdtd_calc::Fdtd_calc(int in_XSize, int in_YSize, int in_ZSize, double in_TX, double in_TY, double in_TZ, int    in_NT, double in_To, double in_dT)
{
    //Размер счетного пространства
    XSize = in_XSize;
    YSize = in_YSize;
    ZSize = in_ZSize;
    //Коэффициенты
    TX = in_TX; //1/dX
    TY = in_TY; //1/dY
    TZ = in_TZ; //1/dZ

    NT = in_NT;
    To = in_To;//Исходное значение времени
    dT = in_dT;

}

//Определение магнитного поля Н
void Fdtd_calc::TimeStepForH (  int      XSize,  int      YSize, int     ZSize,
                                double   TX,     double   TY,    double  TZ,
                                double   ***HX,  double   ***HY, double  ***HZ,
                                double   ***EX,  double   ***EY, double  ***EZ,
                                double   ***AH)
{

}

//Определение электрического поля Е
void Fdtd_calc::TimeStepForE (  int      XSize,  int      YSize, int     ZSize,
                                double   TX,     double   TY,    double  TZ,
                                double   ***HX,  double   ***HY, double  ***HZ,
                                double   ***EX,  double   ***EY, double  ***EZ,
                                double   ***BE,  double   ***AE)
{

}
