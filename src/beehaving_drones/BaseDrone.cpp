#include <beehaving_drones/BaseDrone.h>

// constructor for class
BaseDrone::BaseDrone(std::string connection_url) : dc_()
{
  connection_url_ = connection_url;
}

// constructor for class
BaseDrone::BaseDrone() : dc_()
{
  connection_url_ = "udp://:14540";
}

// check connection with gazebo
bool BaseDrone::check_connection()
{
  connection_result_ = dc_.add_any_connection(connection_url_);

  if (connection_result_ != ConnectionResult::SUCCESS)
  {
      std::cout << ERROR_CONSOLE_TEXT
                << "Connection failed: " << connection_result_str(connection_result_)
                << NORMAL_CONSOLE_TEXT << std::endl;
      return 1;
  }

  return 0;
}

// discover the drone on gazebo
bool BaseDrone::discover_system()
{
  bool discovered_system = false;

  std::cout << "Waiting to discover system..." << std::endl;

  dc_.register_on_discover([&discovered_system](uint64_t uuid) {
      std::cout << "Discovered system with UUID: " << uuid << std::endl;
      discovered_system = true;
  });

  // We usually receive heartbeats at 1Hz, therefore we should find a system after around 2
  // seconds.
  sleep_for(seconds(2));

  if (!discovered_system) {
      std::cout << ERROR_CONSOLE_TEXT << "No system found, exiting." << NORMAL_CONSOLE_TEXT
                << std::endl;
      return 1;
  }

  // We don't need to specify the UUID if it's only one system anyway.
  // If there were multiple, we could specify it with:
  // dc.system(uint64_t uuid);
  System &system = dc_.system();

  telemetry_ = std::make_shared<Telemetry>(system);
  action_ = std::make_shared<Action>(system);

  return 0;
}

// enable listening to drone
bool BaseDrone::enable_telemtry()
{
  // We want to listen to the altitude of the drone at 1 Hz.
  const Telemetry::Result set_rate_result = telemetry_->set_rate_position(1.0);

  if (set_rate_result != Telemetry::Result::SUCCESS)
  {
      std::cout << ERROR_CONSOLE_TEXT
                << "Setting rate failed:" << Telemetry::result_str(set_rate_result)
                << NORMAL_CONSOLE_TEXT << std::endl;
      return 1;
  }

  // Set up callback to monitor altitude while the vehicle is in flight
  telemetry_->position_async([](Telemetry::Position position)
  {
      std::cout << TELEMETRY_CONSOLE_TEXT // set to blue
                << "Altitude: " << position.relative_altitude_m << " m"
                << NORMAL_CONSOLE_TEXT // set to default color again
                << std::endl;
  });

  return 0;
}

// arm the drone
bool BaseDrone::enable_arm()
{
  // Check if vehicle is ready to arm
  while (telemetry_->health_all_ok() != true) {
      std::cout << "Vehicle is getting ready to arm" << std::endl;
      sleep_for(seconds(1));
  }

  // Arm vehicle
  std::cout << "Arming..." << std::endl;
  const ActionResult arm_result = action_->arm();

  if (arm_result != ActionResult::SUCCESS)
  {
      std::cout << ERROR_CONSOLE_TEXT << "Arming failed:" << action_result_str(arm_result)
                << NORMAL_CONSOLE_TEXT << std::endl;
      return 1;
  }

  return 0;
}

// takeoff drone
bool BaseDrone::takeoff()
{
  // Take off
  std::cout << "Taking off..." << std::endl;
  const ActionResult takeoff_result = action_->takeoff();

  if (takeoff_result != ActionResult::SUCCESS)
  {
      std::cout << ERROR_CONSOLE_TEXT << "Takeoff failed:" << action_result_str(takeoff_result)
                << NORMAL_CONSOLE_TEXT << std::endl;
      return 1;
  }

  return 0;
}

// land drone
bool BaseDrone::landing()
{
  std::cout << "Landing..." << std::endl;
  const ActionResult land_result = action_->land();
  if (land_result != ActionResult::SUCCESS) {
      std::cout << ERROR_CONSOLE_TEXT << "Land failed:" << action_result_str(land_result)
                << NORMAL_CONSOLE_TEXT << std::endl;
      return 1;
  }

  // Check if vehicle is still in air
  while (telemetry_->in_air()) {
      std::cout << "Vehicle is landing..." << std::endl;
      sleep_for(seconds(1));
  }
  std::cout << "Landed!" << std::endl;
  return 0;
}

// simplfy performs arming and checks for connection;
bool BaseDrone::start_connection(bool flag_telemtry)
{
  if(check_connection())
    return 1;

  if(discover_system())
    return 1;

  if (flag_telemtry)
    if(enable_telemtry())
      return 1;

  if(enable_arm())
      return 1;

  return 0;
}
