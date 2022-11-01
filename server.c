
/*
        TCP_Server. This Program will will create the Server side for TCP_Socket Programming.
        It will receive the data from the client and then send the same data back to client.
*/

#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/wait.h> 
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr

struct Product{
char description[1000];
int price;
int item_count;
//struct Prduct p={.description="",.price=0,.item_count=2};
}; 

struct Point{
struct Product p;
int purchase;
int sold;
int users[20];
int ucount;
};

int main(void)
{
	int subtotal=0;int usernumber=0;
	int ordersnum=0,successorders=0,failorders=0,totalearned=0;
	int fd1[2];int fd2[2];
	pipe(fd1);pipe(fd2);
	struct Point Catalog[20];
	for(int i=0;i<20;i++){
		char des[50];
		strcpy(des,"Product");
		if(i>9){
			strcat(des,"1");
		}	
		char t[1];
		t[0]=(i%10)+48;
		strcat(des,t);	
		struct Product pd;
		strcpy(pd.description,des);
		pd.price=i*10;pd.item_count=2;
		struct Point pt;
		pt.p=pd;
		pt.purchase=0;
		pt.sold=0;
		pt.ucount=0;
		Catalog[i]=pt;
	}
        int socket_desc, client_sock, client_size; 
        struct sockaddr_in server_addr, client_addr;         //SERVER ADDR will have all the server address
        char server_message[2000], client_message[2000];                 // Sending values from the server and receive from the server we need this

        //Cleaning the Buffers
        
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));     // Set all bits of the padding field//
        
        //Creating Socket
        
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        
        if(socket_desc < 0)
        {
                printf("Could Not Create Socket. Error!!!!!\n");
                return -1;
        }
        
        printf("Socket Created\n");
        
        //Binding IP and Port to socket
        
        server_addr.sin_family = AF_INET;               // Address family = Internet 
        server_addr.sin_port = htons(2013);               // Set port number, using htons function to use proper byte order 
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");    // Set IP address to localhost 
		
		
		
		// BINDING FUNCTION
        
        if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)    // Bind the address struct to the socket.  /
	                            	//bind() passes file descriptor, the address structure,and the length of the address structure
        {
                printf("Bind Failed. Error!!!!!\n");
                return -1;
        }        
        
        printf("Bind Done\n");
        
        //Put the socket into Listening State
        pid_t p1;
for(usernumber=0;usernumber<5;usernumber++){
	subtotal=0;
	p1=fork();
	if(p1==0){
	usernumber=5;
        if(listen(socket_desc, 1) < 0)                               //This listen() call tells the socket to listen to the incoming connections.
     // The listen() function places all incoming connection into a "backlog queue" until accept() call accepts the connection.
        {
                printf("Listening Failed. Error!!!!!\n");
                return -1;
        }
        
        printf("Listening for Incoming Connections.....\n");
        
        //Accept the incoming Connections
        
        client_size = sizeof(client_addr);
		
		
		
        client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);          
        
        if (client_sock < 0)
        {
                printf("Accept Failed. Error!!!!!!\n");
                return -1;
        }
        
        printf("Client Connected with IP: %s and Port No: %i\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
		       //inet_ntoa() function converts the Internet host address in, given in network byte order, to a string in IPv4 dotted-decimal notation
        
        //Receive the message from the client
        for(int i=0;i<10;i++){
        if (recv(client_sock, client_message, sizeof(client_message),0) < 0)
        {
                printf("Receive Failed. Error!!!!!\n");
                return -1;
        }
        
        //printf("Client Message: %s\n",client_message);
	//close(fd1[0]);//close(fd2[0]);close(fd2[1]);
	write(fd1[1],client_message,strlen(client_message));
	//close(fd1[1]);

	//close(fd2[1]);//close(fd1[0]);close(fd1[1]);
	read(fd2[0],server_message,100);
	//close(fd2[0]);
        //printf("Server Message2: %s\n",server_message);
        
        if (send(client_sock, server_message, strlen(server_message),0)<0)
        {
                printf("Send Failed. Error!!!!!\n");
                return -1;
        }
        
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));
	sleep(1);
	}
        }
	if(p1>0){      
        //Closing the Socket
	for(int i=0;i<10;i++){
	//close(fd1[1]);//close(fd2[0]);close(fd2[1]);
	read(fd1[0],client_message,100);
	//close(fd1[0]);
	int req=atoi(client_message);
	ordersnum++;
	(Catalog[req].purchase)++;
        //Send the message back to client
        if((Catalog[req].p).item_count>0){
		strcpy(server_message,"Order Successful!");
		(Catalog[req].p).item_count--;
		(Catalog[req].sold)++;
		subtotal=subtotal+(Catalog[req].p).price;
		successorders++;
	}else{
		Catalog[req].users[Catalog[req].ucount]=usernumber;
		Catalog[req].ucount++;
		strcpy(server_message,"Product not available");
		failorders++;
	}
        //Send the message back to client
        if(i==9){
		char buff[10];
		sprintf(buff,"%d",subtotal);
		strcat(server_message,"\nTotal:");
		strcat(server_message,buff);
		totalearned=totalearned+subtotal;
	}
        //printf("Server Message1: %s\n",server_message);
	//close(fd2[0]);//close(fd1[0]);close(fd1[1]);
	write(fd2[1],server_message,strlen(server_message));
	//close(fd2[1]);
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));
	}
	}
}
	if(p1>0){
	for(int i=0;i<usernumber;i++){
	wait(NULL);
	}

	printf("Summary Report \n");
        for(int i=0;i<20;i++){
	 printf("Product Description: %s\n",(Catalog[i].p).description);
	 printf("Purchase Requests: ");
	 printf("%d",Catalog[i].purchase);
	 printf("\n");
	 printf("Pieces Sold: ");
	 printf("%d",2-((Catalog[i].p).item_count));
	 printf("\n");
	 printf("List of Users: \n");
	 for(int j=0;j<Catalog[i].ucount;j++){
		printf("%d",Catalog[i].users[j]);
	        printf("  ");
	 }
	 printf("\n");
	 printf("\n");
	}
	
	printf("Total Orders:");
	printf("%d",ordersnum);
	printf("\n");
	printf("Successful Orders:");
	printf("%d",successorders);
	printf("\n");
	printf("Failed Orders: ");
	printf("%d",failorders);
	printf("\n");
	printf("Total Money Earned: ");
	printf("%d",totalearned);
	printf("\n");
        close(client_sock);
        close(socket_desc);
	}
        return 0;       
}

