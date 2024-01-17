// Print a list of all files stored on a flash memory chip

#include "SDRAM_t4.h"
SDRAM_t4 sdram;
#define SDRAM_ALLOC (10*1024*1024)
#include <LittleFS.h>
LittleFS_RAM myfs;

// DELETE or EXTEND ? OR just document?

char* buf;
// SDRAM chip on Custom NXP 1062 with PJRC bootloader

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) { ; // wait for Arduino Serial Monitor
  }

  Serial.println("Initializing SDRAM");
  if (sdram.begin()) Serial.print( "\n\tSUCCESS sdram.init()\n");
  else Serial.print( "\n\tFAILED sdram.init()\n");
  buf = (char *)sdram_malloc( SDRAM_ALLOC );
  if (!myfs.begin(buf, SDRAM_ALLOC)) {
    Serial.println("initialization failed!");
    return;
  }

  Serial.print("Space Used = ");
  Serial.println(myfs.usedSize());
  Serial.print("Filesystem Size = ");
  Serial.println(myfs.totalSize());

  printDirectory(myfs);
}


void loop() {
}


void printDirectory(FS &fs) {
  Serial.println("Directory\n---------");
  printDirectory(fs.open("/"), 0);
  Serial.println();
}

void printDirectory(File dir, int numSpaces) {
   while(true) {
     File entry = dir.openNextFile();
     if (! entry) {
       //Serial.println("** no more files **");
       break;
     }
     printSpaces(numSpaces);
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numSpaces+2);
     } else {
       // files have sizes, directories do not
       printSpaces(36 - numSpaces - strlen(entry.name()));
       Serial.print("  ");
       Serial.print(entry.size(), DEC);
       DateTimeFields datetime;
       if (entry.getModifyTime(datetime)) {
         printSpaces(4);
         printTime(datetime);
       }
       Serial.println();
     }
     entry.close();
   }
}

void printSpaces(int num) {
  for (int i=0; i < num; i++) {
    Serial.print(" ");
  }
}

void printTime(const DateTimeFields tm) {
  const char *months[12] = {
    "January","February","March","April","May","June",
    "July","August","September","October","November","December"
  };
  if (tm.hour < 10) Serial.print('0');
  Serial.print(tm.hour);
  Serial.print(':');
  if (tm.min < 10) Serial.print('0');
  Serial.print(tm.min);
  Serial.print("  ");
  Serial.print(tm.mon < 12 ? months[tm.mon] : "???");
  Serial.print(" ");
  Serial.print(tm.mday);
  Serial.print(", ");
  Serial.print(tm.year + 1900);
}
