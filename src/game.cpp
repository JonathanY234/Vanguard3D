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
void Player::move(double forwardAmount, double sidewaysAmount) {
    // double hitbox radius
    
    // https://www.gamedev.net/tutorials/_/technical/game-programming/swept-aabb-collision-detection-and-response-r3084/
    
    // calculate the movement in X and Y directions based on the rotation
    double movementX = forwardAmount * cos(rotation) + sidewaysAmount * cos(rotation + M_PI_2);
    double movementY = forwardAmount * sin(rotation) + sidewaysAmount * sin(rotation + M_PI_2);

    // calculate the hypotenuse of the movement vector
    double length = sqrt(movementX * movementX + movementY * movementY);

    // normalize the movement vector
    if ((length > std::abs(forwardAmount)) && (forwardAmount != 0)) {
        positionX += (movementX / length) * std::abs(forwardAmount);
        positionY += (movementY / length) * std::abs(forwardAmount);
    } else {
        positionX += movementX;
        positionY += movementY;
    }
}
//void Player::moveSideways(double amount) {
//    positionX += amount * cos(rotation + M_PI_2);//M_PI_2 is PI/2
//    positionY += amount * sin(rotation + M_PI_2);
//}
void Player::turn(double amount) {
    rotation += amount;
    //probably dont need to normalise value between 0-2PI
}
