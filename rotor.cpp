#include <map>
#include <iostream>
#include"rotor.h"

using namespace std;

int Plugboard::numericCheck(char* path)
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

bool Rotor::isAlreadyMapped(int value, char* passed_wiring_path)
{
  for (auto it = wiring_map.begin(); it != wiring_map.end(); ++i)
    {
      if (it->second == value)
	return true;
    }

  return false;
}

int Rotor::setUpMapping(char passed_wiring_path[100])
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
	  cerr << "Invalid index " << current_read << " at position " << in_stream.tellg() << " in " << path << endl;
	  return 3;
	}

      //TODO need to put in 'isAlreadyMapped()' function here

      wiring_mapping.insert(pair, <int, int> (iterator_cnt, current_read));
      reverse_wiring_mapping.insert(pair, <int, int> (current_read, iterator_cnt));

      iterator_cnt++;
    }

  //TODO need to put in 'isAllMapped()' func here
  
  return 0;
}
