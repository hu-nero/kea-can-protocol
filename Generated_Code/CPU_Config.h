/** ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : CPU_Config.h
**     Project     : HC03_Project
**     Processor   : SKEAZ64MLH4
**     Version     : Component 01.013, Driver 01.00, CPU db: 3.00.000
**     Repository  : Kinetis
**     Compiler    : GNU C Compiler
**     Date/Time   : 2023-10-10, 10:28, # CodeGen: 9
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
** @file CPU_Config.h                                                  
** @version 01.00
*/         
/*!
**  @addtogroup CPU_Config_module CPU_Config module documentation
**  @{
*/         

#ifndef __CPU_Config_H
#define __CPU_Config_H

/* MODULE CPU_Config.h */

/* Include C integer types declaration header */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 
   Component method constants. Internal methods are not included. 

   When method is enabled in the processor (CPU) component associated constant
   has value 1 otherwise 0.
*/

#define CPU_SET_CLOCK_CONFIGURATION                        0x00U      
#define CPU_GET_CLOCK_CONFIGURATION                        0x00U      
#define CPU_SET_OPERATION_MODE                             0x00U      
#define CPU_ENABLE_INT                                     0x00U      
#define CPU_DISABLE_INT                                    0x00U      
#define CPU_SYSTEM_RESET                                   0x01U

/* Events constants. */

/* Processor (CPU) component's events are called from ISRs implemented in the Cpu.c as 
   code of the CPU ISRs depends on the RTOS Adaptor and may vary. Only exception is OnReset event
   which is called directly as this event doesn't have any ISR. */


/* 
    Processor type constants 

    Constants specifying processor family, type or variant.    
*/

#define CPU_FAMILY_Kinetis             /* Specification of the core type of the selected processor */
#define CPU_DERIVATIVE_SKEAZ128LH4     /* Name of the selected processor derivative */
#define CPU_PARTNUM_SKEAZ64MLH4        /* Part number of the selected processor */
#define CPU_LITTLE_ENDIAN              /* The selected processor uses little endian */

/* 
    Processor clock source constants
    
    Constants containing frequencies of processor reference clock sources.
*/

#define CPU_BUS_CLK_HZ                  20000000U /* Initial value of the bus clock frequency in Hz */
#define CPU_CORE_CLK_HZ                 40000000U /* Initial value of the core/system clock frequency in Hz.  */
#define CPU_XTAL_CLK_HZ                 16000000U /* Value of the external crystal or oscillator clock frequency in Hz */
#define CPU_INT_SLOW_CLK_HZ             37500U /* Value of the slow internal oscillator clock frequency in Hz  */
/* 
    Clock configuration frequency constants

    Following constants contain variety of frequency values generated by the processor
    in the specific Clock configuration.
    Clock configurations are used to control general processor timing 
    (for example core and bus clock or FLL and PLL submodules) and to predefine
    up to 8 different processor timing schemes.
    After reset, processor is set to the Clock configuration 0. During the 
    application run-time SetClockConfiguration() method can be used to switch 
    between predefined Clock configurations.
    Clock configurations are set in the processor (CPU) component: 
    Clock settings\Clock configurations group of properties.
*/

#define CPU_CLOCK_CONFIG_NUMBER         0x01U /* Specifies number of defined clock configurations. */

#define CPU_BUS_CLK_HZ_CLOCK_CONFIG0    20000000U /* Value of the bus clock frequency in the clock configuration 0 in Hz. */
#define CPU_CORE_CLK_HZ_CLOCK_CONFIG0   40000000U /* Value of the core/system clock frequency in the clock configuration 0 in Hz. */

