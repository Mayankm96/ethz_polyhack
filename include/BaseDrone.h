#ifndef __BaseDrone_H_INCLUDED__
#define __BaseDrone_H_INCLUDED__

#include <chrono>
#include <cstdint>
#include <dronecore/action.h>
#include <dronecore/dronecore.h>
#include <dronecore/telemetry.h>
#include <iostream>
#include <thread>

using namespace dronecore;
using namespace std::this_thread;
using namespace std::chrono;

#define ERROR_CONSOLE_TEXT "\033[31m" // Turn text on console red
#define TELEMETRY_CONSOLE_TEXT "\033[34m" // Turn text on console blue
#define NORMAL_CONSOLE_TEXT "\033[0m" // Restore normal console colour

class BaseDrone
{
  private:
    DroneCore dc_;
    std::string connection_url_;
    ConnectionResult connection_result_;
    std::shared_ptr<Telemetry> telemetry_;
    std::shared_ptr<Action> action_;

  public:
    // constructor for class to check connection
    BaseDrone(std::string connection_url);
    BaseDrone();

    // check connection with gazebo
    bool check_connection();

    // discover the drone on gazebo
    bool discover_system();

    // enable listening to drone
    bool enable_telemtry();

    // arm the drone
    bool enable_arm();

    // takeoff drone
    bool takeoff();

    // land drone
    bool landing();

    // simplfy performs arming and checks for connection;
    bool start_connection(bool flag_telemtry = 0);
};

#endif
