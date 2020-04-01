A program for simulating basic virtual memory, using pure demand paging.

- `BACKING_STORE.bin` : file of binary values; can be read using 
`xxd -b BACKING_STORE.bin | less` which displays 10k lines of 6 bytes each, for our total of 65,536 bytes.


In the first part of the project, these will be split into 256 frames of 256 bytes each (256 * 256 = 65,536), representing physical memory split into 256-bit length frames.
2^8 = 256, so 8 bits is sufficient to address all 256 frames and another 8 bits is sufficient to address each of the 256 bytes in any given frame in our physical memory (backing store). Our virtual/logical addresses thus are 16 bits long.

- `addresses.txt` - contains a list of 16-bit virtual addresses (in decimal, 8 bytes page number, 8 bytes offset), to be parsed into corresponding 16-bit physical addresses.


