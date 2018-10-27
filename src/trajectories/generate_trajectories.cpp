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
  path.push_back(Vector3r(origin.x + length, origin.y + length, -5));
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
    path.push_back(Vector3r(origin.x + r1 * cos(t * rate), origin.y + r2 * sin(t * rate), origin.z - t * l));
    t = t + increment_rate;
  }

  return path;
}
