#ifndef ROTOR_H
#define ROTOR_H

#include <map>

class Rotor
{
 public:
  /* paths */
  char wiring_path[100];
  char position_path[100];

  /* maps */ 
  std::map<int, int> wiring_map;
  std::map<int, int> reverse_wiring_map;

  /* position */
  int position; 

  /* methods */
  int numericCheck(char passed_wiring_path[100]); 
  int setUpMapping(char passed_wiring_path[100]);
  int setUpPosition(char passed_position_path[100]);
}
