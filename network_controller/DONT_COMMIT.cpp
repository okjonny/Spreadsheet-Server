// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <string>
#include <iostream>
#define PORT 1100

int SetUpConnection();
int SendMessage();

int sock;
std::string message;
char buffer[4096];
int valread;
   
int main(int argc, char const *argv[]) 
{ 
    SetUpConnection();
    SendMessage();
    return 0;
} 

int SetUpConnection(){
    sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    message = (char *) "2"; 
    buffer[4096] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 

    return 0;

}

int SendMessage(){

    while(message.compare("1000000") || message.compare("-1000000")){
        send(sock , message.c_str() , strlen(message.c_str()) , 0); 
        valread = read( sock , buffer, 4096); 
        printf("%s\n",buffer ); 

        message = buffer;
        std::fill_n(buffer, 4096, 0);
    }
    if(message.compare("1000000") == 0)
        send(sock , "-1" , strlen("-1") , 0 ); 
    if(message.compare("-1000000") == 0)
        send(sock , "2" , strlen("2") , 0 );
    return 0;
}