/* CPU frequencies in clock configuration 0 */
#define CPU_CLOCK_CONFIG_0              0x00U /* Clock configuration 0 identifier */
#define CPU_CORE_CLK_HZ_CONFIG_0        40000000UL /* Core clock frequency in clock configuration 0 */
#define CPU_BUS_CLK_HZ_CONFIG_0         20000000UL /* Bus clock frequency in clock configuration 0 */
#define CPU_FLEXBUS_CLK_HZ_CONFIG_0     0UL /* Flexbus clock frequency in clock configuration 0 */
#define CPU_FLASH_CLK_HZ_CONFIG_0       20000000UL /* FLASH clock frequency in clock configuration 0 */
#define CPU_USB_CLK_HZ_CONFIG_0         0UL /* USB clock frequency in clock configuration 0 */
#define CPU_PLL_FLL_CLK_HZ_CONFIG_0     40000000UL /* PLL/FLL clock frequency in clock configuration 0 */
#define CPU_MCGIR_CLK_HZ_CONFIG_0       37500UL /* MCG internal reference clock frequency in clock configuration 0 */
#define CPU_OSCER_CLK_HZ_CONFIG_0       16000000UL /* System OSC external reference clock frequency in clock configuration 0 */
#define CPU_ERCLK32K_CLK_HZ_CONFIG_0    0UL /* External reference clock 32k frequency in clock configuration 0 */
#define CPU_MCGFF_CLK_HZ_CONFIG_0       31250UL /* MCG fixed frequency clock */


/* Clock configuration structure declaration. Structure is initialized in PE_LDD.c */
typedef struct  {
  uint32_t cpu_core_clk_hz;            /* Core clock frequency in clock configuration */
  uint32_t cpu_bus_clk_hz;             /* Bus clock frequency in clock configuration */
  uint32_t cpu_flexbus_clk_hz;         /* Flexbus clock frequency in clock configuration */
  uint32_t cpu_flash_clk_hz;           /* FLASH clock frequency in clock configuration */
  uint32_t cpu_usb_clk_hz;             /* USB clock frequency in clock configuration */
  uint32_t cpu_pll_fll_clk_hz;         /* PLL/FLL clock frequency in clock configuration */
  uint32_t cpu_mcgir_clk_hz;           /* MCG internal reference clock frequency in clock configuration */
  uint32_t cpu_oscer_clk_hz;           /* System OSC external reference clock frequency in clock configuration */
  uint32_t cpu_erclk32k_clk_hz;        /* External reference clock 32k frequency in clock configuration */
  uint32_t cpu_mcgff_clk_hz;           /* MCG fixed frequency clock */
} TCpuClockConfiguration;

/* The array of clock frequencies in configured clock configurations */
extern const TCpuClockConfiguration PE_CpuClockConfigurations[CPU_CLOCK_CONFIG_NUMBER];

/* 
    Clock generator (ICS) mode constants
    
    Constants and types used to specify ICS mode and clock sources used 
    by the clock generator. Each mode constant consists of unique mode ID 
    number and mode features encoded using bit-mask. Clock sources are encoded
    using just bit-mask describing source features.
*/

/* ICS mode and clock source features - used to fill CPU_TClockGenMode and CPU_TClockSource */
#define CPU_CLOCK_SLOW_MASK                                0x10U     /* Mode uses slow internal reference clock */ 
#define CPU_CLOCK_FAST_MASK                                0x20U     /* Mode uses fast internal reference clock */ 
#define CPU_CLOCK_EXTERNAL_CLOCK_MASK                      0x40U     /* Mode uses external reference clock from external clock input */ 
#define CPU_CLOCK_EXTERNAL_CRYSTAL_MASK                    0x80U     /* Mode uses external reference clock from crystal/resonator reference connection */ 
#define CPU_CLOCK_RTC_OSC_MASK                             0x0100U   /* Mode uses RTC oscillator clock */ 


/* ICS mode IDs - used to fill CPU_TClockGenMode */
#define CPU_ICS_MODE_FEI                                   0x00U     /* FEI mode ID */ 
#define CPU_ICS_MODE_FBI                                   0x01U     /* FBI mode ID */ 
#define CPU_ICS_MODE_FBILP                                 0x02U     /* FBILP mode ID */ 
#define CPU_ICS_MODE_FEE                                   0x03U     /* FEE mode ID */ 
#define CPU_ICS_MODE_FBE                                   0x04U     /* FBE mode ID */ 
#define CPU_ICS_MODE_FBELP                                 0x05U     /* FBELP mode ID */ 
#define CPU_ICS_MODES                                      0x06U     /* Number of available ICS modes */ 
#define CPU_ICS_MODE_INDEX_MASK                            0x0FU     /* Mask of bits where ICS mode ID is encoded */ 

