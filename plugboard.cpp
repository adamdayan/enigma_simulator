#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "enigma.h"

using namespace std; 

/* TODO deal with end of stream fuckery */ 
int Plugboard::setUp(char path[100])
{
  string buf; 
  int iterator_cnt = 0, current_read, last_read;  
  ifstream in_stream;

  wiring_path = path; 
    
  in_stream.open(path);

  if (!in_stream.is_open())
    {
      cerr << "Stream from " << path << " has failed to open\n";
      return 11;
    }
  
  in_stream >> buf;

  while (!in_stream.eof())
    {
      if (in_stream.fail())
	{
	  cerr << "Failed at position " << in_stream.tellg() << " in " << path << endl;
	  return 11;
	}
      
      /* check for non-numeric characters */ 
      for (auto i = buf.begin(); i != buf.end(); i++)
	{
	  if (!(*i > 47 && *i <= 57))
	    {
	      cerr << "Non-numeric character " << buf << " detected in "
		   << path << " at position " << in_stream.tellg() << endl;
	      return 4;
	    }
	}

      current_read = std::stoi(buf);
	  
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
	}
      in_stream >> buf;
      iterator_cnt++; 
    }

  in_stream.close(); 
  
  /* check that number of plugboard parameters is even */ 
  if ((iterator_cnt) % 2 != 0)
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
