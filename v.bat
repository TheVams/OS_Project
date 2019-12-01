gcc -o av_pipe.o -c av_pipe_2.c
gcc sample_pipe.c av_pipe.o -o sample_pipe
gcc sample_pipe2.c av_pipe.o -o sample_pipe2

