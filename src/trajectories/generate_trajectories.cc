#include <trajectories/generate_trajectories.h>

std::vector <Vector3r> trajectories::generate_straight_line(){
  double x = 0;
  std::vector <Vector3r> out_waypoints;
  Vector3r app_waypoint (0,0,0);
  while(x < 5.0){
    app_waypoint.set(x,0,0);
    out_waypoints.push_back(app_waypoint);
    x += 0.1;
  }
  return out_waypoints;
}
