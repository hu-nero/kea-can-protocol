/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : SysTick.h
**     Project     : HC03_Project
**     Processor   : SKEAZ64MLH4
**     Component   : Init_SysTick
**     Version     : Component 01.006, Driver 01.06, CPU db: 3.00.000
**     Repository  : Kinetis
**     Compiler    : GNU C Compiler
**     Date/Time   : 2023-09-05, 18:04, # CodeGen: 11
**     Abstract    :
**          This file implements the SysTick (SysTick) module initialization
**          according to the Peripheral Initialization settings, and
**          defines interrupt service routines prototypes.
**     Settings    :
**          Component name                                 : SysTick
**          Device                                         : SysTick
**          Settings                                       : 
**            Clock source                                 : External clock
**            Reload value                                 : 2
**            Counter period                               : 1.200 us
**          Interrupts                                     : 
**            Interrupt                                    : INT_SysTick
**            Interrupt priority                           : 0
**            ISR Name                                     : SysTick_Handler
**            Timer interrupt                              : Disabled
**          Initialization                                 : 
**            Timer enable                                 : yes
**            Clear counter                                : yes
**            Call Init method                             : yes
**            Utilize after reset values                   : default
**     Contents    :
**         Init - void SysTick_Init(void);
**
**     Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file SysTick.h
** @version 01.06
** @brief
**          This file implements the SysTick (SysTick) module initialization
**          according to the Peripheral Initialization settings, and
**          defines interrupt service routines prototypes.
*/         
/*!
**  @addtogroup SysTick_module SysTick module documentation
**  @{
*/         

#ifndef SysTick_H_
#define SysTick_H_

/* MODULE SysTick. */

#include "SysTick_Init.h"
#include "PE_Types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Initialization method user name */
#define SysTick_Init SysTick_Init
/* PDD macros peripheral base address parameter */
#define SysTick_DEVICE SysTick_BASE_PTR
/*
** ===================================================================
** The interrupt service routine must be implemented by user in one
** of the user modules (see SysTick.c file for more information).
** ===================================================================
*/
PE_ISR(SysTick_Handler);

#ifdef __cplusplus
}
#endif

/* END SysTick. */
#endif /* #ifndef __SysTick_H_ */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/


