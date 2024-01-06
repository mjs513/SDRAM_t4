#include "SDRAM_t4.h"

unsigned int SDRAM_t4::ns_to_clocks(float ns, float freq)
{
    float clocks = ceilf(ns * 1.0e-9f * freq);
    if (clocks < 1.0f) return 1;
    return (unsigned int)clocks;
}

void SDRAM_t4::configure_sdram_pins()
{
	// initialize pins
    IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_00 = 0x0110F9;   //SEMC_D0
                                                /* Slew Rate Field: Fast Slew Rate
                                                 Drive Strength Field: R0/7
                                                 Speed Field: max(200MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Enabled */
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_01 = 0x0110F9;   //SEMC_D0
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_02 = 0x0110F9;   //SEMC_D1
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_03 = 0x0110F9;   //SEMC_D2
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_04 = 0x0110F9;   //SEMC_D3
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_05 = 0x0110F9;   //SEMC_D4
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_06 = 0x0110F9;   //SEMC_D5
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_07 = 0x0110F9;   //SEMC_D6
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_08 = 0x0110F9;   //SEMC_DMO
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_08 = 0x0110F9;   //SEMC_A0
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_10 = 0x0110F9;   //SEMC_A1
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_11 = 0x0110F9;   //SEMC_A2
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_12 = 0x0110F9;   //SEMC_A3
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_13 = 0x0110F9;   //SEMC_A4
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_14 = 0x0110F9;   //SEMC_A5
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_15 = 0x0110F9;   //SEMC_A6
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_16 = 0x0110F9;   //SEMC_A7
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_17 = 0x0110F9;   //SEMC_A8
    
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_18 = 0x0110F9;   //SEMC_A9
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_19 = 0x0110F9;   //SEMC_A11
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_20 = 0x0110F9;   //SEMC_A12
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_21 = 0x0110F9;   //SEMC_BA0
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_22 = 0x0110F9;   //SEMC_BA1
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_23 = 0x0110F9;   //SEMC_A10
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_24 = 0x0110F9;   //SEMC_CAS
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_25 = 0x0110F9;   //SEMC_RAS
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_26 = 0x0110F9;   //SEMC_CLK
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_27 = 0x0110F9;   //SEMC_CKE
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_28 = 0x0110F9;   //SEMC_WE
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_29 = 0x0110F9;   //SEMC_CS0
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_30 = 0x0110F9;   //SEMC_D8
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_31 = 0x0110F9;   //SEMC_D9
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_32 = 0x0110F9;   //SEMC_D10
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_33 = 0x0110F9;   //SEMC_D11
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_34 = 0x0110F9;   //SEMC_D12
    
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_35 = 0x0110F9;   //SEMC_D13
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_36 = 0x0110F9;   //SEMC_D14
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_37 = 0x0110F9;   //SEMC_D15
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_38 = 0x0110F9;   //SEMC_DM1
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_39 = 0x0110F9;   //SEMC_DQS
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_40 = 0x0110F9;   //SEMC_MD0
	IOMUXC_SW_PAD_CTL_PAD_GPIO_EMC_41 = 0x0110F9;   //SEMC_MD1
/* Default to Pauls version
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_00 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_01 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_02 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_03 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_04 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_05 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_06 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_07 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_08 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_08 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_10 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_11 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_12 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_13 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_14 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_15 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_16 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_17 = 0x00;
    
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_18 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_19 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_20 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_21 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_22 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_23 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_24 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_25 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_26 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_27 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_28 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_29 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_30 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_31 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_32 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_33 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_34 = 0x00;
    
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_35 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_36 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_37 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_38 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_39 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_40 = 0x00;
	IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_41 = 0x00;
*/
// configure pins
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_00 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_01 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_02 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_03 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_04 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_05 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_06 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_07 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_08 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_09 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_10 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_11 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_12 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_13 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_14 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_15 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_16 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_17 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_18 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_19 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_20 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_21 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_22 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_23 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_24 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_25 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_26 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_27 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_28 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_29 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_30 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_31 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_32 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_33 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_34 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_35 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_36 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_37 = 0x10;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_38 = 0;
  IOMUXC_SW_MUX_CTL_PAD_GPIO_EMC_39 = 0x10;

}

