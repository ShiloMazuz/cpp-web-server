#include "urlHandler.h"
#include "requestHandler.h"
#include <iostream>
#include <filesystem>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <fstream>

int main(int argc, char* argv[]) {
  using std::string_literals::operator""s;
	bool argDirectory { false };
	for(int i {0}; i < argc; i++) {
	if(argv[i] == "--directory"s)
	argDirectory = true;
	}
	if(argDirectory) {
	std::cout << "using --directory argument\n";
	}
	//creates a socket
	int serverSocket = -1;
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(serverSocket == -1) {
		std::cout << "Error at socket(): " << '\n';
		return 0;
	}
	else {
		std::cout << "socket() is OK!" << '\n';	
	}
	//binds a socket
	sockaddr_in service;
	service.sin_family = AF_INET;
	const char* IP{"127.0.0.1"};
	inet_pton(AF_INET, IP, &service.sin_addr);
	constexpr int port {55555};
	service.sin_port = htons(port);
	if(bind(serverSocket,reinterpret_cast<sockaddr*>(&service) , sizeof(service) ) == -1) {
		std::cout << "Error at bind(): " << '\n';
		close(serverSocket);
		return 0;
	}
	else {
		std::cout << "socket succsessfully bound!" << '\n';
	}
	//listen on the socket
	while(true) {
		constexpr int maxConnections {3};
		if(listen(serverSocket, maxConnections) == -1) {
			std::cout << "listen(): Error listening on socket " << '\n';
		}
		else {
			std::cout << "listen(): is OK, waiting for connections..." << '\n';
		}
		int acceptSocket {};
		acceptSocket = accept(serverSocket, nullptr, nullptr);
		if(acceptSocket == -1) {
			perror("Error at socket()");
			return -1;
		}
		//recieve data into the socket
		char receiveBuffer[300] {};
		std::string str;
		ssize_t recvByteCount = recv(acceptSocket, receiveBuffer,sizeof(receiveBuffer), 0);
		if(recvByteCount == -1) {
			std::cout << "server send error";
			close(acceptSocket);
			return -1;
		}
		else {
			str= receiveBuffer;
			std::cout << "data recieved: \n" << str << '\n';
		}

		requestHandler data{ str };
		data.generateResponseToRequest();

 		//send data to the socket
 		ssize_t byteCount = { data.sendResponse(acceptSocket) };

		if(byteCount == 0) {
			std::cout << "server send error";
			return -1;
		}
		else {
			std::cout << "server sent " << byteCount << " bytes" << '\n';
		}
 		shutdown(acceptSocket, SHUT_WR);
 		close(acceptSocket);
	}
	close(serverSocket);
	return 0;
}
//test
