#ifndef __DogDrone_H_INCLUDED__
#define __DogDrone_H_INCLUDED__

#include "BaseDrone.h"

class DogDrone : public BaseDrone
{
  private:

  public:
    // constructor for class to check connection
    DogDrone(std::string connection_url): BaseDrone(connection_url) {};
    DogDrone() : BaseDrone() {};
};


#endif
