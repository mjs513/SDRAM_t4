#include "SDRAM_t4.h"
uint32_t readRepeat = 3;  // Writes once to Test memory, will repeat Reads and Test compare 'readRepeat' times
uint32_t readFixed = 1; // start loop and run only Fixed Patterns once
uint32_t speed = 198; //  frequencies 173,180,187,196,206,216,227,240,254,270,288,etc
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
uint size = 32;  // SDRAM 32MB Size
uint32_t *memory_begin = (uint32_t *)(0x80000000);
uint32_t *memory_end = (uint32_t *)(0x80000000 + size * 1048576);

uint32_t check_lfsr_pattern(uint32_t seed);
uint32_t check_fixed_pattern(uint32_t pattern);
uint32_t doTest(uint do_one);

// These are the tested PsuedoRandom and FIXED patterns lists to be tested:
static uint32_t lfsrPatt[] = { 2976674124ul, 1438200953ul, 3413783263ul, 1900517911ul, 1227909400ul, 276562754ul, 146878114ul, 615545407ul, 110497896ul, 74539250ul, 4197336575ul, 2280382233ul, 542894183ul, 3978544245ul, 2315909796ul, 3736286001ul, 2876690683ul, 215559886ul, 539179291ul, 537678650ul, 4001405270ul, 2169216599ul, 4036891097ul, 1535452389ul, 2959727213ul, 4219363395ul, 1036929753ul, 2125248865ul, 3177905864ul, 2399307098ul, 3847634607ul, 27467969ul, 520563506ul, 381313790ul, 4174769276ul, 3932189449ul, 4079717394ul, 868357076ul, 2474062993ul, 1502682190ul, 2471230478ul, 85016565ul, 1427530695ul, 1100533073ul };
const uint32_t lfsrCnt = sizeof(lfsrPatt) / sizeof(lfsrPatt[0]); // Count of above Psuedo Random Pattern starting values
static uint32_t fixPatt[] = { 0x5A698421, 0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF, 0x0000FFFF, 0xAAAAAAAA, 0xCCCCCCCC, 0xF0F0F0F0, 0xFF00FF00, 0xFFFF0000, 0xFFFFFFFF, 0x00000000 };
const uint32_t fixPCnt = sizeof(fixPatt) / sizeof(fixPatt[0]); // Count of Fixed patterns used for all writes for each pass

void loop() {
  uint32_t totErrs = 0;

  static bool inputSer = true;
  char chIn;
  while (Serial.available()) {  // send usb TO REPEAT TEST
    chIn = Serial.read();
    if ( '1' == chIn ) readRepeat = 100;
    if ( 'K' == chIn ) readRepeat = 1000;
    if ( 's' == chIn ) readRepeat = 3; // Fast test
    inputSer = true;
  }
  if (inputSer && size > 0) {
    uint32_t testmsec;
    uint32_t testCnt = fixPCnt;
    testCnt += lfsrCnt;

    Serial.printf("\n  --- START %u test patterns ------ with %u reReads ... wait ...\n", testCnt, readRepeat);
#ifdef USB_DUAL_SERIAL
    SerialUSB1.printf("\n  --- START %u test patterns ------ with %u reReads ... wait ...\n", testCnt, readRepeat);
#endif

    testmsec = millis();
    for (uint ii = 0; ii < fixPCnt; ii++) {
      digitalToggle(13);
#ifdef USB_DUAL_SERIAL
      SerialUSB1.printf("\n\t>>**>> FixedPatt(%u) pattern %08X with readRepeat %u  ...", ii, fixPatt[ii], readRepeat);
#endif
      totErrs += check_fixed_pattern(fixPatt[ii]);
    }
    for (uint ii = 0; ii < lfsrCnt; ii++) {
      digitalToggle(13);
#ifdef USB_DUAL_SERIAL
      SerialUSB1.printf("\n\t>>**>> PseudoRand(%u) Seed %u with readRepeat %u  ...", ii, lfsrPatt[ii], readRepeat);
#endif
      totErrs += check_lfsr_pattern(lfsrPatt[ii]);
    }
    testmsec = millis() - testmsec;
    Serial.printf("\nTest result: %u read errors\n\nExtra info: ran for %.2f seconds\n", totErrs, testmsec / 1000.0);
#ifdef USB_DUAL_SERIAL
    SerialUSB1.printf("\nDone with total errors found %u\t(time %.2f secs\n", totErrs, testmsec / 1000.0);
#endif
  }
  digitalWrite(13, HIGH);
  if ( 1 == readFixed ) {
    readFixed = 0;
    readRepeat = 100;
    inputSer = true;
  }
  else {
    inputSer = false;
  }
}

