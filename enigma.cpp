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

int EnigmaMachine::multiRotorSetUp()
{
  int ret = 0, iterator_cnt = 0;
  
  Rotor* current_rotor = nullptr, *last_rotor = nullptr;  

  current_rotor = zero_rotor; 
  
  for (auto i = rotor_wiring_paths.begin(); i <= rotor_wiring_paths.end(); ++i)
    {
      ret = current_rotor->setUpMapping(*i);
      if (ret)
	return ret; 
      ret = current_rotor->setUpPosition(rotor_position_path, iterator_cnt);
      if (ret)
	return ret; 

      current_rotor->prev_rotor = last_rotor;

      rotor_vector.push_back(current_rotor); 

      if (last_rotor != nullptr)
	{
	  last_rotor->next_rotor = current_rotor;
	}

      last_rotor = current_rotor;
      current_rotor = new Rotor;
      iterator_cnt++;
    }

  return 0; 
}


int EnigmaMachine::setUp()
{
  int ret = 0; 

  /* set up plugboard */  
  ret = plugboard.setUp(plugboard_wiring_path); 
  if (ret)
    return ret;

  /* set up all rotors */  
  ret = multiRotorSetUp();
  if (ret)
    return ret;

  return 0; 
}

int transmitForwardsThroughRotors(int message)
{
  bool hit_notch = false;
  int transformed_message = message ;

  for (auto i = rotor_vector.begin(); i != rotor_vector.end(); i++)
    {
      if (i->rotor_index == 0)
	i->rotate();

      if (hit_notch && i->rotor_index != 0)
	i->rotate();

      transformed_message = i->transmitForward(transformed_message);

      hit_notch = i->isAtNotch();
    }

  return transformed_message; 
}
  





  
  


