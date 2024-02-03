#include "SDRAM_t4.h"
const uint32_t speed = 254; // allowed range is 100 to 360 MHz
/********************************************************************
   This test is meant to evaluate how well different capacitors connected to the
   DQS pin (EMC_39) improve timing margin.  If you have created a custom PCB and
   you are wondering which capacitor is best to use on your PCB, you can use this
   test to compare the effect of different capacitors.

   Run this test at a high enough overclock speed for some read errors to occur.
   When run at error free rated speed, you have no way to gauge how well the
   capacitor improves timing.  Only by overclocking to the point where errors occur
   can you see the difference between capacitors.  The capacitor which allow for
   fewest read errors with overclocking can be assumed to give best timing margin
   when running at rated speed.

   Remember, the total capacitance is the sum of the pin's self capacitance,
   the capacitance of your PCB trace and pads, and whatever capacitor you connect.
   Results may vary slightly depending for different PCB designs and stackup specs.

   Example that does extensive pattern write and (re)Read to test memory integrity:

   This example does direct memory addressing from memory_begin to memory_end
   Lists of patterns Random and Fixed that are written and then Read and tested to be as expected

   Successful output below, if a progress indicator "#" or "." shows "F" that test pattern failed.
   If built with Dual Serial a second SerMon can show added in progress output

   Expected Output:
  EXTMEM Memory Test, 32 Mbyte   SDRAM speed 206 Mhz F_CPU_ACTUAL 600 Mhz begin@ 80000000  end@ 82000000

  --- START 57 test patterns ------ with 3 reReads ... wait ...
  #############............................................
  Test result: 0 read errors

  Extra info: ran for 86.35 seconds

  --- START 57 test patterns ------ with 100 reReads ... wait ...
  #############............................................
  Test result: 0 read errors

  Extra info: ran for 2500.23 seconds *****************************************************************************/

// constructor for SDRAM - though here the memory pool is accessed by direct address
SDRAM_t4 sdram;
const uint8_t size = 32;  // SDRAM 32MB Size
uint32_t *memory_begin = (uint32_t *)(0x80000000);
uint32_t *memory_end = (uint32_t *)(0x80000000 + size * 1048576);

uint32_t check_lfsr_pattern(uint32_t seed);
uint32_t check_fixed_pattern(uint32_t pattern);

// These are the tested PsuedoRandom and FIXED patterns lists to be tested:
static uint32_t lfsrPatt[] = { 2976674124ul, 1438200953ul, 3413783263ul, 1900517911ul, 1227909400ul, 276562754ul, 146878114ul, 615545407ul, 110497896ul, 74539250ul, 4197336575ul, 2280382233ul, 542894183ul, 3978544245ul, 2315909796ul, 3736286001ul, 2876690683ul, 215559886ul, 539179291ul, 537678650ul, 4001405270ul, 2169216599ul, 4036891097ul, 1535452389ul, 2959727213ul, 4219363395ul, 1036929753ul, 2125248865ul, 3177905864ul, 2399307098ul, 3847634607ul, 27467969ul, 520563506ul, 381313790ul, 4174769276ul, 3932189449ul, 4079717394ul, 868357076ul, 2474062993ul, 1502682190ul, 2471230478ul, 85016565ul, 1427530695ul, 1100533073ul };
const uint32_t lfsrCnt = sizeof(lfsrPatt) / sizeof(lfsrPatt[0]); // Count of above Psuedo Random Pattern starting values
static uint32_t fixPatt[] = { 0x5A698421, 0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF, 0x0000FFFF, 0xAAAAAAAA, 0xCCCCCCCC, 0xF0F0F0F0, 0xFF00FF00, 0xFFFF0000, 0xFFFFFFFF, 0x00000000 };
const uint32_t fixPCnt = sizeof(fixPatt) / sizeof(fixPatt[0]); // Count of Fixed patterns used for all writes for each pass

