#include <map>
#include <fstream>
#include <iostream>
#include"rotor.h"

using namespace std;

int Rotor::numericCheck(char* path)
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
	  cerr << "Non-numeric character " << ch << " detected in " << path << " at position " << in_stream.tellg() << endl;
	  return 4;
	}
      in_stream.get(ch); 
    }

  in_stream.close();

  return 0;
}

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
  int iterator_cnt = 0, current_read, ret = 0; 
  ifstream in_stream;

  ret = numericCheck(passed_wiring_path);
  if (ret)
    return ret;

  in_stream.open(passed_wiring_path);
  if (!in_stream.is_open())
    {
      cerr << "Failed to open " << passed_wiring_path << endl;
      return 11;
    }

  in_stream >> current_read;

  while (!in_stream.eof())
    {
      if (in_stream.fail())
	{
	  cerr << "Failed at position " << in_stream.tellg() << " in " << passed_wiring_path << endl;
	  return 11;
	}

      if (!(current_read >= 0 && current_read < 26))
	{
	  cerr << "Invalid index " << current_read << " at position " << in_stream.tellg() << " in " << passed_wiring_path << endl;
	  return 3;
	}

      if (iterator_cnt < 26 && isAlreadyMapped(current_read))
	{
	  cerr << "Attempted to map " << current_read << " twice in "
	       << passed_wiring_path << " at position " << in_stream.tellg() << endl;
	  return 7;
	}

      if (iterator_cnt < 26)
	{
	  wiring_map.insert(pair <int, int> (iterator_cnt, current_read));
	  reverse_wiring_map.insert(pair <int, int> (current_read, iterator_cnt));
	}

      else
	notches.push_back(current_read); 
	  

      iterator_cnt++;
      in_stream >> current_read; 
    }

  in_stream.close(); 

  if (!isFullyMapped())
    {
      cerr << "The rotor mapping " << passed_wiring_path << " is not complete\n";
      return 7;
    }
  
  return 0;
}

int Rotor::setUpPosition(char* passed_position_path, int num)
{
  int ret = 0, iterator_cnt = 0, current_read; 
  ifstream in_stream;

  rotor_index = num;
  position_path = passed_position_path; 

  ret = numericCheck(passed_position_path);
  if (ret)
    return ret;


  in_stream.open(passed_position_path);
  if (!in_stream.is_open())
    {
      cerr << "Failed to open " << passed_position_path << endl;
      return 11;
    }

  in_stream >> current_read;

  while (!in_stream.eof())
    {
      if (in_stream.fail())
	{
	  cerr << "Failed at position " << in_stream.tellg() << " in " << passed_position_path << endl;
	  return 11;
	}

      if (!(current_read >= 0 && current_read < 26))
	{
	  cerr << "Invalid index " << current_read << " at position " << in_stream.tellg() << " in " << passed_position_path << endl;
	  return 3;
	}

      if (iterator_cnt == num)
	{
	  position = current_read;
	  break;
	}
      iterator_cnt++;
      in_stream >> current_read; 
    }
  in_stream.close();

  if (iterator_cnt < num)
    {
      cerr << "Insufficient rotor starting positions in " << passed_position_path << " for rotor index " << num ;
      return 8;
    }

  return 0; 
}

/* function that rotates rotor; if rotor hits a notch, it will rotate the next rotor as well */ 
void Rotor::rotate()
{
  if ((position + 1) < 26)
    position++;
  else
    position = (position + 1) - 26;
}

int Rotor::transformForward(int message)
{
  int transformed_message;

  if ((message + position) < 26)
    transformed_message = wiring_map[message + position];
  else
    transformed_message = wiring_map[message + position - 26];

  if ((transformed_message - position) > 0)
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

  if ((transformed_message - position) > 0)
    transformed_message = transformed_message - position;
  else
    transformed_message = transformed_message - position + 26;

  return transformed_message;
}

bool Rotor::isAtNotch()
{
  for (auto i = notches.begin(); i != notches.end(); i++)
    {
      if (position == *i)
	return true;
    }
  return false; 
}
		 

      
  
  
