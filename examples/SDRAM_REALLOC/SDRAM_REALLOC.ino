#include "SDRAM_t4.h"

//constructor for 
SDRAM_t4 sdram;

/******************************************************************************************
 * Example of using Malloc for SDRAM taken from geeksforgeeks
 * https://www.geeksforgeeks.org/dynamic-memory-allocation-in-c-using-malloc-calloc-free-and-realloc/?ref=lbp
 *
 * "... used to dynamically change the memory allocation of a previously allocated memory"
 *
 *********************************************************************************************/

void setup() {
    while(!Serial && millis() < 5000);
    Serial.begin(115200);

    if(CrashReport) {
      Serial.println(CrashReport);
      while(1) ;
    }

    /**********************************************************
     * sdram.begin initializes the available SDRAM Module
     * begin defaults to 32mb but you can specify the size
     * from begin
     * begin(uint16 external_memory_size) where size is in MB
     * 
     * In addition the SDRAM clock speed can be set using
     * begin(32, 166);
     * and if the cap has been removed on EMC_39:
     * begin(32, 166, 0);
     * See library readme for more info.
     *********************************************************/
    if(!sdram.begin()) {
      Serial.println("SDRAM FAILED TO INITIALIZE.....");
      //while(1);
    }
    
	// This pointer will hold the
	// base address of the block created
	int* ptr;
	int n, i;

	// Get the number of elements for the array
	n = 5;
	Serial.printf("Enter number of elements: %d\n", n);

	// Dynamically allocate memory using calloc()
	ptr = (int*)sdram_calloc(n, sizeof(int));

	// Check if the memory has been successfully
	// allocated by malloc or not
	if (ptr == NULL) {
		Serial.printf("Memory not allocated.\n");
		exit(0);
	}
	else {

		// Memory has been successfully allocated
		Serial.printf("Memory successfully allocated using calloc.\n");

		// Get the elements of the array
		for (i = 0; i < n; ++i) {
			ptr[i] = i + 1;
		}

		// Print the elements of the array
		Serial.printf("The elements of the array are: ");
		for (i = 0; i < n; ++i) {
      Serial.printf("%d, ", ptr[i]);
		}

		// Get the new size for the array
		n = 10;
		Serial.printf("\n\nEnter the new size of the array: %d\n", n);

		// Dynamically re-allocate memory using realloc()
    // sdram_realloc(ptr, newSize)
		ptr = (int*)sdram_realloc(ptr, n * sizeof(int));

		// Memory has been successfully allocated
		Serial.printf("Memory successfully re-allocated using realloc.\n");

		// Get the new elements of the array
		for (i = 5; i < n; ++i) {
			ptr[i] = i + 1;
		}

		// Print the elements of the array
		Serial.printf("The elements of the array are: ");
		for (i = 0; i < n; ++i) {
        Serial.printf("%d, ", ptr[i]);
		}

		sdram_free(ptr);
	}
}

void loop() {
  // put your main code here, to run repeatedly:

}
