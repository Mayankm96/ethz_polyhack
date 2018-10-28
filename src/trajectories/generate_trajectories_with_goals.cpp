#include <trajectories/generate_trajectories_with_goals.h>
#include <beehaving_drones/CommonStruct.h>
#include <fstream>
#include <cmath>
#include <sstream>
#include <cmath>

// Square path
std::vector<Vector3r> trajectories_with_goals::generate_square_path(Vector3r goal, Vector3r origin)
{
  std::vector<Vector3r> path;
  // square
  path.push_back(origin);
  path.push_back(Vector3r(origin.x, origin.y + goal.y, origin.z));
  path.push_back(Vector3r(origin.x + goal.x, origin.y + goal.y, origin.z));
  path.push_back(Vector3r(origin.x + goal.x, origin.y + goal.y, origin.z + goal.z));

  return path;
}

// Helical path
std::vector<Vector3r> trajectories_with_goals::generate_helical_path(Vector3r goal, Vector3r origin)
{
  double r1 = 2, r2 = 2, l = 5;
  long int num_of_waypoints = 200;
  double rate = 10;

  std::vector<Vector3r> path;
  double t = 0;
  double increment_rate = 1.0f/num_of_waypoints;

  while( t < 5 || (origin.x + t * l) < goal.x)
  {
    // path.push_back(Vector3r(origin.x + r1 * cos(t * rate), origin.y + r2 * sin(t * rate), origin.z - t * l));
    path.push_back(Vector3r(origin.x + t * l, origin.y + r2 * sin(t * rate), origin.z - r1 * cos(t * rate)));
    t = t + increment_rate;
  }

  path.push_back(Vector3r(origin.x + goal.x, origin.y + goal.y, origin.z + goal.z));

  return path;
}

// Lemniscate path:
std::vector<Vector3r> trajectories_with_goals::generate_lemniscate_gerono_path(int num, Vector3r origin)
{
  double a = 2.5;
  long int num_of_waypoints = 10;

  std::vector<Vector3r> path;
  double t = 0;
  double increment_rate = 1.0f/num_of_waypoints;
  double r;

  origin.z = origin.z - 2;

  while( t < 2 * num *M_PI)
  {
    r = a;
    // path.push_back(Vector3r(origin.x + r*cos(t), origin.y,
    //                         origin.z - r*sin(t)));
    path.push_back(Vector3r(origin.x + r*cos(t), origin.y,
                            origin.z - r*sin(2*t)/2));
    t = t + increment_rate;
  }

  return path;
}


// Tornado path:
std::vector<Vector3r> trajectories_with_goals::generate_tornado_path(Vector3r goal, Vector3r origin)
{
  double a = 0.75;
  long int num_of_waypoints = 5;

  std::vector<Vector3r> path;
  double t = 0;
  double increment_rate = 1.0f/num_of_waypoints;
  double r;

  while( t < 10*M_PI || (origin.z - 0.15 * t) > goal.z)
  {
    r = sqrt(pow(a, 2)*t);
    path.push_back(Vector3r(origin.x +r*cos(t), origin.y + r*sin(t),
                            origin.z - 0.15 * t));
    t = t + increment_rate;
  }

  return path;
}
