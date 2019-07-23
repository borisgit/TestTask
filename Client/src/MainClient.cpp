//============================================================================
// Name        : TestTaskProtey.cpp
// Author      : IvashinnikovBoris
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "TestClient.hpp"

int main(int argc, char **argv) {
	TestTask::TypeSocket ClientType;
	if(!memcmp(argv[1], "TCP", 3)){
				std::cout<<"TCP Client!"<<std::endl;
				ClientType = TestTask::TypeSocket::TypeTCP;
	}else if(!memcmp(argv[1], "UDP", 3)){
				std::cout<<"UDP Client!"<<std::endl;
				ClientType = TestTask::TypeSocket::TypeUDP;
	}
	TestTask::client c(ClientType);
	std::string buf;
	std::string host;
	uint port;
	std::cout<<"Enter hostname: ";
	std::cin>>host;
	std::cout<<"Enter port: ";
	std::cin>>port;
	c.conn(host, port);

	for(;;){
		std::getline(std::cin, buf);
		if(buf=="exit")
			break;
		else c.send_data(buf);
	}
	c.~client();
	//done
	return 0;
}
