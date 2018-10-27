#ifndef __Common_H_INCLUDED__
#define __Common_H_INCLUDED__

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <cstdint>
#include <math.h>

#include <beehaving_drones/CommonStruct.h>

//below are approximate versions and would produce errors of more than 10m for points farther than 1km
//for more accurate versions, please use the version in EarthUtils::nedToGeodetic
Vector3r GeodeticToNedFast(const GeoPoint& geo, const GeoPoint& home);

GeoPoint nedToGeodeticFast(const Vector3r& local, const GeoPoint& home);

#endif
