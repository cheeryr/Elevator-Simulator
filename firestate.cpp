#include "firestate.h"

FireState::FireState(Elevator* ele):State(ele){}

void FireState::move(QString direction){

    int eid = elevator->getEid();
    int floor = elevator->getCurrentFloor();
    int i = 1;
    qInfo()<<"fire state move eid#"<<eid<<"current floor"<<floor;
    if(elevator->getHoldDoorOpen()){
        elevator->setHoldDoorOpen(false);
    }
    if(elevator->getStopState()){
        elevator->setStop(false);
        if(!elevator->isIdle()){
            elevator->cancelOpenDoorTimer();
            elevator->cancelCloseDoorTimer();
        }


    }
    if(floor == ECS::SAFE_FLOOR){
        //qInfo()<<"eid#"<<eid<<"elevaot at floor#1";
        QTimer::singleShot(1000, [this, eid](){emit elevator->handleFireEmergency(eid);});
    }else{
        //elevator->setCloseDoorTimer(0, eid);
        while(1){
            //qInfo()<<"fire elevator#"<<eid<<" at floor#"<<floor;
            QTimer::singleShot(i*1000, [this, floor](){elevator->updateFloor(floor);});
            QTimer::singleShot(i*1000, [this, floor](){handleFire(floor);});
            if(floor == ECS::SAFE_FLOOR){
                break;
            }
            floor--;
            i++;
        }

    }
}

void FireState::handleFire(int floor){
    int eid = elevator->getEid();
    //qInfo()<<"handle file floor#"<<floor;
    //elevator->setCurrentFloor(floor);

    if(floor == ECS::SAFE_FLOOR){
        //elevator->setOpenDoorTimer(1000, eid);
        QTimer::singleShot(1000, [this, eid](){emit elevator->handleFireEmergency(eid);});
    }
}