/* ICS mode type - used to specify ICS mode of each Clock configuration */
typedef uint16_t CPU_TClockGenMode;


/* 
    Clock configuration descriptor
    
    Following types are used to define and store settings related to
    clock generator modules (i.e. ICS and OSC modules), system or common clock 
    dividers and selectors (SIM module) for each predefined Clock configuration. 
    When Clock configuration is switched the processor registers are updated 
    from the following descriptors.
*/

/* ICS and OSC module structure type
   Structure with ICS and OSC configuration. To lower memory footprint the structure 
   doesn't contain full list of ICS and OSC registers but only those with settings 
   necessary to set Clock configuration. */
typedef struct {
  uint8_t ICS_C1_value;
  uint8_t ICS_C2_value;
  uint8_t ICS_C4_value;
  uint8_t ICS_S_value;
  uint8_t OSC_CR_value;
} CPU_TClockGenRegs;

/* Clock system settings structure type
   Structure contains system integration level clock settings - clock source 
   selectors and dividers which control clocks produced by ICS and OSC modules and 
   peripheral clock source selections common for multiple peripheral instances. */
typedef struct {
  uint32_t SIM_CLKDIV_value;
} CPU_TClockSysRegs;

/* Clock configuration descriptor structure type 
   Gathers all Clock configuration settings. Content of this structure is used 
   during Clock configuration set up. */
typedef struct {
  CPU_TClockGenMode Mode;
  CPU_TClockGenRegs GenRegs;
  CPU_TClockSysRegs SysRegs;
  uint32_t BusClock;
} CPU_TClockConfigDescriptor;

/* Clock configuration structure content
   
   Following constants are use to initialize CPU_TClockConfigDescriptor structure
   in the static CPU_Init.c module. Constants for each Clock configuration
   enabled in the processor (CPU) component are generated.

   Properties: Clock settings\Clock configurations,
               Clock settings\Clock source settings,
               Clock settings\Clock sources.
*/

/* Clock configuration 0 */
#define CPU_ICS_MODE_CONFIG_0                              (CPU_ICS_MODE_FEE | CPU_CLOCK_EXTERNAL_CRYSTAL_MASK | CPU_CLOCK_SLOW_MASK) /* Clock generator mode */
#define CPU_ICS_C1_CONFIG_0                                0x22U /* ICS_C1 */
#define CPU_ICS_C2_CONFIG_0                                0x00U /* ICS_C2 */
#define CPU_ICS_C4_CONFIG_0                                0x00U /* ICS_C4 */
#define CPU_ICS_S_CONFIG_0                                 0x00U /* ICS_S */
#define CPU_OSC_CR_CONFIG_0                                0x96U /* OSC_CR */
#define CPU_SIM_CLKDIV_CONFIG_0                            0x01100000UL /* SIM_CLKDIV */

/* 
   Clock generator structure default content
   
   When Clock configurations set in the processor (CPU) component use different ICS 
   modes and transition between them requires passing through some intermediate ICS 
   mode then following constants are used to set up clock generator modules to these 
   intermediate ICS modes. Following constants represent CPU_TClockGenRegs structure 
   content loaded to the clock generator registers to configure the intermediate 
   mode of the generator modules.
   
   If setting is controlled by any property then the associated property is specified. 
   Rest of settings are not controlled by any property and their value is static.
*/


