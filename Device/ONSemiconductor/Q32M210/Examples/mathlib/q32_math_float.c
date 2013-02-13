/* ----------------------------------------------------------------------------
 * Copyright (c) 2009 - 2012 Semiconductor Components Industries, LLC (d/b/a ON
 * Semiconductor), All Rights Reserved
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * q32_math_float.c
 * - Math library floating-point functions source file
 * ----------------------------------------------------------------------------
 * $Revision: 1.3 $
 * $Date: 2012/04/25 21:56:14 $
 * ------------------------------------------------------------------------- */

#include <stdint.h>
#include <q32_math.h>

/* ----------------------------------------------------------------------------
 * Function      : float Math_ExpAvg(float alpha, float x, float y1)
 * ----------------------------------------------------------------------------
 * Description   : Calculate a first-order exponential average. The outputs of
 *                 the exponential average are calculated as:
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
float Math_ExpAvg(float alpha, float x, float y1)
{
    return (alpha * (x - y1) + y1);
}

/* ----------------------------------------------------------------------------
 * Function      : float Math_AttackRelease(float a, float b, float x, float y1)
 * ----------------------------------------------------------------------------
 * Description   : Calculate a first-order attack-release filter. The outputs
 *                 of the attack-release filter are calculated as:
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
float Math_AttackRelease(float a, float b, float x, float y1)
{
    return (((x >= y1) ? a : b) * (x - y1) + y1);
}

/* ----------------------------------------------------------------------------
 * Function      : float Math_LinearInterp(float x0,
 *                                         float x1,
 *                                         float y0,
 *                                         float y1,
 *                                         float x)
 * ----------------------------------------------------------------------------
 * Description   : Calculate linear interpolation on the interval [x0, x1). The
 *                 interpolation is calculated as:
 *                 -   y = y0 + (y1 - y0) / (x1 - x0) * (x - x0)
 * Inputs        : - x0  - First boundary point x-axis value
 *                 - x1  - Second boundary point x-axis value
 *                 - y0  - First boundary point y-axis value
 *                 - y1  - Second boundary point y-axis value
 *                 - x   - Interpolation point
 * Outputs       :   y   - Interpolated value
 * Assumptions   : x0 != x1
 * ------------------------------------------------------------------------- */
float Math_LinearInterp(float x0, float x1, float y0, float y1, float x)
{
    return (y0 + ((y1 - y0) / (x1 - x0)) * (x - x0));
}

/* ----------------------------------------------------------------------------
 * Function      : void Math_SingleVar_Reg(float* x,
 *                                         float* y,
 *                                         uint32_t N,
 *                                         float* a)
 * ----------------------------------------------------------------------------
 * Description   : Find the least-squares solution for a single variable linear
 *                 regression model. A linear regression model with a single
 *                 predictor variable can be represented as:
 *                 -   y[i] = a0 + a1 * x[i] + e[i], i = 0, 1, 2, ..., N-1
 * Inputs        : - x   - Pointer to the input variable vector x[]
 *                 - y   - Pointer to the dependent variable vector y[]
 *                 - N   - Length of vector x[] and y[]
 * Outputs       :   a   - Pointer to the coefficient vector {a0, a1}
 * Assumptions   : - x[] and y[] are of the same length
 *                 - x[] is not a constant vector (constant vector here means
 *                   x[0] = x[1] = ... = x[N-1])
 *                 - a is a pointer to a coefficient vector of length 2
 * ------------------------------------------------------------------------- */
void Math_SingleVar_Reg(float* x, float* y, uint32_t N, float* a)
{
    uint32_t i;
    float sum_x, sum_y, sum_xy, sum_xx;

    sum_x  = 0;
    sum_y  = 0;
    sum_xy = 0;
    sum_xx = 0;

    for (i = 0; i < N; i++)
    {
        sum_x  = sum_x + x[i];
        sum_y  = sum_y + y[i];
        sum_xy = sum_xy + x[i] * y[i];
        sum_xx = sum_xx + x[i] * x[i];
    }
    /* a[0] is the "offset" and a[1] is the "slope" */
    a[0] = (sum_y * sum_xx - sum_x * sum_xy) / (N * sum_xx - sum_x * sum_x);
    a[1] = ((sum_xy - (sum_y * sum_x / N)) / (sum_xx - (sum_x * sum_x / N)));
}
