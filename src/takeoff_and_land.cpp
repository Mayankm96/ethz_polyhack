#include <beehaving_drones/DogDrone.h>
#include <beehaving_drones/BaseDrone.h>
#include <trajectories/generate_trajectories.h>

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

    DogDrone drone(connection_url);

    if(drone.start_connection(0))
      return 1;

    GeoPoint home = drone.get_home_geopoint();
    std::cout << "Home:" << home.to_string() << std::endl;

    if(drone.takeoff())
      return 1;

    // Let it hover for a bit before landing again.
    sleep_for(seconds(10));

    if(drone.landing())
      return 1;

    // We are relying on auto-disarming but let's keep watching the telemetry for a bit longer.
    sleep_for(seconds(3));
    std::cout << "Finished..." << std::endl;

    std::vector <Vector3r> test_waypoints;
    trajectories::generate_straight_line();

    Vector3r app_waypoint;

    std::cout << "Printing straight line waypoints:" << std::endl;

    for(unsigned int i = 0; i < test_waypoints.size(); ++i){
      app_waypoint = test_waypoints[i];
      std::cout << app_waypoint.x << " "
                << app_waypoint.y << " "
                << app_waypoint.z << std::endl;
    }



    return 0;
}
