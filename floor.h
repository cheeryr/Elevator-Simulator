#ifndef FLOOR_H
#define FLOOR_H
#include <QString>
#include <QDebug>
#include <QObject>
#include <QTimer>

/* Class Purpose: Floor class implements the logic of floor in elevator system
 *
 * Data Members:
 * -int fid: floor id
 * -ECS* ecs: hold an instance of ECS.
 * Class Functions:
 * -Getters and Setters
 * -void inform(QString): inform ECS the floor request
 * -void serviced(QString, int): used by ECS to inform the floor that a floor request is serviced
 * -void turnOffFloorButton(int, QString): send signal to MainWIndow to turn off the floor button
 */

class ECS;

class Floor: public QObject
{
    Q_OBJECT
public:
    Floor(int fid, QObject *parent=nullptr);
    void inform(QString);
    int getFloorId();
    void serviced(QString, int);//direction, i

private:
    int fid;//fllor id
    ECS* ecs;
signals:
    void turnOffFloorButton(int, QString);//fid, dir



};

#endif // FLOOR_H
