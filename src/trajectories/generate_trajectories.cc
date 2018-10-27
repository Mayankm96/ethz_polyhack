#include <trajectories/generate_trajectories.h>

void trajectories::generate_straight_line(std::vector <Vector3r> *out_waypoints){
  double x = 0;
  Vector3r app_waypoint (0,0,0);
  while(x < 5.0){
    app_waypoint.set(x,0,0);
    out_waypoints->push_back(app_waypoint);
    x += 0.1;
  }
}
