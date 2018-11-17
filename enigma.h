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
 private:
  /* CONFIGURATION PATHS */ 
  char plugboard_wiring_path[100];
  char reflector_wiring_path[100];
  char rotor_position_path[100];
  char input_file_path[100];
  char output_file_path[100]; 
  std::vector<char*> rotor_wiring_paths; 
  
  /* COMPONENTS */

  /* this boolean is initialised to false and is only converted to true if setUp is executed without 
     issue. it prevents use of an unconfingured EnigmaMachine object */  
  bool is_correctly_configured = false;
  Plugboard plugboard;
  std::vector<Rotor *> rotor_vector;
  Rotor* zero_rotor = new Rotor;
  Reflector reflector; 
  
  /* METHODS */
  int transmitThroughPlugboard(int message); 

  /* method that takes an transforms an integer according to the mappings found on a forwards 
     transmission through the rotors and returns the transformed integer */
  int transmitForwardsThroughRotors(int message);

  /* method that takes an transforms an integer according to the connects found on a backwards 
     transmission through the rotors and returns the transformed integer */ 
  int transmitBackwardsThroughRotors(int message); 

  /* method that takes an integer, applies the plugboard, rotors and reflector to it in the correct 
     order and then returns the transformed integer*/ 
  int encryptNum(int message);

  /* destructor for EnigmaMachine. Needs to delete rotors created on the heap */ 
  ~EnigmaMachine(); 

  /* method that loops through the command line arguments and classifies them by their file 
     extension.if their are the incorrect number of arguments then it will return an error code */
  int getArguments(int argument_cnt, char** argument_array);

  /* method that loops through vector of rotor configuration paths and creates a rotor on the heap 
     for each config file. returns error codes if met, otherwise returns 0 */ 
  int multiRotorSetUp(); 
  
 public:

  /* wrapper method that calls the set up methods for plugboard, reflector and rotors. returns met 
     error code */ 
  int setUp(int argument_cnt, char** argument_array);

  /* method that in a while loop takes character inputs, checks they're valid,
     converts them to integers, processes them using 'encryptNum' and writes them to an 
     output stream. returns error code for invalid inputs or 0 for no error */  
  int encryptMessage(); 

};

#endif


			      