bool SDRAM_t4::SendIPCommand(uint32_t address, uint16_t command, uint32_t write, uint32_t *read)
{
    uint32_t cmdMode;
    bool readCmd = 0;
    bool writeCmd = 0;

    /* Clear status bit */
    SEMC_INTR |= 0x1U;
    /* Set address. */
    SEMC_IPCR0= address;

    /* Check command mode. */
    cmdMode = command & 0xFU;

    readCmd = (cmdMode == 0x08);    /*  SDRAM memory read. */
    writeCmd = (cmdMode == 0x09) || (cmdMode == 0x0a);  /* SDRAM memory write, SDRAM MODE SET */

    if (writeCmd)
    {
        /* Set data. */
        SEMC_IPTXDAT = write;
    }

    /* Set command code. */
    SEMC_IPCMD = command | (((uint32_t)(((uint32_t)(0xA55A)) << 16)) & 0xFFFF0000U);
    
    /* Wait for command done. */
    bool result = IPCommandComplete();

    if (result != true)
    {
        return result;
    }

    if (readCmd)
    {
        /* Get the read data */
        *read = SEMC_IPRXDAT;
    }

    return true;
}

bool SDRAM_t4::IPCommandComplete()
{
    /* Poll status bit till command is done*/
    while (!(SEMC_INTR & 0x1))
    {
    };

    /* Clear status bit */
    SEMC_INTR |= 0x1;

    /* Check error status */
    if (SEMC_INTR & 0x2)
    {
        SEMC_INTR |= 0x2;
        return false;
    }

    return true;
}

