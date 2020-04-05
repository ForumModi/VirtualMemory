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

/*struct TLB {

    int isValid = 1;

}*/

//creates physical memory size # of pages * pagesize
int physicalMemory[65536];

//creates pagetable size of # of pages
int pageTable[256];


//Debugging: prints out address
void printAddresses(int* address, int size){
    
    for (int i=0; i<size; i++) {
        printf("address[%d] = %d\n", i, address[i]);
    }

}

//outputs PhysicalMemory in out2.txt and outputs value in memory in out3.txt
void outputPhysicalMemory(int* address, int size) {

    //creates and opens files out2.txt and out3.txt to write to them
    char* filename = "out2.txt";
    char* filename2 = "out3.txt";
    FILE* pAddressFile;
    FILE* valueFile;
    pAddressFile = fopen(filename, "w");
    valueFile = fopen(filename2, "w");

    //creates pagenum, offset, physicalAddress, and value vars
    unsigned int pagenum;
    unsigned int offset;
    int physicalAddress;
    int value;

    //for the amt of addresses
    for(int i=0; i<size; i++) {

        pagenum = address[i] >> 8; //shifts address to the left by 8 bits to get pagenum
        offset = address[i] & 0b0000000011111111; //masks first 8 bits to get offset

        physicalAddress = (pageTable[pagenum]*256) + offset; //physical address is frmae*256 + offset
        value = physicalMemory[physicalAddress]; //finds the value of index physicalAddress in physicalMemory array

        fprintf(pAddressFile, "%d\n", physicalAddress); //prints physical address to out2.txt
        fprintf(valueFile,"%d\n", value); //prints value to out3.txt
    
    }

}

//generates physicalMemory in an array
void generatePhysicalMemory(int* address, int size){

    //creates and opens BACKING_STORE.bin
    char* bin_filename = "BACKING_STORE.bin";
    FILE* binFile;
    binFile = fopen(bin_filename, "rb"); //opens file and reads in binary

    //creates pagenum and offset vars
    unsigned int pagenum;
    unsigned int offset;

    //initializes frame to 0
    int frame = 0;

    //for all addresses
    for(int i=0; i<size; i++) {

        //gets pagenum and offset
        pagenum = address[i] >> 8;
        offset = address[i] & 0b0000000011111111;

        //makes buffer size of the frame
        char buf[256];
        
        //if this address' pagenum has already been filled move to next address
        if (pageTable[pagenum] > -1) continue;
            
        //points to the start of page in file
        fseek(binFile, (pagenum * 256), SEEK_SET);

        //reads full page and stores in buf
        fread(buf, sizeof(char), 256 ,binFile);

        //maps pagenum to specific frame
        pageTable[pagenum] = frame;

        //stores buf in array that holds physical memory
        for(int ofs=0; ofs<256; ofs++) 
            physicalMemory[ (frame * 256) + ofs] = buf[ofs];    
        
        //increments frame
        frame++;
    }
}

//prints out virtual address in out1.txt
void virtualAddress(int* address, int size){

    //creates and opens out1.txt file for writing
    char* filename = "out1.txt";
    FILE* vAddressFile;
    vAddressFile = fopen(filename, "w");

    //prints out address into out1.txt
    for(int i=0; i<size; i++) {
        fprintf(vAddressFile, "%d\n", address[i]);
    }

    //closes file
    fclose(vAddressFile);

}


int main(int argc, char** argv) {

    //reads in filename from command line
    char* filename = argv[1];

    //makes address array
    int address[10000];

    //initializes size to 0
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

    //scans address from file
    while (fscanf(addressFile, "%d", &address[size]) != EOF) {
        address[size] = address[size] & 0x0000ffff; //masks address to last 16 bits
        size++; //increments size to get amt of addresses
    }

    //intializes page table to -1
    for (int i=0; i<256; i++) {
        pageTable[i] = -1;
    }

    //debugging method to check if address input is correct
    //printAddresses(address, size);

    //closes file because we are done reading it
    fclose(addressFile);

    //calls virtual address (generates out1.txt)
    virtualAddress(address, size);

    //calls generate physical memory, fills physicalmemory array
    generatePhysicalMemory(address, size);
    
    //calls output physicalmemory (generates out2.txt and out3.txt)
    outputPhysicalMemory(address, size);

    //physical address 
    unsigned int physicalAddress[1]; //= address[0] & 0x00000ff;


    

}