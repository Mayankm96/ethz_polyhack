#include <beehaving_drones/BaseDrone.h>
#include <beehaving_drones/Console.h>
#include <trajectories/generate_trajectories_with_goals.h>

#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <string>

void usage(std::string bin_name)
{
    std::cout << NORMAL_CONSOLE_TEXT << "Usage : " << bin_name << " <connection_url>" << std::endl
              << "Connection URL format should be :" << std::endl
              << " For TCP : tcp://[server_host][:server_port]" << std::endl
              << " For UDP : udp://[bind_host][:bind_port]" << std::endl
              << " For Serial : serial:///path/to/serial/dev[:baudrate]" << std::endl
              << "For example, to connect to the simulator use URL: udp://:14540" << std::endl;
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

    std::vector <Vector3r> path;

    Vector3r pose = drone.get_position_NED();
    // fancy takeoff
    path = trajectories_with_goals::generate_tornado_path(Vector3r(0, 0, -6), pose);

    std::cout << "Performing mission 1" << std::endl;
    drone.perform_mission(path, 0.5);

    sleep_for(seconds(10));

    pose = drone.get_position_NED();
    // fance set point movement
    path.clear();
    path = trajectories_with_goals::generate_helical_path(Vector3r(5, 5, -4), pose);

    std::cout << "Performing mission 2" << std::endl;
    drone.perform_mission(path, 0.5);

    sleep_for(seconds(10));

    pose = drone.get_position_NED();
    // fancy humming
    path.clear();
    path = trajectories_with_goals::generate_lemniscate_gerono_path(5, pose);

    std::cout << "Performing mission 3" << std::endl;
    drone.perform_mission(path, 0.5);

    sleep_for(seconds(10));

    drone.return_to_home();

    if(drone.landing())
      return 1;

    // We are relying on auto-disarming but let's keep watching the telemetry for a bit longer.
    sleep_for(seconds(3));
    std::cout << "Finished..." << std::endl;

    return 0;
}
