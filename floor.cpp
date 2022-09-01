using namespace std;
#include "floor.h"
#include "ecs.h"

Floor::Floor(int fid, QObject *parent):QObject(parent)
{
    this->fid = fid;
    ecs = ECS::getECSInstance();
}

void Floor::inform(QString dir)
{
    qInfo()<<"floor id"<<this->getFloorId();
    ecs->floorRequest(getFloorId(), dir);

}

int Floor::getFloorId()
{
    return this->fid;
}

void Floor::serviced(QString direction, int i){
    //turn off floor button
    int floor = this->fid + 1;
    QString dir = direction;
    QTimer::singleShot(i*1000, [this, floor, dir](){emit turnOffFloorButton(floor, dir);});
}
