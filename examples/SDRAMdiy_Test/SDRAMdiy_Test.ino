#include "SDRAM_t4.h"

SDRAM_t4 sdram;

#define ERR_ABORT 20 // will print this many errors and abort this test

#define TEENSY_SDRAM 1 // '1' SDRAM ELSE '0' T_4.1 PSRAM TEST
int readRepeat = 1; // Writes once to Test memory, will repeat reads and test compare 'readRepeat' times
// https://github.com/PaulStoffregen/teensy41_psram_memtest/blob/master/teensy41_psram_memtest.ino

extern "C" uint8_t external_psram_size;

bool check_fixed_pattern(uint32_t pattern);
bool check_lfsr_pattern(uint32_t seed);

uint32_t testMsec[100][6]; // Write us, Read Test us, Val Seed/Pattern, Type {1=Pat, 2=Seed, 3=Fail)
#define vRd 0 // read time
#define vWr 1 // write time
#define vPt 2 // Pattern/Seed
#define vTp 3 // 0=unused, 1=success fixed, 2=succ PRandom, 3=fail fixed, 4=fail=PRandom, 5=succ PRand HalfCopy
#define vCt 4 // Read Cnt
#define vFl 5 // Fail Cnt
int tt = 0;

bool memory_ok = true;
uint32_t *memory_begin, *memory_mid, *memory_end;

bool check_fixed_pattern(uint32_t pattern);
bool check_lfsr_pattern(uint32_t seed);
void SDRAMsetup();
uint8_t size = external_psram_size;

uint32_t getLFSR()
{
  static uint32_t reg = 4197336575ul;
  for (int i = 0; i < 3; i++) {
    if (reg & 1) {
      reg >>= 1;
      reg ^= 0x7A5BC2E3;
    } else {
      reg >>= 1;
    }
  }
  return reg;
}

void tmr_callback() {
  static volatile uint32_t Fcount = 0;
  Fcount++;
  for ( int ii=1; ii<10; ii++ ) {
    Fcount += memory_begin[ getLFSR()%(33554432ul/4) ];
// Serial.printf( "\n %u", Fcount );
// Serial.printf( "\t %u", getLFSR()%(33554432ul/4) );
  }
}

#include "IntervalTimer.h"
IntervalTimer tTimer;
bool bTimer=false;
uint32_t cntLoopTests=0;
void loop()
{
  elapsedMillis aTime;
  digitalWrite(13, HIGH);
  if (!memory_ok) {
    delay(100);
    digitalWrite(13, LOW); // rapid blink if any test fails
    delay(100);
  }
  else {
    aTime = 0;
    doTests();
    cntLoopTests+=1;
    Serial.printf("\t\tLoop doTest() count=%u  readRepeat=%u  bTimer=%u LoopTest() Mins=%.2f\n\n", cntLoopTests, readRepeat, bTimer*1, aTime/60000.0 );
    if ( readRepeat < 50 ) {
      readRepeat += 5;
    }
  }
}

void setup()
{
  while (!Serial) ; // wait
  pinMode(13, OUTPUT);
  if ( CrashReport ) Serial.print( CrashReport );
#if TEENSY_SDRAM
  if (sdram.init()) {
    Serial.print( "\tSUCCESS sdram.init()\n");
  }
  const float clocks[4] = {396.0f, 720.0f, 664.62f, 528.0f};
  const float frequency = clocks[(CCM_CBCMR >> 8) & 3] / (float)(((CCM_CBCMR >> 29) & 7) + 1);
  Serial.printf(" CCM_CBCMR=%08X (%.1f MHz)\n", CCM_CBCMR, frequency);

  size = 32;
  memory_begin = (uint32_t *)(0x80000000);
  memory_mid = (uint32_t *)(0x80000000 + size * 524288);
  memory_end = (uint32_t *)(0x890000000 + size * 1048576);
#else // T_4.1 PSRAM
  const float clocks[4] = {396.0f, 720.0f, 664.62f, 528.0f};
  const float frequency = clocks[(CCM_CBCMR >> 8) & 3] / (float)(((CCM_CBCMR >> 29) & 7) + 1);
  Serial.printf(" CCM_CBCMR=%08X (%.1f MHz)\n", CCM_CBCMR, frequency);
  memory_begin = (uint32_t *)(0x70000000);
  memory_mid = (uint32_t *)(0x70000000 + size * 524288);
  memory_end = (uint32_t *)(0x70000000 + size * 1048576);
#endif

  Serial.printf("Compile Time:: " __FILE__ " " __DATE__ " " __TIME__ "\n");
  Serial.printf("EXTMEM Memory Test, %d Mbyte\t", size);
  Serial.printf("begin, %08X \t", memory_begin);
  Serial.printf("middle, %08X \t", memory_mid);
  Serial.printf("end, %08X \n", memory_end);
  readRepeat = 1;
}

