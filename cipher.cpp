#include <string>
#include "CipherInterface.h"
#include "DES.h"
#include "AES.h"
#include <stdio.h>

using namespace std;
int main(int argc, char** argv) {
  
	/* Create an instance of the DES cipher */	
	CipherInterface* cipher = NULL;
	char* inputfile = argv[4];
	char* outputfile = argv[5];
	unsigned char* key = (unsigned char*)argv[2];
	
        /* Ensure cipher is valid */
	int des = -1;
	if (strcmp(argv[1], "des") == 0 || strcmp(argv[1], "DES") == 0) {
	  cipher = new DES;
	  des = 1;
	}
	else if (strcmp(argv[1], "aes") == 0 || strcmp(argv[1], "AES") == 0) {
	  cipher = new AES;
	  des = 0;
	}
	else {
	  fprintf(stderr, "Error: [%s %s %d]: invalid cipher: %s\n"
		  , __FILE__, __FUNCTION__, __LINE__, argv[1]);
	  exit(-1);
	}

	/* Error checks */
	if(!cipher) {
	  fprintf(stderr, "ERROR [%s %s %d]: could not allocate memory\n",	
	      __FILE__, __FUNCTION__, __LINE__);
	  exit(-1);
	}

	/* Set the encryption key */
	//cipher->setKey(key);

unsigned char hard_keyEnc[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
	
    unsigned char hard_keyDec[] = {0x01, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
	
    cipher->setKey(hard_keyEnc);
    
    unsigned char* text = cipher->encrypt((unsigned char*)"helloworld123456");
    
    cipher->setKey(hard_keyDec);
      unsigned char* decText = cipher->decrypt(text);
    fprintf(stderr, "$$$$$Decrypted: %s\n", decText);
    


	printf("Before encrypting\n");
	
    // do AES
	if (des == 0) 
    {
	    FILE* inputFile = fopen(argv[4], "rb");
	    //fseek(inputFile, 0, SEEK_END);
	    FILE* outputFile = fopen(argv[5], "w");
	    //long fileSize = ftell(inputFile);
	    //fseek(inputFile, 0, SEEK_SET);
	    //int totalBytes = fileSize;
	    if (strcmp(argv[3], "enc") == 0 || strcmp(argv[3], "ENC") == 0) {
	      //if (fileSize % 16 != 0) {
		  //totalBytes = (fileSize / 16 + 1) * 16;
	      //}
	    	    
	      int bytesRead = 0;
	      unsigned char* ciphertext = (unsigned char*) calloc(1, 16);
          if (!ciphertext) {
            exit(-1);
          }
	      char* plaintext = (char*) "helloworld123456";// (char*) calloc(1, 128);
	      
	      while (1) { //!feof(inputFile)) {
		//bytesRead = fread(plaintext, 1, 128, inputFile);
		//add padding stuff
		    ciphertext = cipher->encrypt((unsigned char*)plaintext);
		    fwrite(ciphertext, 1, 16, outputFile);
		    break;
	      }
	    }
	    //fclose(inputFile);
	    //fclose(outputFile);

	    //Decrypting
	        if (strcmp(argv[3], "dec") == 0 || strcmp(argv[3], "DEC") == 0) {
	            //hard_key[0] = 0x11;
	            int bytesRead = 0;
	            char* ciphertext = (char*) calloc(1, 16);
	            unsigned char* plaintext = (unsigned char*) calloc(1, 16);
	            while(!feof(inputFile)) {
		            bytesRead = fread(ciphertext, 1, 16, inputFile);
		            plaintext = cipher->decrypt((unsigned char*)ciphertext);
		            fwrite(plaintext, 1, 16, outputFile);
		            break;
	            }    //end while
	        }//end if
	    }//end aes
	             
	  

	/* If we're using DES */
	if (des == 1)
	{
	  FILE* inputFile = fopen(argv[4], "rb");
	  FILE* outputFile = fopen(argv[5], "w");
	  fseek(inputFile, 0, SEEK_END);
	  long fileSize = ftell(inputFile);
	  fseek(inputFile, 0, SEEK_SET);
	  int totalBytes = fileSize;

	  if (strcmp(argv[3], "enc") == 0 || strcmp(argv[3], "ENC") == 0)
	  {
	    if (fileSize % 8 != 0) // To encrpyt we need a multiple of 8 bytes
	      totalBytes = (fileSize / 8 + 1) * 8;
	   
	    char* plaintext = (char*)calloc(1, totalBytes);
	    unsigned char* ciphertext;
	    int bytesRead = 0;
	    int bytesToPad = 0;
	    char* temp = (char*) calloc(1, 8);
	    if (inputFile && outputFile)
	    {
	      bytesRead = 0;
	      while (!feof(inputFile))
	      { // (strlen(plaintext) < fileSize) {
		bytesRead = fread(plaintext, 1, 8, inputFile);
		printf("Plaintext is: %s\n", plaintext);
		printf("size of plaintext is: %li\n", strlen(plaintext));
		/* Pad the plaintext if necessary */
		if (bytesRead % 8 != 0) {
		  bytesToPad = (8 % bytesRead) - 0;
		  printf("Bytes to pad: %i\n", bytesToPad);
		  char* pad = (char*) malloc(bytesToPad);
		  int counter = 0;
		  for (int j = 0; j < bytesToPad; j++) {
		    pad[j] = '0';
		    counter++;
		  }
		  pad[counter - 1] = counter + '0';
		  plaintext[strlen(plaintext) - 0] = '0';
		  strcat(plaintext, pad);
		  free(pad);
		}
		//printf("plaintext: %s\n", plaintext);
		ciphertext = cipher->encrypt((const unsigned char*) plaintext);
		printf("ciphertext is: %s\n", ciphertext);
		fwrite(ciphertext, 1, 8, outputFile);
	      }//end while
	    }//end if
	    //free(ciphertext);
	    free(plaintext);
	    fclose(inputFile);
	    fclose(outputFile);
	  }//end if strcmp(argv[3] == "enc"
	  else if (strcmp(argv[3], "dec") == 0 || strcmp(argv[3], "DEC") == 0) {
	    /* Assume file is already a multiple of 8 bytes */
	    char* ciphertext = (char*) calloc(1, totalBytes);
	    unsigned char* plaintext;
	    while (!feof(inputFile)) {
	      int bytesRead = fread(ciphertext, 1, 8, inputFile);
	      plaintext = cipher->decrypt((const unsigned char*) ciphertext);
	      fwrite(plaintext, 1, 8, outputFile);
	    }
	    free(ciphertext);
	    fclose(inputFile);
	    fclose(outputFile);
	    
	  }
	}//end if des == 1
	return 0;
}
