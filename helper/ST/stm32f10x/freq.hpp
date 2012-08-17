//helper/ST/stm32f10x/freq
#ifndef _HELPER_ST_STM32F10X_FREQ_HPP_
#define _HELPER_ST_STM32F10X_FREQ_HPP_

#include "helper/helper.hpp"

#include "stm32f10x.h"

#include "misc.hpp"




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

//NOTE: Local to every source file (to avoid having to add another deeply buried source file).
//Always update before use.
namespace{
    struct FreqImpl1{
        struct FreqImpl2{
            U32 SYSCLK;
            U32 HCLK;
            U32 PCLK1;
            U32 PCLK2;
            U32 ADCCLK;
            void update(){
                static const U8 APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};
                static const U8 ADCPrescTable[4] = {2, 4, 6, 8};

                U32 tmp = 0, pllmull = 0, pllsource = 0, presc = 0;

                /* Get SYSCLK source -------------------------------------------------------*/
                tmp = RCC->CFGR & CFGR_SWS_Mask;

                switch (tmp)
                {
                    case 0x00:  /* HSI used as system clock */
                        SYSCLK = HSI_VALUE;
                        break;

                    case 0x04:  /* HSE used as system clock */
                        SYSCLK = HSE_VALUE;
                        break;

                    case 0x08:  /* PLL used as system clock */
                        /* Get PLL clock source and multiplication factor ----------------------*/
                        pllmull = RCC->CFGR & CFGR_PLLMull_Mask;
                        pllmull = ( pllmull >> 18) + 2;

                        pllsource = RCC->CFGR & CFGR_PLLSRC_Mask;

                        if (pllsource == 0x00)
                        {/* HSI oscillator clock divided by 2 selected as PLL clock entry */
                            SYSCLK = (HSI_VALUE >> 1) * pllmull;
                        }
                        else
                        {/* HSE selected as PLL clock entry */

                            if ((RCC->CFGR & CFGR_PLLXTPRE_Mask) != (U32)RESET)
                            {/* HSE oscillator clock divided by 2 */

                                SYSCLK = (HSE_VALUE >> 1) * pllmull;
                            }
                            else
                            {
                                SYSCLK = HSE_VALUE * pllmull;
                            }
                        }
                        break;

                    default:
                        SYSCLK = HSI_VALUE;
                        break;
                }

                /* Compute HCLK, PCLK1, PCLK2 and ADCCLK clocks frequencies ----------------*/
                /* Get HCLK prescaler */
                tmp = RCC->CFGR & CFGR_HPRE_Set_Mask;
                tmp = tmp >> 4;
                presc = APBAHBPrescTable[tmp];

                /* HCLK clock frequency */
                HCLK = SYSCLK >> presc;

                /* Get PCLK1 prescaler */
                tmp = RCC->CFGR & CFGR_PPRE1_Set_Mask;
                tmp = tmp >> 8;
                presc = APBAHBPrescTable[tmp];

                /* PCLK1 clock frequency */
                PCLK1 = HCLK >> presc;

                /* Get PCLK2 prescaler */
                tmp = RCC->CFGR & CFGR_PPRE2_Set_Mask;
                tmp = tmp >> 11;
                presc = APBAHBPrescTable[tmp];

                /* PCLK2 clock frequency */
                PCLK2 = HCLK >> presc;

                /* Get ADCCLK prescaler */
                tmp = RCC->CFGR & CFGR_ADCPRE_Set_Mask;
                tmp = tmp >> 14;
                presc = ADCPrescTable[tmp];

                /* ADCCLK clock frequency */
                ADCCLK = PCLK2 / presc;
            }
        };
        static FreqImpl2 f;
    };
    FreqImpl1::FreqImpl2 FreqImpl1::f;
}
#define FREQ (FreqImpl1::f)

#undef CFGR_PLL_Mask
#undef CFGR_PLLMull_Mask
#undef CFGR_PLLSRC_Mask
#undef CFGR_PLLXTPRE_Mask
#undef CFGR_SWS_Mask
#undef CFGR_SW_Mask
#undef CFGR_HPRE_Reset_Mask
#undef CFGR_HPRE_Set_Mask
#undef CFGR_PPRE1_Reset_Mask
#undef CFGR_PPRE1_Set_Mask
#undef CFGR_PPRE2_Reset_Mask
#undef CFGR_PPRE2_Set_Mask
#undef CFGR_ADCPRE_Reset_Mask
#undef CFGR_ADCPRE_Set_Mask




#endif /* _HELPER_ST_STM32F10X_FREQ_HPP_ */
