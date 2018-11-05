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
  /* fill map in for non-switched numbers */ 
  for (int num = 0; num < 26; num++)
    {
      if (wiring_map.find(num) == wiring_map.end())
	wiring_map.insert(pair <int, int> (num, num));
    }

  return 0; 
}
