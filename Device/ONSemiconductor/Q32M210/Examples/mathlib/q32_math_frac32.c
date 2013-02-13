/* ----------------------------------------------------------------------------
 * Copyright (c) 2009 - 2012 Semiconductor Components Industries, LLC (d/b/a ON
 * Semiconductor), All Rights Reserved
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * q32_math_frac32.c
 * - Math library fixed-point functions source file
 * ----------------------------------------------------------------------------
 * $Revision: 1.4 $
 * $Date: 2012/04/26 17:58:46 $
 * ------------------------------------------------------------------------- */

#include <stdint.h>
#include <q32_math.h>

/* ----------------------------------------------------------------------------
 * Function      : int Math_Mult_frac32(int32_t x, int32_t y)
 * ----------------------------------------------------------------------------
 * Description   : Multiply two 32-bit signed fractional numbers, then saturate.
 *                 The result is either:
 *                 -   x * y,      if x > MIN_FRAC32 or y >  MIN_FRAC32
 *                 -   MAX_FRAC32, if x = MIN_FRAC32 and y =  MIN_FRAC32
 * Inputs        : - x   - Fractional number represented as int
 *                 - y   - Fractional number represented as int
 * Outputs       :   z   - (x * y) with saturation
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int32_t Math_Mult_frac32(int32_t x, int32_t y)
{
    int64_t z;
    if ((x == MIN_FRAC32) && (y == MIN_FRAC32))
    {
        return MAX_FRAC32;
    }
    else
    {
        z =(int64_t) x * (int64_t)y;
        return (z >> 31);
    }
}

/* ----------------------------------------------------------------------------
 * Function      : int Math_Add_frac32(int32_t x, int32_t y)
 * ----------------------------------------------------------------------------
 * Description   : Add two 32-bit signed fractional numbers, then saturate.
 *                 The result is one of the following:
 *                 -   (x + y),    if MIN_FRAC32 <= (x + y) <= MAX_FRAC32
 *                 -   MAX_FRAC32, if (x + y) > MAX_FRAC32
 *                 -   MIN_FRAC32, if (x + y) < MIN_FRAC32
 * Inputs        : - x   - Fractional number represented as int
 *                 - y   - Fractional number represented as int
 * Outputs       :   z   - (x + y) with saturation
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int32_t Math_Add_frac32(int32_t x, int32_t y)
{
    int32_t z;
    z = x + y;
    if ((y > 0) && (x > 0) && (z < 0))
    {
        return MAX_FRAC32;
    }
    else if ((y < 0) && (x < 0) && (z >= 0))
    {
        return MIN_FRAC32;
    }
    else
    {
        return z;
    }
}

/* ----------------------------------------------------------------------------
 * Function      : int32_t Math_Sub_frac32(int32_t x, int32_t y)
 * ----------------------------------------------------------------------------
 * Description   : Subtract one 32-bit signed fractional number from another,
 *                 then saturate. The result is one of the following:
 *                 -   (x - y)   , if MIN_FRAC32 <= (x - y) <= MAX_FRAC32
 *                 -   MAX_FRAC32, if (x - y) > MAX_FRAC32
 *                 -   MIN_FRAC32, if (x - y) < MIN_FRAC32
 * Inputs        : - x   - Fractional number represented as int
 *                 - y   - Fractional number represented as int
 * Outputs       :   z   - (x - y) with saturation
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int32_t Math_Sub_frac32(int32_t x, int32_t y)
{
    int32_t z;
    z = x - y;
    if ((x > 0) && (y < 0) && (z < 0))
    {
        return MAX_FRAC32;
    }
    else if ((x < 0) && (y > 0) && (z > 0))
    {
        return MIN_FRAC32;
    }
    else
    {
        return z;
    }
}

/* ----------------------------------------------------------------------------
 * Function      : int32_t Math_ExpAvg_frac32(int32_t alpha, int32_t x, 
 *                                            int32_t y1)
 * ----------------------------------------------------------------------------
 * Description   : Calculate a fixed-point first-order exponential average. The
 *                 outputs of the exponential average are calculated as:
 *                 -   y[n] = alpha * x[n] + (1 - alpha) * y[n-1]
 *
 *                 Where:
 *                 -   0 < alpha < 1
 * Inputs        : - alpha - Filter coefficient
 *                 - x     - Current input
 *                 - y1    - Previous output
 * Outputs       :   y     - New output
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int32_t Math_ExpAvg_frac32(int32_t alpha, int32_t x, int32_t y1)
{
    int32_t var1, var2, var3;
    var1 = Math_Mult_frac32(alpha, y1);
    var2 = Math_Sub_frac32(y1, var1);
    var3 = Math_Mult_frac32(alpha, x);
    return Math_Add_frac32(var3, var2);
}

/* ----------------------------------------------------------------------------
 * Function      : int32_t Math_AttackRelease_frac32(int32_t a, int32_t b, 
                    int32_t x, int32_t y1)
 * ----------------------------------------------------------------------------
 * Description   : Calculate a fixed-point first-order attack-release filter.
 *                 The outputs of the attack-release filter are calculated as:
 *                 -   y[n] = beta * x[n] + (1 - beta) * y[n-1]
 *
 *                 Where:
 *                 -   beta = a if x[n] >= y[n-1]
 *                 -   beta = b if x[n] < y[n-1]
 *                 -   a is the coefficient for attack, 0 < a < 1
 *                 -   b is the coefficient for release, 0 < b < 1
 * Inputs        : - a   - Filter coefficient
 *                 - b   - Filter coefficient
 *                 - x   - Current input
 *                 - y1  - Previous output
 * Outputs       :   y   - New output
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int32_t Math_AttackRelease_frac32(int32_t a, int32_t b, int32_t x, int32_t y1)
{
    int32_t beta;
    beta = (x >= y1) ? a : b;
    return Math_ExpAvg_frac32(beta, x, y1);
}

/* ----------------------------------------------------------------------------
 * Function      : int Math_LinearInterp_frac32(int32_t y0, int32_t y1, 
                    int32_t x)
 * ----------------------------------------------------------------------------
 * Description   : Calculate fixed-point linear interpolation on the interval
 *                 [0, 1). The interpolation is calculated as:
 *                 -   y = y0 + x * (y1 - y0)
 * Inputs        : - y0  - Left boundary point
 *                 - y1  - Right boundary point
 *                 - x   - Interpolation point
 * Outputs       :   y   - Interpolated value
 * Assumptions   : 0 <= x < 1
 * ------------------------------------------------------------------------- */
int32_t Math_LinearInterp_frac32(int32_t y0, int32_t y1, int32_t x)
{
    /* y = y0 + (y1 * x) + (-y0 * x);
     * Since y0 + (-y0 * x) will never overflow (assuming x is non-negative),
     * add these two terms first before adding (y1 * x):
     * y = y0 + (-y0 * x) + (y1 * x) */
    int32_t var1, var2, var3;
    var1 = Math_Mult_frac32(x, y1);
    var2 = Math_Mult_frac32(x, y0);
    var3 = Math_Sub_frac32(y0, var2);
    return Math_Add_frac32(var1, var3);
}