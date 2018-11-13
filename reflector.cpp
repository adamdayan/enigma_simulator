#include <iostream>
#include <map>
#include <fstream>
#include "errors.h"
#include "reflector.h"

using namespace std;

int Reflector::setUp(char* passed_wiring_path)
{
  string buf; 
  int iterator_cnt = 0, current_read, last_read;  
  ifstream in_stream;

  wiring_path = passed_wiring_path; 
    
  in_stream.open(passed_wiring_path);

  if (!in_stream.is_open())
    {
      cerr << "Stream from " << passed_wiring_path << " has failed to open\n";
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
      
      /* check for non-numeric characters */ 
      for (auto i = buf.begin(); i != buf.end(); i++)
	{
	  if (!(*i > 47 && *i <= 57))
	    {
	      cerr << "Non-numeric character " << buf << " detected in "
		   << passed_wiring_path << " at position " << in_stream.tellg() << endl;
	      return NON_NUMERIC_CHARACTER;
	    }
	}

      current_read = std::stoi(buf);
	  
      /* check stream is still open */ 
      if (in_stream.fail())
	{
	  cerr << "The stream from " << passed_wiring_path << " has failed at " << in_stream.tellg() << endl;
	  return ERROR_OPENING_CONFIGURATION_FILE;
	}

      /* check for index error */ 
      if (!(current_read >= 0 || current_read < 26))
	{
	  cerr << "Number at " << in_stream.tellg() << " in " << passed_wiring_path <<" not between 0 and 25\n";
	  return INVALID_INDEX;
	}

      /* modify behaviour depending on whether current_read is first or second of the switch pair */  
      if ((iterator_cnt % 2) == 0)
	last_read =  current_read;

      else if ((iterator_cnt % 2) != 0)
	{
	  if (last_read == current_read)
	    {
	      cerr << "Number at " << in_stream.tellg() << " in " << passed_wiring_path << " was mapped to itself\n";
	      return INVALID_REFLECTOR_MAPPING;
	    }

	  if (wiring_map.find(last_read) != wiring_map.end())
	    {
	      cerr << "Attempted to map " << last_read << " twice\n";
	      return INVALID_REFLECTOR_MAPPING;
	    }

	  if (wiring_map.find(current_read) != wiring_map.end())
	    {
	      cerr << "Attempted to map " << current_read << " twice\n";
	      return INVALID_REFLECTOR_MAPPING;
	    } 

	  wiring_map.insert(pair <int, int> (last_read, current_read));
	  wiring_map.insert(pair <int, int> (current_read, last_read));
	}
      in_stream >> buf;
      iterator_cnt++; 
    }

  in_stream.close(); 
  
  /* check that there are 26 reflector parameters */ 
  if (iterator_cnt != 26)
    {
      cerr << "Incorrect number of parameters found in " << passed_wiring_path << endl;
      return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
    }

  return NO_ERROR;
}

int Reflector::reflectValue(int value)
{
  return wiring_map[value];
}
