#include <tuple>
#include <cmath>
#include <memory>
#include "game.h"
#include "level.h"

//temp
#include <iostream>

Player::Player(double x, double y, double angle) {
    positionX = x;
    positionY = y;
    rotation = angle;
}
std::tuple<double, double> Player::getPosition() {
    return std::make_tuple(positionX, positionY);
}
double Player::getRotation() {
    return rotation;
}
void Player::moveForward(double amount) {
    // double hitbox radius
    
    //int attemptedMoveX = amount * cos(rotation);
    //int attemptedMoveY = amount * sin(rotation);
    positionX += amount * cos(rotation);
    positionY += amount * sin(rotation);

    //raycast in that direction and if the distance is < amount then reduce amount
}
void Player::moveSideways(double amount) {
    positionX += amount * cos(rotation + M_PI_2);//M_PI_2 is PI/2
    positionY += amount * sin(rotation + M_PI_2);
}
void Player::turn(double amount) {
    rotation += amount;
    //probably dont need to normalise value between 0-2PI
}
