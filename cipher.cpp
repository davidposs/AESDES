#include <string>
#include "CipherInterface.h"
#include "DES.h"
#include "AES.h"
#include <stdio.h>

/**
 * Reads a file, encrypts it, and writes it back
 * @param cipher - the cipher class
 * @param inFile - the input file
 * @param outFile - the output file
 * @param maxBlockSize - the maximum block size
 * @param mode - the mode (0 = encryption, 1 = deycryption)
 * return - the number of bytes processes or -1 on error
 */
int processFile(CipherInterface* cipher, FILE* inputFile, FILE* outputFile, int maxBlockSize, const bool& mode) {
    /* The class pointer is invalid */
    if(!cipher) { fprintf(stderr, "Invalid cipher class!\n"); return -1;}

    /* Invalid file pointers */
    if(!inputFile || !outputFile) {fprintf(stderr, "One of the files is a NULL!\n"); return -1;}

    /* The number of bytes read */
    int bytesRead = -1;

    /* The number of bytes processed */
    int bytesProcessed = 0;

    /* The file buffer */
    char fileBuffer[maxBlockSize];

    /* Read the entire file */
    while(bytesRead) {
        /* Read from the input file */
        bytesRead = fread(fileBuffer, 1, maxBlockSize, inputFile);

        /* The number of bytes read */
        bytesProcessed += bytesRead;

        fprintf(stderr, "Read %d\n", bytesRead);

        /* Something went wrong */
        if(bytesRead < 0)
        {
            fprintf(stderr, "File read failed!\n");
            exit(-1);
        }

        /* We got stuff to process */
        if(bytesRead)
        {
            if(!mode) {
                fprintf(stderr, "In encryption!\n");
                memset(fileBuffer + bytesRead, 0, maxBlockSize - bytesRead);
                if(fwrite(cipher->encrypt((const unsigned char*)fileBuffer), 1, maxBlockSize, outputFile) == 0) {
                    perror("Error writing file\n");
                    exit(-1);
                }
            }
            else {
                if(fwrite(cipher->decrypt((const unsigned char*)fileBuffer), 1, maxBlockSize, outputFile) == 0) {
                    perror("Error writing file\n");
                }
            }
        }
    }
    return bytesProcessed;
}

using namespace std;
int main(int argc, char** argv) {

    /* Create an instance of the DES cipher */	
    CipherInterface* cipher = NULL;
    char* inputfile = argv[4];
    char* outputfile = argv[5];
    unsigned char* key = (unsigned char*)argv[2];

    /* Ensure cipher is valid */
    int cipherMode = -1;
    if (strcmp(argv[1], "des") == 0 || strcmp(argv[1], "DES") == 0) {
        cipher = new DES;
        cipherMode = 1;
    }
    else if (strcmp(argv[1], "aes") == 0 || strcmp(argv[1], "AES") == 0) {
        cipher = new AES;
        cipherMode = 0;
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

    bool mode;
    if (strcmp(argv[1], "AES") == 0) {
        if(strcmp(argv[3], "ENC") == 0) {
            mode = false;
            fprintf(stderr, "Encryption!\n");
            char newKey[17];
            newKey[0] = 0x00;
            strncpy(newKey + 1 , (char*)key, 16);
            cipher->setKey((const unsigned char*)newKey);
        }
        if(strcmp(argv[3], "DEC") == 0){
            fprintf(stderr, "Decryption!\n");
            mode = true;
            char newKey[17];
            newKey[0] = 0x01;
            strncpy(newKey + 1, (char*)key, 16);
            cipher->setKey((const unsigned char*)newKey);
        }
    }


    int blockSize;
    if (cipherMode == 1) {
        blockSize = 8;
    }
    else {
        blockSize = 16;
    }
    
    FILE* inFile = fopen(inputfile, "rb");
    FILE* outFile = fopen(outputfile, "wb");
    fprintf(stderr, "Block size %d\n", blockSize); 
    fprintf(stderr, "Mode %d\n", (int)mode); 

    processFile(cipher, inFile, outFile, blockSize, mode);

    fclose(inFile);
    fclose(outFile);

    return 0;
}
