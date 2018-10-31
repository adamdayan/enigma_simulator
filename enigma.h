#ifndef ENIGMA_H
#define ENIGMA_H

#include <vector>
#include <string>

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
  std::vector<int> plugboard_wiring_settings;
  std::vector<int> reflector_wiring_settings;
  std::vector<int> rotor_position_settings; 
  std::vector<std::vector<int>> rotor_wiring_settings; 

  EnigmaMachine(int argument_cnt, char** argument_array);
  std::vector<int> readSettings(char path[100]);
  void populateSettings();
  

};

#endif


			      
