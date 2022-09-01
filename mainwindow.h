#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>
#include <QButtonGroup>


#include "floor.h"
#include "elevator.h"
#include "ecs.h"

/* Class Purpose: MainWindow class controls all the UI widgets on GUI, it implements Observer pattern
 * by default. Any action on UI can be received in MainWindow
 *
 */
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;


    QButtonGroup *upButtonGroup;
    QButtonGroup *downButtonGroup;
    QButtonGroup *e1_floorButtonGroup;
    QButtonGroup *e2_floorButtonGroup;

    ECS* ecs;



private slots:
    void informUp(int);
    void informDown(int);
    void displayFloor(int, int);
    void turnOffFloorButton(int, QString);
    void openDoor(int);
    void closeDoor(int);
    void sendCarRequestE1(int);
    void sendCarRequestE2(int);
    void turnOffCarFloorButton(int, int);//eid, floor(fid+1)
    void callHelp(int);//eid
    //void displayHelpMsg(int);//eid
    void doorObstacle(int);//eid
    void displayDoorObstacleMsg(int);//eid
    void openDoorButtonPressed(int);
    void closeDoorButtonPressed(int);
    void overLoad(int);//eid
    void fire();
    void powerOut();
    void handleFireEmergency(int);
    void handlePowerOutage(int);

};
#endif // MAINWINDOW_H
