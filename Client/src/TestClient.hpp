/*
 * TestClient.hpp
 *
 *  Created on: 22 июл. 2019 г.
 *      Author: peka
 */

#ifndef TESTCLIENT_HPP_
#define TESTCLIENT_HPP_


#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h> //printf
#include <string> //string
#include <strings.h>
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr
#include <netdb.h> //hostent
#include <stdlib.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <cstdint>
#include <cstring>
#include <algorithm>


namespace TestTask{


enum class TypeSocket {
	TypeUDP,
	TypeTCP
};

class client
{
private:
	int sock_client;
	int sock_client_port;
	std::string address;
	int port_client;
	struct sockaddr_in client_port;
	struct sockaddr_in server;
	client();
	TypeSocket typeClient;
	void handler_receive(int fd);
	static const int size_buf_recv = 1024;
	std::thread *listen_socket_thread;
public:
	client(TypeSocket);
	bool conn(std::string, int);
	bool send_data(std::string data);
	~client();
};

}


#endif /* TESTCLIENT_HPP_ */
