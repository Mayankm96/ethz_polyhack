#include <beehaving_drones/BaseDrone.h>

#include <functional>
#include <future>
#include <iostream>
#include <memory>

using namespace dronecore;
using namespace std::this_thread;
using namespace std::chrono;

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

bool BaseDrone::return_to_home()
{
  std::cout << "Commanding RTL..." << std::endl;
  const ActionResult result = action_->return_to_launch();
  if (result != ActionResult::SUCCESS) {
      std::cout << "Failed to command RTL (" << action_result_str(result) << ")" << std::endl;
      return 1;
  } else {
      std::cout << "Commanded RTL." << std::endl;
  }

  return 0;
}
// get home geopoint
GeoPoint BaseDrone::get_home_geopoint()
{
  // Set up callback to monitor altitude while the vehicle is in flight
  Telemetry::Position position = telemetry_->home_position();
  return GeoPoint(position.latitude_deg, position.longitude_deg, position.relative_altitude_m);
}


// get drone position in NED
Vector3r BaseDrone::get_position_NED()
{
  Telemetry::Position position = telemetry_->position();
  GeoPoint position_geo(position.latitude_deg , position.longitude_deg, position.relative_altitude_m);
  GeoPoint home = get_home_geopoint();
  return GeodeticToNedFast(position_geo, home);
}

// convert GeoPoint into mission item
std::shared_ptr<MissionItem> BaseDrone::make_mission_item(GeoPoint waypoint,
                                               float speed_m_s = 1.0,
                                               bool is_fly_through = false,
                                               float gimbal_pitch_deg = 0,
                                               float gimbal_yaw_deg = 0,
                                               MissionItem::CameraAction camera_action)
{
  std::shared_ptr<MissionItem> new_item(new MissionItem());
  new_item->set_position(waypoint.latitude, waypoint.longitude);
  new_item->set_relative_altitude(waypoint.altitude);
  new_item->set_speed(speed_m_s);
  new_item->set_fly_through(is_fly_through);
  new_item->set_gimbal_pitch_and_yaw(gimbal_pitch_deg, gimbal_yaw_deg);
  new_item->set_camera_action(camera_action);
  return new_item;
}

// convert vector of NED coordinates into mission task
std::vector<std::shared_ptr<MissionItem>> BaseDrone::plan_mission_from_ned(std::vector<Vector3r> ned_waypoints, float speed_m_s, bool is_fly_through )
{
  if(ned_waypoints.empty())
  {
    std::cerr << ERROR_CONSOLE_TEXT << "No waypoints provided!" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::vector<std::shared_ptr<MissionItem>> mission_items;
  GeoPoint home = get_home_geopoint();

  for(std::vector<Vector3r>::iterator it = ned_waypoints.begin(); it != ned_waypoints.end(); ++it)
  {
    GeoPoint point = nedToGeodeticFast(*it, home);
    mission_items.push_back(make_mission_item(point,
                                              speed_m_s,
                                              is_fly_through,
                                              0.0f,
                                              0.0f,
                                              MissionItem::CameraAction::NONE));
  }

  return mission_items;
}

// perform mission
bool BaseDrone::perform_mission(std::vector<Vector3r> ned_waypoints, float speed_m_s, bool is_fly_through)
{
  System &system = dc_.system();
  auto mission = std::make_shared<Mission>(system);

  // convert ned coordinates to mission items
  std::vector<std::shared_ptr<MissionItem>> mission_items;
  mission_items = plan_mission_from_ned(ned_waypoints, speed_m_s, is_fly_through);

  // upload mission
  {
    std::cout << "Uploading mission..." << std::endl;
    // We only have the upload_mission function asynchronous for now, so we wrap it using
    // std::future.
    auto prom = std::make_shared<std::promise<Mission::Result>>();
    auto future_result = prom->get_future();
    mission->upload_mission_async(mission_items,
                                  [prom](Mission::Result result) { prom->set_value(result); });

    const Mission::Result result = future_result.get();
    if (result != Mission::Result::SUCCESS) {
        std::cout << "Mission upload failed (" << Mission::result_str(result) << "), exiting."
                  << std::endl;
        return 1;
    }
    std::cout << "Mission uploaded." << std::endl;

    std::atomic<bool> want_to_pause{false};
    // Before starting the mission, we want to be sure to subscribe to the mission progress.
    mission->subscribe_progress([&want_to_pause](int current, int total) {
        std::cout << "Mission status update: " << current << " / " << total << std::endl;

        if (current >= 2) {
            // We can only set a flag here. If we do more request inside the callback,
            // we risk blocking the system.
            want_to_pause = true;
        }
    });
  }

  // Start mission
  {
    std::cout << "Starting mission." << std::endl;
    auto prom = std::make_shared<std::promise<Mission::Result>>();
    auto future_result = prom->get_future();
    mission->start_mission_async([prom](Mission::Result result) {
        prom->set_value(result);
        std::cout << "Started mission." << std::endl;
    });

    const Mission::Result result = future_result.get();
    if (result != Mission::Result::SUCCESS) {
        std::cerr << ERROR_CONSOLE_TEXT << "Mission start failed:" << Mission::result_str(result)
                  << NORMAL_CONSOLE_TEXT << std::endl;
        exit(EXIT_FAILURE);
    }
  }

  // Wait for mission to finish
  while (!mission->mission_finished()) {
      sleep_for(seconds(1));
  }

  return 0;
}
