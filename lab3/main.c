#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "introduceerror.h"

uint16_t gencrc(uint8_t data[]);
uint16_t checkcrc(uint8_t data[], uint16_t crc);
void initcrc(void);

#define WIDTH  (8 * sizeof(uint8_t))
#define TOPBIT (1 << (WIDTH - 1))
#define POLYNOMIAL 0x4811
uint16_t  crcTable[256];

/*
* Optionally add arguments for the Bit Error Rate and Data
* format: ./main <Bit Error Rate> <Data String>
* the Bit Error Rate must be between (0.00001 and 1)
* the Data String must be under 1024 bytes or it will be truncated.
*/
int main(int argc, char **argv) {

  uint16_t crc = 0;
  // Buffer limited to 1024 bytes
  char data[1024];
  // Clear buffer, set bits to 0
  memset(data, 0, sizeof(char) * 1024);

  float ber = 0;

  if (argc == 3 ){   // Exactly 3 args: invocation, arg1, arg2
    sscanf(argv[1], "%f", &ber);
    if(ber > 1 || ber < 0.00001){  // Range check the BER
      printf("Error: Bit Error Rate out of range: %f =/ (0.00001, 1)\n", ber);
      return 1;
    }

    if (strlen(argv[2]) > 1024) {
      printf("Error: Data String too large, limit to 1024 bytes.\n" );
      return 1;
    } else {
      sscanf(argv[2], "%s",  &data);
    }
    
  } else {
    // Use default BER, prompt for Data String
    printf("Enter Data: ");
    fgets(data, 1024, stdin);               // Accept data chars from user
  }

  initcrc();

  crc = gencrc(data);         // Generate the CRC
  printf("CRC: 0x%04" PRIx16 "\n", crc);    // Display the CRC in Hexadecimal

  data[strlen(data)] = crc;                 // form the codeword

  printf("Introducing error...\n");
  IntroduceError(data, ber);                // corrupt codeword, IntroduceError

  uint16_t remainder = checkcrc(data, crc); // Apply the crc checking routine
  if(!remainder) {
    printf("No Error: 0x%04" PRIx16 "\n", remainder);
  } else {
    printf("Error detected: 0x%04" PRIx16 " != 0x%04" PRIx16 "\n", crc, remainder);
  }

  return 0;
}

/*
* Generate a crc code from a data sting and
*/
uint16_t gencrc(uint8_t data[]){
  uint8_t bits;
  uint8_t remainder = 0;
  int numbytes = strlen(data);

  for (int byte = 0; byte < numbytes; ++byte)
  {
      bits = data[byte] ^ (remainder >> (WIDTH - 8));
      remainder = crcTable[bits] ^ (remainder << 16);
  }

  return remainder;
}

/*
* Take a data string and a generated crc and returns the result of the
* Cyclic Redundancy Check. A "0" implies no errors found, any other value
* signifies an error.
*/
uint16_t checkcrc(uint8_t data[], uint16_t crc){
  uint8_t   bits;
  uint16_t  remainder = 0;
  int       nBytes = strlen(data) - 1;

  for (int byte = 0; byte < nBytes; ++byte) {
      bits = data[byte] ^ (remainder >> (WIDTH - 8));
      remainder = crcTable[bits] ^ (remainder << 16);
  }

  return remainder;
}

/*
* Initializes the CRC table used to calculate the CRC code.
*/
void initcrc(void) {
  uint8_t  remainder;
  uint8_t  bit;
  int      dividend;

  for (dividend = 0; dividend < 256; ++dividend) {
      remainder = dividend << (WIDTH - 8);
      for (bit = 8; bit > 0; --bit) {
          if (remainder & TOPBIT) {
              remainder = (remainder << 1) ^ POLYNOMIAL;
          } else {
              remainder = (remainder << 1);
          }
      }
      crcTable[dividend] = remainder;
  }
}
