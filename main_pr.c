/* 
Project2
Operating Systems
Forum Modi & AJ Viola
4/11/2020
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <wait.h>


//creates physical memory size # of frames(128) * pagesize(256)
int physicalMemory[128 * 256];

//creates pagetable size of # of pages
int pageTable[256];

//counters for amount of hits and page faults
int TLB_hits = 0;
int page_faults = 0;

//TLB struct that holds page number and frame number
typedef struct TLBentry {

    int page_number;
    int frame_number; 

}TLBentry;

//creates 16 TLBentries
struct TLBentry TLB[16];



//Debugging: prints out address
void printAddresses(int* address, int size){
    
    for (int i=0; i<size; i++) {
        printf("address[%d] = %d\n", i, address[i]);
    }

}

//uses demand paging on physical mem
void dpagingPhysicalMemory(int* address, int size){

    //creates and opens files out2.txt and out3.txt to write to them
    char* filename = "out2.txt";
    char* filename2 = "out3.txt";
    FILE* pAddressFile;
    FILE* valueFile;
    pAddressFile = fopen(filename, "w");
    valueFile = fopen(filename2, "w");

    //creates and opens BACKING_STORE.bin
    char* bin_filename = "BACKING_STORE.bin";
    FILE* binFile;
    binFile = fopen(bin_filename, "rb"); //opens file and reads in binary

    //creates pagenum, offset, physicalAddress, and value vars
    unsigned int pagenum;
    unsigned int offset;
    int physicalAddress;
    int value;

    //boolean for if a TLBhit
    int isTLBhit = 0;

    //initializes frame to -1
    int frame = -1;
    int prevFrame = -1;

    int TLB_entry_num = 0;

    //for all addresses
    for(int i=0; i<size; i++) {

        frame = prevFrame;

        isTLBhit = 0;

        //gets pagenum and offset
        pagenum = address[i] >> 8;
        offset = address[i] & 0b0000000011111111;

        //makes buffer size of the frame
        char buf[256];
        
        //if this address' pagenum has not been filled, fill it in phys mem
        if (pageTable[pagenum] == -1) {

            frame = (frame + 1) % 128;
            prevFrame = frame;

            //if physical memory exists at this frame
            if (physicalMemory[frame*256] != -1) {

                for (int j=0;j<256;j++) {
                    if (pageTable[j] == frame) {
                        pageTable[j] = -1;
                    }
                }

                /*HOW DOES THE TLB WORK IF YOU REMOVE A FRAME FROM MEMORY IF THE TLB
                WHAT RESPECTS FIFO*/

                for (int j=0;j<16;j++) {
                    if (TLB[j].frame_number == frame) {
                        TLB[j].frame_number = -1;
                        TLB[j].page_number = -1;
                    }
                }

            } 


            //points to the start of page in file
            fseek(binFile, (pagenum * 256), SEEK_SET);

            //reads full page and stores in buf
            fread(buf, sizeof(char), 256 ,binFile);

            //maps pagenum to specific frame
            pageTable[pagenum] = frame;

            //stores buf in array that holds physical memory
            for(int ofs=0; ofs<256; ofs++) 
                physicalMemory[ (frame * 256) + ofs] = buf[ofs];

            page_faults++;


        }    

        //check if in TLB
        else {

            for (int i=0;i<16;i++) {
                //if in TLB
                if (TLB[i].page_number == pagenum) {
                    
                    frame = TLB[i].frame_number;
                    
                    //Debugging print statement prints out TLB values
                    /*printf("TLB hit %d: page:%d\n", TLB_hits, pagenum);
                    for (int i=0; i<16; i++) {
                        printf("TLB[%d]: page, %d frame %d \n", i, TLB[i].page_number, TLB[i].frame_number);
                    }
                    printf("\n");*/
                    
                    isTLBhit = 1;
                    TLB_hits++;
                    break;
                }

            }

        }

        if (!isTLBhit) {
            
            //Debugging print statement prints out TLB values

            /*printf("TLB miss: %d\n", pagenum);
            for (int i=0; i<16; i++) {
                printf("TLB[%d]: page, %d frame %d \n", i, TLB[i].page_number, TLB[i].frame_number);
            }
            printf("Entry Num:%d\n", TLB_entry_num);
            printf("\n");*/

            TLB[TLB_entry_num].page_number = pagenum;
            TLB[TLB_entry_num].frame_number = pageTable[pagenum];
            frame = pageTable[pagenum];
            printf("TLB NUM: %d\n", TLB_entry_num);

            TLB_entry_num = (TLB_entry_num + 1) % 16;
            

        }

        physicalAddress = (frame*256) + offset; //physical address is frame*256 + offset
        value = physicalMemory[physicalAddress]; //finds the value of index physicalAddress in physicalMemory array

        fprintf(pAddressFile, "%d\n", physicalAddress); //prints physical address to out2.txt
        fprintf(valueFile,"%d\n", value); //prints value to out3.txt

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

    //intializes physical Memory to -1
    for (int i = 0; i < 32768; i++) {
        physicalMemory[i] = -1;
    }

    //intializes page table to -1
    for (int i=0; i<256; i++) {
        pageTable[i] = -1;
    }

    for (int i=0; i<16; i++) {
        (TLB[i]).page_number=-1;
        (TLB[i]).frame_number=-1;
    }

    //debugging method to check if address input is correct
    //printAddresses(address, size);

    //closes file because we are done reading it
    fclose(addressFile);

    //calls virtual address (generates out1.txt)
    virtualAddress(address, size);

    //calls dpagingPhysical Memory to use demand paging on physical memory and generate out2.txt and out3.txt
    dpagingPhysicalMemory(address, size);

    printf("Page Faults: %d / %d = %f\n", page_faults, size, page_faults/ (float)size );
    printf("TLB hits: %d / %d = %f\n", TLB_hits, size, TLB_hits / (float)size );

}