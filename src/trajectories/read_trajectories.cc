#include <trajectories/read_trajectories.h>
#include <fstream>
#include <sstream>

void trajectories::read_trajectory_from_txt(std::string in_path, std::vector <Vector3r> *out_waypoints){
  std::ifstream in_file(in_path.c_str());
  if(!in_file){
    std::cerr << "Error in opening file " << in_path << std::endl;
    return;
  }

  double x, y, z;
  std::string line;
  while (std::getline(in_file, line)){
    std::istringstream iss(line);
    if(!(iss >> x >> y >> z)){
      std::cerr << "Error in reading the file " << in_path
                << ": line not coherent." << std::endl;
      break;
    }
    out_waypoints->push_back(Vector3r(x, y, z));
  }
  in_file.close();
}