void setup() {
  while (!Serial)
    ;  // wait
  pinMode(13, OUTPUT);
  if (CrashReport) Serial.print(CrashReport);

  Serial.println("Test capacitor effect effect on SDRAM read timing margin");
  // sdram.begin initializes the available SDRAM Module.  See library readme for more info.
  if (sdram.begin(size, speed, true)) { // always UseDQS to test capacitance
    Serial.print("\n\tSDRAM hardware initialized.\n");
    Serial.print("\n\tThis test takes approximately 5 minutes to complete.");
    Serial.print("\n\tProgress:: '#'=fixed pattern, '.'=PsuedoRand patterns, and 'f' or 'F' shows Failed test pattern");
    Serial.print("\n\tIf built with DUAL Serial second SerMon will show details.\n\n");
  } else {
    Serial.print("\n\tCould not initialize SDRAM hardware\n");
    return;
  }

  const uint32_t testCnt = fixPCnt + lfsrCnt;
  Serial.printf("\n  --- START %u test patterns ... wait ...\n", testCnt);
#ifdef USB_DUAL_SERIAL
  SerialUSB1.printf("\n  --- START %u test patterns ... wait ...\n", testCnt);
#endif
  uint32_t testmsec = millis();
  uint32_t totalErrs = 0;
  for (uint ii = 0; ii < fixPCnt; ii++) {
    digitalToggle(13);
#ifdef USB_DUAL_SERIAL
    SerialUSB1.printf("\n\t>>**>> FixedPatt(%u) pattern %08X ...", ii, fixPatt[ii]);
#endif
    totalErrs += check_fixed_pattern(fixPatt[ii]);
  }
  for (uint ii = 0; ii < lfsrCnt; ii++) {
    digitalToggle(13);
#ifdef USB_DUAL_SERIAL
    SerialUSB1.printf("\n\t>>**>> PseudoRand(%u) Seed %u ...", ii, lfsrPatt[ii]);
#endif
    totalErrs += check_lfsr_pattern(lfsrPatt[ii]);
  }
  testmsec = millis() - testmsec;
  const double totalReads = (double)size/sizeof(uint32_t) * 1048576 * 25 * testCnt;
  const float errPercent = totalErrs / totalReads * 100.0;
  Serial.printf("\nTest result: %u read errors (%.4f%%)\n\n", totalErrs, errPercent);
  if (totalErrs == 0) {
    Serial.println("Normally no read errors is good, but for comparing the effect of your");
    Serial.println("capacitor connected to DQS (EMC_39) you need errors!  When all reads are");
    Serial.println("correct, you can not know if the error-free result is because you have");
    Serial.println("good timing margin or your SDRAM reads are just barely succeeding.  Only");
    Serial.println("when you see errors can you estimate your capacitor's effect on timing.");
    Serial.println("Please increase the speed and re-run this test for each capacitor.  Use");
    Serial.println("a speed fast enough for all tests to give at least some errors.  The");
    Serial.println("difference in number of errors allows you to gauge the relative effect");
    Serial.println("of various capacitors to choose the best capacitor for your PCB design.");
  }
  Serial.printf("Extra info: ran for %.2f seconds\n", testmsec / 1000.0);
#ifdef USB_DUAL_SERIAL
  SerialUSB1.printf("\nDone with total errors found %llu\t(time %.2f secs\n", totalErrs, testmsec / 1000.0);
#endif

  Serial.println("\n\n");
  Serial.printf("Compile Time:: " __FILE__ " " __DATE__ " " __TIME__ "\n");
  Serial.printf("EXTMEM Memory Test, %u Mbyte   ", size);
  Serial.printf("SDRAM speed %.2f Mhz ", sdram.getFrequency());
  Serial.printf("F_CPU_ACTUAL %u Mhz ", F_CPU_ACTUAL / 1000000);
  Serial.printf("begin@ %08X  ", memory_begin);
  Serial.printf("end@ %08X \n", memory_end);
}

void loop() {
}

// fill the Low half of RAM with a pseudo-random sequence, then check it against copy made to Upper half
// Test: Write same Psuedo Random values to Lower and Upper (16MB of 32MB SDRAM)
//
uint32_t check_lfsr_pattern(uint32_t seed) {
  volatile uint32_t *p;
  uint32_t testMsec;
  uint32_t reg;
  uint32_t MemResSum = 0;
  char cRes = '.';

  reg = seed;
  testMsec = micros();
  for (p = memory_begin; p < memory_end; p++) {
    *p = reg;
    for (int i = 0; i < 3; i++) {
      if (reg & 1) {
        reg >>= 1;
        reg ^= 0x7A5BC2E3;
      } else {
        reg >>= 1;
      }
    }
  }

  for (uint32_t ii = 0; ii < 25; ii++) {
    // validate the expected value are still read as expected
    arm_dcache_flush_delete((void *)memory_begin, (uint32_t)memory_end - (uint32_t)memory_begin);
    reg = seed;
    for (p = memory_begin; p < memory_end; p++) {
      if (*p != reg) MemResSum++;
      for (int i = 0; i < 3; i++) {
        if (reg & 1) {
          reg >>= 1;
          reg ^= 0x7A5BC2E3;
        } else {
          reg >>= 1;
        }
      }
    }
  }

  testMsec = micros() - testMsec;
  if (0 != MemResSum) {
    cRes = 'F';
#ifdef USB_DUAL_SERIAL
    SerialUSB1.printf("\n\tFail pseuRand seq. not same with seed=%u", seed);
#endif
  }
  Serial.printf("%c", cRes);
#ifdef USB_DUAL_SERIAL
  SerialUSB1.printf("\nTest pseudo-random sequence Compare time secs %.2f with \tError Cnt %u\n", testMsec / 1000000.0, MemResSum);
#endif
  return MemResSum;
}

// fill the entire RAM with a fixed pattern, then check it
uint32_t check_fixed_pattern(uint32_t pattern) {
  volatile uint32_t *p;
  uint32_t testMsec;
  uint32_t MemResSum = 0;
  char cRes = '#';

  testMsec = micros();
  for (p = memory_begin; p < memory_end; p++) {
    *p = pattern;
  }
  for (uint ii = 0; ii < 25; ii++) {
    arm_dcache_flush_delete((void *)memory_begin, (uint32_t)memory_end - (uint32_t)memory_begin);
    for (p = memory_begin; p < memory_end; p++) {
      uint32_t actual = *p;
      if (actual != pattern) MemResSum++;
    }
  }
  testMsec = micros() - testMsec;
  if (0 != MemResSum) {
    cRes = 'f';
#ifdef USB_DUAL_SERIAL
    SerialUSB1.printf("\n\tFail fixed pattern not same with seed=%u", pattern);
#endif
  }
  Serial.printf("%c", cRes);
#ifdef USB_DUAL_SERIAL
  SerialUSB1.printf("\nTest fixed pattern Compare time secs %.2f with \tError Cnt %u\n", testMsec / 1000000.0, MemResSum);
#endif
  return MemResSum;
}
