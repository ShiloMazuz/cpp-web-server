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
