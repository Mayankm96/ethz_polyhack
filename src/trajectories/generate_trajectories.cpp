#include <trajectories/generate_trajectories.h>
#include <beehaving_drones/CommonStruct.h>
#include <fstream>
#include <cmath>
#include <sstream>
#include <cmath>

// Read waypoints from the txt files at in_path
std::vector <Vector3r> trajectories::read_trajectory_from_txt(std::string in_path)
{
  std::ifstream in_file(in_path.c_str());
  if(!in_file)
  {
    std::cerr << "Error in opening file " << in_path << std::endl;
    exit(EXIT_FAILURE);
  }

  std::vector <Vector3r> out_waypoints;
  double x, y, z;
  std::string line;

  while (std::getline(in_file, line))
  {
    std::istringstream iss(line);
    if(!(iss >> x >> y >> z)){
      std::cerr << "Error in reading the file " << in_path
                << ": line not coherent." << std::endl;
      break;
    }
    out_waypoints.push_back(Vector3r(x, y, - abs(z)));
  }
  in_file.close();

  return out_waypoints;
}

// Straight line trajectory
std::vector <Vector3r> trajectories::generate_straight_line(Vector3r goal)
{
  std::vector <Vector3r> out_waypoints;
  out_waypoints.push_back(goal);
  return out_waypoints;
}

// Square path
std::vector<Vector3r> trajectories::generate_square_path(double length, Vector3r origin)
{
  std::vector<Vector3r> path;
  // square
  path.push_back(origin);
  path.push_back(Vector3r(origin.x, origin.y + length, origin.z));
  path.push_back(Vector3r(origin.x + length, origin.y + length, origin.z));
  path.push_back(Vector3r(origin.x + length, origin.y, origin.z));
  path.push_back(Vector3r(origin.x, origin.y, origin.z));

  return path;
}

// Helical path
std::vector<Vector3r> trajectories::generate_helical_path(double rate, double r1, double r2, double l, long int num_of_waypoints, Vector3r origin)
{
  std::vector<Vector3r> path;
  double t = 0;
  double increment_rate = 1.0f/num_of_waypoints;

  while( t < 1)
  {
    // path.push_back(Vector3r(origin.x + r1 * cos(t * rate), origin.y + r2 * sin(t * rate), origin.z - t * l));
    path.push_back(Vector3r(origin.x + t * l, origin.y + r2 * sin(t * rate), origin.z - r1 * cos(t * rate)));
    t = t + increment_rate;
  }

  return path;
}

// Epicycloidal path
std::vector<Vector3r> trajectories::generate_epicycloidal_path(double a, double b, long int num_of_waypoints, Vector3r origin)
{
  std::vector<Vector3r> path;
  double t = 0;
  double increment_rate = 1.0f/num_of_waypoints;

  while( t < 10*M_PI)
  {
    path.push_back(Vector3r(origin.x + 0.1*(a+b)*cos(t)-b*cos((a/b+1)*t), origin.y,
                            origin.z - 0.1*(a+b)*sin(t)-b*sin((a/b+1)*t)));
    t = t + increment_rate;
  }

  return path;
}

// Epitrochoidal path
std::vector<Vector3r> trajectories::generate_epitrochoidal_path(double a, double b, double c, long int num_of_waypoints, Vector3r origin)
{
  std::vector<Vector3r> path;
  double t = 0;
  double increment_rate = 1.0f/num_of_waypoints;

  while( t < 2*M_PI   )
  {
    path.push_back(Vector3r(origin.x + 0.1*(a+b)*cos(t)-c*cos((a/b+1)*t),
                            origin.y + 0.1*(a+b)*sin(t)-c*sin((a/b+1)*t),
                            origin.z));
    t = t + increment_rate;
  }

  return path;
}

// Lemniscate path:
std::vector<Vector3r> trajectories::generate_lemniscate_gerono_path(double a, long int num_of_waypoints, Vector3r origin)
{
  std::vector<Vector3r> path;
  double t = 0;
  double increment_rate = 1.0f/num_of_waypoints;
  double r;

  while( t < 4*M_PI)
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

// Abs sine path:
std::vector<Vector3r> trajectories::generate_abs_sine_path(double amplitude, long int num_of_waypoints, Vector3r origin)
{
    std::vector<Vector3r> path;
    double t = 0;
    double increment_rate = 1.0f/num_of_waypoints;

    while( t < 10*M_PI )
    {
      path.push_back(Vector3r(origin.x + t,
                              origin.y,
                              origin.z - abs(amplitude*sin(t))));
      t = t + increment_rate;
    }

    return path;
}

// Fermat path:
std::vector<Vector3r> trajectories::generate_fermat_path(double a, long int num_of_waypoints, Vector3r origin)
{
  std::vector<Vector3r> path;
  double t = 0;
  double increment_rate = 1.0f/num_of_waypoints;
  double r;

  while( t < 10*M_PI )
  {
    r = sqrt(pow(a, 2)*t);
    path.push_back(Vector3r(origin.x +r*cos(t), origin.y + r*sin(t),
                            origin.z));
    t = t + increment_rate;
  }

  return path;
}

// Fermat path:
std::vector<Vector3r> trajectories::generate_tornado_path(double a, long int num_of_waypoints, Vector3r origin)
{
  std::vector<Vector3r> path;
  double t = 0;
  double increment_rate = 1.0f/num_of_waypoints;
  double r;

  while( t < 10*M_PI )
  {
    r = sqrt(pow(a, 2)*t);
    path.push_back(Vector3r(origin.x +r*cos(t), origin.y + r*sin(t),
                            origin.z - 0.15 * t));
    t = t + increment_rate;
  }

  return path;
}

// Hypocycloid path:
std::vector<Vector3r> trajectories::generate_hypocycloid_path(double a, double b, long int num_of_waypoints, Vector3r origin)
{
  std::vector<Vector3r> path;
  double t = 0;
  double increment_rate = 1.0f/num_of_waypoints;

  while( t < 10*M_PI )
  {
    double x = (a - b) * cos(t) + b * cos((a/b - 1) * t);
    double y = (a - b) * sin(t) - b * sin((a/b - 1) * t);

    path.push_back(Vector3r(origin.x + x, origin.y + y,
                            origin.z));
    t = t + increment_rate;
  }

  return path;
}

std::vector<Vector3r> trajectories::generate_agnesi_path(double a, long int num_of_waypoints, Vector3r origin)
{
  std::vector<Vector3r> path;
  double t = -2;
  double increment_rate = 1.0f/num_of_waypoints;

  while( t < 2 )
  {
    double x = a * t;
    double y = a / (1 + t * t);

    path.push_back(Vector3r(origin.x + x, origin.y,
                            origin.z - y));
    t = t + increment_rate;
  }

  return path;
}
