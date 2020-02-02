#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#define PORT 1234
#define MAX 1024

	FILE *fp;
        char *line, *pStart, *pColon, *pSpace,*pSpace1;
        char *pMatchStr,*pmatchstr1;
        char itf_name[256];
        char itf_name1[256];
        char flag[256];
        char mtu1[256];
        char inet[256];
        char inet6[256];
        char ether[256];
        int line_count=0;

struct ifconfig_data1
{
        char *interface;
        char *HW_address;
        char *IPV4_address;
        char *IPV6_address;
        char *mtu;
        char *link_status;
};
struct ifconfig_data1 *ifconfig_ptr;

	void* multithread(void *arg)
{
	int newsock_fd=(int)arg;
	int len = 0;
        FILE *fptr;
        char *buffer1;
   	buffer1=(char *)malloc(1024 *sizeof(char)); 
	memset(buffer1,0,sizeof(buffer1));
	fptr=popen("ifconfig | head -100", "r");
        if (fptr == NULL) {
                fprintf(stderr, "ERROR: popen failed\n");
                return;
	}
	while(recv(newsock_fd,buffer1,1024,0)!=NULL)
	{	
	fwrite(buffer1,sizeof(char),strlen(buffer1),fptr);
	fscanf(fptr,"[^\n]",buffer1);
        printf("%s",buffer1);

	/*	FILE *fp;
        char *line,*pStart, *pColon, *pSpace,*pSpace1;
        char *pMatchStr,*pmatchstr1;
        char itf_name[256];
        char itf_name1[256];
        char flag[256];
        char mtu1[256];
        char inet[256];
        char inet6[256];
        char ether[256];
        int line_count=0;
        pStart = NULL;
        pColon = NULL;
        pSpace = NULL;
        pMatchStr = NULL;
        pmatchstr1 = NULL;
        pSpace1 = NULL;
*/
	line=(char *)malloc(1024*sizeof(char));
        ifconfig_ptr=(struct ifconfig_data1*)malloc(1024*sizeof(struct ifconfig_data1));
	
	 memset(&itf_name, 0, sizeof(itf_name));
        memset(&itf_name1, 0, sizeof(itf_name1));
        memset(&flag, 0, sizeof(flag));
        memset(&mtu1, 0, sizeof(mtu1));
        memset(&inet, 0, sizeof(inet));
        memset(&inet6, 0, sizeof(inet6));
        memset(&ether, 0, sizeof(ether));
        memset(line,0,sizeof(line));

  /*      fp=popen("ifconfig | head -100", "r");
        if (fp == NULL) {
                fprintf(stderr, "ERROR: popen failed\n");
                return;
        }*/

	while(fgets(line,MAX,fptr)!=NULL)
        {
                //If it is the first line
                if (++line_count == 1){
                        // Parse the line for ':' and take the first part
                        pStart = line;
                        pColon = strchr(line, ':');
                        // Get the interface name from part1
                        strncpy(itf_name, line, pColon-pStart);
                        ifconfig_ptr->interface=itf_name;
                        printf("interface = (%s)\n",ifconfig_ptr->interface);
                        //Now take the part after the colon
                        pStart=pColon++;

                }
                if(line_count=='\n') {
                        pStart = line;
                        pColon = strchr(line, ':');
                        strncpy(itf_name1, line, pColon-pStart);
                        ifconfig_ptr->interface=itf_name1;
                        printf("interface = (%s)\n",ifconfig_ptr->interface);
                }
	
		 else {
                        pStart = line;
                }

                //Getting the Flag as link_status
                if ((pMatchStr = strstr(pStart,"flags")) != NULL) {
                        if(pmatchstr1=strstr(pMatchStr, "<")){
                                pSpace= strchr(pmatchstr1,'<');
                                strncpy(flag, pmatchstr1, pSpace+3- (++pmatchstr1));
                                ifconfig_ptr->link_status=flag;
                                printf("link_status = (%s)\n",ifconfig_ptr->link_status);
                                pStart = pSpace++;
                                pMatchStr=NULL;
                        }

                }
		
			 //Getting the mtu as mtu
                if ((pMatchStr = strstr(pStart,"mtu")) != NULL) {
                        if ((pSpace = strchr(pMatchStr+4, ' ')) == NULL){
                                pSpace = strchr(pMatchStr+4, '\n');
                        }

                        strncpy(mtu1, pMatchStr+4, pSpace - (pMatchStr+4));
                        ifconfig_ptr->mtu=mtu1;
                        printf("mtu = (%s)\n",ifconfig_ptr->mtu);
                        pStart = pSpace++;
                        pMatchStr=NULL;
                }
                //Getting the inet6 as IPV6_address
               if ((pMatchStr = strstr(pStart,"inet6")) != NULL) {
                         pSpace = strchr(pMatchStr+5, ' ');
                         strcpy(inet6,pSpace);
                         pSpace1=strtok(inet6," ");
                         ifconfig_ptr->IPV6_address=pSpace1;
                         printf("IPV6_address = (%s)\n",ifconfig_ptr->IPV6_address);
                         pStart = pSpace++;
                         pMatchStr=NULL;
                }

	              //Getting ether as HW_address

                if ((pMatchStr = strstr(pStart,"ether")) != NULL) {
                        pSpace = strchr(pMatchStr+6, ' ');
                        strncpy(ether, pMatchStr+6, pSpace - (pMatchStr+6));
                        ifconfig_ptr->HW_address = ether;
                        printf("HW_address = (%s)\n",ifconfig_ptr->HW_address);
			printf("\n");
                        pStart = pSpace++;
                        pMatchStr=NULL;
                }
                //Getting inet as IPV4_address

                else if ((pMatchStr = strstr(pStart,"inet")) != NULL) {
                        pSpace = strchr(pMatchStr+5, ' ');
                        strncpy(inet, pMatchStr+5, pSpace - (pMatchStr+5));
                        ifconfig_ptr->IPV4_address=inet;
                        printf("IPV4_address = (%s)\n",ifconfig_ptr->IPV4_address);
                        pStart = pSpace++;
                        pMatchStr=NULL;
                }
        }
 //       pclose(fp);
	}
	fclose(fptr);
close(newsock_fd);
}

	int main()
{
	int sock_fd,newsock_fd,cliaddrlen;
	
	char *buffer;
	struct sockaddr_in servaddr,cliaddr;
	pthread_t serverThread;
	sock_fd=socket(AF_INET,SOCK_STREAM,0);
        printf("\nSocket created");
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(PORT);
	bind(sock_fd,(struct sockaddr*)&servaddr,sizeof(servaddr));
        printf("\nBinding");	
        listen(sock_fd,5);
        printf("\nlistening");
	while(1)
	{
		printf("\nSERVER WAITING FOR NEW CONNECTION\n");
		cliaddrlen=sizeof(cliaddr);
		newsock_fd=accept(sock_fd,(struct sockaddr*)&cliaddr,&cliaddrlen);
		printf("CONNECTED TO CLIENT\n");
		pthread_create(&serverThread,NULL,&multithread,(void *)newsock_fd);	
       	}
	return 0;
}


