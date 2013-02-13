/* Copyright (c) 2012 Semiconductor Components Industries, LLC
 * (d/b/a ON Semiconductor). All Rights Reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor. The
 * terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * adc.h
 * - RTC functions for the RPM sample application.
 * ----------------------------------------------------------------------------
 * $Revision: 1.2 $
 * $Date: 2012/04/18 15:14:19 $
 * ------------------------------------------------------------------------- */
#ifndef RTC_H
#define RTC_H

/* Function prototypes */
extern void RTC_SetControl(unsigned int config, int wait);
extern void RTC_SetConfig(unsigned int rate, unsigned int config,
                          unsigned int count, unsigned int alarm,
                          int wait);

#endif /* RTC_H */
