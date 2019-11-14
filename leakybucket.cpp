#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#define bktSize 512
using namespace std;

void bktInput(int a,int b);
int main()
{
	int op,pktSize;
	cout<<"Enter output rate";
	cin>>op;
	for(int i=1;i<=5;i++)
	{
		usleep(rand()%1000);
		pktSize=rand()%1000;
		cout<<"\n Packet number"<<i<<"\t Packetsize="<<pktSize;
		bktInput(pktSize,op);
	}
	return 0;
}

void bktInput(int a,int b)
{

	if(a>bktSize)
	
		cout<<"\t Overflow";
	else
	{
		usleep(500);
		while(a > b)
		{
			cout<<"\n\t\t"<<b<<"bytes outputed";
			a-=b;
			usleep(500);
		}
		if(a>0)
		{
		cout<<"\n\t\t Last"<<a<<"bytes sent \n";
		cout<<"\n\t\t Bucket output successfull";
		}
	}
}

/*
 g++ -o leak leakybucket.cpp
 
 ./leak
Enter output rate100

 Packet number1	 Packetsize=886	 Overflow
 Packet number2	 Packetsize=915	 Overflow
 Packet number3	 Packetsize=335
		100bytes outputed
		100bytes outputed
		100bytes outputed
		 Last35bytes sent 

		 Bucket output successfull
 Packet number4	 Packetsize=492
		100bytes outputed
		100bytes outputed
		100bytes outputed
		100bytes outputed
		 Last92bytes sent 

		 Bucket output successfull
 Packet number5	 Packetsize=421
		100bytes outputed
		100bytes outputed
		100bytes outputed
		100bytes outputed
		 Last21bytes sent 

		 Bucket output successfull
