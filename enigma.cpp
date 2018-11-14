#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <map>
#include "enigma.h"
#include "errors.h"
#include "plugboard.h"
#include "reflector.h"

using namespace std;



/*---------------- ENIGMA MACHINE METHODS -----------------*/
 
/* function that loops through the command line arguments and classifies them by their file extension.
   if their are the incorrect number of arguments then it will return an error code */ 
int EnigmaMachine::getArguments(int argument_cnt, char** argument_array)
{
  int plugboard_check = 0, reflector_check = 0, rotor_position_check = 0, rotor_wiring_check = 0; 

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
	{
	  rotor_wiring_paths.push_back(argument_array[i]);
	  rotor_wiring_check = 1;
	}
      else if (strncmp(".pos", argument_array[i] + (argument_len-4), 4) == 0)
	{
	  strcpy(rotor_position_path, argument_array[i]);
	  rotor_position_check++; 
	}
    }

  if (plugboard_check != 1 || reflector_check != 1 || (rotor_position_check != rotor_wiring_check))
    {
      cerr << "You have failed to include the correct number of command line argumnents. Check you havea .pos file, a .pb file and a .rf file\n";
      return 1;
    }

  return 0; 
}

int EnigmaMachine::multiRotorSetUp()
{
  int ret = 0, iterator_cnt = 0;
  
  Rotor* current_rotor = nullptr, *last_rotor = nullptr;  

  current_rotor = zero_rotor; 
  
  for (auto i = rotor_wiring_paths.begin(); i < rotor_wiring_paths.end(); ++i)
    {
      ret = current_rotor->setUpMapping(*i);
      if (ret)
	return ret; 
      ret = current_rotor->setUpPosition(rotor_position_path, iterator_cnt,  rotor_wiring_paths.size() - (iterator_cnt+1));
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


int EnigmaMachine::setUp(int argument_cnt, char** argument_array)
{
  int ret = 0;

  ret = getArguments(argument_cnt, argument_array);
  if (ret)
    return ret; 

  /* set up plugboard */  
  ret = plugboard.setUp(plugboard_wiring_path); 
  if (ret)
    return ret;

  /* set up all rotors */  
  ret = multiRotorSetUp();
  if (ret)
    return ret;

  ret = reflector.setUp(reflector_wiring_path);
  if (ret)
    return ret;

  return 0; 
}

int EnigmaMachine::transmitForwardsThroughRotors(int message)
{
  bool hit_notch = false;
  int transformed_message = message ;

  for (auto i = rotor_vector.rbegin(); i != rotor_vector.rend(); i++)
    {
      if ((*i)->rotor_index == 0)
	(*i)->rotate();

      if (hit_notch && (*i)->rotor_index != 0)
	(*i)->rotate();

      transformed_message = (*i)->transformForward(transformed_message);
      
      hit_notch = (*i)->isAtNotch();
    }

  return transformed_message; 
}

int EnigmaMachine::transmitBackwardsThroughRotors(int message)
{
  int transformed_message = message ;

  for (auto i = rotor_vector.begin(); i != rotor_vector.end(); i++)
    {
      transformed_message = (*i)->transformBackward(transformed_message);
    }

  return transformed_message; 
}
  
int EnigmaMachine::encryptNum(int message)
{
  int transformed_message;

  transformed_message = plugboard.encryptValue(message);

  transformed_message = transmitForwardsThroughRotors(transformed_message);

  transformed_message = reflector.reflectValue(transformed_message);

  transformed_message = transmitBackwardsThroughRotors(transformed_message);

  transformed_message = plugboard.encryptValue(transformed_message);

  return transformed_message;
}

int EnigmaMachine::encryptMessage()
{
  char input_ch, output_ch;
  int input_int, output_int; 

  cout << "START MESSAGE"; 
  cin >> ws >> input_ch;

  while (!cin.eof())
    {
      if (!(input_ch >= 65 && input_ch < 91))
	{
	  cerr << "Invalid character " << input_ch << " input into Enigma" << endl;
	  return INVALID_INPUT_CHARACTER;
	}

      input_int = input_ch - 65;
      output_int = encryptNum(input_int);
      output_ch = output_int + 65;

      cout << output_ch;

      cin >> ws >> input_ch; 
    }

  return NO_ERROR;
}

      
      




  
  


