#ifndef GAME_H
#define GAME_H

class Player {
private:
    double positionX;
    double positionY;
    double rotation;
public:
    Player(double x, double y, double angle);
    std::tuple<double, double> getPosition();
    double getRotation();
    void move(double forwardAmount, double sidewaysAmount);
    //void moveSideways(double amount);
    void turn(double amount);
};



#endif // GAME_H