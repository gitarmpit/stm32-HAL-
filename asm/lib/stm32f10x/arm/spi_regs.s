

SPI1_BASE  EQU 0x40013000  ;//APB2

SPI1_CR1   EQU   SPI1_BASE
SPI1_CR2   EQU   SPI1_BASE + 0x4
SPI1_SR    EQU   SPI1_BASE + 0x8    
SPI1_DR    EQU   SPI1_BASE + 0xc        
SPI1_CRCPR  EQU   SPI1_BASE + 0x10      
SPI1_RXCRCR EQU SPI1_BASE + 0x14       
SPI1_TXCRCR EQU SPI1_BASE + 0x18           


    IF :DEF:STM32F10X_MD || :DEF:STM32F10X_MD_VL || :DEF:STM32F10X_HD \
        || :DEF:STM32F10X_HD_VL || :DEF:STM32F10X_XL || :DEF:STM32F10X_CL \
        || :DEF:STM32F4  || :DEF:STM32L0

SPI2_BASE  EQU 0x40003800  ;//APB1

SPI2_CR1   EQU   SPI2_BASE
SPI2_CR2   EQU   SPI2_BASE + 0x4
SPI2_SR    EQU   SPI2_BASE + 0x8    
SPI2_DR    EQU   SPI2_BASE + 0xc        
SPI2_CRCPR EQU   SPI2_BASE + 0x10       
SPI2_RXCRCR EQU SPI2_BASE + 0x14       
SPI2_TXCRCR EQU SPI2_BASE + 0x18           

    ENDIF

    IF :DEF: STM32F10X_HD || :DEF: STM32F10X_HD_VL || :DEF: STM32F10X_XL || :DEF: STM32F10X_CL \
        || :DEF: STM32F4



SPI3_BASE  EQU 0x40003C00  ;//APB1  EQU (High density 100x, all 10x)
SPI3_CR1   EQU   SPI3_BASE
SPI3_CR2   EQU   SPI3_BASE + 0x4
SPI3_SR    EQU   SPI3_BASE + 0x8    
SPI3_DR    EQU   SPI3_BASE + 0xc        
SPI3_CRCPR EQU   SPI3_BASE + 0x10       
SPI3_RXCRCR EQU SPI3_BASE + 0x14       
SPI3_TXCRCR EQU SPI3_BASE + 0x18           

    ENDIF


SPI_CR1_CPHA_FIRST        EQU (0<<0)
SPI_CR1_CPHA_SECOND       EQU (1<<0)
SPI_CR1_CPOL_0            EQU (0<<1)
SPI_CR1_CPOL_1            EQU (1<<1)
SPI_CR1_SLAVE             EQU (0<<2)
SPI_CR1_MASTER            EQU (1<<2)
SPI_CR1_BR_FPCLK_DIV_2    EQU (0<<3)
SPI_CR1_BR_FPCLK_DIV_4    EQU (1<<3) 
SPI_CR1_BR_FPCLK_DIV_8    EQU (2<<3) 
SPI_CR1_BR_FPCLK_DIV_16   EQU (3<<3)
SPI_CR1_BR_FPCLK_DIV_32   EQU (4<<3)
SPI_CR1_BR_FPCLK_DIV_64   EQU (5<<3)
SPI_CR1_BR_FPCLK_DIV_128  EQU (6<<3)
SPI_CR1_BR_FPCLK_DIV_256  EQU (7<<3)
SPI_CR1_SPE_ENABLED       EQU (1<<6)
SPI_CR1_LSBFIRST_OFF      EQU (0<<7)
SPI_CR1_LSBFIRST_ON       EQU (1<<7)
SPI_CR1_SSI               EQU (1<<8)
SPI_CR1_SSM               EQU (1<<9) 
SPI_CR1_RXONLY            EQU (1<<10) 
SPI_CR1_DFF_8_BIT         EQU (0<<11)
SPI_CR1_DFF_16_BIT        EQU (1<<11)
SPI_CR1_CRCNEXT           EQU (1<<12) 
SPI_CR1_CRCEN             EQU (1<<13) 
SPI_CR1_BIDIOE            EQU (1<<14) 
SPI_CR1_BIDIMODE          EQU (1<<14) 


SPI_CR2_RXDMAEN          EQU (1<<0)
SPI_CR2_TXDMAEN          EQU (1<<1)
SPI_CR2_SSOE             EQU (1<<2)
SPI_CR2_ERRIE            EQU (1<<5)
SPI_CR2_RXNEIE           EQU (1<<6) 
SPI_CR2_TXEIE            EQU (1<<7) 

SPI_SR_RXNE              EQU (1<<0) 
SPI_SR_TXE               EQU (1<<1) 
SPI_SR_CRCERR            EQU (1<<4)
SPI_SR_MODF              EQU (1<<5) 
SPI_SR_OVR               EQU (1<<6) 
SPI_SR_BSY               EQU (1<<7) 


SPI_BAUD_RATE_2 EQU SPI_CR1_BR_FPCLK_DIV_2
SPI_BAUD_RATE_4 EQU SPI_CR1_BR_FPCLK_DIV_4
SPI_BAUD_RATE_8 EQU SPI_CR1_BR_FPCLK_DIV_8
SPI_BAUD_RATE_16 EQU SPI_CR1_BR_FPCLK_DIV_16
SPI_BAUD_RATE_32 EQU SPI_CR1_BR_FPCLK_DIV_32
SPI_BAUD_RATE_64 EQU SPI_CR1_BR_FPCLK_DIV_64
SPI_BAUD_RATE_128 EQU SPI_CR1_BR_FPCLK_DIV_128
SPI_BAUD_RATE_256 EQU SPI_CR1_BR_FPCLK_DIV_256


    END