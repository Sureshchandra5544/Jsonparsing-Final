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
struct ifconfig_data1 *ifconfig_ptr1;

int main()
{
        FILE *fp,*fp1;
        char *line,*pStart, *pColon, *pSpace,*pSpace1;
        char *pMatchStr,*pmatchstr1;
	char itf_name[256];
	char itf_name1[256];
	char flag[256];
	char mtu1[256];
	char inet[256];
	char inet6[256];
	char ether[256];
	int  line_count=0;
        char result[100];
	char result1[100];
	int  HW_SET_FLAG = 0;

	pStart = NULL;
	pColon = NULL;
	pSpace = NULL;
	pMatchStr = NULL;
	pmatchstr1 = NULL;
	pSpace1 = NULL;

        line=(char *)malloc(1024*sizeof(char));
	ifconfig_ptr1=(struct ifconfig_data1*)malloc(1024*sizeof(struct ifconfig_data1));

	memset(&itf_name, 0, sizeof(itf_name));
	memset(&result, 0, sizeof(result));
	memset(&itf_name1, 0, sizeof(itf_name1));
	memset(&flag, 0, sizeof(flag));
	memset(&mtu1, 0, sizeof(mtu1));
	memset(&inet, 0, sizeof(inet));
	memset(&inet6, 0, sizeof(inet6));
	memset(&ether, 0, sizeof(ether));
        memset(line,0,sizeof(line));
	memset(ifconfig_ptr1,0,sizeof(ifconfig_ptr1));

	fp=popen("ifconfig | head -100", "r");
	if (fp == NULL) {
		fprintf(stderr, "ERROR: popen failed\n");
		return -1;
	}
	fp1=fopen("config.json","w+");
	if (fp1 == NULL) {
                fprintf(stderr, "ERROR: file open failed\n");
                return -1;
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
			ifconfig_ptr1->interface=itf_name;
			sprintf(result,"{\n\"ifconfig.result\":[{\n\"interface\":\"%s\",\n",ifconfig_ptr1->interface);
			fwrite(result,strlen(result),1,fp1);
				
			//Now take the part after the colon
			pStart=pColon++;

		}
		if(line_count=='\n') {
			// Parse the line for ':' and take the first part from the second interface
		 	pStart = line;
                        pColon = strchr(line, ':');
			strncpy(itf_name1, line, pColon-pStart);
			ifconfig_ptr1->interface=itf_name1;
			sprintf(result,"\{\n\"interface\":\"%s\",\n",ifconfig_ptr1->interface);
			fwrite(result,strlen(result),1,fp1);
			pStart=pColon++;

		}
		else {
			pStart = line;
		}
		
		//Getting the Flag as link_status
		if ((pMatchStr = strstr(pStart,"flags")) != NULL) {
		       	if(pmatchstr1=strstr(pMatchStr, "<")){
				pSpace= strchr(pmatchstr1,'<');	
				strncpy(flag, pmatchstr1, pSpace+3- (++pmatchstr1));
				ifconfig_ptr1->link_status=flag;
				sprintf(result,"\"Link-Status\":\"%s\",\n",ifconfig_ptr1->link_status);
                       		fwrite(result,strlen(result),1,fp1);

				pStart = pSpace++;
				pMatchStr=NULL;
	                }   

		}

		//Getting the inet6 as IPV6_address
	       if ((pMatchStr = strstr(pStart,"inet6")) != NULL) {
                         pSpace = strchr(pMatchStr+5, ' ');
			 strcpy(inet6,pSpace);
	          	 pSpace1=strtok(inet6," ");
                         ifconfig_ptr1->IPV6_address=pSpace1;
                         sprintf(result,"\"IPV6_address\":\"%s\",\n",ifconfig_ptr1->IPV6_address);

			 fwrite(result,strlen(result),1,fp1);
                         pStart = pSpace++;
                         pMatchStr=NULL;
	
		}

	        //Getting ether as HW_address
	         if ((pMatchStr = strstr(pStart,"ether")) != NULL) {
                        pSpace = strchr(pMatchStr+6, ' ');
                        strncpy(ether, pMatchStr+6, pSpace - (pMatchStr+6));
                        ifconfig_ptr1->HW_address = ether;
                        sprintf(result,"\"HW-Address\":\"%s\"\n},\n",ifconfig_ptr1->HW_address);
                        fwrite(result,strlen(result),1,fp1);
			HW_SET_FLAG = 1;	
                        pStart = pSpace++;
                        pMatchStr=NULL;
			
                }
		
		//Getting the mtu as mtu
                if ((pMatchStr = strstr(pStart,"mtu")) != NULL) {
                        if ((pSpace = strchr(pMatchStr+4, ' ')) == NULL){
                                pSpace = strchr(pMatchStr+4, '\n');
                        }

                        strncpy(mtu1, pMatchStr+4, pSpace - (pMatchStr+4));
                        ifconfig_ptr1->mtu=mtu1;
                        sprintf(result,"\"MTU\":\"%s\",\n",ifconfig_ptr1->mtu);
                        fwrite(result,strlen(result),1,fp1);

                        pStart = pSpace++;
                        pMatchStr=NULL;

                }

		//Getting inet as IPV4_address
        	else if ((pMatchStr = strstr(pStart,"inet")) != NULL) {
                        pSpace = strchr(pMatchStr+5, ' ');
                        strncpy(inet, pMatchStr+5, pSpace - (pMatchStr+5));
                        ifconfig_ptr1->IPV4_address=inet;
			sprintf(result,"\"IPV4-Address\":\"%s\",\n",ifconfig_ptr1->IPV4_address);
                        fwrite(result,strlen(result),1,fp1);

                        pStart = pSpace++;
                        pMatchStr=NULL;
                }
	}
	if(HW_SET_FLAG = 1)
                        {
                        ifconfig_ptr1->HW_address = "NONE";
                        sprintf(result,"\"HW-Address\":\"%s\"\n",ifconfig_ptr1->HW_address);
                        fwrite(result,strlen(result),1,fp1);
                        }

	printf("Successfully stored the structure form of ifconfig_data in config.json file \n");

	if(fp1 != NULL)
	{
		fseek(fp1,0,SEEK_END);
		sprintf(result1,"]}\n}\n");
		fwrite(result1,strlen(result1),1,fp1);
	}

	pclose(fp);
	fclose(fp1);


    
}






