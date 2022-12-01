#include "OutText.hpp"


void ArrOutText (std::string strPath, std::string base, int N, int M, int Np_s, int Np_p, QVector<material_par> &arr_out)
{

    double temp_mu, temp_epsilon, temp_sigma;
    char filename_mu[100]= {0};
    char filename_epsilon[100]= {0};
    char filename_sigma[100]= {0};
    char basename[80]= {0};
    char c_Path [80] = {0};
    FILE *out_mu;
    FILE *out_epsilon;
    FILE *out_sigma;
    std::string str_out_mu;
    std::string str_out_epsilon;
    std::string str_out_sigma;

    for (int i = 0; i < 80; i ++)
        basename[i] = base[i]; // store data as a float
    for (std::string::size_type i = 0; i < strPath.length (); i ++) // строковый тип в тип char []
        {
            c_Path[i]=strPath[i];
        }

    sprintf(filename_mu, "%s%s_mu.txt", c_Path, basename);//s -string, d - double
    out_mu = fopen(filename_mu, "w");

    sprintf(filename_epsilon, "%s%s_epsilon.txt", c_Path, basename);//s -string, d - double
    out_epsilon = fopen(filename_epsilon, "w");

    sprintf(filename_sigma, "%s%s_sigma.txt", c_Path, basename);//s -string, d - double
    out_sigma = fopen(filename_sigma, "w");

    str_out_mu = std::to_string(M) + " ";
    fputs(str_out_mu.c_str(), out_mu);
    str_out_mu = std::to_string(N) + "\n";
    fputs(str_out_mu.c_str(), out_mu);

    str_out_epsilon = std::to_string(M) + " ";
    fputs(str_out_epsilon.c_str(), out_epsilon);
    str_out_epsilon = std::to_string(N) + "\n";
    fputs(str_out_epsilon.c_str(), out_epsilon);

    str_out_sigma = std::to_string(M) + " ";
    fputs(str_out_sigma.c_str(), out_sigma);
    str_out_sigma = std::to_string(N) + "\n";
    fputs(str_out_sigma.c_str(), out_sigma);

    int p = 0;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            temp_mu = arr_out[j + i*M].mu;
            temp_epsilon = arr_out[j + i*M].epsilon;
            temp_sigma = arr_out[j + i*M].sigma;
            if (j != (M-1))
            {
                str_out_mu = std::to_string(temp_mu) + "\t";
                str_out_epsilon = std::to_string(temp_epsilon) + "\t";
                str_out_sigma = std::to_string(temp_sigma) + "\t";
            }
            else
            {
                str_out_mu = std::to_string(temp_mu);
                str_out_epsilon = std::to_string(temp_epsilon);
                str_out_sigma = std::to_string(temp_sigma);
            }

            fputs(str_out_mu.c_str(), out_mu);
            fputs(str_out_epsilon.c_str(), out_epsilon);
            fputs(str_out_sigma.c_str(), out_sigma);
        }
        fputs("\n", out_mu);
        fputs("\n", out_epsilon);
        fputs("\n", out_sigma);
        p++;

        if (p == Np_s)
        {
            fputs("\n", out_mu);
            fputs("\n", out_epsilon);
            fputs("\n", out_sigma);
        }

        if (p == Np_p + Np_s)
        {
            fputs("\n", out_mu);
            fputs("\n", out_epsilon);
            fputs("\n", out_sigma);
            p = 0;
        }
    }
    fclose(out_mu); // close file
    fclose(out_epsilon); // close file
    fclose(out_sigma); // close file
}

