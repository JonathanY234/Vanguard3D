#ifndef GAME_H
#define GAME_H

class Sprite {
protected:
    double positionX;
    double positionY;
    double rotation;
    double size;
public:
    Sprite(double x, double y, double rotation, double size);
};
class Player : public Sprite {
private:
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