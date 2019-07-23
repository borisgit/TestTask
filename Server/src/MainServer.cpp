//============================================================================
// Name        : TestTaskProtey.cpp
// Author      : IvashinnikovBoris
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "TestServer.h"

int main() {
	uint port;
	std::cin>>port;
	TestTask::server s(port);
	std::string buf;
	for(;;){
		std::cin>>buf;
		if(buf=="exit")
			break;
	}
	s.~server();
	//done
	return 0;
}
