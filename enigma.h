#ifndef ENIGMA_H
#define ENIGMA_H

#include <vector>
#include <string>
#include <map>
#include "plugboard.h"

class EnigmaMachine
{
 public:
  /* paths */ 
  char plugboard_wiring_path[100];
  char reflector_wiring_path[100];
  char rotor_position_path[100];
  char input_file_path[100];
  char output_file_path[100]; 
  std::vector<char*> rotor_wiring_paths;

  /* raw settings */ 
  

  int return_code; 
  
  /* internal objects */
  Plugboard plugboard; 
  
  /* methods */ 
  EnigmaMachine(int argument_cnt, char** argument_array);
  int setUp(); 
  

};

#endif


			      
