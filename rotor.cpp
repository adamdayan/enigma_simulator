#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include"rotor.h"
#include "errors.h"

using namespace std;


bool Rotor::isAlreadyMapped(int value)
{
  for (auto it = wiring_map.begin(); it != wiring_map.end(); ++it)
    {
      if (it->second == value)
	return true;
    }

  return false;
}

bool Rotor::isFullyMapped()
{
  if (wiring_map.size() != 26)
    return false; 
  for (int i = 0; i < 26; i++)
    {
      if (wiring_map.find(i) == wiring_map.end())
	return false;
    }
  return true;
}
  

int Rotor::setUpMapping(char* passed_wiring_path)
{
  string buf; 
  int iterator_cnt = 0, current_read;  
  ifstream in_stream;


  in_stream.open(passed_wiring_path);
  if (!in_stream.is_open())
    {
      cerr << "Failed to open " << passed_wiring_path << endl;
      return ERROR_OPENING_CONFIGURATION_FILE;
    }

  in_stream >> buf;

  while (!in_stream.eof())
    {
      if (in_stream.fail())
	{
	  cerr << "Failed at position " << in_stream.tellg() << " in " << passed_wiring_path << endl;
	  return ERROR_OPENING_CONFIGURATION_FILE;
	}

      /* checks for non_numeric characters */  
      for (auto i = buf.begin(); i != buf.end(); i++)
	{
	  if (!(*i > 47 && *i <= 57))
	    {
	      cerr << "Non-numeric character '" << buf << "' detected in "
		   << passed_wiring_path << " at position " << in_stream.tellg() << endl;
	      return NON_NUMERIC_CHARACTER;
	    }
	}

      /* converts configuration string to an integer */
      current_read = std::stoi(buf); 

      /* checks that current read is a number between 0-25 */
      if (!(current_read >= 0 && current_read < 26))
	{
	  cerr << "Invalid index " << current_read << " at position "
	       << in_stream.tellg() << " in " << passed_wiring_path << endl;
	  return INVALID_INDEX;
	}

      /* ensures no number other than a notch can be mapped twice */
      if (iterator_cnt < 26 && isAlreadyMapped(current_read))
	{
	  cerr << "Attempted to map " << current_read << " twice in "
	       << passed_wiring_path << " at position " << in_stream.tellg() << endl;
	  return INVALID_ROTOR_MAPPING;
	}

      /* inserts current read into a mapping for both forward and backward transmission through 
	 the rotors */ 
      if (iterator_cnt < 26)
	{
	  wiring_map.insert(pair <int, int> (iterator_cnt, current_read));
	  reverse_wiring_map.insert(pair <int, int> (current_read, iterator_cnt));
	}

      /* if the read is past the 26th number in the 26th number in the configuration file then 
	 adds it to the notch vector */
      else
	notches.push_back(current_read); 
	   
  
      in_stream >> buf;
      iterator_cnt++; 
    }

  in_stream.close(); 

  /* checks whether every number has a mapping */ 
  if (!isFullyMapped())
    {
      cerr << "The rotor mapping " << passed_wiring_path << " is not complete\n";
      return INVALID_ROTOR_MAPPING;
    }
  
  return NO_ERROR;
}

int Rotor::setUpPosition(char* passed_position_path, int config_index, int true_index)
{
  string buf; 
  int iterator_cnt = 0, current_read; 
  ifstream in_stream;

  /* sets rotor index data member */ 
  rotor_index = true_index;

  /* sets position configuration filepath data member */ 
  position_path = passed_position_path; 

  in_stream.open(passed_position_path);
  if (!in_stream.is_open())
    {
      cerr << "Failed to open " << passed_position_path << endl;
      return ERROR_OPENING_CONFIGURATION_FILE;
    }

  in_stream >> buf;
  
  while (!in_stream.eof())
    {
      if (in_stream.fail())
	{
	  cerr << "Failed at position " << in_stream.tellg() << " in " << passed_position_path << endl;
	  return ERROR_OPENING_CONFIGURATION_FILE;
	}

      /* checks for non-numeric characters */ 
      for (auto i = buf.begin(); i != buf.end(); i++)
	{
	  if (!(*i >= static_cast<int> ('0') && *i <= static_cast<int> ('9')))
	    {
	      cerr << "Non-numeric character '" << buf << "' detected in "
		   << passed_position_path << " at position " << in_stream.tellg() << endl;
	      return NON_NUMERIC_CHARACTER;
	    }
	}

      /* converts string of current read into an integer */ 
      current_read = std::stoi(buf); 

      /* checks if current read is a licit number */ 
      if (!(current_read >= 0 && current_read < 26))
	{
	  cerr << "Invalid index " << current_read << " at position "
	       << in_stream.tellg() << " in " << passed_position_path << endl;
	  return INVALID_INDEX;
	}

      /* finds the relevant starting position in the configuration file for the rotor in 
	 question */ 
      if (iterator_cnt == config_index)
	{
	  position = current_read;
	  iterator_cnt++; 
	  break;
	}

      in_stream >> buf;
      iterator_cnt++; 
    }
  
  in_stream.close();

  /* checks that sufficient starting positions have been provided given the number of rotors */
  if (iterator_cnt <= config_index)
    {
      cerr << "Insufficient rotor starting positions in "
	   << passed_position_path << " for rotor index " << config_index << endl;
      return 8;
    }

  return 0; 
}

/* function that rotates rotor; if rotor hits a notch, it will rotate the next rotor as well */ 
void Rotor::rotate()
{
  /* rotor position 26 is in reality position 0 given its circular nature */ 
  if ((position + 1) < 26)
    position++;
  else
    position = (position + 1) - 26;
}

int Rotor::transformForward(int message)
{
  int transformed_message;

  /* normalise message on input and output */ 
  if ((message + position) < 26)
    transformed_message = wiring_map[message + position];
  else
    transformed_message = wiring_map[message + position - 26];

  if ((transformed_message - position) >= 0)
    transformed_message = transformed_message - position;
  else
    transformed_message = transformed_message - position + 26;

  return transformed_message;
}

int Rotor::transformBackward(int message)
{
  int transformed_message;

  if ((message + position) < 26)
    transformed_message = reverse_wiring_map[message + position];
  else
    transformed_message = reverse_wiring_map[message + position - 26];

  if ((transformed_message - position) >= 0)
    transformed_message = transformed_message - position;
  else
    transformed_message = transformed_message - position + 26;

  return transformed_message;
}

bool Rotor::isAtNotch()
{
  /* potentially multiple notches to vital to loop through notch vecotr */ 
  for (auto i = notches.begin(); i != notches.end(); i++)
    {
      if (position == *i)
	return true;
    }
  return false; 
}

Rotor* Rotor::getNextRotorPtr()
{
  return next_rotor;
}

Rotor* Rotor::getPrevRotorPtr()
{
  return prev_rotor;
}

void Rotor::setNextRotorPtr(Rotor* rotor_ptr)
{
  next_rotor = rotor_ptr;
}

void Rotor::setPrevRotorPtr(Rotor* rotor_ptr)
{
  prev_rotor = rotor_ptr;
}

int Rotor::getRotorIndex()
{
  return rotor_index;
}
  
		 

      
  
  
