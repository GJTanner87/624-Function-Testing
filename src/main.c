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

/* Max Encoder position 9C40 */
uint32_t pos_list[4] = {0x100, 0x1020, 0x200, 0x400};
uint8_t mem_write[10] = {1,2,3,4,5,6,7,8,9,10};
uint8_t mem_read[10];

uint32_t i;
uint32_t pos_target;
uint32_t counter;
uint8_t us;
uint32_t count, ms, position;
bool forward, direction, zero_point;

static void EIC_Pin8_Handler(uintptr_t context)
{
    if (i == 0)
    {
        direction = true;
    }
    //BUSY_LED_Toggle();
    i++;
}

static void EIC_Pin9_Handler(uintptr_t context)
{
    if (i == 0)
    {
        direction = false;
    }
    i++;
}

static void EIC_Index_Handler(uintptr_t context)
{
    zero_point = true;
    BUSY_LED_Toggle();
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
    EIC_CallbackRegister(EIC_PIN_7, EIC_Index_Handler, 0);
    
    position = 0;
    forward = true;
    zero_point = false;
    counter = 0;
    pos_target = pos_list[0];
    
    BUSY_LED_Set();
    RST_Set();
    DIR_Clear();
    STP_Clear();
    MODE_Clear();
    
    RAM_Read((uint32_t*) mem_read, 10, RAM_ADDRESS_START);
    RAM_Write((uint32_t*) mem_write, 15, RAM_ADDRESS_START);
    RAM_Read((uint32_t*) mem_read, 15, RAM_ADDRESS_START);
    while (zero_point == false)
    {
        STP_Set();        
        delay_ms(20);
        STP_Clear();
        delay_ms(20);
    }

    DIR_Set();
    i = 0;
    position = 0;
    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        
        if (position > pos_target)
        {
            forward = false;
            DIR_Clear();
            if ((position - pos_target) < 20)
            {
                MODE_Set();
            }
            else
            {
                MODE_Clear();
            }
        }
        else
        {
            forward = true;
            DIR_Set();
            if ((pos_target - position) < 20)
            {
                MODE_Set();
            }
            else
            {
                MODE_Clear();
            }
        }
        
        if (forward == true)
        {
            STP_Set();        
            delay_ms(20);
            STP_Clear();
            delay_ms(20);
        }
        else
        {
            STP_Set();        
            delay_ms(20);
            STP_Clear();
            delay_ms(20);
        }
        
        if (position == pos_target)
        {
            counter++;
            if (counter > 3)
            {
                counter = 0;
            }
            pos_target = pos_list[counter];
        }
        
        if (i != 0)
        {
            if (forward == true)
            {
                position = position + i;
            }
            else
            {
                if (i > position)
                {
                    position = 0;
                }
                else
                {
                    position = position - i;
                }
            }
            i = 0;
        }
        delay_ms(100);

    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