void doTests() {
  if (size == 0) return;
  uint32_t msec = 0;
  tt=0;

  uint ii;
  static uint32_t fixPatt[ 13 ] = { 0x5A698421, 0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF, 0x0000FFFF, 0xAAAAAAAA, 0xCCCCCCCC, 0xF0F0F0F0, 0xFF00FF00, 0xFFFF0000, 0xFFFFFFFF, 0x00000000 };

  static uint32_t lfsrPatt[ 44 ] = { 2976674124ul, 1438200953ul, 3413783263ul, 1900517911ul, 1227909400ul, 276562754ul, 146878114ul, 615545407ul, 110497896ul, 74539250ul, 4197336575ul, 2280382233ul, 542894183ul, 3978544245ul, 2315909796ul, 3736286001ul, 2876690683ul, 215559886ul, 539179291ul, 537678650ul, 4001405270ul, 2169216599ul, 4036891097ul, 1535452389ul, 2959727213ul, 4219363395ul, 1036929753ul, 2125248865ul, 3177905864ul, 2399307098ul, 3847634607ul, 27467969ul, 520563506ul, 381313790ul, 4174769276ul, 3932189449ul, 4079717394ul, 868357076ul, 2474062993ul, 1502682190ul, 2471230478ul, 85016565ul, 1427530695ul, 1100533073ul };

  for ( ii=0; ii< (sizeof(fixPatt) / sizeof(fixPatt[0])); ii++ ) {
    check_fixed_pattern( fixPatt[ii] );

    bool inputSer=false;
    while ( Serial.available() ) {
      Serial.read();
      inputSer=true;
    }
    if ( inputSer ) {
      if (bTimer) {
        tTimer.end();
        Serial.printf("\n\tintervalTimer OFF\n\n");
        bTimer = false;
      }
      else {
        Serial.printf("\n\tintervalTimer ON\n\n");
        tTimer.begin(tmr_callback, 10);  // us
        bTimer = true;
      }
    }
  }
  Serial.printf("\t%u FIXED Pattern\n", ii);

  for ( ii=0; ii< (sizeof(lfsrPatt) / sizeof(lfsrPatt[0])); ii++ ) {
    check_lfsr_pattern( lfsrPatt[ii] );

    bool inputSer=false;
    while ( Serial.available() ) {
      Serial.read();
      inputSer=true;
    }
    if ( inputSer ) {
      if (bTimer) {
        tTimer.end();
        Serial.printf("\n\tintervalTimer OFF\n\n");
        bTimer = false;
      }
      else {
        Serial.printf("\n\tintervalTimer ON\n\n");
        tTimer.begin(tmr_callback, 10);  // us
        bTimer = true;
      }
    }
  }
  Serial.printf("\t%u PsudoRand Seeds\n", ii);


  msec = 0;
  int tstMBp=0;
  int rdMSp=0;
  int wrMSp=0;
  int tstMBr=0;
  int rdMSr=0;
  int wrMSr=0;

  memory_ok = true;
  for ( int ii = 0; ii < 100; ii++ ) {
    if ( 1 == testMsec[ii][vTp] ) {
      Serial.printf("test Pattern %08X Write %u us Read/Test %u us Read Cnt %u\n", testMsec[ii][vPt], testMsec[ii][vWr], testMsec[ii][vRd], testMsec[ii][vCt] );
      msec += testMsec[ii][vRd] + testMsec[ii][vWr];
      tstMBp +=2;
      rdMSp += testMsec[ii][vRd];
      wrMSp += testMsec[ii][vWr];
      if ( testMsec[ii][vFl] != 0 ) {
        Serial.printf("\t\ttest FAIL : Fixed Pattern %u Read Cnt %u Fail Cnt %u\n", testMsec[ii][vPt], testMsec[ii][vCt], testMsec[ii][vFl] );
        memory_ok = false;
      }
    }
  }
  for ( int ii = 0; ii < 100; ii++ ) {
    if ( 2 == testMsec[ii][vTp] ) {
      Serial.printf("test RndSeed %u Write %u us Read/Test %u us Read Cnt %u\n", testMsec[ii][vPt], testMsec[ii][vWr], testMsec[ii][vRd], testMsec[ii][vCt] );
      msec += testMsec[ii][vRd] + testMsec[ii][vWr];
      tstMBr +=2;
      rdMSr += testMsec[ii][vRd];
      wrMSr += testMsec[ii][vWr];
      if ( testMsec[ii][vFl] != 0 ) {
      Serial.printf("\t\ttest FAIL : Peusdo Random Seed %08X Read Cnt %u Fail Cnt %u\n", testMsec[ii][vPt], testMsec[ii][vCt], testMsec[ii][vFl] );
      memory_ok = false;
      }
    }
  }
  for ( int ii = 0; ii < 100; ii++ ) {
    if ( 5 == testMsec[ii][vTp] ) {
      Serial.printf("test RndSeed HalfCopy %u Write %u us Read/Test %u us Read Cnt %u\n", testMsec[ii][vPt], testMsec[ii][vWr], testMsec[ii][vRd], testMsec[ii][vCt] );
      msec += testMsec[ii][vRd] + testMsec[ii][vWr];
      tstMBr +=2;
      rdMSr += testMsec[ii][vRd];
      wrMSr += testMsec[ii][vWr];
      if ( testMsec[ii][vFl] != 0 ) {
        Serial.printf("\t\ttest FAIL : Peusdo Random Seed %08X Read Cnt %u Fail Cnt %u\n", testMsec[ii][vPt], testMsec[ii][vCt], testMsec[ii][vFl] );
        memory_ok = false;
      }
    }
  }
  for ( int ii = 0; ii < 100; ii++ ) {
    if ( 3 == testMsec[ii][vTp] ) {
      Serial.printf("\t\ttest FAIL : Fixed Pattern %u Read Cnt %u Fail Cnt %u\n", testMsec[ii][vPt], testMsec[ii][vCt], testMsec[ii][vFl] );
      memory_ok = false;
    }
    if ( 4 == testMsec[ii][vTp] ) {
      Serial.printf("\t\ttest FAIL : Peusdo Random Seed %08X Read Cnt %u Fail Cnt %u\n", testMsec[ii][vPt], testMsec[ii][vCt], testMsec[ii][vFl] );
      memory_ok = false;
    }
  }
  Serial.printf(" test ran for %.2f seconds\n", (float)msec / 1000000.0f);
  Serial.printf("Fixed Pattern Write ran for %.2f and Read/Test %.2f secs\n", (float)wrMSp / 1000000.0f , (float)rdMSp / 1000000.0f);
  Serial.printf("Fixed Pattern Test %.2f MB per sec\n", (float) (size*tstMBp) / ((float)(wrMSp + rdMSp) / 1000000.0f));
  Serial.printf("Fixed Pattern Test WRITES %.2f MB per sec\n", (float) (size*tstMBp/2) / ((float)(wrMSp) / 1000000.0f));
  Serial.printf("Fixed Pattern Test & READ %.2f MB per sec\n", (float) (size*tstMBp/2) / ((float)(rdMSp) / 1000000.0f));
  Serial.printf("PsuedoRnd Patt Write ran for %.2f and Read/Test %.2f secs\n", (float)wrMSr / 1000000.0f , (float)rdMSr / 1000000.0f);
  Serial.printf("PsuedoRnd Patt Test %.2f MB per sec\n", (float) (size*tstMBr) / ((float)(wrMSr + rdMSr) / 1000000.0f));
  if (memory_ok)
    Serial.printf("All memory tests passed :-) (readRepeat %u)\n", readRepeat);
}

