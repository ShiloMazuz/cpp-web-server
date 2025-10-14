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

bool checkEcho(std::string& path) {
	return (path.starts_with("/echo/"));
}

std::string fetchParameter(std::string requestStr, std::string parameter) {
	std::size_t parameterPosition = requestStr.find(parameter);
	std::size_t lineEnd = requestStr.find("\r", parameterPosition);
  return (requestStr.substr(parameterPosition + parameter.size(), lineEnd) );
}

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
			//std::string str(receiveBuffer, static_cast<size_t>(recvByteCount));
			str= receiveBuffer;
			std::cout << "data recieved: \n" << str << '\n';
		}

		requestHandler data{ str };
		//std::istringstream data { str };
		//std::string header;
		//std::getline(data, header);
		//std::string method, urlPath, version;
		//std::istringstream headerLine {header};
		//headerLine >> method >> urlPath >> version;
		//URL path {urlPath};

		std::stringstream response;
		std::cout << "path: " << data.getRoute() << '\n';
		//std::cout << "path: " << path.getPath() << '\n';

		if(data.getRoute() == "/") {
			response <<
 				"HTTP/1.1 200 OK\r\n"
    		"Content-Type: text/html; charset=UTF-8\r\n\r\n";
  		std::cout << "sending code 200\n";
		}

		else if (data.getRoute() == "/user-agent") {
			std::string userAgent { fetchParameter(data.getRequestString(), "User-Agent: ") };
			response <<
 				"HTTP/1.1 200 OK\r\n"
    		"Content-Type: text/html; charset=UTF-8\r\n"
    		"Content-Length: " << userAgent.size() << "\r\n"
    		"\r\n" <<
    		userAgent;
  		std::cout << "sending code 200\n";
		}
		else if(data.getRoute().substr(0, 6) == "/echo/") {
			data.setRoute(data.getRoute().erase(0, 6));
			response <<
 				"HTTP/1.1 200 OK\r\n"
  			"Content-Type: text/plain\r\n"
  			"Content-Length: " << data.getRoute().size() << "\r\n"
  			"\r\n" << data.getRoute() << '\n';
  		std::cout << "sending code 200\n";

		}

		else if(data.getRoute().substr(0, 7) == "/files/" && argDirectory) {
			data.setRoute(data.getRoute().erase(0, 7));
			std::fstream file {};
			file.open(data.getRoute(), std::ios::in);
			if(file.is_open() && !std::filesystem::is_directory(data.getRoute())) {
				std::string line{};
				response <<
 					"HTTP/1.1 200 OK\r\n"
  				"Content-Type: text/html\r\n"
  				"Content-Length: " << std::filesystem::file_size(data.getRoute()) << "\r\n"
					//  			"Connection: Close\r\n"
  				"\r\n"; 
				while(std::getline(file, line)) {
  				response << line << '\n';	
  			}
  			//	response << '\n';
  			std::cout << "sending code 200\n";
  		}
			else {
				std::fstream file404{};
				std::string path404 {"404.html"};
				file404.open(path404, std::ios::in);
				response << 
					"HTTP/1.1 404 Not Found\r\n"
    			"Content-Type: text/html; charset=UTF-8\r\n"
  				"Content-Length: " << std::filesystem::file_size(path404) << "\r\n"
  				"\r\n"; 
				std::string line404 {};
				while(std::getline(file404, line404)) {
  				response << line404 << '\n';	
  			}
  			std::cout << data.getRoute() << " is not a valid path, sending 404\n";
			}
		}

		else {
			std::fstream file404{};
			std::string path404 {"404.html"};
			file404.open(path404, std::ios::in);
			response << 
				"HTTP/1.1 404 Not Found\r\n"
    		"Content-Type: text/html; charset=UTF-8\r\n"
  			"Content-Length: " << std::filesystem::file_size(path404) << "\r\n"
  			"\r\n"; 
			std::string line404 {};
			while(std::getline(file404, line404)) {
  			response << line404 << '\n';	
  		}
  		std::cout << data.getRoute() << " is not a valid path, sending 404\n";
		}

		//send data to the socket
		ssize_t byteCount = { send(acceptSocket, response.str().c_str(), response.str().size(), 0) };
		close(acceptSocket);
		if(byteCount == 0) {
			std::cout << "server send error";
			return -1;
		}
		else {
			std::cout << "server sent " << byteCount << " bytes" << '\n';
		}
	}
	close(serverSocket);
	return 0;
}
//test
