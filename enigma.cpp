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


EnigmaMachine::~EnigmaMachine()
{
  for (auto i = rotor_vector.begin(); i != rotor_vector.end(); i++)
    {
      delete (*i);
    }
}


int EnigmaMachine::getArguments(int argument_cnt, char** argument_array)
{
  int plugboard_check = 0, reflector_check = 0, rotor_position_check = 0, rotor_wiring_check = 0; 

  for (int i = 1; i < argument_cnt; i++)
    {
      int argument_len = strlen(argument_array[i]); 
      /* check file extensions to identify configuration files */ 
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

  /* ensure correct number of command line arguments have been passed */ 
  if (plugboard_check != 1 || reflector_check != 1 || (rotor_position_check != rotor_wiring_check))
    {
      cerr << "You have failed to include the correct number of command line argumnents. Check you have a .pb file and a .rf file. If you've included a .rot then you'll need a .pos\n";
      return INSUFFICIENT_NUMBER_OF_PARAMETERS;
    }

  return NO_ERROR; 
}

int EnigmaMachine::multiRotorSetUp()
{
  int ret = 0, iterator_cnt = 0;
  
  Rotor* current_rotor = nullptr, *last_rotor = nullptr;  

  current_rotor = zero_rotor; 

  /* loop through all rotor config file paths, create a Rotor object on the heap for each and store the pointers in a vector */ 
  for (auto i = rotor_wiring_paths.begin(); i < rotor_wiring_paths.end(); ++i)
    {
      ret = current_rotor->setUpMapping(*i);
      if (ret)
	return ret; 
      ret = current_rotor->setUpPosition(rotor_position_path, iterator_cnt,  rotor_wiring_paths.size() - (iterator_cnt+1));
      if (ret)
	return ret; 

      current_rotor->setPrevRotorPtr(last_rotor);

      rotor_vector.push_back(current_rotor); 

      if (last_rotor != nullptr)
	{
	  last_rotor->setNextRotorPtr(current_rotor);
	}

      last_rotor = current_rotor;
      if ((static_cast<size_t> (iterator_cnt + 1) < rotor_wiring_paths.size()))
	current_rotor = new Rotor;
      iterator_cnt++;
    }

  return NO_ERROR; 
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

  /* set configuration boolean to true. if this isn't done, the EnigmaMachine object will 
     not be allowed to employ any other features */ 
  is_correctly_configured = true; 
  
  return NO_ERROR; 
}

int EnigmaMachine::transmitForwardsThroughRotors(int message)
{
  bool hit_notch = false;
  int transformed_message = message ;

  /* loop through all rotors, rotate if necessary and transmit integer message through them */ 
  for (auto i = rotor_vector.rbegin(); i != rotor_vector.rend(); i++)
    {
      /* first rotor should always rotate */ 
      if ((*i)->getRotorIndex() == 0)
	(*i)->rotate();

      /* non-first rotors should only rotate if previous rotor has hit a notch */ 
      if (hit_notch && (*i)->getRotorIndex() != 0)
	(*i)->rotate();

      transformed_message = (*i)->transformForward(transformed_message);

      /* check if rotor has hit a notch and store bool in variable outside loop so next 
	 rotor can access */ 
      hit_notch = (*i)->isAtNotch();
    }

  return transformed_message; 
}

int EnigmaMachine::transmitBackwardsThroughRotors(int message)
{
  int transformed_message = message ;

  /* loop through all rotors and transmit integer message through them. rotors don't 
     rotate on backwards transmission */ 
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

  /* don't allow encryption if EnigmaMachine object not correctly configured */  
  if (!is_correctly_configured)
    {
      cerr << "EnigmaMachine object not provided with set-up arguments" ; 
      return INSUFFICIENT_NUMBER_OF_PARAMETERS;
    }
  
  cin >> ws >> input_ch;

  /* loop to accept char inputs from input stream, encrypt inputs and write them to output stream */
  while (!cin.eof())
    {
      if (!(input_ch >= 65 && input_ch < 91))
	{
	  /* check input char is A-Z */ 
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
  

      
      




  
  


