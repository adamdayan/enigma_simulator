#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include"enigma.h"

using namespace std;

/* constructor for the EnigmaMachine class */ 
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

std::vector<int> EnigmaMachine::readSettings(char path[100])
{
  ifstream in_stream;
  std::vector<int> target_vector;
  int current_read; 

  in_stream.open(path);    

  if (!in_stream.is_open())
    {
      cerr << "The stream has failed to open\n";
      exit(11);
    }	      

  in_stream >> current_read;

  while (!in_stream.eof())
    {
      if (in_stream.fail())
	{
	  cerr << "The stream failed either while opening or reading\n";
	  exit(11);
	}
      target_vector.push_back(current_read);
      in_stream >> current_read;
    }

  return target_vector; 
}

void EnigmaMachine::populateSettings()
{
  plugboard_wiring_settings = readSettings(plugboard_wiring_path);
  reflector_wiring_settings = readSettings(reflector_wiring_path);
  rotor_position_settings = readSettings(rotor_position_path); 

  for (std::vector<char*>::const_iterator i = rotor_wiring_paths.begin(); i != rotor_wiring_paths.end(); ++i)
    {
      rotor_wiring_settings.push_back(readSettings(*i));
    }
}





  
  


