#include <beehaving_drones/BaseDrone.h>
#include <beehaving_drones/Console.h>
#include <dronecore/mission.h>
#include <functional>
#include <future>
#include <iostream>
#include <memory>

void usage(std::string bin_name)
{
    std::cout << NORMAL_CONSOLE_TEXT << "Usage : " << bin_name << " <connection_url>" << std::endl
              << "Connection URL format should be :" << std::endl
              << " For TCP : tcp://[server_host][:server_port]" << std::endl
              << " For UDP : udp://[bind_host][:bind_port]" << std::endl
              << " For Serial : serial:///path/to/serial/dev[:baudrate]" << std::endl
              << "For example, to connect to the simulator use URL: udp://:14540" << std::endl;
}

std::shared_ptr<MissionItem> make_mission_item(double latitude_deg,
                                               double longitude_deg,
                                               float relative_altitude_m,
                                               float speed_m_s,
                                               bool is_fly_through,
                                               float gimbal_pitch_deg,
                                               float gimbal_yaw_deg,
                                               MissionItem::CameraAction camera_action)
{
    std::shared_ptr<MissionItem> new_item(new MissionItem());
    new_item->set_position(latitude_deg, longitude_deg);
    new_item->set_relative_altitude(relative_altitude_m);
    new_item->set_speed(speed_m_s);
    new_item->set_fly_through(is_fly_through);
    new_item->set_gimbal_pitch_and_yaw(gimbal_pitch_deg, gimbal_yaw_deg);
    new_item->set_camera_action(camera_action);
    return new_item;
}

std::vector<Vector3r> return_helical(Vector3r origin, double rate, double r1, double r2, double l, long int num_of_waypoints)
{
  std::vector<Vector3r> path;
  double t = 0;
  double increment_rate = 1.0f/num_of_waypoint;
  while( t < 1)
  {
    path.push_back(Vector3r(origin.x + r1 * cos(t * rate), origin.y + r2 * sin(t * rate), -(abs(origin.z) + t * l)));
    t = t + increment_rate;
  }

  return path;
}

std::vector<Vector3r> return_square(Vector3r origin, double length)
{
  std::vector<Vector3r> path;
  // square
  path.push_back(origin);
  path.push_back(Vector3r(origin.x, origin.y + length, origin.z));
  path.push_back(Vector3r(origin.x + length, origin.y + length, -5));
  path.push_back(Vector3r(origin.x + length, origin.y, origin.z));
  path.push_back(Vector3r(origin.x, origin.y, origin.z));

  return path;
}

int main(int argc, char **argv)
{
    // take input string
    std::string connection_url;

    if (argc == 2) {
        connection_url = argv[1];
    } else {
        usage(argv[0]);
        return 1;
    }

    BaseDrone drone(connection_url);

    if(drone.start_connection(0))
      return 1;

    GeoPoint home = drone.get_home_geopoint();
    std::cout << "Home:" << home.to_string() << std::endl;

    if(drone.takeoff())
      return 1;

    // Let it hover for a bit before landing again.
    sleep_for(seconds(10));

    // // follow a mission path
    // std::vector<Vector3r> path = return_helical();
    // drone.perform_mission(path, 2);

    sleep_for(seconds(5));

    drone.return_to_home();

    if(drone.landing())
      return 1;

    // We are relying on auto-disarming but let's keep watching the telemetry for a bit longer.
    sleep_for(seconds(3));
    std::cout << "Finished..." << std::endl;

    return 0;
}
