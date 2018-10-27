#include <beehaving_drones/BaseDrone.h>
#include <beehaving_drones/Console.h>
#include <dronecore/mission.h>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <trajectories/generate_trajectories.h>
#include <trajectories/read_trajectories.h>
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


    std::vector <Vector3r> waypoints;
    std::string txt_file = "/home/suriro/ethz_polyhack/trajectories_xy_heart.txt";
    std::cout << "Extracting trajectory from file " << txt_file << std::endl;
    trajectories::read_trajectory_from_txt(txt_file, &waypoints);

    std::cout << "Printing waypoints files:" << std::endl;

    Vector3r app_waypoint;
    for(unsigned int i = 0; i < waypoints.size(); ++i){
      app_waypoint = waypoints[i];
      std::cout << app_waypoint.x << " "
                << app_waypoint.y << " "
                << app_waypoint.z << std::endl;
    }


    trajectories::generate_straight_line(&waypoints);
    /*std::vector<Vector3r> waypoints;

    // square
    waypoints.push_back(Vector3r(0, 0, -5));
    waypoints.push_back(Vector3r(0, 10, -5));
    waypoints.push_back(Vector3r(10, 10, -5));
    waypoints.push_back(Vector3r(10, 0, -5));*/
    std::cout << "Performing straight line mission" << std::endl;
    drone.perform_mission(waypoints);

    sleep_for(seconds(10));

    drone.return_to_home();

    if(drone.landing())
      return 1;

    // We are relying on auto-disarming but let's keep watching the telemetry for a bit longer.
    sleep_for(seconds(3));
    std::cout << "Finished..." << std::endl;

    return 0;
}
