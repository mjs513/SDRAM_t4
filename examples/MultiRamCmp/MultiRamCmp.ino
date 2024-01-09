#include "SDRAM_t4.h"
SDRAM_t4 sdram;

#define CHUNKS 128*1024 // allocate in 128KB byte chunks
#define CHUNKS_4B (CHUNKS / sizeof(uint32_t)) // 4 byte uint32_T's per CHUNK
uint32_t ram1DTCM[CHUNKS_4B]; // Fixed RAM1 compile time allocation
uint32_t* ram2Ptr[5]; // create pointers for RAM2 allocations
uint32_t* sdramPtr[257]; // create pointers for SDRAM allocations

void setup() {
  uint ii, jj, kk;
  while (!Serial) ; // wait
  if ( CrashReport ) Serial.print( CrashReport );

  for ( ii = 0; ii < CHUNKS_4B; ii++ ) // Initialize RAM1 data area to some value
    ram1DTCM[ii] = ii;
  for ( ii = 0; ii < 4; ii++ )  // Alloc from RAM2/DMAMEM with malloc
    ram2Ptr[ii] = (uint32_t *)malloc( CHUNKS );
  jj = 0;
  while ( ram2Ptr[jj] ) { // Initialize each chunk of RAM2 with value from RAM1
    for ( ii = 0; ii < CHUNKS_4B; ii++ )
      ram2Ptr[jj][ii] = ram1DTCM[ii];
    jj++;
  }
  // Do begin on SDRAM then Allocate and Initialize the SDRAM chunks
  if (sdram.begin()) Serial.print( "\n\tSUCCESS sdram.init()\n");
  else Serial.print( "\n\tFAILED sdram.init()\n");
  for ( ii = 0; ii < 256; ii++ )
    sdramPtr[ii] = (uint32_t *)sdram_malloc( CHUNKS );
  jj = 0;
  while ( sdramPtr[jj] ) {
    for ( ii = 0; ii < CHUNKS_4B; ii++ )
      sdramPtr[jj][ii] = ram1DTCM[ii];
    jj++;
  }
  jj = kk = 0;
  while ( ram2Ptr[jj] ) {
    if ( 0 == memcmp( ram2Ptr[jj], ram1DTCM, CHUNKS ) ) kk++;
    jj++;
  }
  if ( kk == jj ) Serial.printf( "%u RAM2/DMAMEM compares\n", kk );
  jj = kk = 0;
  while ( sdramPtr[jj] ) {
    if ( 0 == memcmp( sdramPtr[jj], ram1DTCM, CHUNKS ) ) kk++;
    jj++;
  }
  if ( kk == jj ) Serial.printf( "%u SDRAM compares\n", kk );
Serial.println( "Completed: Should see 3 RAM2 compares and 255 for 32MB of SDRAM if present." );
}
void loop() {
}
