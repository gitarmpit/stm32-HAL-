//////////////////////////////////////////////////////////
#define RCC_CFGR  (*(volatile uint32_t*)(RCC_BASE + 0x8))

//clock source
#define RCC_CFGR_SW_HSI      (0<<0)
#define RCC_CFGR_SW_HSE      (1<<0)
#define RCC_CFGR_SW_PLL_P    (2<<0)
#define RCC_CFGR_SW_PLL_R    (3<<0)
#define RCC_CFGR_SW_CLEARMASK    (3<<0)

#define RCC_CFGR_SWS_HSI      (0<<2)
#define RCC_CFGR_SWS_HSE      (1<<2)
#define RCC_CFGR_SWS_PLL_P    (2<<2)
#define RCC_CFGR_SWS_PLL_R    (3<<2)

#define RCC_CFGR_AHB_HCLK_PRE_POS 4
#define RCC_CFGR_AHB_HCLK_PRE_SYSCLK_DIV1   (0<<4) 
#define RCC_CFGR_AHB_HCLK_PRE_SYSCLK_DIV2   (8<<4) 
#define RCC_CFGR_AHB_HCLK_PRE_SYSCLK_DIV4   (9<<4)
#define RCC_CFGR_AHB_HCLK_PRE_SYSCLK_DIV8   (10<<4) 
#define RCC_CFGR_AHB_HCLK_PRE_SYSCLK_DIV16  (11<<4) 
#define RCC_CFGR_AHB_HCLK_PRE_SYSCLK_DIV64  (12<<4) 
#define RCC_CFGR_AHB_HCLK_PRE_SYSCLK_DIV128 (13<<4) 
#define RCC_CFGR_AHB_HCLK_PRE_SYSCLK_DIV256 (14<<4) 
#define RCC_CFGR_AHB_HCLK_PRE_SYSCLK_DIV512 (15<<4)
#define RCC_CFGR_AHB_HCLK_PRE_SYSCLK_CLEARMASK (15<<4)

#define RCC_CFGR_APB1_PCLK1_PRE_POS 10
#define RCC_CFGR_APB1_PCLK1_PRE_AHB_HCLK_DIV1  (0<<10) 
#define RCC_CFGR_APB1_PCLK1_PRE_AHB_HCLK_DIV2  (4<<10)
#define RCC_CFGR_APB1_PCLK1_PRE_AHB_HCLK_DIV4  (5<<10)
#define RCC_CFGR_APB1_PCLK1_PRE_AHB_HCLK_DIV8  (6<<10)
#define RCC_CFGR_APB1_PCLK1_PRE_AHB_HCLK_DIV16 (7<<10)
#define RCC_CFGR_APB1_PCLK1_PRE_AHB_HCLK_CLEARMASK (7<<10)

#define RCC_CFGR_APB1_PCLK2_PRE_POS  13
#define RCC_CFGR_APB2_PCLK2_PRE_AHB_HCLK_DIV1  (0<<13)
#define RCC_CFGR_APB2_PCLK2_PRE_AHB_HCLK_DIV2  (4<<13)
#define RCC_CFGR_APB2_PCLK2_PRE_AHB_HCLK_DIV4  (5<<13) 
#define RCC_CFGR_APB2_PCLK2_PRE_AHB_HCLK_DIV8  (6<<13) 
#define RCC_CFGR_APB2_PCLK2_PRE_AHB_HCLK_DIV16 (7<<13) 
#define RCC_CFGR_APB2_PCLK2_PRE_AHB_HCLK_CLEARMASK (7<<13)

//HSE division factor for RTC clock
//RTC clock has to be 1Mhz
#define RCC_CFGR_RTCPRE  16
#define RCC_CFGR_RTCPRE_CLEARMASK (0x1f<<16)

#define RCC_CFGR_MCO1_HSI  (0<<21)  
#define RCC_CFGR_MCO1_LSE  (1<<21)  
#define RCC_CFGR_MCO1_HSE  (2<<21)  
#define RCC_CFGR_MCO1_PLL  (3<<21)  
#define RCC_CFGR_MCO1_CLEARMASK (3<<21)

#define RCC_CFGR_MCO1_PRE_DIV1 (0<<24)
#define RCC_CFGR_MCO1_PRE_DIV2 (4<<24)
#define RCC_CFGR_MCO1_PRE_DIV3 (5<<24)
#define RCC_CFGR_MCO1_PRE_DIV4 (6<<24)
#define RCC_CFGR_MCO1_PRE_DIV5 (7<<24)
#define RCC_CFGR_MCO1_PRE_CLEARMASK (3<<24)

#define RCC_CFGR_MCO2_PRE_DIV1 (0<<27)
#define RCC_CFGR_MCO2_PRE_DIV2 (4<<27)
#define RCC_CFGR_MCO2_PRE_DIV3 (5<<27)
#define RCC_CFGR_MCO2_PRE_DIV4 (6<<27)
#define RCC_CFGR_MCO2_PRE_DIV5 (7<<27)
#define RCC_CFGR_MCO2_PRE_CLEARMASK (3<<27)

#define RCC_CFGR_MCO2_SYSCLK  (0<<30)  
#define RCC_CFGR_MCO2_PLLI2S  (1<<30)  
#define RCC_CFGR_MCO2_HSE  (2<<30)  
#define RCC_CFGR_MCO2_PLL  (3<<30)  
#define RCC_CFGR_MCO2_CLEARMASK (3<<30)
