#include <iostream>
#include <string>

enum RequestType {
  GET,
  POST,
  ERROR,
};

class requestHandler {
private:
std::string m_requestString {};
RequestType m_requestType{ GET };
URL m_route { "/" };
public:
  void setReqeustType(RequestType requestType) m_requestType = requestType;
};
