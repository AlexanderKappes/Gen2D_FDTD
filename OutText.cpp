#include "OutText.hpp"


void ArrOutText (std::string strPath, std::string base, int N, int M, int Np_s, int Np_p, QVector<material_par> &arr_out)
{

    double temp;
    char filename[100]= {0};
    char basename[80]= {0};
    char c_Path [80] = {0};
    FILE *out;
    std::string str_out;

    for (int i = 0; i < 80; i ++)
        basename[i] = base[i]; // store data as a float
    for (std::string::size_type i = 0; i < strPath.length (); i ++) // строковый тип в тип char []
        {
            c_Path[i]=strPath[i];
        }

    sprintf(filename, "%s%s.txt", c_Path, basename);//s -string, d - double
    out = fopen(filename, "w");

    str_out = std::to_string(M) + " ";
    fputs(str_out.c_str(), out);
    str_out = std::to_string(N) + "\n";
    fputs(str_out.c_str(), out);

    int p = 0;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            //temp = arr_out[j + i*M].mu;
            //temp = arr_out[j + i*M].epsilon;
            temp = arr_out[j + i*M].sigma;
            str_out = std::to_string(temp) + "\t";
            fputs(str_out.c_str(), out);
        }
        fputs("\n", out);
        p++;

        if (p == Np_s)
        {
            fputs("\n", out);
        }

        if (p == Np_p + Np_s)
        {
            fputs("\n", out);
            p = 0;
        }
    }
    fclose(out); // close file
}

void ArrOutText (std::string strPath, std::string base, int N, int M, int Np_s, int Np_p, double *arr_out)
{

    double temp;
    char filename[100]= {0};
    char basename[80]= {0};
    char c_Path [80] = {0};
    FILE *out;
    std::string str_out;

    for (int i = 0; i < 80; i ++)
        basename[i] = base[i]; // store data as a float
    for (std::string::size_type i = 0; i < strPath.length (); i ++) // строковый тип в тип char []
        {
            c_Path[i]=strPath[i];
        }

    sprintf(filename, "%s%s.txt", c_Path, basename);//s -string, d - double
    out = fopen(filename, "w");

    str_out = std::to_string(M) + " ";
    fputs(str_out.c_str(), out);
    str_out = std::to_string(N) + "\n";
    fputs(str_out.c_str(), out);

    int p = 0;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            temp = arr_out[j + i*M];
            str_out = std::to_string(temp*1000) + "\t";
            fputs(str_out.c_str(), out);
        }
        fputs("\n", out);
        p++;

        if (p == Np_s)
        {
            fputs("\n", out);
        }

        if (p == Np_p + Np_s)
        {
            fputs("\n", out);
            p = 0;
        }
    }
    fclose(out); // close file
}

void ArrOutText (std::string strPath, std::string base, int N, int M, int Np_s, int Np_p, QVector<double> &arr_out)
{

    double temp;
    char filename[100]= {0};
    char basename[80]= {0};
    char c_Path [80] = {0};
    FILE *out;
    std::string str_out;

    for (int i = 0; i < 80; i ++)
        basename[i] = base[i]; // store data as a float
    for (std::string::size_type i = 0; i < strPath.length (); i ++) // строковый тип в тип char []
        {
            c_Path[i]=strPath[i];
        }

    sprintf(filename, "%s%s.txt", c_Path, basename);//s -string, d - double
    out = fopen(filename, "w");

    str_out = std::to_string(M) + " ";
    fputs(str_out.c_str(), out);
    str_out = std::to_string(N) + "\n";
    fputs(str_out.c_str(), out);
    fputs("\n", out);

    int p = 0;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            temp = arr_out[j + i*M];
            str_out = std::to_string(temp) + "\t";
            fputs(str_out.c_str(), out);
        }
        fputs("\n", out);
        p++;

        if (p == Np_s)
        {
            fputs("\n", out);
        }

        if (p == Np_p + Np_s)
        {
            fputs("\n", out);
            p = 0;
        }
    }
    fclose(out); // close file
}
