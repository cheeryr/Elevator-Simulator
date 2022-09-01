#include "elevator.h"
#include "ecs.h"

#include "state.h"
#include "normalstate.h"
#include "firestate.h"
#include "poweroutstate.h"


QMutex Elevator::mutex;
int Elevator::obstacleCount = 0;

Elevator::Elevator(int eid, QObject *parent):QObject(parent), eid(eid), idle(true), stopState(true), currentFloor(1), existObstacle(false), holdDoorOpen(false), currentWeight(0)
{
    this->currentDirection = ECS::UP_DIRECTION;
    this->ecs = ECS::getECSInstance();
    this->closeDoorTimer = new QTimer();
    this->openDoorTimer = new QTimer();

    this->normalState = new NormalState(this);
    this->fireState = new FireState(this);
    this->powerOutState = new PowerOutState(this);

    this->currentState = normalState;
}
Elevator::~Elevator(){

    delete closeDoorTimer;
    delete openDoorTimer;
}

bool Elevator::isIdle()
{
    return this->idle;
}

void Elevator::setIdle(bool idleState)
{
    this->idle = idleState;
}

int Elevator::getEid(){return this->eid;}

int Elevator::getCurrentFloor()
{
    return this->currentFloor;
}

void Elevator::setCurrentFloor(int floor)
{
    mutex.lock();
    this->currentFloor = floor;
    mutex.unlock();
}

bool Elevator::getHoldDoorOpen(){return this->holdDoorOpen;}
void Elevator::setHoldDoorOpen(bool open){this->holdDoorOpen = open;}

void Elevator::move(QString dir)
{
    qInfo()<<"move elevator"<<eid;
    currentState->move(dir);
}

void Elevator::updateFloor(int floor){
    setCurrentFloor(floor);
    qInfo()<<"elevator#"<<this->eid<<"set current floor"<<currentFloor;
    emit displayFloor(floor, this->eid);
}


void Elevator::setStop(bool stop){

    this->stopState = stop;

}

bool Elevator::getStopState(){
    return this->stopState;
}

void Elevator::setDoorObstacle(){
    if(stopState){//if in stop mode
        existObstacle = true;
    }
}

void Elevator::removeObstacle(){
    existObstacle = false;
}

bool Elevator::getObstacle(){
    return this->existObstacle;
}

QString Elevator::getCurrentDirection(){return this->currentDirection;}

void Elevator::setCurrentDirection(QString dir){this->currentDirection = dir;}



void Elevator::setCurrentWeight(double weight){
    this->currentWeight = weight;
}
bool Elevator::getOverload(){return currentWeight > MAX_WEIGHT;}


void Elevator::handleDoorObstacle(){
    int elevatorId = this->eid;
    obstacleCount++;
    if(obstacleCount >= 3){
        QTimer::singleShot(2000, [this, elevatorId](){emit displayDoorObstacleMsg(elevatorId);});
        obstacleCount = 0;//set count back to 0
        return;
    }
    //open door
    QTimer::singleShot(1500, [this, elevatorId](){emit openDoor(elevatorId);});

    //close door after 4.5s
    QTimer::singleShot(4500, [this, elevatorId](){emit closeDoor(elevatorId);});



}

void Elevator::cancelCloseDoorTimer(){
    closeDoorTimer->stop();
}

void Elevator::cancelOpenDoorTimer(){
    openDoorTimer->stop();
}

void Elevator::setCloseDoorTimer(int time, int eid){
    int elevatorId = eid;
    closeDoorTimer = new QTimer();
    connect(closeDoorTimer, &QTimer::timeout, [this, elevatorId](){emit closeDoor(elevatorId);});
    closeDoorTimer->setSingleShot(true);
    closeDoorTimer->start(time);
}

void Elevator::setOpenDoorTimer(int time, int eid){
    int elevatorId = eid;
    openDoorTimer = new QTimer();
    connect(openDoorTimer, &QTimer::timeout, [this, elevatorId](){emit openDoor(elevatorId);});
    openDoorTimer->setSingleShot(true);
    openDoorTimer->start(time);
}



void Elevator::setFireState(){
    this->currentState = fireState;
}

void Elevator::setPowerOutState(){
    this->currentState = powerOutState;
}

void Elevator::setNormalState(){
    this->currentState = normalState;
}










