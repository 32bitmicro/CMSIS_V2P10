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
// $Date: 2010/12/01 16:44:37 $
//-----------------------------------------------------------------------------

#include "hidlib.h"
#include <stdio.h>

#define REPORT_SIZE 16

int main()
{
    hidlib_t hidhandle;
    unsigned char data[REPORT_SIZE+1];
    int i, reportCount;
    int error;

    error = hidOpenDevice( 0x0EE2, 0x2003, &hidhandle );
    if( error != HIDLIB_OK ) {
        printf( "Could not find device: %d\n", error );
        return 1;
    }
    printf( "Found device\n" );

    reportCount = 0;
    for( ;; ) {
        // Read and display input report
        if( hidRecvReport( hidhandle, REPORT_SIZE+1, data, NULL ) != HIDLIB_OK ) {
            printf( "Recv failed\n" );
            break;
        }

        for( i=0; i < REPORT_SIZE; i ++ ) {
            printf( "%02x ", data[i+1] );
        }

        reportCount ++;
        if( reportCount % 4 == 0 ) {
            // Send a report
            data[0] = 0;
            data[1] = reportCount / 4;
            if( hidSendReport( hidhandle, REPORT_SIZE+1, data, NULL ) != HIDLIB_OK ) {
                printf( "Send failed\n" );
                break;
            }
        }
        printf("\n");
    }

    hidCloseDevice( hidhandle );

    return 0;
}