/* Clock generator default state in FEI mode 
   Clock source:                        Slow internal reference, disabled in the STOP mode
   Clock source frequency:              37.5 kHz (Property: Clock settings\Clock sources\Internal oscillator\Slow internal reference clock)
   FLL:                                 Enabled, engaged
   FLL factor:                          1280
   Internal reference clock (ICSIRCLK): Disabled
   External reference clock (OSCERCLK): Disabled
   External clock monitor:              Disabled
   Loss of clock reset:                 Disabled
*/
/* ICS_C1: CLKS=0,IREFS=1,IRCLKEN=0,IREFSTEN=0 */
#define CPU_DEFAULT_FEI_ICS_C1                             0x04U     /* ICS_C1 value in FEI default state */ 
/* ICS_C2:  */
#define CPU_DEFAULT_FEI_ICS_C2                             0x00U     /* ICS_C2 value in FEI default state */ 
/* ICS_C4: LOLIE=0,CME=0 */
#define CPU_DEFAULT_FEI_ICS_C4                             0x00U     /* ICS_C4 value in FEI default state */ 
/* ICS_S: LOLS=1 */
#define CPU_DEFAULT_FEI_ICS_S                              0x80U     /* ICS_S value in FEI default state */ 
/* OSC_CR:  */
#define CPU_DEFAULT_FEI_OSC_CR                             0x00U     /* OSC_CR value in FEI default state */ 

/* Clock generator default state in FBI mode
   Clock source:                        Slow internal reference, disabled in the STOP mode
   Clock source frequency:              37.5 kHz (Property: Clock settings\Clock sources\Internal oscillator\Slow internal reference clock)
   FLL:                                 Enabled, bypassed
   FLL factor:                          1280
   Internal reference clock (ICSIRCLK): Disabled
   External reference clock (OSCERCLK): Disabled
   External clock monitor:              Disabled
   Loss of clock reset:                 Disabled
*/
/* ICS_C1: CLKS|=1,IREFS=1,IRCLKEN=0,IREFSTEN=0 */
#define CPU_DEFAULT_FBI_ICS_C1                             0x44U     /* ICS_C1 value in FBI default state */ 
/* ICS_C2:  */
#define CPU_DEFAULT_FBI_ICS_C2                             0x00U     /* ICS_C2 value in FBI default state */ 
/* ICS_C4: LOLIE=0,CME=0 */
#define CPU_DEFAULT_FBI_ICS_C4                             0x00U     /* ICS_C4 value in FBI default state */ 
/* ICS_S: LOLS=1 */
#define CPU_DEFAULT_FBI_ICS_S                              0x80U     /* ICS_S value in FBI default state */ 
/* OSC_CR:  */
#define CPU_DEFAULT_FBI_OSC_CR                             0x00U     /* OSC_CR value in FBI default state */ 

/* Clock generator default state in FBILP mode
   Clock source:                        Slow internal reference, disabled in the STOP mode
   Clock source frequency:              37.5 kHz (Property: Clock settings\Clock sources\Internal oscillator\Slow internal reference clock)
   FLL:                                 Disabled
   Internal reference clock (ICSIRCLK): Disabled
   External reference clock (OSCERCLK): Disabled
   External clock monitor:              Disabled
   Loss of clock reset:                 Disabled
*/
/* ICS_C1: CLKS|=1,IREFS=1,IRCLKEN=0,IREFSTEN=0 */
#define CPU_DEFAULT_FBILP_ICS_C1                           0x44U     /* ICS_C1 value in FBILP default state */ 
/* ICS_C2: LP=1 */
#define CPU_DEFAULT_FBILP_ICS_C2                           0x10U     /* ICS_C2 value in FBILP default state */ 
/* ICS_C4: LOLIE=0,CME=0 */
#define CPU_DEFAULT_FBILP_ICS_C4                           0x00U     /* ICS_C4 value in FBILP default state */ 
/* ICS_S: LOLS=1 */
#define CPU_DEFAULT_FBILP_ICS_S                            0x80U     /* ICS_S value in FBILP default state */ 
/* OSC_CR:  */
#define CPU_DEFAULT_FBILP_OSC_CR                           0x00U     /* OSC_CR value in FBILP default state */ 

