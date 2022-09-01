#include <QDebug>
#include "ecs.h"

const QString ECS::UP_DIRECTION = "UP";
const QString ECS::DOWN_DIRECTION = "DOWN";
//singleton
Q_GLOBAL_STATIC(ECS, myECSInstance)

ECS::ECS(QObject *parent):QObject(parent), isFireEmergency(false), isPowerOut(false)
{
}



ECS::~ECS()
{
    qInfo("ECS destructor");
    for(int i = 0; i < elevatorList.size(); i++){
        delete elevatorList[i];
    }
    for(int i = 0; i < floorList.size(); i++){
        delete floorList[i];
    }
}
//implement Singleton pattern
ECS* ECS::getECSInstance(){

    return myECSInstance();
}

void ECS::addElevator(Elevator* elevator){

    elevatorList.append(elevator);
}
void ECS::addFloor(Floor* floor){
    floorList.append(floor);
}



Floor* ECS::getFloor(int fid)
{
    return this->floorList[fid];

}

Elevator* ECS::getElevator(int eid)
{
    return this->elevatorList[eid];
}

void ECS::floorRequest(int fid, QString dir)
{
    qInfo()<<"Floor#"<<fid<<" request dirction "<<dir;    
    addFloorRequest(fid, dir);
    if(!checkIdle()){
        qInfo("has idle");
        moveIdle(fid);
    }

}

void ECS::carRequest(int eid, int fid){
    addCarRequest(eid, fid);
    int curretFloor = elevatorList[eid]->getCurrentFloor()-1;
    if(elevatorList[eid]->isIdle()){
        elevatorList[eid]->setIdle(false);
        if(fid > curretFloor){
            qInfo()<<"elevator is idle!!!"<<eid;
            elevatorList[eid]->move(UP_DIRECTION);
        }else{
            elevatorList[eid]->move(DOWN_DIRECTION);
        }

    }
}

bool ECS::checkIdle()
{
    bool allBusy = true;
    for(int i = 0; i < NUMBER_OF_ELEVATOR; i++){
        if (elevatorList[i]->isIdle() && !elevatorList[i]->getObstacle() && !elevatorList[i]->getHoldDoorOpen() && !elevatorList[i]->getOverload()){
            allBusy = false;
        }
    }
    return allBusy;
}

void ECS::moveIdle(int fid){
    for(int i = 0; i < NUMBER_OF_ELEVATOR; i++){
        if (elevatorList[i]->isIdle() && !elevatorList[i]->getObstacle() && !elevatorList[i]->getHoldDoorOpen() && !elevatorList[i]->getOverload()){
            qInfo()<<"move idle"<<i;
            elevatorList[i]->setIdle(false);
            int curretFloor = elevatorList[i]->getCurrentFloor();
            if(fid > curretFloor){
                elevatorList[i]->move(UP_DIRECTION);
            }else{
                elevatorList[i]->move(DOWN_DIRECTION);
            }


            break;
        }
    }
}

void ECS::addFloorRequest(int fid, QString dir){
    //update floor request map

    if(floorRequestMap.contains(fid)){
        if(!floorRequestMap[fid].contains(dir)){
                //qInfo()<<"add floor request1"<<fid<<"dir"<<dir;
                floorRequestMap[fid].append(dir);
        }
    }else{
        //qInfo()<<"add floor request2"<<fid<<"dir"<<dir;
        QList<QString> dirList;
        dirList.append(dir);
        floorRequestMap[fid] = dirList;
    }
}

void ECS::addCarRequest(int eid, int fid){
    if(carRequestMap.contains(eid)){
        if(!carRequestMap[eid].contains(fid)){
                //qInfo()<<"add floor request1"<<fid<<"dir"<<dir;
                carRequestMap[eid].append(fid);
        }
    }else{
        //qInfo()<<"add floor request2"<<fid<<"dir"<<dir;
        QList<int> floorList;
        floorList.append(fid);
        carRequestMap[eid] = floorList;
    }
}

void ECS::newFloor(int fid, int eid, QString direction, int i){
    qInfo()<<"new floor#"<<fid<<"eid: "<<eid;
    if(makeDecision(eid, fid, direction, i)){
        elevatorList[eid]->setStop(true);
    }

}

