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

  // Epicycloid path:
  // a and b are constants of the parametric equations:
  // x(t) = (a+b)*cos(t)-b*cos((a/b+1)t);
  // y(t) = (a+b)*sin(t)-b*sin((a/b+1)t);
  std::vector<Vector3r> generate_epicycloidal_path(double a=3.0, double b=2.0, long int num_of_waypoints = 1000, Vector3r origin = Vector3r(0, 0, 0));

  // Epitrochoidal path:
  // a and b are constants of the parametric equations:
  // x(t) = (a+b)*cos(t)-c*cos((a/b+1)t);
  // y(t) = (a+b)*sin(t)-c*sin((a/b+1)t);
  std::vector<Vector3r> generate_epitrochoidal_path(double a=3.0, double b=2.0, double c=1.0, long int num_of_waypoints = 1000, Vector3r origin = Vector3r(0, 0, 0));

  // Lemniscate path:
  // a is a constant of the parametric equation (in polar coordinates):
  // r^2 = a^2*cos(2*theta)/cos^4(theta), where theta is the parameter
  std::vector<Vector3r> generate_lemniscate_gerono_path(double a=1.0, long int num_of_waypoints = 500, Vector3r origin = Vector3r(0, 0, 0));

  // Abs sine path:
  std::vector<Vector3r> generate_abs_sine_path(double amplitude=1.0, long int num_of_waypoints = 500, Vector3r origin = Vector3r(0, 0, 0));

  // Fermat path:
  // a is a constant of the parametric equation:
  // r^2 = a^2*theta
  std::vector<Vector3r> generate_fermat_path(double a=1.0, long int num_of_waypoints = 500, Vector3r origin = Vector3r(0, 0, 0));

  // Tornado path:
  // a is a constant of the parametric equation:
  // r^2 = a^2*theta
  std::vector<Vector3r> generate_tornado_path(double a=1.0, long int num_of_waypoints = 500, Vector3r origin = Vector3r(0, 0, 0));

  // Hypocycloid path:
  std::vector<Vector3r> generate_hypocycloid_path(double a=3.0, double b = 1.0, long int num_of_waypoints = 500, Vector3r origin = Vector3r(0, 0, 0));


  // witch of agnesi path:
  std::vector<Vector3r> generate_agnesi_path(double a=3.0, long int num_of_waypoints = 500, Vector3r origin = Vector3r(0, 0, 0));

};