/* Clock generator default state in FEE mode
   Clock source:                        External crystal (oscillator) (Property: Clock settings\Clock sources\System oscillator 0\Clock source)
   Clock source frequency:              16 MHz (Property: Clock settings\Clock sources\System oscillator 0\Clock source\Clock frequency)
   External frequency range:            High
   FLL external reference divider:      512
   FLL:                                 Enabled, engaged
   FLL factor:                          1280
   Internal reference clock (ICSIRCLK): Disabled
   External reference clock (OSCERCLK): Disabled
   External clock monitor:              Disabled
   Loss of clock reset:                 Disabled
*/
/* ICS_C1: CLKS=0,RDIV|=4,IREFS=0,IRCLKEN=0,IREFSTEN=0 */
#define CPU_DEFAULT_FEE_ICS_C1                             0x20U     /* ICS_C1 value in FEE default state */ 
/* ICS_C2:  */
#define CPU_DEFAULT_FEE_ICS_C2                             0x00U     /* ICS_C2 value in FEE default state */ 
/* ICS_C4: LOLIE=0,CME=0 */
#define CPU_DEFAULT_FEE_ICS_C4                             0x00U     /* ICS_C4 value in FEE default state */ 
/* ICS_S: LOLS=1 */
#define CPU_DEFAULT_FEE_ICS_S                              0x80U     /* ICS_S value in FEE default state */ 
/* OSC_CR: OSCEN=1,OSCOS=1,RANGE=1 */
#define CPU_DEFAULT_FEE_OSC_CR                             0x94U     /* OSC_CR value in FEE default state */ 

/* Clock generator default state in FBE mode
   Clock source:                        External crystal (oscillator) (Property: Clock settings\Clock sources\System oscillator 0\Clock source)
   Clock source frequency:              16 MHz (Property: Clock settings\Clock sources\System oscillator 0\Clock source\Clock frequency)
   External frequency range:            High
   FLL external reference divider:      512
   FLL:                                 Enabled, bypassed
   FLL factor:                          1280
   Internal reference clock (ICSIRCLK): Disabled
   External reference clock (OSCERCLK): Disabled
   External clock monitor:              Disabled
   Loss of clock reset:                 Disabled
*/
/* ICS_C1: CLKS|=2,RDIV|=4,IREFS=0,IRCLKEN=0,IREFSTEN=0 */
#define CPU_DEFAULT_FBE_ICS_C1                             0xA0U     /* ICS_C1 value in FBE default state */ 
/* ICS_C2:  */
#define CPU_DEFAULT_FBE_ICS_C2                             0x00U     /* ICS_C2 value in FBE default state */ 
/* ICS_C4: LOLIE=0,CME=0 */
#define CPU_DEFAULT_FBE_ICS_C4                             0x00U     /* ICS_C4 value in FBE default state */ 
/* ICS_S: LOLS=1 */
#define CPU_DEFAULT_FBE_ICS_S                              0x80U     /* ICS_S value in FBE default state */ 
/* OSC_CR: OSCEN=1,OSCOS=1,RANGE=1 */
#define CPU_DEFAULT_FBE_OSC_CR                             0x94U     /* OSC_CR value in FBE default state */ 

/* Clock generator default state in FBELP mode
   Clock source:                        External crystal (oscillator) (Property: Clock settings\Clock sources\System oscillator 0\Clock source)
   Clock source frequency:              16 MHz (Property: Clock settings\Clock sources\System oscillator 0\Clock source\Clock frequency)
   External frequency range:            High
   FLL external reference divider:      512
   FLL:                                 Disabled
   Internal reference clock (ICSIRCLK): Disabled
   External reference clock (OSCERCLK): Disabled
   External clock monitor:              Disabled
   Loss of clock reset:                 Disabled
*/
/* ICS_C1: CLKS|=2,RDIV|=4,IREFS=0,IRCLKEN=0,IREFSTEN=0 */
#define CPU_DEFAULT_FBELP_ICS_C1                           0xA0U     /* ICS_C1 value in FBELP default state */ 
/* ICS_C2: LP=1 */
#define CPU_DEFAULT_FBELP_ICS_C2                           0x10U     /* ICS_C2 value in FBELP default state */ 
/* ICS_C4: LOLIE=0,CME=0 */
#define CPU_DEFAULT_FBELP_ICS_C4                           0x00U     /* ICS_C4 value in FBELP default state */ 
/* ICS_S: LOLS=1 */
#define CPU_DEFAULT_FBELP_ICS_S                            0x80U     /* ICS_S value in FBELP default state */ 
/* OSC_CR: OSCEN=1,OSCOS=1,RANGE=1 */
#define CPU_DEFAULT_FBELP_OSC_CR                           0x94U     /* OSC_CR value in FBELP default state */ 

