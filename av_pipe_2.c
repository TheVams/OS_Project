//*******************************************
//
// OS mini project source code.
// Author: Vivek John Sudhakar, Arun Ramana
//                    
//*******************************************

#include<sys/types.h>
#include<unistd.h>
#include<sys/ipc.h>

#include<sys/shm.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "av_pipe.h"


#define K 1024
#define BUF_SIZE 2048
#define MAX_PIPES 12

// LOG value of 1 enables Logging, value of 0 disables Logging
#define LOG 0 

//int shmid;
int start_read=50;
int start_write=200;

static int SHMKEY;
static int count=0;
struct avpipe
{
	int shmid;
	int read_pipe_id;
	int write_pipe_id;
	char *addr_write;
	char *addr_read;
	int bytes_written;
	int bytes_read;
}pipe_descriptor[MAX_PIPES];


void avpipe_init()
{
	if(LOG)
		printf("in avpipe_init\n");
	int i;
	if(count==0)
	{
		if(LOG)
			printf("First call to init.\n");
		count++;
		for(i=0;i<MAX_PIPES;i++)
		{
			// initializing the avpipe structure for this pipe
			pipe_descriptor[i].shmid=-1;
			pipe_descriptor[i].read_pipe_id=-1;
			pipe_descriptor[i].write_pipe_id=-1;
			pipe_descriptor[i].bytes_written=0;
			pipe_descriptor[i].bytes_read=0;
		}
	}
		else{
			count++;
		}
	
//	shmid=shmget(SHMKEY,2048,0777|IPC_CREAT);
//	SHMKEY++;
	
	if(LOG)
		printf("End of init\n");
}

int avpipe(int fd[])
{

	int i=0;
	if(LOG)
		printf("In avpipe\n");
	avpipe_init();
	for(i=0;i<MAX_PIPES;i++)
	{
		if((pipe_descriptor[i].read_pipe_id==-1)&&(pipe_descriptor[i].write_pipe_id==-1))
		{
			if(LOG)
				printf("Found unused descriptor\n");
			pipe_descriptor[i].shmid=shmget(SHMKEY,2048,0777|IPC_CREAT);
			SHMKEY++;
			pipe_descriptor[i].addr_write=shmat(pipe_descriptor[i].shmid,0,0);
			pipe_descriptor[i].addr_read=shmat(pipe_descriptor[i].shmid,0,0);
			pipe_descriptor[i].read_pipe_id=start_read;
			pipe_descriptor[i].write_pipe_id=start_write;
			start_read++;
			start_write++;
			fd[0]=pipe_descriptor[i].read_pipe_id;
			fd[1]=pipe_descriptor[i].write_pipe_id;
			return 0;
		}
	}
	return -1;
}

int avpipe_read(int fd0,char *buf,int size)
{

	if(LOG)
		printf("In avpipe_read\n");
	int i;
	for(i=0;i<MAX_PIPES;i++)
	{
		if(pipe_descriptor[i].read_pipe_id==fd0)
		{
			if(LOG)
				printf("Read descriptor found: %d\n",pipe_descriptor[i].read_pipe_id);

			break;
		}
	}
//	if((pipe_descriptor[i].bytes_written>0)&&(pipe_descriptor[i].bytes_read<pipe_descriptor[i].bytes_written))
//	{
		if(LOG)

			printf("\nReading\n");
		memcpy(buf,pipe_descriptor[i].addr_read,size);
		pipe_descriptor[i].addr_read+=size;
		pipe_descriptor[i].bytes_read+=size;
//	}
//	else{
//		printf("Pipe empty");
//		return -1;
//	}
	return 0;
	
}

int avpipe_write(int fd1,char *buf,int n)
{
	if(LOG)
		printf("In avpipe_write\n");
	int i;
	for(i=0;i<MAX_PIPES;i++)
	{
		if(pipe_descriptor[i].write_pipe_id==fd1)
		{
			if(LOG)
				printf("Write descriptor found: %d\n",pipe_descriptor[i].write_pipe_id);
			break;
		}
	}
	int rem=BUF_SIZE-pipe_descriptor[i].bytes_written;

	if(n>BUF_SIZE){
		printf("Error: Size exceeds limit.\n");
		return -1;
	}
	if(n>rem){
		printf("Error: Bytes to write exceed available space\n");
		return -1;
	}

	if((pipe_descriptor[i].bytes_written<BUF_SIZE))
	{
		if(LOG)
			printf("Copying Data\n");
		memcpy(pipe_descriptor[i].addr_write,buf,n);
		pipe_descriptor[i].addr_write+=n;
		pipe_descriptor[i].bytes_written+=n;
	}
	else{
		printf("Error: Segment value reached");
		return -1;
	}
	return 0;
}

void avpipe_close(int fd)
{
	if(LOG)
		printf("In avpipe_close.\n");
	int i;
	for(i=0;i<MAX_PIPES;i++)
	{
		if((pipe_descriptor[i].read_pipe_id==fd))
		{
			if(LOG)
				printf("\nFile descriptor to close found");
			pipe_descriptor[i].read_pipe_id=-1;
			break;
		}
		if((pipe_descriptor[i].write_pipe_id==fd))
		{
			pipe_descriptor[i].write_pipe_id=-1;
			break;
	}
	}

//	shmctl(pipe_descriptor[i].shmid,IPC_RMID,0);
}


