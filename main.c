/* 
Project2
Operating Systems
Forum Modi & AJ Viola
4/1/2020
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <wait.h>


void printAddresses(int* address, int size){
    
    
    for (int i=0; i<size; i++) {
        printf("address[%d] = %d\n", i, address[i]);
    }

}

void physicalAddress(int* address, int size){

    char* filename = "out2.txt";
    FILE* pAddressFile;
    pAddressFile = fopen(filename, "w");

    unsigned int pagenum;
    unsigned int offset;

    for(int i=0; i<size; i++) {

        pagenum = address[i] >> 8;
        offset = address[i] & 0b0000000011111111;

        fprintf(pAddressFile, "virtual address: %d offset: %d pagenum: %d\n", address[i], offset, pagenum);
    }

    fclose(pAddressFile);

}

void virtualAddress(int* address, int size){

    char* filename = "out1.txt";
    FILE* vAddressFile;

    vAddressFile = fopen(filename, "w");

    for(int i=0; i<size; i++) {
        fprintf(vAddressFile, "%d\n", address[i]);
    }

    fclose(vAddressFile);

}

int main(int argc, char** argv) {

    char* filename = argv[1];
    int address[10000];
    int size = 0;

    /*creates file and name of input file*/
    FILE* addressFile;

    /*opens the file in addresses */
    addressFile = fopen(filename, "r");

    /*checks if could not open file*/
    if (addressFile == NULL) {
        fprintf(stderr, "input file issue:");
        exit(1);
    }

    while (fscanf(addressFile, "%d", &address[size]) != EOF) {
        size++;
    }

    //closes file because we are done reading it
    fclose(addressFile);

    virtualAddress(address, size);

    physicalAddress(address, size);
    //physical address 
    unsigned int physicalAddress[1]; //= address[0] & 0x00000ff;


    

}