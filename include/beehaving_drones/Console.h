#ifndef __Console_H_INCLUDED__
#define __Console_H_INCLUDED__

#include <dronecore/action.h>
#include <dronecore/dronecore.h>
#include <dronecore/mission.h>
#include <dronecore/telemetry.h>
#include <functional>
#include <future>
#include <iostream>
#include <memory>

#define ERROR_CONSOLE_TEXT "\033[31m" // Turn text on console red
#define TELEMETRY_CONSOLE_TEXT "\033[34m" // Turn text on console blue
#define NORMAL_CONSOLE_TEXT "\033[0m" // Restore normal console colour

using namespace dronecore;
using namespace std::placeholders; // for `_1`
using namespace std::chrono; // for seconds(), milliseconds()
using namespace std::this_thread; // for sleep_for()

// Handles Action's result
inline void handle_action_err_exit(ActionResult result, const std::string &message);
// Handles Mission's result
inline void handle_mission_err_exit(Mission::Result result, const std::string &message);
// Handles Connection result
inline void handle_connection_err_exit(ConnectionResult result, const std::string &message);

#endif
