#ifndef LEVEL_H
#define LEVEL_H

// distance, wall face, pos within the wall, wall num
std::tuple<double, int, double, int> raycast(double x, double y, double angle);
bool isWall(int x, int y);

#endif