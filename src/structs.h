/**
 * @file structs.cc
 * @author Garrett
 * @brief Useful structs
 */

#ifndef STRUCT_GUARD
#define STRUCT_GUARD

using namespace std;

struct RectPoint {
    double x = 0;
    double y = 0;
};

struct PolarPoint {
    double r = 0;
    double theta = 0;
};

struct Pixel {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
};

struct PixelA {
    uint8_t rgb[3] = {0, 0, 0};
};

#endif