// OS mini project header file.
// By Vivek John Sudhakar and Arun Ramana.
//
//

void avpipe_init(); // To instantiate the shared memory region.

int avpipe(int f[]); // To attach a pipe to a process.Returns -1 on error.

int avpipe_read(int,char *,int); // To read data from the pipe.

int avpipe_write(int,char *,int); // To write data into the pipe.

void avpipe_close(int); // To close the pipe.

