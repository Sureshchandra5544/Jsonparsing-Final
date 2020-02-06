#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#define PORT 1234
#define server_ip "127.0.0.1"
//#define max 100
int main()
{
        int sock_fd;
	FILE *fp;
        struct sockaddr_in servaddr;
       // int n;  
        char *buffer;
        buffer=(char *)malloc(1024 *sizeof(char));
	strcpy(buffer,"ifconfig | head -100 > ifconfig.txt");
	system(buffer);
	memset(buffer,0,sizeof(buffer));
	fp=fopen("ifconfig.txt","r");
	fread(buffer,sizeof(char),1024,fp);
//	printf("%s", buffer);
        fclose(fp);
        sock_fd=socket(AF_INET,SOCK_STREAM,0);
        printf("\nSocket created");
        memset(&servaddr,0,sizeof(servaddr));
        servaddr.sin_family=AF_INET;
        servaddr.sin_addr.s_addr=inet_addr(server_ip);
        servaddr.sin_port=htons(PORT);
        connect(sock_fd,(struct sockaddr*)&servaddr,sizeof(servaddr));
        printf("\nConnected\n");
      //while(1)
        //{
                //printf("ENTER MESSAGE in client:");
                //fgets(buffer,max,stdin);
                //if(buffer==NULL)

                        //break;
                //n=strlen(buffer)+1;
                send(sock_fd,buffer,1024,0);
                while(recv(sock_fd,buffer,1024,0)!=NULL)
{
		if(buffer==NULL)
			break;
		else
			continue;
}

                //printf("SERVER: %s\n",buffer1);
        //}
        close(sock_fd);
        return 0;

}


