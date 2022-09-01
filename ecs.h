#ifndef ECS_H
#define ECS_H
#include <QString>
#include <QList>
#include <QMutex>
#include <QMap>

#include "elevator.h"
#include "floor.h"

/* Class Purpose: ECS class serves as the elevator control system, which implements the Mediator Pattern
 * it sends commends to Floors and Elevators. It also implements Singleton Pattern to unsure there only exist one instance of the ECS class
 *
 * Data Members:
 * -QList<Elevator*> elevatorList: a QList holds all elevator instances
 * -QList<Floor*> floorList: a QList holds all floor instances
 * -QMap<int, QList<QString>> floorRequestMap: a QMap stores floor id with requested firection {fid:[direction]}
 * -QMap<int, QList<int>> carRequestMap: a QMap maps stores car# and its corresponding floor request {eid:[fid}}
 * -bool isFireEmergency: a boolean value, set true if there is fire emergency
 * -bool isPowerOut: a boolean value, set true if there is power outage
 *
 * Class Functions:
 * -Getters and Setters
 * -void floorRequest(int, QString): add floor request to floorRequestMap
 * -void carRequest(int, int): add car request to carRequestMap
 * -void addElevator(Elevator*): add elevator instance to elevatorList
 * -void addFloor(Floor*): add floor instance to floorList
 * -void newFloor(int, int, QString, int): notify ecs of new floor
 * -void readyToMove(int): indicate an elevator is ready to move, ecs decide if the elevator should keep moving or set to idle
 * -void fireEmergency(bool): set elevator to fire state or normal state depends on the boolean passed in.
 * -void powerOut(bool): set elevator to power out state or normal state depends on the boolean passed in.
 */

class Floor;
class Elevator;

class ECS: public QObject
{
    Q_OBJECT
public:
    ECS(QObject *parent=nullptr);
    ~ECS();
    ECS(ECS &other) = delete;//delete copy constructor
    void operator=(const ECS &) = delete;//not assignable

    static const QString UP_DIRECTION;
    static const QString DOWN_DIRECTION;
    static const int NUMBER_OF_FLOOR = 6;
    static const int NUMBER_OF_ELEVATOR = 2;
    static const int SAFE_FLOOR = 1;

    //static QMutex mutex;

    static ECS* getECSInstance();
    void floorRequest(int, QString);//fid, direction
    void carRequest(int, int);//eid, fid
    Elevator* getElevator(int);
    Floor* getFloor(int);
    void addElevator(Elevator*);
    void addFloor(Floor*);
    void newFloor(int, int, QString, int);//fid, eid, direction, i
    void readyToMove(int);//eid
    void fireEmergency(bool);//isFireEmergency
    void powerOut(bool);//isPowerOut


private:


    QList<Elevator*> elevatorList;
    QList<Floor*> floorList;
    QMap<int, QList<QString>> floorRequestMap;//fid:direction
    QMap<int, QList<int>> carRequestMap;//eid:fid
    bool isFireEmergency;
    bool isPowerOut;

    bool checkIdle();
    void moveIdle(int);//fid
    void addFloorRequest(int, QString);//fid, direction
    bool makeDecision(int, int, QString, int);//eid, fid, direction, i
    void addCarRequest(int, int);
    bool checkFloorRequest(int, int, QString);//eid, fid, direction
    bool checkCarRequest(int, int);//eid, fid
    QString carRequestDirection(int, int);//eid, currentFloor



};



#endif // ECS_H
