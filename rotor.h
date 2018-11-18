#ifndef ROTOR_H
#define ROTOR_H

#include <map>
#include <vector>

class Rotor
{
 private:
  /* PATHS */
  char* wiring_path;
  char* position_path;

  /* MAPS */ 
  std::map<int, int> wiring_map;
  std::map<int, int> reverse_wiring_map;

  /* POSIITION */
  int position;
  int rotor_index; 
  std::vector<int> notches;
   
 /* ROTOR POINTERS */ 
  Rotor* next_rotor;
  Rotor* prev_rotor;
  
  /* PRIVATE METHODS */

  /* method that checks whether a value is already contained in the rotor wiring map
     returns true if it is, false if not */
  bool isAlreadyMapped(int value); 

  /* method that checks whether the rotor wiring map has the correct number of values. returns true
     if map size is correct, false if not */ 
  bool isFullyMapped();

 public:

  /* PUBLIC METHODS */ 

  /* method that sets up the rotor wiring maps based on a configuration file path parameter, 
     checks for various errors and returns either the corresponding error code or a no error code */ 
  int setUpMapping(char* passed_wiring_path);

  /* method that sets up the rotor starting position based on a configuration file path parameter.
     checks for various errors and returns either the corresponding error code or a no error code */ 
  int setUpPosition(char* passed_position_path, int config_index, int true_index);

  /* method that rotates the rotor by incrementing position by 1 (taking into account the rotor's 
     circular nature */ 
  void rotate();

  /* method that returns true if a rotor position is equal to one of its notch values. */  
  bool isAtNotch(); 
  int transformForward(int message);
  int transformBackward(int message);
  Rotor* getNextRotorPtr();
  Rotor* getPrevRotorPtr(); 
  
}; 

#endif
