#ifndef ENIGMA_H
#define ENIGMA_H

#include <vector>
#include <string>
#include <map>
#include "plugboard.h"
#include "rotor.h"
#include "reflector.h"

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
 
  int return_code; 
  
  /* internal objects */
  Plugboard plugboard;
  std::vector<Rotor *> rotor_vector;
  Rotor* zero_rotor = new Rotor;
  Reflector reflector; 
  
  /* methods */
  ~EnigmaMachine(); 
  int getArguments(int argument_cnt, char** argument_array);
  int multiRotorSetUp(); 
  int setUp(int argument_cnt, char** argument_array);
  int transmitThroughPlugboard(int message); 
  int transmitForwardsThroughRotors(int message);
  int transmitBackwardsThroughRotors(int message); 
  int encryptNum(int message);
  int encryptMessage(); 

};

#endif


			      