bool fail_message(volatile uint32_t *location, uint32_t actual, uint32_t expected)
{
  Serial.printf(" Error at %08X, read %08X but expected %08X\n",
                (uint32_t)location, actual, expected);
  return false;
}

// fill the entire RAM with a fixed pattern, then check it
bool check_fixed_pattern(uint32_t pattern)
{
  volatile uint32_t *p;

  testMsec[tt][vTp] = 3;
  testMsec[tt][vCt] = 0;
  testMsec[tt][vFl] = 0;
  testMsec[tt][vPt] = pattern;
  Serial.printf("testing with fixed pattern %08X\t", pattern);
  testMsec[tt][vWr] = micros();
  for (p = memory_begin; p < memory_end; p++) {
    *p = pattern;
  }
  testMsec[tt][vWr] = micros() - testMsec[tt][vWr];
  arm_dcache_flush_delete((void *)memory_begin,
                          (uint32_t)memory_end - (uint32_t)memory_begin);
  for ( int ii = 0; ii < readRepeat; ii++) {
    testMsec[tt][vRd] = micros();
    for (p = memory_begin; p < memory_end; p++) {
      uint32_t actual = *p;
      if (actual != pattern) {
        if ( testMsec[tt][vFl] == 0 ) Serial.println();
        fail_message(p, actual, pattern);
        testMsec[tt][vFl]++;
        if ( testMsec[tt][vFl] > ERR_ABORT ) {
          Serial.printf("\t\tAborting after %u errors\n", testMsec[tt][vFl]);
          testMsec[tt][vTp] = 1;
          testMsec[tt][vRd] = micros() - testMsec[tt][vRd];
          tt++;
          return false;
        }
      }
    }
    testMsec[tt][vRd] = micros() - testMsec[tt][vRd];
    testMsec[tt][vCt]++;
    if ( readRepeat > 1 && !(ii%(1+(readRepeat/60) )) ) Serial.print('.');
  }
  testMsec[tt][vTp] = 1;
  tt++;
  Serial.println();
  return true;
}

