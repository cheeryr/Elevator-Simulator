#ifndef ELEVATOR_H
#define ELEVATOR_H
#include <QObject>
#include <QMutex>
#include <QTimer>

/* Class Purpose: Elevator class simulate the elevator behavior in elevator system
 * Data Members:
 * -int eid: elevator id
 * -bool idle: if elevator is idle
 * -int currentFloor: current floor the elevator located
 * -bool stopState: true if the elevator temprarily stops at a floor to serve request
 * -bool existObstacle: true if there exist door obstacle
 * -bool holdDoorOpen: true if the "Open Door" button is hold depressed
 * -double currentWeight: store the current cargo weight
 * -static int obstacleCount: count the time that elevator tries to open and close the door when there is door obstacle exists
 * -QTimer *openDoorTimer:default open door timer
 * -QTimer *closeDoorTimer: default close door timer(set to 10 seconds)
 * -static const int MAX_WEIGHT: The max weight the elevator can carry
 * -ECS* ecs: how an instance of ECS
 * -QString currentDirection: track the current moving direction of the elevator
 * -State *currentState: hold the current state
 * -State *normalState: hold a instance of NormalState
 * -State *fireState: hold a instance of FireState
 * -State *powerOutState: hold a instance of PowerOutState
 *
 *
 *
 * Class Functions:
 * -Getters and Setters
 * -void move(QString): call move method in state class
 * -void cancelCloseDoorTimer(): cancel default close door timer
 * -void cancelOpenDoorTimer(): cancel default open door timer
 * -void updateFloor(int)
 */

class ECS;
class State;


class Elevator : public QObject
{
    Q_OBJECT
public:
    Elevator(int, QObject *parent=nullptr);
    ~Elevator();
    static QMutex mutex;
    QTimer *openDoorTimer;
    QTimer *closeDoorTimer;
    static const int MAX_WEIGHT = 200;

    int getEid();
    int getCurrentFloor();
    bool getStopState();
    bool isIdle();
    void setIdle(bool);
    void move(QString);//direction
    void setStop(bool);
    void setDoorObstacle();
    bool getObstacle();
    void setCurrentWeight(double);
    bool getOverload();
    void handleDoorObstacle();
    void removeObstacle();
    bool getHoldDoorOpen();
    void setHoldDoorOpen(bool);
    void cancelCloseDoorTimer();
    void cancelOpenDoorTimer();
    void setCloseDoorTimer(int, int);//time, eid
    void setOpenDoorTimer(int, int);//time , eid
    QString getCurrentDirection();
    void setCurrentDirection(QString);//direction
    void updateFloor(int);//floor
    void setFireState();
    void setNormalState();
    void setPowerOutState();
    void setCurrentFloor(int);


private:
    int eid;
    bool idle;
    int currentFloor;
    bool stopState;
    bool existObstacle;
    bool holdDoorOpen;
    double currentWeight;
    static int obstacleCount;

    ECS* ecs;
    QString currentDirection;
    State *currentState;
    State *normalState;
    State *fireState;
    State *powerOutState;






signals:
    void displayFloor(int, int);
    void turnOffCarFloorButton(int, int);//eid, floor(fid+1)
    void openDoor(int);//eid
    void closeDoor(int);//eid
    void displayDoorObstacleMsg(int);//eid
    void handleFireEmergency(int);//eid
    void handlePowerOutage(int);//eid



};

#endif // ELEVATOR_H
