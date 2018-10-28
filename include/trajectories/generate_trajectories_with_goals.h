#include <vector>
#include <beehaving_drones/CommonStruct.h>

namespace trajectories_with_goals
{
  // Straight line trajectory
  std::vector <Vector3r> generate_straight_line(Vector3r goal);

  // Square path
  std::vector<Vector3r> generate_square_path(Vector3r goal, Vector3r origin = Vector3r(0, 0, 0));

  // Helical path
  std::vector<Vector3r> generate_helical_path(Vector3r goal, Vector3r origin = Vector3r(0, 0, 0));
  // Lemniscate path:
  // a is a constant of the parametric equation (in polar coordinates):
  // r^2 = a^2*cos(2*theta)/cos^4(theta), where theta is the parameter
  std::vector<Vector3r> generate_lemniscate_gerono_path(int num, Vector3r origin = Vector3r(0, 0, 0));

  // Tornado path:
  // a is a constant of the parametric equation:
  // r^2 = a^2*theta
  std::vector<Vector3r> generate_tornado_path(Vector3r goal, Vector3r origin = Vector3r(0, 0, 0));

};
