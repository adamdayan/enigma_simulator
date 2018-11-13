#include "enigma.h"

using namespace std;

int main (int argc, char** argv)
{
  EnigmaMachine e1;

  int ret;

  ret = e1.setUp(argc, argv);
  if (ret)
    return ret;

  ret = e1.encryptMessage();
  return ret;
}

