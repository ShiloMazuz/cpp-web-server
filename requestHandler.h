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

  void generateResponseToRequest() {
    methodToResponse();
  }

  std::string getResponseStr() {
    return m_response.str();
  }

  MethodType methodToEnum(std::string method);
  //setter functions
  void setMethodType(MethodType methodType) { m_methodType = methodType; }
  void setMethodType(std::string& methodType) { m_methodType = methodToEnum(methodType); }
  void setRoute(std::string route) { m_route.setPath(route); };
};

#endif
