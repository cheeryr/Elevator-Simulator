#include "normalstate.h"
#include <QDebug>
#include <QTimer>


NormalState::NormalState(Elevator* ele):State(ele){}

void NormalState::move(QString direction){

    int floor = elevator->getCurrentFloor();
    int i = 1;
    int elevatorId = elevator->getEid();
    QString dir;
    elevator->setCurrentDirection(direction);
    elevator->setStop(false);
    qInfo()<<"move elevator"<<elevatorId;

    while(1){

        if(floor == 1){
            elevator->setCurrentDirection(ECS::UP_DIRECTION);
        }
        if(floor == ECS::NUMBER_OF_FLOOR){
            elevator->setCurrentDirection(ECS::DOWN_DIRECTION);
        }
        dir = elevator->getCurrentDirection();
        QTimer::singleShot(i*1000, [this, floor](){elevator->updateFloor(floor);});
        i++;
        ecs->newFloor(floor-1, elevatorId, dir, i);


        if(elevator->getStopState()){
            //trun off elevator floor button
            QTimer::singleShot(i*1000, [this, elevatorId, floor](){emit elevator->turnOffCarFloorButton(elevatorId, floor);});

            //open door
            //QTimer::singleShot(i*1000, [this, elevatorId](){emit openDoor(elevatorId);});
            elevator->setOpenDoorTimer(i*1000, elevatorId);

            //close the door after 10s
            //QTimer::singleShot(i*1000 + 5000, [this, elevatorId](){emit closeDoor(elevatorId);});
            elevator->setCloseDoorTimer(i*1000 + 10000, elevatorId);

            qInfo()<<"Current floor#"<<floor<<"STOP!!!!";
            break;
        }
        if(elevator->getCurrentDirection() == ECS::UP_DIRECTION){
            floor++;
        }else if(elevator->getCurrentDirection() == ECS::DOWN_DIRECTION){
            floor--;
        }
    }
}
