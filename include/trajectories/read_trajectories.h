#include <string>
#include <beehaving_drones/CommonStruct.h>

namespace trajectories{

// Read waypoints from the txt files at in_path
std::vector <Vector3r> read_trajectory_from_txt(std::string in_path);
};
