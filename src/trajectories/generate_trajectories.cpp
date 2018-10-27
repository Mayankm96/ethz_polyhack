#include <trajectories/generate_trajectories.h>
#include <fstream>
#include <sstream>

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
    out_waypoints.push_back(Vector3r(x, y, z));
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
std::vector<Vector3r> generate_epicycloidal_path(double a, double b, long int num_of_waypoints = 1000, Vector3r origin = Vector3r(0, 0, 0))
{
  std::vector<Vector3r> path;
  double t = 0;
  double increment_rate = 1.0f/num_of_waypoints;

  while( t < 1)
  {
    path.push_back(Vector3r(origin.x + (a+b)*cos(t)-b*cos((a/b+1)*t), origin.y,
                            origin.z + (a+b)*sin(t)-b*sin((a/b+1)*t)));
    t = t + increment_rate;
  }

  return path;
}