bool SDRAM_t4::init()
{
    // use PLL3 PFD1 664.62 divided by 4 or 5, for 166 or 133 MHz
    // 5 = 133mhz
    // 4 = 166mhz - SDRAM rated,  see post #60
    // 3 = 221mhz
    const unsigned int clockdiv = 4;

    CCM_CBCDR = (CCM_CBCDR & ~(CCM_CBCDR_SEMC_PODF(7))) |
        CCM_CBCDR_SEMC_CLK_SEL | CCM_CBCDR_SEMC_ALT_CLK_SEL |
        CCM_CBCDR_SEMC_PODF(clockdiv-1);
        
    /* Experimental note (see https://forum.pjrc.com/index.php?threads/call-to-arms-teensy-sdram-true.73898/post-335619):
    *  if you want to try 198 MHz overclock
    *  const unsigned int clockdiv = 2; // PLL2_PFD2 / 2 = 396 / 2 = 198 MHz
    *
    *    CCM_CBCDR = (CCM_CBCDR & ~(CCM_CBCDR_SEMC_PODF(7) | CCM_CBCDR_SEMC_ALT_CLK_SEL)) |
    *    CCM_CBCDR_SEMC_CLK_SEL | CCM_CBCDR_SEMC_PODF(clockdiv-1);  
    * If it doesn't work, maybe try soldering a 5pF or 10pF capacitor at C29 
    */    
        
    delayMicroseconds(1);
    const float freq = 664.62e6 / (float)clockdiv;
    CCM_CCGR3 |= CCM_CCGR3_SEMC(CCM_CCGR_ON);
    


    // software reset
    SEMC_BR0 = 0;
    SEMC_BR1 = 0;
    SEMC_BR2 = 0;
    SEMC_BR3 = 0;
    SEMC_BR4 = 0;
    SEMC_BR5 = 0;
    SEMC_BR6 = 0;
    SEMC_BR7 = 0;
    SEMC_BR8 = 0;
    SEMC_MCR = SEMC_MCR_SWRST;
    elapsedMicros timeout = 0;
    while (SEMC_MCR & SEMC_MCR_SWRST) {
        if (timeout > 1500) return false;
    }

    configure_sdram_pins();

    //if(NOCAP == 1) {
    //    SEMC_MCR |= SEMC_MCR_MDIS | SEMC_MCR_CTO(0xFF) | SEMC_MCR_BTO(0x1F) | SEMC_MCR_DQSMD;
    //} else  { // enable SEMC_MCR_DQSMD (EMC_39
        SEMC_MCR |= SEMC_MCR_MDIS | SEMC_MCR_CTO(0xFF) | SEMC_MCR_BTO(0x1F);
    //}

    // TODO: reference manual page 1364 says "Recommend to set BMCR0 with 0x0 for
    // applications that require restrict sequence of transactions", same on BMCR1
    SEMC_BMCR0 = SEMC_BMCR0_WQOS(5) | SEMC_BMCR0_WAGE(8) |
        SEMC_BMCR0_WSH(0x40) | SEMC_BMCR0_WRWS(0x10);
    SEMC_BMCR1 = SEMC_BMCR1_WQOS(5) | SEMC_BMCR1_WAGE(8) |
        SEMC_BMCR1_WPH(0x60) | SEMC_BMCR1_WRWS(0x24) | SEMC_BMCR1_WBR(0x40);

    SEMC_MCR &= ~SEMC_MCR_MDIS;

    // configure SDRAM parameters
    SEMC_BR0 = 0x90000000 | SEMC_BR_MS(13 /*13 = 32 Mbyte*/) | SEMC_BR_VLD;
    SEMC_SDRAMCR0 = SEMC_SDRAMCR0_CL(3) |
        SEMC_SDRAMCR0_COL(3) |  // 3 = 9 bit column
        SEMC_SDRAMCR0_BL(3) |   // 3 = 8 word burst length
        SEMC_SDRAMCR0_PS;       // use 16 bit data
    SEMC_SDRAMCR1 =
        SEMC_SDRAMCR1_ACT2PRE((ns_to_clocks(42, freq)-1)) | // tRAS: ACTIVE to PRECHARGE
        SEMC_SDRAMCR1_CKEOFF((ns_to_clocks(42, freq)-1)) |  // self refresh
        SEMC_SDRAMCR1_WRC((ns_to_clocks(12, freq)-1)) |     // tWR: WRITE recovery
        SEMC_SDRAMCR1_RFRC((ns_to_clocks(67, freq)-1)) |    // tRFC or tXSR: REFRESH recovery
        SEMC_SDRAMCR1_ACT2RW((ns_to_clocks(18, freq)-1)) |  // tRCD: ACTIVE to READ/WRITE
        SEMC_SDRAMCR1_PRE2ACT((ns_to_clocks(18, freq)-1));  // tRP: PRECHARGE to ACTIVE/REFRESH
    SEMC_SDRAMCR2 = 0; // TODO... page 1425
    SEMC_SDRAMCR3 = 0; // TODO...page 1426
    SEMC_SDRAMCR2 =
        SEMC_SDRAMCR2_SRRC((ns_to_clocks(67, freq) - 1)) |
        SEMC_SDRAMCR2_REF2REF(ns_to_clocks(60, freq)) |           /* No Minus one to keep with RM */
        SEMC_SDRAMCR2_ACT2ACT(ns_to_clocks(60, freq)) | /* No Minus one to keep with RM */
        SEMC_SDRAMCR2_ITO(0);  //sdk has: idle = config->tIdleTimeout_Ns / config->tPrescalePeriod_Ns


    uint32_t prescaleperiod = 160 * (1000000000 / freq);
    uint16_t prescale = prescaleperiod / 16 / (1000000000 / freq);

    if (prescale > 256)
    {
        Serial.println("Invalid Timer Setting");
        while(1){}
    }

    uint16_t refresh = (64 * 1000000 / 8192)/ prescaleperiod;
    uint16_t urgentRef = refresh;
    //uint16_t idle = 0 / prescaleperiod;
    
    SEMC_SDRAMCR3 = SEMC_SDRAMCR3_REBL((1 - 1)) |
                     /* N * 16 * 1s / clkSrc_Hz = config->tPrescalePeriod_Ns */
                     SEMC_SDRAMCR3_PRESCALE(prescale) | SEMC_SDRAMCR3_RT(refresh) | SEMC_SDRAMCR3_UT(urgentRef);

    SEMC_IPCR1 = 2; // IP commadns, data is 16 bits wide
    SEMC_IPCR2 = 0;

    //  send IP commands to initialize SDRAM chip:
    //  precharge all
    //  auto refresh (NXP SDK sends this twice, why?)
    //  mode set
    bool result_cmd = SendIPCommand(0x90000000, 0x0f, 0, NULL);  //Prechargeall
    if (result_cmd != true)
    {
        return result_cmd;
    }
    result_cmd = SendIPCommand(0x90000000, 0x0c, 0, NULL);        //AutoRefresh
    if (result_cmd != true)
    {
        return result_cmd;
    }
    result_cmd = SendIPCommand(0x90000000, 0x0c, 0, NULL);         //AutoRefresh
    if (result_cmd != true)
    {
        return result_cmd;
    }
    /* Mode setting value. */
    uint16_t mode = (uint16_t)3| (uint16_t)(3 << 4);
    result_cmd = SendIPCommand(0x90000000, 0x0a, mode, NULL);       //Modeset
    if (result_cmd != true)
    {
        return result_cmd;
    }
    // enable refresh
    SEMC_SDRAMCR3 |= SEMC_SDRAMCR3_REN;

    if(result_cmd == false) return false;
    return true; // hopefully SDRAM now working at 80000000 to 81FFFFFF
}