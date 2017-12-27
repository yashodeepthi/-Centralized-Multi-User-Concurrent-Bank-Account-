#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fstream>
#include <arpa/inet.h>
#include <cstring>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>

#define MAXTHREAD 50
#define MAXCLIENTS 100
using namespace std;

char msg[256],ms[256],buffer[256];
int server_fd, new_socket, valread;
struct sockaddr_in address;
int min_bal=3000;
int opt = 1,n;
int addrlen = sizeof(address);
int acc[1000],i;
float bal[1000];
string name[1000];

void *performOperations(void *);
void deposit(int inde,float amt);
void withdrawal(int inde,float amt);

pthread_mutex_t mutex;

//searching account
 int search(int buff)
 {
   for(int k=0;k<i;k++)
   {
      if(acc[k]==buff)
{
       return k;
}
   }
return -1;

 }
int main(int argc, char const *argv[])
{


int portno;
string filepath=argv[2];
if(argc<3)
{
cout<<"error:portno,filename"<<"\n";
exit(1);
}

 ifstream myReadFile;
myReadFile.open(filepath.c_str());
portno=atoi(argv[1]);
  if(myReadFile.is_open())
{
 i=0;

  while( myReadFile >>acc[i]>>name[i]>>bal[i])
{

i++;
}
}
myReadFile.close();

cout<<"account "<<"name "<<"balance "<<"\n";
 for(int l=0;l<i;l++)
{
cout<<acc[l]<<" "<<name[l]<<" "<<bal[l]<<" "<<"\n" ;
}

     // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        cout<<"socket failed"<<"\n";
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( portno );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        cout<<"bind failed"<<"\n";
        exit(1);
    }

    if (listen(server_fd,MAXCLIENTS)!= 0)
    {
        cout<<"listen"<<"\n";
        exit(EXIT_FAILURE);
    }

pthread_t threadH[MAXTHREAD];
//thread creations
for(int k=0;k<MAXTHREAD;k++)
{
     new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen);
    {

     pthread_create(&threadH[k] , NULL ,  &performOperations ,&new_socket);


       }
}
close(server_fd);
return 0;

 }

//performing transaction(withdrawal and deposit)
void *performOperations(void* args)
{
	int *temp= (int*) args;
	int newsockfd=*temp;
	//cout<<newsockfd<<"\n";
	bzero(buffer,255);
	pthread_mutex_init(&mutex,NULL);
	while(n=read(newsockfd,buffer,255))
	{

	if(n<0)
	{
	break;
	}
	//spliting the string from client
	 char *pch,type[5];
	 int acc_c;
	float amt;
	 pch=strtok(buffer,",");
	  if(pch)
	  {
	 float a=atof(pch);
	 amt=a;
	  cout<<"data received from client:"<<amt<<"\n";
	  }
	  pch=strtok(NULL,",");
	  if(pch)
	  {

	strcpy(type,pch);
	  cout<<"data received from client:"<<type<<"\n";
	  }
	  pch=strtok(NULL,",");
	  if(pch)
	  {
	int a=atoi(pch);
	acc_c=a;
	cout<<"data received from client:"<<acc_c<<"\n";
	  }
	 pch=strtok(NULL,",");
	 int index=search(acc_c);
	if(index==-1)
	{
	n=write(newsockfd,"not found",56);

	 if(n<0)
	  {
	exit(1);
	  }

	}
      else
      {
      cout<<"index="<<index<<"\n";
      if(strcmp(type,"w") == 0)
      {
        pthread_mutex_lock(&mutex);
         withdrawal(index,amt);
         n=write(newsockfd,ms,180);
        cout<<"balance="<<bal[index]<<"\n";
          if(n<0)
          {
            exit(1);
           }
           pthread_mutex_unlock(&mutex);
      }

    else if(strcmp(type,"d")==0)
     {
        pthread_mutex_lock(&mutex);
        deposit(index,amt);
        n=write(newsockfd,"deposisted",180);
        cout<<"current bal:"<<bal[index]<<"\n";
        if(n<0)
        {
        exit(1);
        }

        pthread_mutex_unlock(&mutex);
         }
   else
   {
    n=write(newsockfd,"terminated",180);
   }

 }
}
 close(newsockfd);

}

void withdrawal(int inde,float amt)
{
 if(bal[inde]>=bal[inde]+min_bal-amt)
    {
       bal[inde]=bal[inde]-amt;
        strcpy(ms,"withdrawal successful");

      }
  else
   {
      strcpy(ms,"can't withdraw");

    }
}
void deposit(int inde,float amt)
{
  bal[inde]=bal[inde]+amt;
  strcpy(ms,"Depoisted");

}
