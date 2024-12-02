#include <tuple>
#include <cmath>
#include <memory>
#include "game.h"
#include "level.h"

//temp
#include <iostream>

Sprite::Sprite(double x, double y, double rotation, double size, int spriteType)
    : positionX(x), positionY(y), rotation(rotation), size(size), spriteType(spriteType) {}

double Sprite::getDistanceFrom(double x, double y) {
    return sqrt((x-positionX)*(x-positionX) + (y-positionY)*(y-positionY));
}
std::tuple<double, double> Sprite::getPosition() {
    return std::make_tuple(positionX, positionY);
}

Player::Player(double x, double y, double rotation, double size)
    : Sprite(x, y, rotation, size, 0) {}
    

double Player::getRotation() {
    return rotation;
}
void Player::move(double forwardAmount, double sidewaysAmount) {
    
    // calculate the movement in X and Y directions based on the rotation
    double movementX = forwardAmount * cos(rotation) + sidewaysAmount * cos(rotation + M_PI_2);
    double movementY = forwardAmount * sin(rotation) + sidewaysAmount * sin(rotation + M_PI_2);

    // proposed new position
    double newX;
    double newY;

    // calculate the hypotenuse of the movement vector
    double length = sqrt(movementX * movementX + movementY * movementY);

    // normalize the movement vector
    if ((length > std::abs(forwardAmount)) && (forwardAmount != 0)) {
        newX = positionX + (movementX / length) * std::abs(forwardAmount);
        newY = positionY + (movementY / length) * std::abs(forwardAmount);
    } else {
        newX = positionX + movementX;
        newY = positionY + movementY;
    }
    // correct for any collisions
    // yes this is bad it repeats code, need to improve
    if (std::abs(forwardAmount) > std::abs(sidewaysAmount)) {
        if (isPlayerCollidingWall(newX, positionY)) { // collision caused by X movement
            if (movementX > 0) {
                newX = std::floor(newX+size) - (size+0.0001);// +0.01 to avoid floating point inaccuracies
                
            } else {
                newX = std::ceil(newX-size) + (size+0.0001);
            }
            
        }
        if (isPlayerCollidingWall(positionX, newY)) { // collision caused by Y movement
            if (movementY > 0) {
                newY = std::floor(newY+size) - (size+0.0001);
            } else {
                newY = std::ceil(newY-size) + (size+0.0001);
            }
        }
    } else {
        if (isPlayerCollidingWall(positionX, newY)) { // collision caused by Y movement
            if (movementY > 0) {
                newY = std::floor(newY+size) - (size+0.001);
            } else {
                newY = std::ceil(newY-size) + (size+0.001);
            }
        }
        if (isPlayerCollidingWall(newX, positionY)) { // collision caused by X movement
            if (movementX > 0) {
                newX = std::floor(newX+size) - (size+0.001);// +0.01 to avoid floating point inaccuracies
                
            } else {
                newX = std::ceil(newX-size) + (size+0.001);
            }
        }
    }
    positionX = newX;
    positionY = newY;

    
}
bool Player::isPlayerCollidingWall(double playerX, double playerY) {
    // swastica pattern, check four corners
    return (isWall(playerX + size, playerY + size) || 
            isWall(playerX - size, playerY - size) || 
            isWall(playerX - size, playerY + size) || 
            isWall(playerX + size, playerY - size));
}

void Player::turn(double amount) {
    rotation += amount;
    //probably dont need to normalise value between 0-2PI
}
