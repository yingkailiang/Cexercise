#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>

#define MESSAGE "the is dup test\n"

int main()
{
 
 char buf[30];
 char buf2[3];
 int i;
 struct flock fl;

 for(i=0;i<3;i++)
   buf2[i]='!';

 /* 
  * open 
  */
 char *file ="test.txt";
 int f = open(file,O_RDWR);

 if(f<0) {
  perror("open()");
  close(f);
  return 1;
 } 
  
 /*
  * fcntl()
  */

 fl.l_type = F_WRLCK;
 fl.l_whence = SEEK_SET;
 fl.l_start =0;
 fl.l_len = 40;
 
 int tFcntl = fcntl(f,F_SETLK,&fl); 
 
 if(tFcntl ==-1) { 
  perror("fcntl()1");
  close(f);
  return 1;
 }

 fl.l_type = F_UNLCK;
 fl.l_whence = SEEK_SET;
 fl.l_start =0;
 fl.l_len = 40;
 tFcntl = fcntl(f,F_SETLK,&fl); 

 if(tFcntl ==-1) { 
  perror("fcntl()2");
  close(f);
  return 1;
 }

 /*
  * write 
  */
 int tWrite = write(f,buf2,3); 
 if(tWrite<0) {
  perror("write()");
  close(f);
  return 1;
 }
 
 /*
  * read 
  */
 int tRead = read(f,buf,12); 
 if(tRead<0) {
  perror("read()");
  close(f);
  return 1;
 }
 
 /*
  * pipe 
  */
 pid_t cpid;
 char pbuf;
 int pipefd[2];
 char arg[10];
 char arg2[10];
 strcpy(arg,"pipetest"); 

 if(pipe(pipefd) == -1) {
  perror("pipe");
  return 1;
 }
 
 cpid = fork(); 
 if ( cpid == -1) {
  perror("fork");
  return 1;
 } 
 if(cpid==0) { //child process 
   printf("in child process \n");
   close(pipefd[1]); /* close write end */
   while(read(pipefd[0], &pbuf,1) >0) {
     
      write(STDOUT_FILENO, &pbuf, 1);
   }   
   write(STDOUT_FILENO,"\n",1);
   close(pipefd[0]);
 } else { //parent process 
   printf("in parent process \n");
   close(pipefd[0]); /* close read end */
   write(pipefd[1],arg,strlen(arg));
   close(pipefd[1]);
   wait(NULL);
 }

 /*
  * select 
  */ 
  fd_set rfds;
  struct timeval tv;
  int retval;
 
  FD_ZERO(&rfds);
  FD_SET(0,&rfds);
  /* wait for 5 sec */
  tv.tv_sec = 1;
  tv.tv_usec=0;
  retval = select(1,&rfds, NULL, NULL, &tv);
  
  if(retval ==-1)
     perror("select");
  else if (retval)
     printf(" select() data is ready \n");
  else 
     printf("no data within five sec \n"); 
 /*
  * dup
  */
 int newfd = dup(STDOUT_FILENO); /* call dup */ 
 char sbuf[] = MESSAGE;
 
 if(newfd ==-1) {
  perror("dup()");
 } else if (write(newfd, sbuf, sizeof(sbuf)) ==-1) {
     perror("dup write()");    
   } 
 
 /*
  * dup2
  */
 char dbuf[]="this is dup2 test \n";
 
 printf("newfd change to stand input \n");
 if(dup2(f,newfd)==-1) {/* change file descriptor to stand input */
  perror("dup2");
 } else if( write(newfd,dbuf, sizeof(dbuf)) == -1) {
  perror("dup2 write()");
 }


 /*
  * close
  */
 close(f);

 printf("syscall finish %s \n",buf);
 return 0; 
}
