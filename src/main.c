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

uint32_t i;
uint32_t counter;
uint8_t us;
uint32_t count, ms;
bool forward;

static void EIC_Pin8_Handler(uintptr_t context)
{
    //BUSY_LED_Toggle();
    i++;
}

static void EIC_Pin9_Handler(uintptr_t context)
{
    BUSY_LED_Toggle();
    i++;
}

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
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

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );

    EIC_CallbackRegister(EIC_PIN_9, EIC_Pin9_Handler, 0);
    EIC_CallbackRegister(EIC_PIN_8, EIC_Pin8_Handler, 0);
    
    forward = true;
    
    BUSY_LED_Set();
    RST_Set();
    DIR_Set();
    STP_Clear();
    MODE_Clear();
    
    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        
        /*while(i>0)
        {
            i=i-1;
        }*/
        if (forward == true)
        {
            STP_Set();        
            delay_ms(20);
            STP_Clear();
            delay_ms(20);
            //if (i > 0x300)
            //{
                //forward = false;
                //DIR_Clear();
            //}
        }
        else
        {
            STP_Set();        
            delay_ms(20);
            STP_Clear();
            delay_ms(20);
            if (i< 0x10)
            {
                forward = true;
                DIR_Set();
            }
        }
        delay_ms(100);

    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