void ArrOutText (std::string strPath, std::string base, int N, int M, int Np_s, int Np_p, QVector<grid_data> &arr_out, int gd)
{

    double temp;
    char filename[100]= {0};
    char basename[80]= {0};
    char c_Path [80] = {0};
    FILE *out;
    std::string str_out;
    char name_gd[10];
    // 1 - x;
    // 2 - y;
    // 3 - sector;
    // 4 - point_num;
    // 5 - EH;
    // 6 - material;
    // 7 - source;
    // 8 - EHnumX;
    // 9 - EHnumY;
    switch (gd) {
        case 1:
            strcpy(name_gd, "x");
        break;
        case 2:
            strcpy(name_gd, "y");
        break;
        case 3:
            strcpy(name_gd, "sector");
        break;
        case 4:
            strcpy(name_gd, "point_num");
        break;
        case 5:
            strcpy(name_gd, "EH");
        break;
        case 6:
            strcpy(name_gd, "material");
        break;
        case 7:
            strcpy(name_gd, "source");
        break;
        case 8:
            strcpy(name_gd, "EHnumX");
        break;
        case 9:
            strcpy(name_gd, "EHnumY");
        break;
        default:
            strcpy(name_gd, "EH");
    }

    for (int i = 0; i < 80; i ++)
        basename[i] = base[i]; // store data as a float
    for (std::string::size_type i = 0; i < strPath.length (); i ++) // строковый тип в тип char []
        {
            c_Path[i]=strPath[i];
        }

    sprintf(filename, "%s%s_%s.txt", c_Path, basename, name_gd);//s -string, d - double
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

            switch (gd) {
                case 1:
                    temp = arr_out[j + i*M].x;
                break;
                case 2:
                    temp = arr_out[j + i*M].y;
                break;
                case 3:
                    temp = arr_out[j + i*M].source;
                break;
                case 4:
                    temp = arr_out[j + i*M].point_num;
                break;
                case 5:
                    temp = arr_out[j + i*M].EH;
                break;
                case 6:
                    temp = arr_out[j + i*M].material;
                break;
                case 7:
                    temp = arr_out[j + i*M].source;
                break;
                case 8:
                    temp = arr_out[j + i*M].EHnumX;
                break;
                case 9:
                    temp = arr_out[j + i*M].EHnumY;
                break;
                default:
                    temp = arr_out[j + i*M].EH;
            }

            if (j != (M-1))
                str_out = std::to_string(temp) + "\t";
            else
                str_out = std::to_string(temp);

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

void ArrOutText (std::string strPath, std::string base, int N, QVector<join_grid_data> &arr_out)
{

    char filename[100]= {0};
    char basename[80]= {0};
    char c_Path [80] = {0};
    FILE *out;
    std::string str_out;
    // 1 - x;
    // 2 - y;
    // 3 - Fi_j;
    // 4 - arg;
    // 5 - arg1;
    // 6 - arg2;
    // 7 - dx;
    // 8 - dy;
    // 9 - val;



    for (int i = 0; i < 80; i ++)
        basename[i] = base[i]; // store data as a float
    for (std::string::size_type i = 0; i < strPath.length (); i ++) // строковый тип в тип char []
        {
            c_Path[i]=strPath[i];
        }

    sprintf(filename, "%s%s.txt", c_Path, basename);//s -string, d - double
    out = fopen(filename, "w");

    str_out = std::to_string(N) + "\n";
    fputs(str_out.c_str(), out);

    int p = 0;

    std::string str_out_x, str_out_y, str_out_Fi_j, str_out_arg,
                str_out_arg1, str_out_arg2, str_out_dx, str_out_dy, str_out_val;



    str_out = std::string("x") + "\t" + "\t" + "\t" + std::string("y") +"\t"+ "\t" + "\t" + std::string("Fi_j") +"\t" +"\t" + std::string("arg") +"\t" + "\t" + "\t" + std::string("arg1") + "\t" + "\t" + std::string("arg2") + "\t" + "\t" + std::string("dx") + "\t" + "\t" + "\t" + std::string("dy") + "\t" + "\t" + "\t" + std::string("val");
    fputs(str_out.c_str(), out);
    fputs("\n", out);

    for (int i = 0; i < N; i++)
    {
        str_out_x = std::to_string(arr_out[i].x);
        str_out_y = std::to_string(arr_out[i].y);
        str_out_Fi_j = std::to_string(arr_out[i].Fi_j);
        str_out_arg = std::to_string(arr_out[i].arg);
        str_out_arg1 = std::to_string(arr_out[i].arg1);
        str_out_arg2 = std::to_string(arr_out[i].arg2);
        str_out_dx = std::to_string(arr_out[i].dx);
        str_out_dy = std::to_string(arr_out[i].dy);
        str_out_val = std::to_string(arr_out[i].val);

        str_out = str_out_x +"\t" + str_out_y +"\t" + str_out_Fi_j +"\t" + str_out_arg +"\t" + str_out_arg1 +"\t" + str_out_arg2 +"\t" + str_out_dx +"\t" + str_out_dy +"\t" + str_out_val;
        fputs(str_out.c_str(), out);
        fputs("\n", out);
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
            if (j != (M-1))
                str_out = std::to_string(temp) + "\t";
            else
                str_out = std::to_string(temp);

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
            if (j != (M-1))
                str_out = std::to_string(temp) + "\t";
            else
                str_out = std::to_string(temp);

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
