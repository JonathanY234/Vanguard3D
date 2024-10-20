#ifndef LEVEL_H
#define LEVEL_H

std::tuple<double, int> raycast(double x, double y, double angle);
bool isWall(int x, int y);

#endif