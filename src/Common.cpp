#include <CommonStruct.h>
#include <Common.h>

//below are approximate versions and would produce errors of more than 10m for points farther than 1km
//for more accurate versions, please use the version in EarthUtils::nedToGeodetic
Vector3r GeodeticToNedFast(const GeoPoint& geo, const GeoPoint& home)
{
    double d_lat = geo.latitude - home.latitude;
    double d_lon = geo.longitude - home.longitude;
    double d_alt = static_cast<double>(home.altitude - geo.altitude);
    double x = static_cast<double>(utils::degreesToRadians(d_lat) * EARTH_RADIUS);
    double y = static_cast<double>(utils::degreesToRadians(d_lon) * EARTH_RADIUS * cos(utils::degreesToRadians(geo.latitude)));
    return Vector3r(x, y, d_alt);
}

GeoPoint nedToGeodeticFast(const Vector3r& local, const GeoPoint& home)
{
    GeoPoint r;
    double d_lat = local.x / EARTH_RADIUS;
    double d_lon = local.y / (EARTH_RADIUS * cos(utils::degreesToRadians(home.latitude)));
    r.latitude = home.latitude + utils::radiansToDegrees(d_lat);
    r.longitude = home.longitude + utils::radiansToDegrees(d_lon);
    r.altitude = home.altitude - local.z;

    return r;
}
