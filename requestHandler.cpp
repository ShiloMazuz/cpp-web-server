#include <iostream>
#include <string>
#include <string_view>
#include "requestHandler.h"

requestHandler::requestHandler(std::string& requestStr)
: m_requestString {requestStr}
{
  getRequestHeader();
}

MethodType requestHandler::methodToEnum(std::string method) {
  if(method == "GET") return MethodType::GET;
  if(method == "POST") return MethodType::POST;
  return MethodType::ERROR;
}

void requestHandler::getRequestHeader () {
  std::string requestLine{};
  std::getline(m_requestString, requestLine);
	std::string method, urlPath, version;
	std::istringstream headerLine {requestLine};
	headerLine >> method >> urlPath >> version;
	setMethodType(method);
	m_route.setPath(urlPath);
}

std::string requestHandler::fetchParameter(std::string requestStr, std::string parameter) {
	std::size_t parameterPosition = requestStr.find(parameter);
	std::size_t lineEnd = requestStr.find("\r", parameterPosition);
  return (requestStr.substr(parameterPosition + parameter.size(), lineEnd) );
}

void requestHandler::methodToResponse() {
  switch (m_methodType) {
    case MethodType::GET:
      generateGetResponse();
      break;
    default:
      generate404();
  }
}

void requestHandler::generateGetResponse() {
	if(getRoute() == "/") {
		m_response <<
 			"HTTP/1.1 200 OK\r\n"
    	"Content-Type: text/html; charset=UTF-8\r\n\r\n";
  	std::cout << "sending code 200\n";
	}

	else if (getRoute() == "/user-agent") {
		std::string userAgent { fetchParameter(getRequestString(), "User-Agent: ") };
		m_response <<
 			"HTTP/1.1 200 OK\r\n"
    	"Content-Type: text/html; charset=UTF-8\r\n"
    	"Content-Length: " << userAgent.size() << "\r\n"
    	"\r\n" <<
    	userAgent;
  	std::cout << "sending code 200\n";
	}
	else if(getRoute().substr(0, 6) == "/echo/") {
		setRoute(getRoute().erase(0, 6));
		m_response <<
 			"HTTP/1.1 200 OK\r\n"
  		"Content-Type: text/plain\r\n"
  		"Content-Length: " << getRoute().size() << "\r\n"
  		"\r\n" << getRoute() << '\n';
  	std::cout << "sending code 200\n";

	}

	//else if(getRoute().substr(0, 7) == "/files/" && argDirectory) {
	else if(getRoute().substr(0, 7) == "/files/") {
		setRoute(getRoute().erase(0, 7));

		if(getRoute().substr(0, 7) == "images/") {
			std::fstream file {};
			file.open(getRoute(), std::ios::in);
			if(file.is_open() && !std::filesystem::is_directory(getRoute())) {
				std::string line{};
				m_response <<
 					"HTTP/1.1 200 OK\r\n"
  				"Content-Type: image/png\r\n"
  				"Content-Length: " << std::filesystem::file_size(getRoute()) << "\r\n"
					//  			"Connection: Close\r\n"
  				"\r\n"; 
				while(std::getline(file, line)) {
  				m_response << line << '\n';	
  			}
  			//	 m_response << '\n';
  			std::cout << "sending code 200\n";
			}
		}
		else {
			std::fstream file {};
			file.open(getRoute(), std::ios::in);
			if(file.is_open() && !std::filesystem::is_directory(getRoute())) {
				std::string line{};
				m_response <<
 					"HTTP/1.1 200 OK\r\n"
  				"Content-Type: text/html\r\n"
  				"Content-Length: " << std::filesystem::file_size(getRoute()) << "\r\n"
					//  			"Connection: Close\r\n"
  				"\r\n"; 
				while(std::getline(file, line)) {
  				m_response << line << '\n';	
  			}
  			//	 m_response << '\n';
  			std::cout << "sending code 200\n";
  		}
			else {
		  	generate404();
			}
  	}
  }
	else {
    generate404();
	}
}
void requestHandler::generate404() {
	std::fstream file404{};
	std::string path404 {"404.html"};
	file404.open(path404, std::ios::in);
	m_response << 
		"HTTP/1.1 404 Not Found\r\n"
    "Content-Type: text/html; charset=UTF-8\r\n"
  	"Content-Length: " << std::filesystem::file_size(path404) << "\r\n"
  	"\r\n"; 
	std::string line404 {};
	while(std::getline(file404, line404)) {
  	m_response << line404 << '\n';	
  }
  std::cout << getRoute() << " is not a valid path, sending 404\n";
}