//#define vRd 0 // read time
//#define vWr 1 // write time
//#define vPt 2 // Pattern/Seed
//#define vTp 3 // 0=unused, 1=success fixed, 2=succ PRandom, 3=fail fixed, 4=fail=PRandom, 5=succ PRand HalfCopy
//#define vCt 4 // Read Cnt
//#define vFl 5 // Fail Cnt


// fill the entire RAM with a pseudo-random sequence, then check it
bool check_lfsr_patternPJRC(uint32_t seed)
{
  volatile uint32_t *p;
  uint32_t reg;

  testMsec[tt][vTp] = 4;
  testMsec[tt][vPt] = seed;
  testMsec[tt][vCt] = 0;
  testMsec[tt][vFl] = 0;
  Serial.printf("testing with pseudo-random sequence, seed=%u\t", seed);
  reg = seed;
  testMsec[tt][vWr] = micros();
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
  testMsec[tt][vWr] = micros() - testMsec[tt][vWr];
  for ( int ii = 0; ii < readRepeat; ii++) {
    arm_dcache_flush_delete((void *)memory_begin,
                            (uint32_t)memory_end - (uint32_t)memory_begin);
    reg = seed;

    testMsec[tt][vRd] = micros();
    for (p = memory_begin; p < memory_end; p++) {
      uint32_t actual = *p;
      if (actual != reg) {
        if ( testMsec[tt][vFl] == 0 ) Serial.println();
        fail_message(p, actual, reg);
        testMsec[tt][vFl]++;
        if ( testMsec[tt][vFl] > ERR_ABORT ) {
          Serial.printf("\t\tAborting after %u errors\n", testMsec[tt][vFl]);
          testMsec[tt][vTp] = 2;
          testMsec[tt][vRd] = micros() - testMsec[tt][vRd];
          tt++;
          return false;
        }
      }
      //Serial.printf(" reg=%08X\n", reg);
      for (int i = 0; i < 3; i++) {
        if (reg & 1) {
          reg >>= 1;
          reg ^= 0x7A5BC2E3;
        } else {
          reg >>= 1;
        }
      }
    }
    testMsec[tt][vRd] = micros() - testMsec[tt][vRd];
    testMsec[tt][vCt]++;
    if ( readRepeat > 1 && !(ii%(1+(readRepeat/60) )) ) Serial.print('.');
  }
  testMsec[tt][vTp] = 2;
  tt++;
  Serial.println();
  return true;
}

