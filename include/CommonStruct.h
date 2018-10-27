#ifndef __CommonStruct_H_INCLUDED__
#define __CommonStruct_H_INCLUDED__

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <cstdint>
#include <math.h>

using namespace std;

#ifndef M_PIf
#define M_PIf static_cast<float>(3.1415926535897932384626433832795028841972)
#endif

#ifndef M_PI
#define M_PI static_cast<double>(3.1415926535897932384626433832795028841972)
#endif

#ifndef M_PIl
#define M_PIl static_cast<long double>(3.1415926535897932384626433832795028841972)
#endif

#define EARTH_RADIUS (6378137.0f)


namespace utils
{
  double degreesToRadians(double degrees);
  float degreesToRadians(float degrees);
  double radiansToDegrees(double radians);
  float radiansToDegrees(float radians);
}

struct GeoPoint
{
  double latitude = 0, longitude = 0;
  float altitude = 0;

  GeoPoint()
  {}

  GeoPoint(double latitude_val, double longitude_val, float altitude_val)
  {
      set(latitude_val, longitude_val, altitude_val);
  }

  void set(double latitude_val, double longitude_val, float altitude_val)
  {
      latitude = latitude_val;
      longitude = longitude_val;
      altitude = altitude_val;
  }

  std::string to_string()
  {
      return std::to_string(latitude) + string(", ") + std::to_string(longitude) + string(", ") + std::to_string(altitude);
  }
};


struct Vector3r
{
  double x = 0, y = 0, z = 0;

  Vector3r()
  {}

  Vector3r(double x_val, double y_val, double z_val)
  {
      set(x_val, y_val, z_val);
  }

  void set(double x_val, double y_val, double z_val)
  {
      x = x_val; y = y_val; z = z_val;
  }

  std::string to_string()
  {
      return std::to_string(x) + string(", ") + std::to_string(y) + string(", ") + std::to_string(z);
  }
};

#endif