bool ECS::makeDecision(int eid, int fid, QString direction, int i){

    bool isRequestingFloor = checkFloorRequest(eid, fid, direction);
    bool existsCarRequest = checkCarRequest(eid, fid);
    bool exist = isRequestingFloor || existsCarRequest;
    if(isRequestingFloor){//turn off floor button
        floorList[fid]->serviced(elevatorList[eid]->getCurrentDirection(), i);
    }
    qInfo()<<"Floor#"<<fid<<"Elevator#"<<eid<<" make decision:"<<exist<<"direction"<<direction;
    return exist;
}

bool ECS::checkFloorRequest(int eid, int fid, QString direction){
    bool exist = false;
    if(floorRequestMap.contains(fid)){
        if(floorRequestMap[fid].size()>0){
            exist = true;
            QString dir = floorRequestMap[fid].at(0);
            floorRequestMap[fid].removeAt(0);
            elevatorList[eid]->setCurrentDirection(dir);
        }

    }
    return exist;
}

bool ECS::checkCarRequest(int eid, int fid){
    bool exist = false;
    if(carRequestMap.contains(eid)){
        if(carRequestMap[eid].contains(fid)){
            qInfo()<<"car request exist!! eid:"<<eid<<"fid:"<<fid;
            exist = true;
            for(int i = 0; i < carRequestMap[eid].size(); i++){
                if(carRequestMap[eid].at(i) == fid){
                    carRequestMap[eid].removeAt(i);
                    break;
                }
            }
        }
    }
    return exist;
}

void ECS::readyToMove(int eid){
    bool hasRequest = false;
    bool isEmergency = isFireEmergency || isPowerOut;
    int currentFloor = elevatorList[eid]->getCurrentFloor() - 1;


    if(isEmergency){
        elevatorList[eid]->move(DOWN_DIRECTION);
        return;
    }

    if(carRequestMap[eid].size() != 0){//eid:[fid]
        hasRequest = true;
        elevatorList[eid]->move(carRequestDirection(eid, currentFloor));
    }

    if(!hasRequest){
        QMapIterator<int, QList<QString>> i(floorRequestMap);//fid:direction
        while(i.hasNext()){
            i.next();
            if(i.value().size() != 0){//has request wait
                if(i.key() > currentFloor){
                    qInfo()<<"have up floor request"<<i.key()<<"currentFLoor"<<currentFloor;
                    elevatorList[eid]->move(UP_DIRECTION);
                }else{
                    qInfo()<<"have down floor request"<<i.key()<<"currentFLoor"<<currentFloor;
                    elevatorList[eid]->move(DOWN_DIRECTION);
                }
                hasRequest = true;
                break;
            }
        }
    }

    if(!hasRequest){
        elevatorList[eid]->setIdle(true);
    }

}

QString ECS::carRequestDirection(int eid, int currentFloor){
    int index;
    int closest = INT_MAX;
    for (int i = 0; i < carRequestMap[eid].size(); i++){
        int diff = carRequestMap[eid][i]-currentFloor > 0? carRequestMap[eid][i]-currentFloor : currentFloor - carRequestMap[eid][i];
        if (diff < closest){
            closest = diff;
            index = i;
        }
    }
    if(carRequestMap[eid][index] > currentFloor){
        return UP_DIRECTION;
    }else{
        return DOWN_DIRECTION;
    }
}


void ECS::fireEmergency(bool emergency){
    this->isFireEmergency = emergency;
    if(isFireEmergency){
        for(int i = 0; i < elevatorList.size(); i++){
            elevatorList[i]->setFireState();
            elevatorList[i]->move(DOWN_DIRECTION);
        }
    }else{
        for(int i = 0; i < elevatorList.size(); i++){
            elevatorList[i]->setIdle(true);
            elevatorList[i]->setNormalState();
        }
    }
}

void ECS::powerOut(bool emergency){
    this->isPowerOut = emergency;
    if(isPowerOut){
        for(int i = 0; i < elevatorList.size(); i++){
            elevatorList[i]->setPowerOutState();
            elevatorList[i]->move(DOWN_DIRECTION);
        }
    }else{
        for(int i = 0; i < elevatorList.size(); i++){
            elevatorList[i]->setIdle(true);
            elevatorList[i]->setNormalState();
        }
    }
}





