/**
 *  Copyright (c) 1982 - 1989 Donald H. House <x@unknown.com>
 *  Copyright (c) 1989 Robert Allen <x@unknown.com>
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "gauss.h"
#include <math.h>
#include <stdlib.h>
#include <QtGlobal>

#ifdef Q_OS_WIN
// quoting DRAND48(3) man-page:
// These functions are declared obsolete by  SVID  3,
// which  states  that rand(3) should be used instead.
#define srand48 srand
#define drand48() (static_cast<double>(qrand()) / static_cast<double>(RAND_MAX))
#endif

double gauss::gaussian(double mean, double std, int seed)
{

#define itblmax 20 /* length - 1 of table describing F inverse */
#define didu 40.0 /* delta table position / delta ind. variable */
    /* itblmax / 0.5 */

    /* interpolation table for F inverse */
    static double tbl[] = {0.00000E+00, 6.27500E-02, 1.25641E-01, 1.89000E-01,
                           2.53333E-01, 3.18684E-01, 3.85405E-01, 4.53889E-01,
                           5.24412E-01, 5.97647E-01, 6.74375E-01, 7.55333E-01,
                           8.41482E-01, 9.34615E-01, 1.03652E+00, 1.15048E+00,
                           1.28167E+00, 1.43933E+00, 1.64500E+00, 1.96000E+00,
                           3.87000E+00
                          };

    static int first_time = 1;

    double u;
    double di;
    int index, minus;
    double delta, gaussian_random_value;

    if (first_time) {
        srand48(seed);
        first_time = 0;
    }
    /*
      compute uniform random number between 0.0 - 0.5, and a sign with
      probability 1/2 of being either + or -
    */
    u = drand48();
    if (u >= 0.5) {
        minus = 0;
        u = u - 0.5;
    }
    else {
        minus = 1;
    }
    /* interpolate gaussian random number using table */

    index = (int)(di = (didu * u));
    if (index == itblmax) {
        delta = tbl[index];
    }
    else {
        di -= index;
        delta =  tbl[index] + (tbl[index + 1] - tbl[index]) * di;
    }
    gaussian_random_value = mean + std * (minus ? -delta : delta);

    return(gaussian_random_value);
}
