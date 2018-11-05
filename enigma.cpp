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

int EnigmaMachine::numericCheck(char path[100])
{
  ifstream in_stream;
  char ch;

  in_stream.open(path);

  in_stream.get(ch);

  if (!in_stream.is_open())
    {
      cerr << "Stream from " << path << " has failed to open\n";
      return 11;
    }

  while (!in_stream.eof())
    {
      if (!((ch > 47 && ch <= 57) || (ch == 32) ||(ch == 10)))
	{
	  cerr << "Non-numeric character " << ch <<" detected in " << path << " at position " << in_stream.tellg() << endl;
	  return 4;
	}
      in_stream.get(ch); 
    }

  in_stream.close(); 

  return 0;
}

int EnigmaMachine::readSettings(char path[100], std::vector<int>& target_vector)
{
  ifstream in_stream;
  int current_read, ret;

  ret = numericCheck(path);
  if (ret)
    return ret; 

  in_stream.open(path);    

  if (!in_stream.is_open())
    {
      cerr << "Stream from " << path << " has failed to open\n" << endl;
      return 11;
    }	      

  in_stream >> current_read;

  while (!in_stream.eof())
    {
      if (in_stream.fail())
	{
	  cerr << "Stream from " << path << " has failed at " << in_stream.tellg() << endl;
	  return 11;
	}
      if (current_read < 0 || current_read > 25)
	{
	  cerr << "'" << current_read << "' is not between 0 and 25. Found at "
	       << in_stream.tellg() << " in " << path << endl; 
	}
      target_vector.push_back(current_read);
      in_stream >> current_read;
    }

  in_stream.close(); 

  return 0;
}

int EnigmaMachine::populateSettings()
{
  int ret = 0;
  ret = readSettings(plugboard_wiring_path, plugboard_wiring_settings);
  if (ret)
    return ret; 
  ret = readSettings(reflector_wiring_path, reflector_wiring_settings);
  if (ret)
    return ret; 
  ret = readSettings(rotor_position_path, rotor_position_settings);
  if (ret)
    return ret; 

  for (std::vector<char*>::const_iterator i = rotor_wiring_paths.begin(); i != rotor_wiring_paths.end(); ++i)
    {
      std::vector<int> current_rotor_wiring_settings; 
      ret = readSettings(*i, current_rotor_wiring_settings); 
      if (ret)
	return ret; 
      rotor_wiring_settings.push_back(current_rotor_wiring_settings);
    }

  return ret; 
}

int EnigmaMachine::setUp()
{
  int ret = plugboard.setUp(plugboard_wiring_path, plugboard_wiring_settings); 
  if (ret)
    return ret;
  else
    return 0; 
}

int EnigmaMachine::readInput(





  
  