void setup() {
  while (!Serial)
    ;  // wait
  pinMode(13, OUTPUT);
  if (CrashReport) Serial.print(CrashReport);

  /**********************************************************
       sdram.begin initializes the available SDRAM Module
        Here >> begin(SIZE, SPEED, USEDQS);:
       begin(32, 166, 1);
       See library readme for more info.
     *********************************************************/
  if (sdram.begin(size, speed, true)) { // always UseDQS to test capacitance
    Serial.print("\n\tSUCCESS sdram.init()\n");
    Serial.print("\n\tSEND USB to repeat test after completion");
    Serial.print("\n\tSend '1' for 100 or 'k' gives 1K read repeats and 's' returns to start short test value.");
    Serial.print("\n\tProgress:: '#'=fixed pattern, '.'=PsuedoRand patterns, and 'F' shows Failed test pattern");
    Serial.print("\n\tIf built with DUAL Serial second SerMon will show details.\n\n");
  }
  Serial.printf("Compile Time:: " __FILE__ " " __DATE__ " " __TIME__ "\n");
  Serial.printf("EXTMEM Memory Test, %u Mbyte   ", size);
  Serial.printf("SDRAM speed %f Mhz ", sdram.getFrequency());
  Serial.printf("F_CPU_ACTUAL %u Mhz ", F_CPU_ACTUAL / 1000000);
  Serial.printf("begin@ %08X  ", memory_begin);
  Serial.printf("end@ %08X \n", memory_end);
  readFixed = 1; // start loop and run only Fixed Patterns once
}

// fill the Low half of RAM with a pseudo-random sequence, then check it against copy made to Upper half
// Test: Write same Psuedo Random values to Lower and Upper (16MB of 32MB SDRAM)
//
uint32_t check_lfsr_pattern(uint32_t seed) {
  volatile uint32_t *p;
  uint32_t testMsec;
  uint32_t reg;
  uint32_t MemResSum, MemRes;
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
  MemResSum = 0;

  for (uint32_t ii = 0; ii < readRepeat; ii++) {
    MemRes = 0;
    // validate the expected value are still read as expected
    arm_dcache_flush_delete((void *)memory_begin, (uint32_t)memory_end - (uint32_t)memory_begin);
    reg = seed;
    for (p = memory_begin; p < memory_end; p++) {
      if (*p != reg) MemRes++;
      for (int i = 0; i < 3; i++) {
        if (reg & 1) {
          reg >>= 1;
          reg ^= 0x7A5BC2E3;
        } else {
          reg >>= 1;
        }
      }
    }
    MemResSum += MemRes;
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
  uint32_t MemResSum = 0, MemRes;
  char cRes = '#';

  testMsec = micros();
  for (p = memory_begin; p < memory_end; p++) {
    *p = pattern;
  }
  arm_dcache_flush_delete((void *)memory_begin,
                          (uint32_t)memory_end - (uint32_t)memory_begin);
  for (uint ii = 0; ii < readRepeat; ii++) {
    MemRes = 0;
    for (p = memory_begin; p < memory_end; p++) {
      uint32_t actual = *p;
      if (actual != pattern) MemRes++;
    }
    MemResSum += MemRes;
  }
  testMsec = micros() - testMsec;
  if (0 != MemResSum) {
    cRes = 'F';
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
