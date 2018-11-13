#ifndef REFLECTOR_H
#define REFLECTOR_H

#include <vector>
#include <map>

class Reflector
{
 public:
  
  char* wiring_path;

  /* maps */ 
  std::map<int, int> wiring_map;

  /* methods */
  int setUp(char* passed_wiring_path);
  int reflectValue(int value); 
}; 

#endif 