/* 
   Low power mode settings
  
   Following constants are used for SetOperationMode() method parameterization.
   This method switches 4 driver operation modes - RUN, WAIT, SLEEP and STOP.
   These driver operation modes represents higher-level abstraction and maps 
   hardware power modes as follows:

   Driver mode      Hardware mode
   RUN   :          RUN
   WAIT  :          WAIT - Low power mode
   STOP  :          STOP - Low power mode
   
*/

/* WAIT operation mode settings */
#define CPU_LOW_POWER_WAIT_SLEEP_ON_EXIT                   0x00U     /* ARM sleep-on-exit is disabled in WAIT operation mode (when the lowest priority ISR is exited then processor state unstacking is done before system goes back to low-power mode) */ 

/* SLEEP operation mode settings */
#define CPU_LOW_POWER_SLEEP_SLEEP_ON_EXIT                  0x00U     /* ARM stop-on-exit is disabled in SLEEP operation mode (when the lowest priority ISR is exited then processor state unstacking is done before system goes back to low-power mode) */ 

/* STOP operation mode settings */
#define CPU_LOW_POWER_STOP                                 0x00U     /* ARM STOP operation mode */ 

/* After reset values optimization */

/* Property: Common settings\Utilize after reset values */
#define CPU_AFTER_RESET_VALUES                             0x00U     /* After reset values optimization is disabled */ 

/* 
   Startup - parameterization
   
   Following constants contains parameterization of the MCU startup sequence 
   placed in the __init_hardware() method according to Processor Expert 
   CPU component settings but can be used to parameterize any user startup 
   code.   
   Values of the constants are generated from the component properties 
   specified in comments. If not specified differently, value 0 = feature 
   disabled and 1 = feature enabled. If constant has no defined value it 
   means feature is not used.
*/
  
/* Watchdog initialization */

/* Property: Common settings\Watchdog disable */
#define STARTUP_WDOG                                       0x01U     /* Watchdog disabled */ 

/* System clock initialization */
  
/* Internal reference clock trim initialization 
   Properties: Clock settings\Clock sources\Internal oscillator\Initialize slow trim value */
#undef STARTUP_CLOCK_INTERNAL_SLOW_TRIM_ADDRESS                      /* Slow oscillator not trimmed */
#undef STARTUP_CLOCK_INTERNAL_SLOW_FINE_TRIM_ADDRESS                 /* Slow oscillator not trimmed */
/* Properties: Clock settings\Clock sources\Internal oscillator\Initialize fast trim value */
#undef STARTUP_CLOCK_INTERNAL_FAST_TRIM_ADDRESS                      /* Fast oscillator not trimmed */
#undef STARTUP_CLOCK_INTERNAL_FAST_FINE_TRIM_ADDRESS                 /* Fast oscillator not trimmed */

/* 
   Startup - register values
   
   Following constants represents the full set of registers accessed during 
   MCU startup sequence placed in the __init_hardware() method and their values 
   depends on Processor Expert's processor (CPU) component settings. 
   The constants have values of registers after the startup is finished.
   Although the list of registers accessed during the startup sequence may vary 
   across different CPU component settings, following constants represents 
   a superset of all registers which might be accessed. If register or register 
   bitfield is not modified during the particular startup sequence
   then its after reset value is presented.
   If register or register bitfield is accessed multiple times during the startup
   then the last written value is used.
*/

/* WDOG */
#define STARTUP_WDOG_CS1_VALUE                             0x20U     /* Disable module and allow updates */ 
#define STARTUP_WDOG_CS2_VALUE                             0x01U     /* 1-kHz clock source */ 
#define STARTUP_WDOG_TOVAL_VALUE                           0xE803U   /* WDOG Timeout value */ 

/* ICS */

