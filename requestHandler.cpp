#include <iostream>
#include <string>
#include <string_view>
#include "requestHandler.h"

class requestHandler {
  requestHandler(std::string requestStr)
  : requestString {requestStr}
  {
  }
  requestType getRequestType(std::string request) {
  if(request == "GET") return GET;
  if(request == "POST") return POST;
    return ERROR;
  }

  void getRequestHeader (std::string_view requestStr) {
    std::string header {};
    std::getline(requestStr, header);
		std::string method, urlPath, version;
		std::istringstream headerLine {header};
		headerLine >> method >> urlPath >> version;
		setRequestType(method);
  }
};
