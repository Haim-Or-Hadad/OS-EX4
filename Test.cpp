/**
 * @file Test.cpp
 * @author Ilan Gold and Haim Or Hadad
 * @brief tests 
 * @version 0.1
 * @date 2022-05-09
 * @copyright Copyright (c) 2022
 */

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <string.h>
#include <iostream>

using namespace std;

#define PORT 3490

int main()
{
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PORT);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        cout <<"create socket failed" << endl;
        exit(1);
    }
    cout << "socket up" << endl;   

    //try connect to the server///
    size_t serv_size = sizeof(server_address);
    
    if (connect(sock,(struct sockaddr*)&server_address, serv_size)!= 0)
    {
        //cout << "connect failed" << endl;
        perror("connect failed");
        exit(1);
    }
    cout << "connected.." << endl;
    
    char buffer_[1024]="PUSH shalom";

    send(sock, buffer_ , 2048, 0);

    strcpy(buffer_,"PUSH how are you netzer?");
    send(sock, buffer_, 1024, 0);
    strcpy(buffer_,  "PUSH this is Ilan" );
    send(sock, buffer_, 1024, 0);
    strcpy(buffer_, "TOP"); 
    send(sock, buffer_, 1024, 0 );
    
    char buffer[1024]= "this is Ilan";
    
    recv(sock, buffer, 1024 ,0);
    cout << buffer << endl;
    
    
    assert(!strcmp(buffer, "OUTPUT: this is Ilan"));
    
    send(sock, "POP" , 1024, 0);
    recv(sock, buffer, 1024, 0);
     cout << "send:" << buffer << endl;
    assert(!strcmp(buffer, "this is Ilan"));

    send(sock, "POP", 1024, 0);
    recv(sock, buffer ,1024, 0);
    cout << "send:" << buffer << endl;
    assert(!strcmp(buffer, "how are you netzer?"));

    send(sock, "PUSH end", 1024, 0);
    send(sock, "POP", 1024, 0);
    recv(sock, buffer, 1024, 0);
    if (!strcmp(buffer,"end"))
    {
        cout << "the client and server works good" << endl;
    }
}