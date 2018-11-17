#ifndef PLUGBOARD_H
#define PLUGBOARD_H

class Plugboard
{
 private: 
  /* PATHS */
  char* wiring_path;

  /* MAPS */
  std::map<int, int> wiring_map;

 public:
  /* METHODS */

  /* method that sets up the plugboard based on a configuration file path parameter, checks for
     varous errors and returns either the corresponding error code or a no error code */ 
  int setUp(char* passed_wiring_path);

  /* method that takes an integer parameter and returns value after passage through the
     plugboard */
  int encryptValue(int value); 
};

#endif 
