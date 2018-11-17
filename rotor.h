#ifndef ROTOR_H
#define ROTOR_H

#include <map>
#include <vector>

class Rotor
{
 private:
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
   
 /* rotor pointers */ 
  Rotor* next_rotor;
  Rotor* prev_rotor;
  
  /* private methods */
  bool isAlreadyMapped(int value); 
  bool isFullyMapped();

 public:

  /* public methods */ 
  int setUpMapping(char* passed_wiring_path);
  int setUpPosition(char* passed_position_path, int config_index, int true_index);
  void rotate();
  bool isAtNotch(); 
  int transformForward(int message);
  int transformBackward(int message);
  Rotor* getNextRotorPtr();
  Rotor* getPrevRotorPtr(); 
  
}; 

#endif
