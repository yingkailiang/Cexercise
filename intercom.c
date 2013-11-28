#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <string.h>

int main()
{
    /*
     * open
     */
    char *file ="test.txt";
    int f = open(file,O_RDWR | O_CREAT);
    
    if(f<0) {
        perror("open()");
        close(f);
        return 1;
    } 
   
    /*
     * pipe from client to server
     */
    pid_t cpid;
    char pbuf;
    char arg2[10];
    int pipefd[2]; /* two pipe */
    int pipefd2[2];
    
    char arg[10];
    int input1,input2; //get input from client
    char input3[1];
    strcpy(arg,"pipetest");
    /* client --> server */
    if(pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }
    
    /* server --> client */
    if(pipe(pipefd2) == -1) {
        perror("pipe");
        return 1;
    }

    cpid = fork();
    if ( cpid == -1) {
        perror("fork");
        return 1;
    }
    if(cpid==0) { //child process: read input and write to parent
        close(pipefd[0]); /* close read end */
        close(pipefd2[1]); /*close write end */
        printf("input operation:\n");
        scanf("%s",&input3);
        printf("input first number:\n");
        scanf("%d",&input1);
        printf("input second number:\n");
        scanf("%d",&input2);
        
        sprintf(arg,"%d%d%s",input1,input2,input3);
        printf("child get string : %s \n",arg);
        write(pipefd[1],arg,strlen(arg));
        
        
        //read data from server
        fd_set rfds2;
        struct timeval tv2;
        int retval2;
        int maxfd2;
        char arg3[10];
        
        FD_ZERO(&rfds2);
        FD_SET(pipefd2[0],&rfds2);
        tv2.tv_sec=10;
        tv2.tv_usec =0;
        maxfd2=pipefd2[0];
        retval2 = select(maxfd2+1, &rfds2,NULL,NULL,NULL);
        if(retval2 ==-1)
            perror("select");
        else if (FD_ISSET(pipefd2[0],&rfds2) && read(pipefd2[0], arg3,3) >0) {
            write(STDOUT_FILENO, arg3, 3);
            write(STDOUT_FILENO,"\n",1);
        }

        
        close(pipefd[1]);
    } else { //parent process: calculate
        close(pipefd[1]); /* close write end */
        close(pipefd2[0]); /* close read end */
        
        fd_set rfds;
        struct timeval tv;
        int retval;
        int maxfd;
        maxfd=pipefd[0];
        
        FD_ZERO(&rfds);
        FD_SET(pipefd[0],&rfds);
        tv.tv_sec =10;
        tv.tv_usec=0;
        
        retval = select(maxfd+1, &rfds,NULL,NULL,NULL);
        if(retval ==-1)
            perror("select");
        else if ( FD_ISSET(pipefd[0],&rfds) && read(pipefd[0], arg2,1) >0) {
            write(STDOUT_FILENO, arg2, 1);
            write(STDOUT_FILENO,"\n",1);
        }
        else
            printf("no data within five sec \n");

        //calculate here
        int a1,a2,result;
       
        a1=(int)arg2[0];
        a2=(int)arg2[1];
        printf("operating:%d %d %c\n",arg2[0],arg2[1],arg2[2]);
        if(arg2[2]=='q')
       
           exit(0);
        else if (arg2[2] =='+') {
            result = a1+a2;
        }
        else if (arg2[2] =='*') {
            result = a1*a2;
        }
        else if (arg2[2] =='-') {
            result = a1-a2;
        }
        else if (arg2[2] =='/') {
            result = a1/a2;
        }
        else
            printf(" no operation match\n");
        char result2[10];
        sprintf(result2,"%d",result);
        write(pipefd2[1],result2,strlen(result2));
        close(pipefd2[1]);
        close(pipefd[0]);
        wait(NULL);
    }
   

   printf("test \n");
   return 1;
}