void memory_copy32(uint32_t *dest, const uint32_t *src, uint32_t *dest_end)
{
#if 0
  if (dest == src) return;
  do {
    *dest++ = *src++;
  } while (dest < dest_end);
#else
  asm volatile(
  " cmp %[src], %[dest]   \n"
  " beq.n 2f      \n"
  "1: ldr.w r3, [%[src]], #4  \n"
  " str.w r3, [%[dest]], #4 \n"
  " cmp %[end], %[dest]   \n"
  " bhi.n 1b      \n"
  "2:         \n"
  : [dest] "+r" (dest), [src] "+r" (src) : [end] "r" (dest_end) : "r3", "memory");
#endif
}

void memory_copy16(uint32_t *d32, const uint32_t *s32, uint32_t *de32)
{
  uint16_t *dest = (uint16_t *)d32;
  uint16_t *src = (uint16_t *)s32;
  uint16_t *dest_end = (uint16_t *)de32;
  if (dest == src) return;
  do {
    *dest++ = *src++;
  } while (dest < dest_end);
}

void memory_copy8(uint32_t *d32, const uint32_t *s32, uint32_t *de32)
{
  uint8_t *dest = (uint8_t *)d32;
  uint8_t *src = (uint8_t *)s32;
  uint8_t *dest_end = (uint8_t *)de32;
  if (dest == src) return;
  do {
    *dest++ = *src++;
  } while (dest < dest_end);
}

// fill the entire RAM with a pseudo-random sequence, then check it
bool check_lfsr_pattern(uint32_t seed)
{
  volatile uint32_t *p;
  uint32_t reg;
  static uint16_t idxHalfCopy=32;

  testMsec[tt][vTp] = 4;
  testMsec[tt][vPt] = seed;
  testMsec[tt][vCt] = 0;
  testMsec[tt][vFl] = 0;
  Serial.printf("testing with pseudo-random sequence HalfCopy %d, seed=%u\t", idxHalfCopy, seed );
  reg = seed;
  testMsec[tt][vWr] = micros();
  for (p = memory_begin; p < memory_mid; p++) {
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
  testMsec[tt][vWr] = micros() - testMsec[tt][vWr];

  switch( idxHalfCopy ) {
    case 32:
    default:
      memory_copy32( memory_mid, memory_begin, memory_end );
      idxHalfCopy = 16;
      break;
    case 16:
      memory_copy16( memory_mid, memory_begin, memory_end );
      idxHalfCopy = 8;
      break;
    case 8:
      memory_copy8( memory_mid, memory_begin, memory_end );
      idxHalfCopy = 32;
      break;
  }
  for ( int ii = 0; ii < readRepeat; ii++) {
    arm_dcache_flush_delete((void *)memory_begin,
                            (uint32_t)memory_end - (uint32_t)memory_begin);
    reg = seed;

    testMsec[tt][vRd] = micros();
    for (p = memory_mid; p < memory_end; p++) {
      uint32_t actual = *p;
      if (actual != reg) {
        if ( testMsec[tt][vFl] == 0 ) Serial.println();
        fail_message(p, actual, reg);
        testMsec[tt][vFl]++;
        if ( testMsec[tt][vFl] > ERR_ABORT ) {
          Serial.printf("\t\tAborting after %u errors\n", testMsec[tt][vFl]);
          testMsec[tt][vTp] = 5;
          testMsec[tt][vRd] = micros() - testMsec[tt][vRd];
          tt++;
          return false;
        }
      }
      //Serial.printf(" reg=%08X\n", reg);
      for (int i = 0; i < 3; i++) {
        if (reg & 1) {
          reg >>= 1;
          reg ^= 0x7A5BC2E3;
        } else {
          reg >>= 1;
        }
      }
    }
    testMsec[tt][vRd] = micros() - testMsec[tt][vRd];
    testMsec[tt][vCt]++;
    if ( readRepeat > 1 && !(ii%(1+(readRepeat/60) )) ) Serial.print('.');
  }
  testMsec[tt][vTp] = 5;
  tt++;
  Serial.println();
  return true;
}
