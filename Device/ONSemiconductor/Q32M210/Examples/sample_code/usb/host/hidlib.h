//-----------------------------------------------------------------------------
// Copyright (c) 2010 Semiconductor Components Industries, LLC
// (d/b/a ON Semiconductor). All rights reserved.
// 
// This code is the property of ON Semiconductor and may not be redistributed
// in any form without prior written permission from ON Semiconductor. The
// terms of use and warranty for this code are covered by contractual
// agreements between ON Semiconductor and the licensee.
//-----------------------------------------------------------------------------
// $Revision: 1.3 $
// $Date: 2012/04/26 15:54:28 $
//-----------------------------------------------------------------------------
#ifndef _HIDLIB_H_
#define _HIDLIB_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long hidlib_t;

int hidOpenDevice( unsigned short vendor, unsigned short product, \
                   hidlib_t *outHidhandle );

int hidSendReport( hidlib_t hidhandle, int reportLen, unsigned char *data, \
                   int *actual );
int hidRecvReport( hidlib_t hidhandle, int reportLen, unsigned char *data, \
                   int *actual );

int hidCloseDevice( hidlib_t hidhandle );

#define HIDLIB_OK                   0
#define HIDLIB_ERR_OUT_OF_MEMORY    -1000
#define HIDLIB_ERR_NOT_FOUND        -1001
#define HIDLIB_ERR_COMM_FAILURE     -1002
#define HIDLIB_ERR_INTERNAL         -1003

#ifdef __cplusplus
}
#endif

#endif
