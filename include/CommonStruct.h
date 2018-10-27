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
  double degreesToRadians(double degrees) {
      return static_cast<double>(M_PIl * degrees / 180.0);
  }
  float degreesToRadians(float degrees) {
      return static_cast<float>(M_PI * degrees / 180.0f);
  }
  double radiansToDegrees(double radians) {
      return static_cast<double>(radians * 180.0 / M_PIl);
  }
  float radiansToDegrees(float radians) {
      return static_cast<float>(radians * 180.0f / M_PI);
  }
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

//below are approximate versions and would produce errors of more than 10m for points farther than 1km
//for more accurate versions, please use the version in EarthUtils::nedToGeodetic
Vector3r GeodeticToNedFast(const GeoPoint& geo, const GeoPoint& home);

GeoPoint nedToGeodeticFast(const Vector3r& local, const GeoPoint& home);

#endif
