#include<beehaving_drones/CommonStruct.h>

double utils::degreesToRadians(double degrees) {
    return static_cast<double>(M_PIl * degrees / 180.0);
}
float utils::degreesToRadians(float degrees) {
    return static_cast<float>(M_PI * degrees / 180.0f);
}
double utils::radiansToDegrees(double radians) {
    return static_cast<double>(radians * 180.0 / M_PIl);
}
float utils::radiansToDegrees(float radians) {
    return static_cast<float>(radians * 180.0f / M_PI);
}
