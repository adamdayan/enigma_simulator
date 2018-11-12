#ifndef PLUGBOARD_H
#define PLUGBOARD_H

class Plugboard
{
 public:

  char* wiring_path; 
  std::map<int, int> wiring_map;

  /* methods */  
  int setUp(char* passed_wiring_path);
  int encryptValue(int value); 
};

#endif 
