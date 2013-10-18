// Try to find maximum allowed simultaneous connexions for a given POP3 server
// from a unique IP source.
// i use this to optimize bruteforce  ...  because brute force will never die ! <3
// tvass

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <fcntl.h> 
#include <sys/time.h> 
#include <time.h> 
   
#define LEN 255 


int pop(struct hostent *he,int port, int count) {
        struct sockaddr_in servaddr; 
        char buf[LEN+1]="\0"; 
	int sd, num;
	char cmd[LEN+1] = "user me@domain.com\r\n";

	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(port); 
	servaddr.sin_addr = *((struct in_addr *)he->h_addr); 
	memset(&(servaddr.sin_zero), '\0', 8); 
  
         if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // open a socket 
         { 
             perror("socket");
	     return 0;
         } 

	if (connect(sd, (struct sockaddr *)&servaddr,sizeof(servaddr)) < 0) // connect 
	{ 
		if (errno!=EINPROGRESS)
			return 0;
	}
         	 	num=recv(sd, buf, LEN, 0); 
         	 	//buf[num]=0; 
	         			  buf[num]='\0'; 
		 	if(strlen(buf) > 0) {
         	 		printf("%i %s", count+1, buf);
    				if(strncmp(buf, "+OK", 3) == 0) {
				          if (send(sd, cmd, strlen(cmd)+1,0) == -1) {
 			                 	perror("send");
						return 0;
					  }
         				  num=recv(sd, buf, LEN, 0); 
	         			  buf[num]='\0';
	         			  printf("%i %s\n",count+1,buf); 
					  int res = strncmp(buf, "+OK", 3);
					  if(res == 0) {
					 	return 1;
					  } else {
					 	return 0;
					  }
				}
				else {
					return 0;
				}
		 	}
	//close(sd); 
}
  
int main(int argc, char *argv[]) 
{ 
         int count, i=0, port=0, n=0; 
         struct hostent *he; 
         FILE *fd; 
  
	 count = 0;

         if (argc != 3) 
         { 
             printf("usage: %s <host> <port>\n", argv[0]); 
             exit(1); 
         } 

         if ((he=gethostbyname(argv[1])) == NULL) 
         { 
             perror("gethostbyname"); 
	     exit(1);
         } 

         if ((port=atoi(argv[2])) == 0 ) port=110; 
  
	 while(pop(he,port, count) == 1) {
		count++;
	 } 
  
         printf("\n\nMy best score is %i\n", count); 
 }
