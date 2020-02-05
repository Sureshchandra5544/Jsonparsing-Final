#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX 1024
#include<ctype.h>


struct ifconfig_data1
{
        char interface[20];
        char HW_addresss[20];
        char IPV4_addresss[20];
        char mtu[20];
        char link_status[20];
};
struct ifconfig_data1 *ptr1;

void main()
{
        FILE *fp;
        char *line,*value, *pStart, *pColon, *pSpace, *pSpace1;
        char *buffer1,*buffer, *pMatchStr,*pmatchstr1;
	char itf_name[256];
	char flag[256];
	char mtu[256];
	char inet6[256];
	char inet[256];
	char ether[256];
	int line_count=0;

	pStart = NULL;
	pColon = NULL;
	pSpace = NULL;
	pSpace1 = NULL;
	pMatchStr = NULL;
	pmatchstr1 = NULL;


        line=(char *)malloc(1024*sizeof(char));

	memset(&itf_name, 0, sizeof(itf_name));
	memset(&flag, 0, sizeof(flag));
	memset(&mtu, 0, sizeof(mtu));
	memset(&inet6, 0, sizeof(inet6));
	memset(&inet, 0, sizeof(inet));
	memset(&ether, 0, sizeof(ether));
        memset(line,0,sizeof(line));

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
	//		printf ("pStart = (%s)\n", pStart);
	//		printf ("pColon = (%s)\n", pColon);
	//		printf ("line = (%s)\n", line);
	//		printf ("itf_name = (%s)\n", itf_name);

			//Now take the part after the colon
			pStart=pColon++;
		} else {
			pStart = line;
			}

		if ((pMatchStr = strstr(pStart,"flags")) != NULL) {
		        if(pmatchstr1=strstr(pMatchStr, "<")){
	                pSpace= strchr(pmatchstr1,'<');	
			strncpy(flag, pmatchstr1, pSpace+3 - (++pmatchstr1));
			printf ("flags = (%s)\n", flag);
			pStart = pSpace++;
			pMatchStr=NULL;
	                     }   

		}

		if ((pMatchStr = strstr(pStart,"mtu")) != NULL) {
			if ((pSpace = strchr(pMatchStr+4, ' ')) == NULL){
				pSpace = strchr(pMatchStr+4, '\n');
			}

			strncpy(mtu, pMatchStr+4, pSpace - (pMatchStr+4));
			printf ("mtu = (%s)\n", mtu);
			pStart = pSpace++;
			pMatchStr=NULL;
		}

    /*          if ((pMatchStr = strstr(pStart,"inet")) != NULL) {

	       	        pSpace = strchr(pMatchStr+5, ' ');
                        strncpy(inet, pMatchStr+5, pSpace - (pMatchStr+5));
                        printf ("inet  = (%s)\n", inet);
                        pStart = pSpace++;
                        pMatchStr=NULL;
                }*/
		if ((pMatchStr = strstr(pStart,"inet6 ")) != NULL) {

                        pSpace = strchr(pMatchStr+5, ' ');
                        strcpy(inet6, pSpace);
                        pSpace1 = strtok(inet6, " ");
                        printf ("inet6 = (%s)\n", pSpace1);

                        pStart = pSpace++;
                        pMatchStr=NULL;
                }

		if ((pMatchStr = strstr(pStart,"ether")) != NULL) {
                        pSpace = strchr(pMatchStr+6, ' ');
                        strncpy(ether, pMatchStr+6, pSpace - (pMatchStr+6));

                        printf ("ether = (%s)\n",ether);
                        pStart = pSpace++;
                        pMatchStr=NULL;
                }
		else if ((pMatchStr = strstr(pStart,"inet")) != NULL) {

                        pSpace = strchr(pMatchStr+5, ' ');
                        strncpy(inet, pMatchStr+5, pSpace - (pMatchStr+5));
                        printf ("inet  = (%s)\n", inet);
                        pStart = pSpace++;
                        pMatchStr=NULL;
                }

	}
	pclose(fp);
}












