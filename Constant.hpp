#ifndef CONSTANT_H
#define CONSTANT_H
#include <cmath>

#ifndef c_light //
#define c_light 2.997925 * pow(10.0,8.0)
#endif

#ifndef epsilon_o //
#define epsilon_o pow(10.0,7.0)/(4.0*M_PI*pow(c_light,2.0))
#endif

#ifndef nu_o //
#define nu_o 4.0*M_PI/pow(10.0,7.0)
#endif

#endif // CONSTANT_H
