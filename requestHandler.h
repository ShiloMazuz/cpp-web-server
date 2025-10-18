#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H
#include <iostream>
#include <string>
#include <sstream>
#include "urlHandler.h"

enum class MethodType {
  GET,
  POST,
  ERROR,
};

class requestHandler {
public:
  requestHandler(std::string& requestStr);

private:
  std::stringstream m_requestString {};
  MethodType m_methodType{ MethodType::GET };
  URL m_route { "/" };
  std::stringstream m_response {};
  std::string::size_type m_headerSize {};

private:
  void methodToResponse();
  void generateGetResponse();
  std::string fetchParameter(std::string requestStr, std::string parameter);
	void generate404();

public:
  //getter functions
  void getRequestHeader ();
  std::string getRoute()          { return m_route.getPath(); };
  std::string getRequestString()  { return m_requestString.str(); };
  MethodType getMethodType()      { return m_methodType; };
  std::string::size_type getHeaderSize()            { return m_headerSize; };
  ssize_t getHeaderSizeT()            { return static_cast<ssize_t>(m_headerSize); };

  void generateResponseToRequest() {
    methodToResponse();
  }

  std::string getResponseStr() {
    return m_response.str();
  }

  ssize_t sendResponse(int acceptSocket);
  MethodType methodToEnum(std::string method);
  //setter functions
  void setMethodType(MethodType methodType) { m_methodType = methodType; }
  void setMethodType(std::string& methodType) { m_methodType = methodToEnum(methodType); }
  void setRoute(std::string route) { m_route.setPath(route); };
  void setHeaderSize(std::string::size_type size) { m_headerSize = size; };
};

#endif
