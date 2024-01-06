#include "SDRAM_t4.h"

SDRAM_t4 sdram;

/************************************************************************/
/*                       SETUP EXAMPLE TEST CASE                        */
/************************************************************************/
#define PRINTF  Serial.printf
#define EXAMPLE_SEMC_START_ADDRESS 0x90000000

#define SEMC_EXAMPLE_DATALEN (0x1000U)
#define SEMC_EXAMPLE_WRITETIMES (1000U)

uint32_t sdram_writeBuffer[SEMC_EXAMPLE_DATALEN];
uint32_t sdram_readBuffer[SEMC_EXAMPLE_DATALEN];

uint32_t now;
/************************************************************************/
/*                       END SETUP EXAMPLE TEST CASE                        */
/************************************************************************/

void setup() {
    Serial.begin(9600);
    Serial.println("SDRAM Init Experiment");
    if (sdram.init()) {
        Serial.println(":-)");
        /* 32Bit data read and write. */
        SEMC_SDRAMReadWrite32Bit();
        /* 16Bit data read and write. */
        SEMC_SDRAMReadWrite16Bit();
        /* 8Bit data read and write. */
        SEMC_SDRAMReadWrite8Bit();
        PRINTF("\r\n SDRAM Example End.\r\n");
    } else {
        Serial.println("nope :(");
    }
}

void loop() {

}

void SEMC_SDRAMReadWrite32Bit(void)
{
    uint32_t index;
    uint32_t datalen = SEMC_EXAMPLE_DATALEN;
    uint32_t *sdram = (uint32_t *)EXAMPLE_SEMC_START_ADDRESS; /* SDRAM start address. */
    bool result = true;

    PRINTF("\r\n SDRAM Memory 32 bit Write Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);

    now = micros();
    /* Prepare data and write to SDRAM. */
    for (index = 0; index < datalen; index++)
    {
        sdram_writeBuffer[index] = index;
        sdram[index] = sdram_writeBuffer[index];
    }

    PRINTF("\tSDRAM Memory 32 bit Write Time (uS) %u\n", (micros() - now));

    PRINTF("\r\n SDRAM Read 32 bit Data Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);
    /* Read data from the SDRAM. */
    now = micros();
    for (index = 0; index < datalen; index++)
    {
        sdram_readBuffer[index] = sdram[index];
    }
    PRINTF("\tSDRAM Memory 32 bit Read Time (uS) %u\n", (micros() - now));

    PRINTF("\r\n SDRAM 32 bit Data Write and Read Compare Start!\r\n");
    /* Compare the two buffers. */
    now = micros();
    while (datalen--)
    {
        if (sdram_writeBuffer[datalen] != sdram_readBuffer[datalen])
        {
            result = false;
            break;
        }
    }

    if (!result)
    {
        PRINTF("\r\n SDRAM 32 bit Data Write and Read Compare Failed!\r\n");
    }
    else
    {
        PRINTF("\tSDRAM Memory 32 bit R/W Time (uS) %u", (micros() - now));
        PRINTF("\r\n SDRAM 32 bit Data Write and Read Compare Succeed!\r\n");
    }
}

static void SEMC_SDRAMReadWrite16Bit(void)
{
    uint32_t index;
    uint32_t datalen = SEMC_EXAMPLE_DATALEN;
    uint16_t *sdram = (uint16_t *)EXAMPLE_SEMC_START_ADDRESS; /* SDRAM start address. */
    bool result = true;

    PRINTF("\r\n SDRAM Memory 16 bit Write Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);

    memset(sdram_writeBuffer, 0, sizeof(sdram_writeBuffer));
    memset(sdram_readBuffer, 0, sizeof(sdram_readBuffer));

    /* Prepare data and write to SDRAM. */
    now = micros();
    for (index = 0; index < datalen; index++)
    {
        sdram_writeBuffer[index] = index % 0xFFFF;
        sdram[index] = sdram_writeBuffer[index];
    }
    PRINTF("\tSDRAM Memory 16 bit Write Time (uS) %u", (micros() - now));

    PRINTF("\r\n SDRAM Read 16 bit Data Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);
    /* Read data from the SDRAM. */
    now = micros();
    for (index = 0; index < datalen; index++)
    {
        sdram_readBuffer[index] = sdram[index];
    }
    PRINTF("\r\n SDRAM Memory 16 bit Read Time (uS) %u", (micros() - now));

    PRINTF("\r\n SDRAM 16 bit Data Write and Read Compare Start!\r\n");
    /* Compare the two buffers. */
    now = micros();
    while (datalen--)
    {
        if (sdram_writeBuffer[datalen] != sdram_readBuffer[datalen])
        {
            result = false;
            break;
        }
    }

    if (!result)
    {
        PRINTF("\r\n SDRAM 16 bit Data Write and Read Compare Failed!\r\n");
    }
    else
    {
        PRINTF("\t SDRAM Memory 16 bit Read/Write Time (uS) %u", (micros() - now));
        PRINTF("\r\n SDRAM 16 bit Data Write and Read Compare Succeed!\r\n");
    }
}

static void SEMC_SDRAMReadWrite8Bit(void)
{
    uint32_t index;
    uint32_t datalen = SEMC_EXAMPLE_DATALEN;
    uint8_t *sdram = (uint8_t *)EXAMPLE_SEMC_START_ADDRESS; /* SDRAM start address. */
    bool result = true;

    PRINTF("\r\n SDRAM Memory 8 bit Write Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);

    memset(sdram_writeBuffer, 0, sizeof(sdram_writeBuffer));
    memset(sdram_readBuffer, 0, sizeof(sdram_readBuffer));

    /* Prepare data and write to SDRAM. */
    now = micros();
    for (index = 0; index < datalen; index++)
    {
        sdram_writeBuffer[index] = index % 0x100;
        sdram[index] = sdram_writeBuffer[index];
    }
    PRINTF("\tSDRAM Memory 8 bit Write Time (uS) %u", (micros() - now));

    PRINTF("\r\n SDRAM Read 8 bit Data Start, Start Address 0x%x, Data Length %d !\r\n", sdram, datalen);
    /* Read data from the SDRAM. */
    now = micros();
    for (index = 0; index < datalen; index++)
    {
        sdram_readBuffer[index] = sdram[index];
    }
    PRINTF("\tSDRAM Memory 8 bit Read Time (uS) %u", (micros() - now));

    PRINTF("\r\n SDRAM 8 bit Data Write and Read Compare Start!\r\n");
    /* Compare the two buffers. */
    now = micros();
    while (datalen--)
    {
        if (sdram_writeBuffer[datalen] != sdram_readBuffer[datalen])
        {
            result = false;
            break;
        }
    }

    if (!result)
    {
        PRINTF("\r\n SDRAM 8 bit Data Write and Read Compare Failed!\r\n");
    }
    else
    {
        PRINTF("\r\n SDRAM 8 bit Data Write and Read Compare Succeed!\r\n");
        PRINTF("\tSDRAM Memory 8 bit Read/Write Time (uS) %u", (micros() - now));
    }
}

