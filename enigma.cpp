#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <map>
#include "enigma.h"
#include "plugboard.h"

using namespace std;



/*---------------- ENIGMA MACHINE METHODS -----------------*/
/* constructor for the EnigmaMachine class
TODO: need to work out how to return error code from constructor  */ 
EnigmaMachine::EnigmaMachine(int argument_cnt, char** argument_array)
{
  int plugboard_check = 0, reflector_check = 0, rotor_position_check = 0; 

  for (int i = 1; i < argument_cnt; i++)
    {
      int argument_len = strlen(argument_array[i]); 

      if (strncmp(".pb", argument_array[i] + (argument_len - 3), 3) == 0)
	{
	  strcpy(plugboard_wiring_path, argument_array[i]);
	  plugboard_check++;
	}
      else if (strncmp(".rf", argument_array[i]+ (argument_len-3), 3) == 0)
	{
	  strcpy(reflector_wiring_path, argument_array[i]);
	  reflector_check++;
	}
      else if (strncmp(".rot", argument_array[i]+ (argument_len-4), 4) == 0)
	rotor_wiring_paths.push_back(argument_array[i]);
      else if (strncmp(".pos", argument_array[i] + (argument_len-4), 4) == 0)
	{
	  strcpy(rotor_position_path, argument_array[i]);
	  rotor_position_check++; 
	}
    }

  if (rotor_position_check != 1 || plugboard_check != 1 || reflector_check != 1)
    {
      cerr << "You have failed to include the correct number of command line argumnents. Check you havea .pos file, a .pb file and a .rf file\n";
      exit(1);
    }
}

int EnigmaMachine::setUp()
{
  int ret = plugboard.setUp(plugboard_wiring_path); 
  if (ret)
    return ret;
  else
    return 0; 
}


  





  
  


