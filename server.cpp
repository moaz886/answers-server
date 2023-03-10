//
// Created by moaz on 12/15/22.
//
#include<iostream>
#include <cstdio>
#include <netdb.h>
#include <netinet/in.h>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#include <vector>
#include <string>
#include <thread>

#define MAX 10000
#define PORT 8080

#define NUMBER_OF_STUDENTS 1000000
#define SA struct sockaddr

using namespace std;

vector<string>studentAnswers;
string s ="";
// Function designed for chat between client and server.
int counter1=0 ,counter2=0;
void func(int id,int connfd, int sockfd)
{
    struct sockaddr_in  cli;
    int len = sizeof(cli);
    while(1){
        connfd = accept(sockfd, (SA*)&cli, reinterpret_cast<socklen_t *>(&len));
        if (connfd < 0) {
            printf("server accept failed...\n");
            continue;;
        }
        char buff[MAX];
        int n;
        bzero(buff, MAX);
        int receivedBytes = 0 , totalReceivedBytes = 0;

        //while (totalReceivedBytes < 900) {
            receivedBytes+=recv(connfd, buff+totalReceivedBytes , MAX-totalReceivedBytes ,0);
            totalReceivedBytes+=receivedBytes;
            //cout<<"totalReceivedBytes from "<<id<<" = "<<totalReceivedBytes <<endl;
        //}
        //if(close(connfd) != 0)
        //   cout<<"error while closing connection\n";
        string received= string(buff ,buff + totalReceivedBytes);
        if( id == 1)
            counter1++;
        else 
            counter2++;
        close(connfd);
    }
}


// Driver function
int main()
{
    //freopen("out.txt","w",stdout);
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
    //studentAnswers.resize(NUMBER_OF_STUDENTS);
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    //else
        //printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    int count = 0;
    vector<thread> threads;
    for (int id = 1; id <=2; id++)
        threads.emplace_back(func,id,connfd,sockfd);
    while(1){
        if ((listen(sockfd, 300)) != 0) {
            printf("Listen failed...\n");
            exit(0);
        }
        //else
            //printf("Server listening..\n");
        

        //func(connfd,sockfd);

        //count++ ;
        //if(count %10 == 0)cout<<count<<endl;
        
        if(count !=(counter1+counter2)  && (counter1+counter2) % 1000 ==0){
        cout<<counter1+counter2<<endl;
        count =(counter1+counter2);
        }
    }
    
    
    //
    for (auto& thread : threads)
        thread.join();
    close(sockfd);
}
