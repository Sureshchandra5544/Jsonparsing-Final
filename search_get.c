#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX 1024
#include<ctype.h>


struct ifconfig_data1
{
        char *interface;
        char *HW_address;
        char *IPV4_address;
	char *IPV6_address;
        char *mtu;
        char *link_status;
};
struct ifconfig_data1 *ptr1;

void main()
{
        FILE *fp;
        char *line,*pStart, *pColon, *pSpace,*pSpace1;
        char *pMatchStr,*pmatchstr1;
	char itf_name[256];
	char itf_name1[256];
	char flag[256];
	char mtu1[256];
	char inet6[256];
	char inet[256];
	char ether[256];
	int line_count=0;

	pStart = NULL;
	pColon = NULL;
	pSpace = NULL;
	pMatchStr = NULL;
	pmatchstr1 = NULL;
	pSpace1 = NULL;

        line=(char *)malloc(1024*sizeof(char));
	ptr1=(struct ifconfig_data1*)malloc(1024*sizeof(struct ifconfig_data1));

	memset(&itf_name, 0, sizeof(itf_name));
	memset(&itf_name1, 0, sizeof(itf_name1));
	memset(&flag, 0, sizeof(flag));
	memset(&mtu1, 0, sizeof(mtu1));
	memset(&inet6, 0, sizeof(inet6));
	memset(&inet, 0, sizeof(inet));
	memset(&ether, 0, sizeof(ether));
        memset(line,0,sizeof(line));
	memset(ptr1,0,sizeof(ptr1));

	fp=popen("ifconfig | head -100", "r");
	if (fp == NULL) {
		fprintf(stderr, "ERROR: popen failed\n");
		return;
	}
	while(fgets(line,MAX,fp)!=NULL)
      	{
		//If it is the first line
		if (++line_count == 1){
			// Parse the line for ':' and take the first part 
			pStart = line;
			pColon = strchr(line, ':');
			// Get the interface name from part1
			strncpy(itf_name, line, pColon-pStart);
			ptr1->interface=itf_name;
			printf("interface = (%s)\n",ptr1->interface);
			//Now take the part after the colon
			pStart=pColon++;

		}
		if(line_count=='\n') {
		 	pStart = line;
                        pColon = strchr(line, ':');
			strncpy(itf_name1, line, pColon-pStart);
			ptr1->interface=itf_name1;
                        printf("interface = (%s)\n",ptr1->interface);
		}


	       
		else {
			pStart = line;
		}
		
		//Getting the Flag as link_status
		if ((pMatchStr = strstr(pStart,"flags")) != NULL) {
		        if(pmatchstr1=strstr(pMatchStr, "<")){
	                	pSpace= strchr(pmatchstr1,'<');	
				strncpy(flag, pmatchstr1, pSpace+3- (++pmatchstr1));
				ptr1->link_status=flag;
				printf("link_status = (%s)\n",ptr1->link_status);
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
			ptr1->mtu=mtu1;
			printf("mtu = (%s)\n",ptr1->mtu);
			pStart = pSpace++;
			pMatchStr=NULL;
		}
		//Getting the inet6 as IPV6_address
	       if ((pMatchStr = strstr(pStart,"inet6")) != NULL) {
                         pSpace = strchr(pMatchStr+5, ' ');
			 strcpy(inet6,pSpace);
	          	 pSpace1=strtok(inet6," ");
                         ptr1->IPV6_address=pSpace1;
                         printf("IPV6_address = (%s)\n",ptr1->IPV6_address);
                         pStart = pSpace++;
                         pMatchStr=NULL;
		}
	       //Getting ether as HW_address             

        	if ((pMatchStr = strstr(pStart,"ether")) != NULL) {
                        pSpace = strchr(pMatchStr+6, ' ');
                        strncpy(ether, pMatchStr+6, pSpace - (pMatchStr+6));
 			ptr1->HW_address = ether;
			printf("HW_address = (%s)\n",ptr1->HW_address);
                        pStart = pSpace++;
                        pMatchStr=NULL;
                }
		//Getting inet as IPV4_address
	
        	else if ((pMatchStr = strstr(pStart,"inet")) != NULL) {
                        pSpace = strchr(pMatchStr+5, ' ');
                        strncpy(inet, pMatchStr+5, pSpace - (pMatchStr+5));
                        ptr1->IPV4_address=inet;
                        printf("IPV4_address = (%s)\n",ptr1->IPV4_address);
                        pStart = pSpace++;
                        pMatchStr=NULL;
                }
	}
	pclose(fp);
}












