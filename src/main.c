/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes


uint32_t counter;
uint8_t us;
uint32_t count, ms;


void delay_us(uint32_t target)
{
    count = 0;
    us = 0;
    while (count < target)
    {
        us = us +1;

        if (us > 10)
        {
            us = 0;
            count++;
        }
    }

}

void delay_ms(uint32_t target)
{
    count = 0;
    ms = 0;
    while (count < target)
    {
        ms = ms +1;

        if (ms > 17000)
        {
            ms = 0;
            count++;
        }
    }

}
// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    uint16_t i = 0;
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    RST_Set();
    DIR_Clear();
    STP_Clear();
    MODE_Clear();
    

    BUSY_LED_Set();
    
    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );

/*        if (counter > 100000)
        {
            counter = 0;
            BUSY_LED_Toggle();
        }
        
        counter++;*/
        
        DIR_Set();
        BUSY_LED_Set();
        while (i < 100)
        {
            STP_Set();
            delay_ms(10);
            STP_Clear();
            delay_ms(10);
            i++;
        }
        
        
        BUSY_LED_Clear();
        //delay_ms(1000);
        DIR_Clear();
        i=0;
        BUSY_LED_Set();
        while (i < 100)
        {
            STP_Set();
            delay_ms(10);
            STP_Clear();
            delay_ms(10);
            i++;
        }
        
        i=0;
        BUSY_LED_Clear();

    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

