#include <vector>
#include <beehaving_drones/CommonStruct.h>

namespace trajectories
{
  // Read waypoints from the txt files at in_path
  std::vector <Vector3r> read_trajectory_from_txt(std::string in_path);

  // Straight line trajectory
  std::vector <Vector3r> generate_straight_line(Vector3r goal);

  // Square path
  std::vector<Vector3r> generate_square_path(double length, Vector3r origin = Vector3r(0, 0, 0));

  // Helical path
  std::vector<Vector3r> generate_helical_path(double rate, double r1, double r2, double l, long int num_of_waypoints = 1000, Vector3r origin = Vector3r(0, 0, 0));
};
