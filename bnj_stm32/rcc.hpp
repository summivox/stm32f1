//rcc.hpp: Reset and Clock Control
#ifndef _RCC_HPP_
#define _RCC_HPP_
#pragma thumb

#include "stm32f10x.h"
#include "common.hpp"
#include "register.hpp"

namespace{//workaround--make the "freq" local. 

#undef RCC
    struct RCC{
        enum addr{
            base        =   RCC_BASE,
            CR          =	base+0x00,
            CFGR        =	base+0x04,
            CIR         =	base+0x08,
            APB2RSTR    =	base+0x0C,
            APB1RSTR    =	base+0x10,
            AHBENR      =	base+0x14,
            APB2ENR     =	base+0x18,
            APB1ENR     =	base+0x1C,
            BDCR        =	base+0x20,
            CSR         =	base+0x24,
            AHBRSTR     =	base+0x28,
            CFGR2       =	base+0x2C
        };

        //20120218: Finally decided to copy the ST firmware implementation.
        /* CFGR register bit mask */
#define CFGR_PLL_Mask             0xFFC0FFFFu
#define CFGR_PLLMull_Mask         0x003C0000u
#define CFGR_PLLSRC_Mask          0x00010000u
#define CFGR_PLLXTPRE_Mask        0x00020000u
#define CFGR_SWS_Mask             0x0000000Cu
#define CFGR_SW_Mask              0xFFFFFFFCu
#define CFGR_HPRE_Reset_Mask      0xFFFFFF0Fu
#define CFGR_HPRE_Set_Mask        0x000000F0u
#define CFGR_PPRE1_Reset_Mask     0xFFFFF8FFu
#define CFGR_PPRE1_Set_Mask       0x00000700u
#define CFGR_PPRE2_Reset_Mask     0xFFFFC7FFu
#define CFGR_PPRE2_Set_Mask       0x00003800u
#define CFGR_ADCPRE_Reset_Mask    0xFFFF3FFFu
#define CFGR_ADCPRE_Set_Mask      0x0000C000u
        struct freq_t{
            bool ready;
            U32 SYSCLK;
            U32 HCLK;
            U32 PCLK1;
            U32 PCLK2;
            U32 ADCCLK;
            freq_t():ready(false){}
            void update(){
                static const U8 APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};
                static const U8 ADCPrescTable[4] = {2, 4, 6, 8};

                U32 tmp = 0, pllmull = 0, pllsource = 0, presc = 0;

                /* Get SYSCLK source -------------------------------------------------------*/
                tmp = VU32_AT(CFGR) & CFGR_SWS_Mask;

                switch (tmp)
                {
                    case 0x00:  /* HSI used as system clock */
                        SYSCLK = HSI_Value;
                        break;

                    case 0x04:  /* HSE used as system clock */
                        SYSCLK = HSE_Value;
                        break;

                    case 0x08:  /* PLL used as system clock */
                        /* Get PLL clock source and multiplication factor ----------------------*/
                        pllmull = VU32_AT(CFGR) & CFGR_PLLMull_Mask;
                        pllmull = ( pllmull >> 18) + 2;

                        pllsource = VU32_AT(CFGR) & CFGR_PLLSRC_Mask;

                        if (pllsource == 0x00)
                        {/* HSI oscillator clock divided by 2 selected as PLL clock entry */
                            SYSCLK = (HSI_Value >> 1) * pllmull;
                        }
                        else
                        {/* HSE selected as PLL clock entry */

                            if ((VU32_AT(CFGR) & CFGR_PLLXTPRE_Mask) != (U32)RESET)
                            {/* HSE oscillator clock divided by 2 */

                                SYSCLK = (HSE_Value >> 1) * pllmull;
                            }
                            else
                            {
                                SYSCLK = HSE_Value * pllmull;
                            }
                        }
                        break;

                    default:
                        SYSCLK = HSI_Value;
                        break;
                }

                /* Compute HCLK, PCLK1, PCLK2 and ADCCLK clocks frequencies ----------------*/
                /* Get HCLK prescaler */
                tmp = VU32_AT(CFGR) & CFGR_HPRE_Set_Mask;
                tmp = tmp >> 4;
                presc = APBAHBPrescTable[tmp];

                /* HCLK clock frequency */
                HCLK = SYSCLK >> presc;

                /* Get PCLK1 prescaler */
                tmp = VU32_AT(CFGR) & CFGR_PPRE1_Set_Mask;
                tmp = tmp >> 8;
                presc = APBAHBPrescTable[tmp];

                /* PCLK1 clock frequency */
                PCLK1 = HCLK >> presc;

                /* Get PCLK2 prescaler */
                tmp = VU32_AT(CFGR) & CFGR_PPRE2_Set_Mask;
                tmp = tmp >> 11;
                presc = APBAHBPrescTable[tmp];

                /* PCLK2 clock frequency */
                PCLK2 = HCLK >> presc;

                /* Get ADCCLK prescaler */
                tmp = VU32_AT(CFGR) & CFGR_ADCPRE_Set_Mask;
                tmp = tmp >> 14;
                presc = ADCPrescTable[tmp];

                /* ADCCLK clock frequency */
                ADCCLK = PCLK2 / presc;

                ready=true;
            }
        };
        static freq_t freq;
    };

    RCC::freq_t RCC::freq;
};

#endif//_RCC_HPP_
