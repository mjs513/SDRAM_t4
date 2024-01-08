#include "SDRAM_t4.h"
SDRAM_t4 sdram;

#define CHUNKS 128*1024
uint32_t ram1DTCM[CHUNKS / sizeof(uint32_t)];
uint32_t ram2DMA[4];
uint32_t sdramPtr[256];

void setup() {
  uint ii, jj, kk;
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(13, HIGH);
  while (!Serial) ; // wait
  if ( CrashReport ) Serial.print( CrashReport );
  for ( int ii = 0; ii < 4; ii++ )
    ram2DMA[ii] = (uint32_t *)malloc( CHUNKS );
  if (sdram.begin())
    Serial.print( "\n\tSUCCESS sdram.init()\n");
  else
    Serial.print( "\n\tFAILED sdram.init()\n");
  for ( int ii = 0; ii < 256; ii++ )
    ram2DMA[ii] = (uint32_t *)sdram_malloc( CHUNKS );
  for ( int ii = 0; ii < CHUNKS / sizeof(uint32_t); ii++ )
    ram1DTCM[ii] = ii;
  jj = 0;
  while ( ram2DMA[jj++] ) {
    for ( int ii = 0; ii < CHUNKS / sizeof(uint32_t); ii++ )
      ram2DMA[jj][ii] = ram1DTCM[ii];
  }
  jj = 0;
  while ( sdramPtr[jj++] ) {
    for ( int ii = 0; ii < CHUNKS / sizeof(uint32_t); ii++ )
      ram2DMA[jj][ii] = ram1DTCM[ii];
  }
  jj = 0;
  kk = 0;
  while ( ram2DMA[jj++] ) {
    if ( 0 == memcmp( ram2DMA[jj], ram1DTCM, CHUNKS ) ) kk++;
  }
  if ( kk == jj ) Serial.print( "%u RAM2/DMAMEM compares", kk );
  jj = 0;
  kk = 0;
  while ( sdramPtr[jj++] ) {
    if ( 0 == memcmp( sdramPtr[jj], ram1DTCM, CHUNKS ) ) kk++;
  }
  if ( kk == jj ) Serial.print( "%u SDRAM compares", kk );


}

void loop() {

}
