#include "converter.h"

std::vector<int> decimalToHex(int num) {
  std::vector<int> hexForm{};
  while(num>0) {
    hexForm.push_back(num%16);
    num = num / 16;
  }
  return hexForm;
}

//takes int vector representing a hex number and returns a string
std::string hexVectorToString(std::vector<int> hexForm) {
  std::string hexString {};
  for(std::size_t i {hexForm.size()}; 0 != i; i--) {
    switch (hexForm[i-1]) 
    {
      case 10:
        hexString.push_back('A');
        break;
      case 11:
        hexString.push_back('B');
        break;
      case 12:
        hexString.push_back('C');
        break;
      case 13:
        hexString.push_back('D');
        break;
      case 14:
        hexString.push_back('E');
        break;
      case 15:
        hexString.push_back('F');
        break;
      default:
        char temp (static_cast<char>(hexForm[i-1]+48));
        hexString.push_back(temp);
        break;
    }
  }
  return hexString;
}

//takes in a number and returns it as hex string
std::string decimalToHexString(int num) {
  return hexVectorToString(decimalToHex(num));
}
