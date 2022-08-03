#ifndef OUTTEXT_HPP
#define OUTTEXT_HPP
#include "Header.hpp"

void ArrOutText (std::string strPath, std::string base, int N, int M, int Np_s, int Np_p ,QVector<material_par> &arr_out);
void ArrOutText (std::string strPath, std::string base, int N, int M, int Np_s, int Np_p ,double *arr_out);
void ArrOutText (std::string strPath, std::string base, int N, int M, int Np_s, int Np_p ,QVector<double> &arr_out);


#endif // OUTTEXT_HPP
