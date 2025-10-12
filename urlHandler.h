#ifndef URLHANDLER_H
#define URLHANDLER_H

#include <iostream>
//handles the socket
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

//handles files
#include <filesystem>
#include <fstream>

#include <sstream>
#include <string>


class URL {
private:
  std::string m_path {"/"};
public:
  URL(std::string_view path);
  void setPath(std::string path);

private:
  void normalizePath();
  std::string normalizePath(std::string path);

public:
  void erase(std::size_t a, std::size_t b);
  std::string getPath() { return m_path; }
};

#endif
