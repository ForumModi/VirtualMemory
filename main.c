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

/*struct pagetable {

    int isValid = 1;

}*/

int physicalMemory[65536];
int pageTable[256];


void printAddresses(int* address, int size){
    
    for (int i=0; i<size; i++) {
        printf("address[%d] = %d\n", i, address[i]);
    }

}

void outputPhysicalMemory(int* address, int size) {

    char* filename = "out2.txt";
    char* filename2 = "out3.txt";
    FILE* pAddressFile;
    FILE* valueFile;
    pAddressFile = fopen(filename, "w");
    valueFile = fopen(filename2, "w");
    unsigned int pagenum;
    unsigned int offset;
    int physicalAddress;
    int value;

    
    for(int i=0; i<size; i++) {

        pagenum = address[i] >> 8;
        offset = address[i] & 0b0000000011111111;

        physicalAddress = (pageTable[pagenum]*256) + offset;
        value = physicalMemory[physicalAddress];

        fprintf(pAddressFile, "%d\n", physicalAddress);
        fprintf(valueFile,"%d\n", value);
    
    }

}

void generatePhysicalMemory(int* address, int size){

    char* filename = "test.txt";
    char* bin_filename = "BACKING_STORE.bin";
    FILE* pAddressFile;
    FILE* binFile;
    pAddressFile = fopen(filename, "w");
    binFile = fopen(bin_filename, "rb");

    unsigned int pagenum;
    unsigned int offset;
    int frame = 0;

    for(int i=0; i<size; i++) {


        pagenum = address[i] >> 8;
        offset = address[i] & 0b0000000011111111;

        //makes buffer size of the frame
        char buf[256];
        
        if (pageTable[pagenum] > -1) continue;
            
        fseek(binFile, (pagenum * 256), SEEK_SET);
        fread(buf, sizeof(char), 256 ,binFile);
        pageTable[pagenum] = frame;
        
        

        for(int ofs=0; ofs<256; ofs++) {
            physicalMemory[ (frame * 256) + ofs] = buf[ofs];    
        }
        frame++;
    
    }

    /*for(int i=0; i<size; i++) {

        pagenum = address[i] >> 8;
        offset = address[i] & 0b0000000011111111;

        int physicalAddress = (pageTable[pagenum]*256) + offset;

        fprintf(pAddressFile, "paddress: %d value: %d\npagenum: %d offset: %d \n\n", physicalAddress, physicalMemory[physicalAddress], pagenum, offset);
        printf("paddress: %d value: %d\npagenum: %d offset: %d \n\n", physicalAddress, physicalMemory[physicalAddress], pagenum, offset);
    
    }

    /*for(int i=0; i<size; i++) 
        printf("value: %d\n", physicalMemory[(pagenum * 256) + offset]);*/

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
        address[size] = address[size] & 0x0000ffff;
        size++;
    }

    for (int i=0; i<256; i++) {
        pageTable[i] = -1;
    }

    //printAddresses(address, size);

    //closes file because we are done reading it
    fclose(addressFile);

    virtualAddress(address, size);

    generatePhysicalMemory(address, size);
    
    outputPhysicalMemory(address, size);
    //physical address 
    unsigned int physicalAddress[1]; //= address[0] & 0x00000ff;


    

}