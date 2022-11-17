#include <iostream>
#include <map>
//#include <cstring>
int main(void) {
  std::map<char, std::string> bin_map = {
    {'0', "0000"},
    {'1', "0001"},
    {'2', "0010"},
    {'3', "0011"},
    {'4', "0100"},
    {'5', "0101"},
    {'6', "0110"},
    {'7', "0111"},
    {'8', "1000"},
    {'9', "1001"},
    {'a', "1010"},
    {'b', "1011"},
    {'c', "1100"},
    {'d', "1101"},
    {'e', "1110"},
    {'f', "1111"}
  };
/* iterate through mapping
  for (std::map<char, std::string>::iterator i = bin_map.begin(); i != bin_map.end(); i++) {
    std::cout << i->first << ": " << i->second << std::endl;
  }
*/
  //char hex_string[] = "0x5765206c6f76652057656243202d3e205765623320616e6420432f432b2b";
  //if (hex_string[0] == '0') memmove(hex_string, hex_string+2, strlen(hex_string));

  std::string hex_string = "0x5765206c6f76652057656243202d3e205765623320616e6420432f432b2b";
  if (hex_string[0] == '0') hex_string.erase(0, 2);

  std::string bin_string;
  for (int i=0; i<hex_string.length(); i++)
    if (bin_map.count(hex_string[i])) bin_string.append(bin_map.at(hex_string[i]));

  std::cout << bin_string << std::endl;
  std::cout << hex_string << std::endl;
}
