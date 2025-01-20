#ifndef GAME_H
#define GAME_H

class Sprite {
protected:
    double positionX;
    double positionY;
    double rotation;
    double size;
    int spriteType;
public:
    Sprite(double x, double y, double rotation, double size, int spriteType);
    double getDistanceFrom(double x, double y) const;
    double getSquaredDistanceFrom(double x, double y) const;
    std::tuple<double, double> getPosition() const;
};
class Player : public Sprite {
private:
    bool isPlayerCollidingWall(double playerX, double playerY);
public:
    Player(double x, double y, double rotation, double size);
    double getRotation() const;
    void move(double forwardAmount, double sidewaysAmount);
    //void moveSideways(double amount);
    void turn(double amount);
};



#endif // GAME_H