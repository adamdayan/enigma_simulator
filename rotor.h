#ifndef ROTOR_H
#define ROTOR_H

#include <map>
#include <vector>

class Rotor
{
 public:
  /* paths */
  char* wiring_path;
  char* position_path;

  /* maps */ 
  std::map<int, int> wiring_map;
  std::map<int, int> reverse_wiring_map;

  /* position */
  int position;
  int rotor_index; 
  std::vector<int> notches;
  Rotor* next_rotor = NULL;
  Rotor* prev_rotor = NULL; 

  /* methods */
  int decimaliser(int num, int since_ws_cnt); 
  bool isAlreadyMapped(int value); 
  bool isFullyMapped(); 
  int setUpMapping(char* passed_wiring_path);
  int setUpPosition(char* passed_position_path, int num);
  void rotate();
  bool isAtNotch(); 
  int transformForward(int message);
  int transformBackward(int message);
  
}; 

#endif
