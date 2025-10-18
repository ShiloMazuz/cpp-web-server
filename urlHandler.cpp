#include "urlHandler.h"

URL::URL(std::string_view path) 
: m_path {path}
{
  normalizePath();
}

void URL::setPath(std::string path){
  m_path = normalizePath(path);
}

void URL::erase(std::size_t a, std::size_t b) {
  m_path.erase(a, b);
  return;
}

void URL::normalizePath() {
  while(m_path.find("/../") != std::string::npos) {
    m_path.erase(m_path.find("/../")+1, 3);
  }
  std::cout << "normalized path: " <<  getPath() << '\n';
  return;
}

std::string URL::normalizePath(std::string path) {
  while(path.find("/../") != std::string::npos) {
    path.erase(path.find("/../")+1, 3);
  }
  std::cout << "normalized path: " <<  path << '\n';
  return path;
}
