#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <string>

bool checkEcho(std::string& path) {
return (path.starts_with("/echo/"));
}

int main() {
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
		//std::string str(receiveBuffer, static_cast<size_t>(recvByteCount));
		str= receiveBuffer;
		std::cout << "data recieved: \n" << str << '\n';
	}
	
	std::istringstream data { str };
	std::string header;
	std::getline(data, header);
	std::string method, path, version;
	std::istringstream headerLine {header};
	headerLine >> method >> path >> version;
	
	std::stringstream response;
	std::cout << "path: " << path << '\n';

	if(path == "/") {
		response <<
 			"HTTP/1.1 200 OK\r\n\r\n"
    	"Content-Type: text/html; charset=UTF-8\r\n";
  	std::cout << "sending code 200\n";
	}

	else if(checkEcho(path)) {
		path.erase(0, 6);
		response <<
 			"HTTP/1.1 200 OK\r\n\r\n"
  		"Content-Type: text/plain\r\n"
  		"Content-Length: " << path.size() << "\r\n"
  		"\r\n" << path;
  	std::cout << "sending code 200\n";
	
	}
	else {
	response << 
			"HTTP/1.1 404 Not Found\r\n\r\n"
    	"Content-Type: text/html; charset=UTF-8\r\n";
  		std::cout << path << " is not a valid path, sending 404\n";
	}

	//send data to the socket
	ssize_t byteCount = { send(acceptSocket, response.str().c_str(), response.str().size(), 0) };

	if(byteCount == 0) {
		std::cout << "server send error";
		return -1;
	}
	else {
		std::cout << "server sent " << byteCount << " bytes" << '\n';
	}
	close(serverSocket);
	return 0;
}
//test
