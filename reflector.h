#ifndef REFLECTOR_H
#define REFLECTOR_H

#include <vector>
#include <map>

class Reflector
{
 private: 
  /* PATHS */ 
  char* wiring_path;

  /* MAPS */ 
  std::map<int, int> wiring_map;

 public:
  /* METHODS */

  /* method that sets up the reflector based on a configuration file path parameter. checks for 
     various errors and returns either the corresponding error code or a no error code */ 
  int setUp(char* passed_wiring_path);

  /* method that takes an integer parameter and returns the corresponding reflected value */ 
  int reflectValue(int value);
  }; 

#endif 
