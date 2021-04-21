
// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <string>
#include <cstdlib>
#include <thread>
#include <mutex> 
#include <iostream>
#define PORT 8080 

struct Client{
    int socket;
};

int SetUpServer();
void CountUp(int threadid);
void CountDown(int threadid);
void ListenForNewClients();

char* message;
char buffer[1024];
int new_socket;
int valread;
int server_fd;
struct sockaddr_in address; 
int opt = 1; 
int addrlen = sizeof(address);
std::thread threads[10];
int rc;
int ThreadID;
std::mutex mtx;
Client clients[5];

int main(int argc, char const *argv[]) { 
    SetUpServer();
    while(true){

    }
}

int SetUpServer(){
    ThreadID = 0;

    buffer[1024] = {0}; 
    message = "2"; 
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    }
    ListenForNewClients();
    return 0;
}

void ListenForNewClients(){
    while(true){
        Client c;
        if (listen(server_fd, 3) < 0) 
        { 
            perror("listen"); 
            exit(EXIT_FAILURE); 
        } 
        if ((c.socket = accept(server_fd, (struct sockaddr *)&address,  
                        (socklen_t*)&addrlen))<0) 
        { 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        } 

        if(threads[ThreadID].joinable()){
            threads[ThreadID].join();
        }
        
        if(ThreadID == 1){
            threads[ThreadID] = std::thread(CountDown,ThreadID);
        }else{
            threads[ThreadID] = std::thread(CountUp,ThreadID);
        }
        
        clients[ThreadID] = c;
        
        ThreadID++;
    }
}

void CountUp(int threadid){
    std::cout << "Operating on Threadid " << (long)threadid << std::endl;
    while(true){
        mtx.lock();
        std::fill_n(buffer, 1024, 0);
        valread = read( clients[threadid].socket , buffer, 1024); 
        std::cout << threadid << ":" << buffer << std::endl;
        std::string s = buffer;
        int value = std::stoi(s);
        if(value == -1){
            mtx.unlock();
            break;
        }
        value++;
        std::string sToSend = std::to_string(value);
        send(clients[threadid].socket , sToSend.c_str() , strlen(sToSend.c_str()) , 0 ); 
        std::fill_n(buffer, 1024, 0);
        mtx.unlock();
    }
    ThreadID--;
}

void CountDown(int threadid){
    std::cout << "Operating on Threadid " << (long)threadid << std::endl;
    while(true){
        mtx.lock();
        std::fill_n(buffer, 1024, 0);
        valread = read( clients[threadid].socket , buffer, 1024); 
        std::cout << threadid << ":" << buffer << std::endl;
        std::string s = buffer;
        int value = std::stoi(s);
        if(value == 2){
            mtx.unlock();
            break;
        }
        value--;
        std::string sToSend = std::to_string(value);
        send(clients[threadid].socket , sToSend.c_str() , strlen(sToSend.c_str()) , 0 ); 
        std::fill_n(buffer, 1024, 0);
        mtx.unlock();
    }
    ThreadID--;
}
