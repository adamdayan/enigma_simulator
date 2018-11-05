#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include "enigma.h"

using namespace std; 

/* ---------------- PLUGBOARD METHODS -------------- */
int Plugboard::setUp(char* passed_wiring_path, std::vector<int> plugboard_wiring_settings)
{
  int iterator_cnt = 0; 
  //std::map<int, int> wiring_map;

  wiring_path = passed_wiring_path; 

  if (plugboard_wiring_settings.size() % 2 != 0)
    {
      cerr << "There is an odd number of plugboard numbers in " << wiring_path;
      return 6;
    }
    
  for (std::vector<int>::const_iterator i = plugboard_wiring_settings.begin(); i != plugboard_wiring_settings.end(); ++i)
    {
      if (iterator_cnt % 2 == 0)
	{
	  if (*i == *(i+1))
	    {
	      cerr << "Attempted to map number to itself in " << wiring_path << endl;
	      return 5;
	    }
	  wiring_map.insert(pair <int, int> (*i, *(i+1)));
	}
      if (iterator_cnt % 2 != 0)
	{
	  wiring_map.insert(pair <int, int> (*i, *(i-1)));
	}
      iterator_cnt++; 
    }
 
  return 0; 
}

int Plugboard::numericCheck(char path[100])
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

int Plugboard::setUp(char path[100])
{
  int iterator_cnt = 0, current_read, last_read, ret; 
  ifstream in_stream;

  ret = numeric_check(path);
  if (ret)
    return ret;
    
  in_stream.open(path);

  if (!in_stream.is_open())
    {
      cerr << "Stream from " << path << " has failed to open\n";
      return 11;
    }
  
  in_stream >> current_read;

  while (!in_stream.eof())
    {
      /* check stream is still open */ 
      if (in_stream.fail())
	{
	  cerr << "The stream from " << path << " has failed at " << in_stream.tellg() << endl;
	  return 11;
	}

      /* check for index error */ 
      if (!(current_read >= 0 || current_read < 26))
	{
	  cerr << "Number at " << in_stream.tellg() << " in " << path <<" not between 0 and 25\n";
	  return 3;
	}

      /* modify behaviour depending on whether current_read is first or second of the switch pair */  
      if ((iterator_cnt % 2) == 0)
	last_read =  current_read;

      else if ((iterator_cnt % 2) != 0)
	{
	  if (last_read == current_read)
	    {
	      cerr << "Number at " << in_stream.tellg() << " in " << path << " was mapped to itself\n";
	      return 5;
	    }

	  wiring_map.insert(pair <int, int> (last_read, current_read));
	  wiring_map.insert(pair <int, int> (current_read, last_read));

	  in_stream >> current_read;
	  iterator++;
	}
    }
  /* check that number of plugboard parameters is even */ 
  if ((iterator_cnt+1) % 2 != 0)
    {
      cerr << "Odd number of parameters found in " << path << endl;
      return 6;
    }

   /* fill map in for non-switched numbers */ 
  for (int num = 0; num < 26; num++)
    {
      if (wiring_map.find(num) == wiring_map.end())
	wiring_map.insert(pair <int, int> (num, num));
    }


  return 0;
}
      
  

int Plugboard::encryptValue(int value)
{
  return wiring_map[value];
}
