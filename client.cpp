#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fstream>
#include <cstring>
#include <ctime>
//#define PORT 8080
using namespace std;

int i,n,portno;
string amt[1000],acc[1000];
double stamp[600];
char p[256];
string name[1000];
string type[1000];

int main(int argc, char const *argv[])
{

        
        struct hostent *server;
        string filepath=argv[4];
	float timestep=atof(argv[3]);
    struct sockaddr_in address;
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024];
        if (argc < 3) {
       cout<<"usage %s hostname port"<<"\n";
       exit(0);
    }
        portno = atoi(argv[2]);
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout<< "Socket creation error"<<"\n";
        return -1;
    }
        server = gethostbyname(argv[1]);
    if (server == NULL) {
        cout<<"ERROR, no such host"<<"\n";
        exit(0);
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    
      if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cout<<"Connection Failed"<<"\n";
        return -1;
    }
                //read file
         fstream myReadFile;

         myReadFile.open(filepath.c_str());

         if (myReadFile.is_open())
         {
                 i=0;
                 while (myReadFile>>stamp[i]>>acc[i]>>type[i]>>amt[i])
                 {
                        i++;
                }
		int previoustimestep=-1;
                for(int k=0;k<i;k++)
                {
                        bzero(buffer,255);
                        strcpy(p,(amt[k]+","+type[k]+","+acc[k]+",").c_str());
                       
                        
                        if(previoustimestep!=-1)
		
			{		
			
			sleep((stamp[k]-previoustimestep)*timestep);
		
				}
                        
                          
                                n=write(sock,p,strlen(p));
                                n = read(sock,buffer,255);
                                  cout<<"message from server :"<<buffer<<"\n";
                                
                        }
                }
       
        myReadFile.close();


                cout<<"timestamp "<<"accNo "<<"type "<<"amount"<<"\n";
                for(int l=0;l<i;l++)
                {
                cout<<stamp[l]<<" "<<acc[l]<<" "<<type[l]<<" "<<amt[l]<<"\n";
                }

        close(sock);
        return 0;
}