#define STARTUP_ICS_C1_VALUE                               0x22U     /* ICS_C1 value */ 
#define STARTUP_ICS_C2_VALUE                               0x00U     /* ICS_C2 value */ 
#define STARTUP_ICS_C3_VALUE                               0x00U     /* ICS_C3 value */ 
#define STARTUP_ICS_C4_VALUE                               0x00U     /* ICS_C4 value */ 
#define STARTUP_ICS_S_VALUE                                0x00U     /* ICS_S value */ 

/* OSC */

#define STARTUP_OSC_CR_VALUE                               0x96U     /* OSC_CR value */ 

/* SIM */

#define STARTUP_SIM_CLKDIV_VALUE                           0x01100000UL /* SIM_CLKDIV value */ 
  
/* 
   PE_low_level_init() parameterization constants 

   Following constants contains parameterization of the PE_low_level_init() 
   method called at the main() method.
   Values of the constants are generated from the component properties 
   specified in comments. If not specified differently, value 0 = feature 
   disabled and 1 = feature enabled. If constant has no defined value it 
   means feature is not used.   
*/
		
/* Non-maskable interrupt pin initialization 
   Property: Common settings\NMI pin */
#define CPU_NMI_PIN                                        0x00U     /* NMI pin not initialized */ 

/* Reset pin initialization 
   Property: Common settings\Reset pin */
#define CPU_RESET_PIN                                      0x01U     /* Reset pin initialized */ 

/* SWD DIO pin initialization 
   Property: Common settings\Debug interface (SWD)\DIO pin */
#define CPU_SWD_DIO_PIN                                    0x01U     /* SWD DIO pin initialized */ 
	
	/* SWD CLK pin initialization 
	Property: Common settings\Debug interface (SWD)\CLK pin */
#define CPU_SWD_CLK_PIN                                    0x01U     /* SWD CLK pin initialized */ 

/* Low power modes protection initialization 
   Properties: Low power mode settings\Allowed low power modes */
     
/* Flash configuration field constants */
#define CPU_FLASH_CONFIG_FIELD \
               /* NV_BACKKEY0: KEY=0xFF */ \
               0xFFU, \
               /* NV_BACKKEY1: KEY=0xFF */ \
               0xFFU, \
               /* NV_BACKKEY2: KEY=0xFF */ \
               0xFFU, \
               /* NV_BACKKEY3: KEY=0xFF */ \
               0xFFU, \
               /* NV_BACKKEY4: KEY=0xFF */ \
               0xFFU, \
               /* NV_BACKKEY5: KEY=0xFF */ \
               0xFFU, \
               /* NV_BACKKEY6: KEY=0xFF */ \
               0xFFU, \
               /* NV_BACKKEY7: KEY=0xFF */ \
               0xFFU, \
               /* Reserved */ \
               0xFFU, \
               /* Reserved */ \
               0xFFU, \
               /* Reserved */ \
               0xFFU, \
               /* Reserved */ \
               0xFFU, \
               /* Reserved */ \
               0xFFU, \
               /* NV_FPROT: FPOPEN=1,??=1,FPHDIS=1,FPHS=0,FPLDIS=1,FPLS=0 */ \
               0xE4U, \
               /* NV_FSEC: KEYEN=3,??=1,??=1,??=1,??=1,SEC=2 */ \
               0xFEU, \
               /* NV_FOPT: ??=1,??=1,??=1,??=1,??=1,??=1,??=1,??=1 */ \
               0xFFU 

/* Common_Init() parameterization settings */
  
#define CPU_COMMON_INIT                                    0x01U     /* Call Common_Init() method in PE_low_level_init() */ 

/* Peripherals_Init() parameterization settings */
  
#define CPU_PERIPHERALS_INIT                               0x01U     /* Call Peripherals_Init() method in PE_low_level_init() */ 

/* Components_Init() parameterization settings */

#define CPU_COMPONENTS_INIT                                0x00U     /* Components_Init() method is not called in PE_low_level_init() */ 

/* Interrupts priority level settings */
  
/* Property: Common settings\Initialization priority */
#define CPU_INT_PRIORITY                                   0x01U     /* Priority level constant of enabled interrupts initialized in PE_low_level_init() */ 

#ifdef __cplusplus
}
#endif

#endif
/* __CPU_Config_H */

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

