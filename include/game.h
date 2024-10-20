#ifndef GAME_H
#define GAME_H

class Player {
private:
    double positionX;
    double positionY;
    double rotation;
    double size;
    //void pushOutOfWalls(double directionX, double directionY);
    bool isPlayerCollidingWall(double playerX, double playerY);
public:
    Player(double x, double y, double rotation, double size);
    std::tuple<double, double> getPosition();
    double getRotation();
    void move(double forwardAmount, double sidewaysAmount);
    //void moveSideways(double amount);
    void turn(double amount);
};



#endif // GAME_H