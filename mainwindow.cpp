using namespace std;
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtDebug>
#include <QTimer>
#include <QRegExpValidator>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //get ecs instance
    ecs = ECS::getECSInstance();

    //initialize elevator
    for(int i = 0; i < ECS::NUMBER_OF_ELEVATOR; i++){
        ecs->addElevator(new Elevator(i));
    }


    //initialize floor
    for(int i = 0; i < ECS::NUMBER_OF_FLOOR; i++){
        ecs->addFloor(new Floor(i));
    }

    //connect up button with slot
    upButtonGroup = new QButtonGroup(this);
    upButtonGroup->addButton(ui->f1_up, 1);
    upButtonGroup->addButton(ui->f2_up, 2);
    upButtonGroup->addButton(ui->f3_up, 3);
    upButtonGroup->addButton(ui->f4_up, 4);
    upButtonGroup->addButton(ui->f5_up, 5);
    upButtonGroup->addButton(ui->f6_up, 6);

    QSignalMapper* upFloorMapper = new QSignalMapper(this);

    for(QAbstractButton *btn:upButtonGroup->buttons())
    {
        connect(btn, SIGNAL(pressed()), upFloorMapper, SLOT(map()));
        upFloorMapper->setMapping(btn, upButtonGroup->id(btn));

    }
    connect(upFloorMapper, SIGNAL(mapped(int)), this, SLOT(informUp(int)));

    //connect down button with slot
    downButtonGroup = new QButtonGroup(this);
    downButtonGroup->addButton(ui->f1_down, 1);
    downButtonGroup->addButton(ui->f2_down, 2);
    downButtonGroup->addButton(ui->f3_down, 3);
    downButtonGroup->addButton(ui->f4_down, 4);
    downButtonGroup->addButton(ui->f5_down, 5);
    downButtonGroup->addButton(ui->f6_down, 6);
    QSignalMapper* downFloorMapper = new QSignalMapper(this);

    for(QAbstractButton *btn:downButtonGroup->buttons())
    {
        connect(btn, SIGNAL(pressed()), downFloorMapper, SLOT(map()));
        downFloorMapper->setMapping(btn, downButtonGroup->id(btn));
    }
    connect(downFloorMapper, SIGNAL(mapped(int)), this, SLOT(informDown(int)));

    //connect elevator#1 floor button with slot
    e1_floorButtonGroup = new QButtonGroup(this);
    e1_floorButtonGroup->addButton(ui->e1_f1, 1);
    e1_floorButtonGroup->addButton(ui->e1_f2, 2);
    e1_floorButtonGroup->addButton(ui->e1_f3, 3);
    e1_floorButtonGroup->addButton(ui->e1_f4, 4);
    e1_floorButtonGroup->addButton(ui->e1_f5, 5);
    e1_floorButtonGroup->addButton(ui->e1_f6, 6);
    QSignalMapper* e1_carFloorMapper = new QSignalMapper(this);
    for(QAbstractButton *btn:e1_floorButtonGroup->buttons())
    {
        connect(btn, SIGNAL(pressed()), e1_carFloorMapper, SLOT(map()));
        e1_carFloorMapper->setMapping(btn, e1_floorButtonGroup->id(btn));
    }
    connect(e1_carFloorMapper, SIGNAL(mapped(int)), this, SLOT(sendCarRequestE1(int)));

    //connect elevator#2 floor button with slot
    e2_floorButtonGroup = new QButtonGroup(this);
    e2_floorButtonGroup->addButton(ui->e2_f1, 1);
    e2_floorButtonGroup->addButton(ui->e2_f2, 2);
    e2_floorButtonGroup->addButton(ui->e2_f3, 3);
    e2_floorButtonGroup->addButton(ui->e2_f4, 4);
    e2_floorButtonGroup->addButton(ui->e2_f5, 5);
    e2_floorButtonGroup->addButton(ui->e2_f6, 6);
    QSignalMapper* e2_carFloorMapper = new QSignalMapper(this);
    for(QAbstractButton *btn:e2_floorButtonGroup->buttons())
    {
        connect(btn, SIGNAL(pressed()), e2_carFloorMapper, SLOT(map()));
        e2_carFloorMapper->setMapping(btn, e2_floorButtonGroup->id(btn));
    }
    connect(e2_carFloorMapper, SIGNAL(mapped(int)), this, SLOT(sendCarRequestE2(int)));


    //connect elevator signals with mainwindow slots
    for(int i = 0; i < ECS::NUMBER_OF_ELEVATOR; i++){
        connect(ecs->getElevator(i), SIGNAL(displayFloor(int, int)), this, SLOT(displayFloor(int, int)));
        connect(ecs->getElevator(i), SIGNAL(openDoor(int)), this, SLOT(openDoor(int)));
        connect(ecs->getElevator(i), SIGNAL(closeDoor(int)), this, SLOT(closeDoor(int)));
        connect(ecs->getElevator(i), SIGNAL(turnOffCarFloorButton(int, int)), this, SLOT(turnOffCarFloorButton(int, int)));
        connect(ecs->getElevator(i), SIGNAL(displayDoorObstacleMsg(int)), this, SLOT(displayDoorObstacleMsg(int)));
        connect(ecs->getElevator(i), SIGNAL(handleFireEmergency(int)), this, SLOT(handleFireEmergency(int)));
        connect(ecs->getElevator(i), SIGNAL(handlePowerOutage(int)), this, SLOT(handlePowerOutage(int)));
    }

    //connect floor signal with mainwindow slots

    for(int i = 0; i < ECS::NUMBER_OF_FLOOR; i++){
        connect(ecs->getFloor(i), SIGNAL(turnOffFloorButton(int,QString)), this, SLOT(turnOffFloorButton(int, QString)));
    }

    //connect help buttons with slot
    QSignalMapper* helpButtonMapper = new QSignalMapper(this);
    connect(ui->e1_help, SIGNAL(pressed()), helpButtonMapper, SLOT(map()));
    connect(ui->e2_help, SIGNAL(pressed()), helpButtonMapper, SLOT(map()));
    helpButtonMapper->setMapping(ui->e1_help, 0);
    helpButtonMapper->setMapping(ui->e2_help, 1);
    connect(helpButtonMapper, SIGNAL(mapped(int)), this, SLOT(callHelp(int)));
    //connect(ECS, SIGNAL(callHelp(int)), this, SLOT(displayHelpMsg(int)));

    //connect door obstacle buttons with slot
    QSignalMapper* obstacleButtonMapper = new QSignalMapper(this);
    connect(ui->e1_obstacle, SIGNAL(pressed()), obstacleButtonMapper, SLOT(map()));
    connect(ui->e2_obstacle, SIGNAL(pressed()), obstacleButtonMapper, SLOT(map()));
    obstacleButtonMapper->setMapping(ui->e1_obstacle, 0);
    obstacleButtonMapper->setMapping(ui->e2_obstacle, 1);
    connect(obstacleButtonMapper, SIGNAL(mapped(int)), this, SLOT(doorObstacle(int)));

    //connect open door button with slots
    QSignalMapper* openButtonMapper = new QSignalMapper(this);
    connect(ui->e1_open, SIGNAL(pressed()), openButtonMapper, SLOT(map()));
    connect(ui->e2_open, SIGNAL(pressed()), openButtonMapper, SLOT(map()));
    openButtonMapper->setMapping(ui->e1_open, 0);
    openButtonMapper->setMapping(ui->e2_open, 1);
    connect(openButtonMapper, SIGNAL(mapped(int)), this, SLOT(openDoorButtonPressed(int)));

    //connect close door button with slots
    QSignalMapper* closeButtonMapper = new QSignalMapper(this);
    connect(ui->e1_close, SIGNAL(pressed()), closeButtonMapper, SLOT(map()));
    connect(ui->e2_close, SIGNAL(pressed()), closeButtonMapper, SLOT(map()));
    closeButtonMapper->setMapping(ui->e1_close, 0);
    closeButtonMapper->setMapping(ui->e2_close, 1);
    connect(closeButtonMapper, SIGNAL(mapped(int)), this, SLOT(closeDoorButtonPressed(int)));




    //set validator for weight line edit to only take numbers
    ui->e1_weight->setValidator(new QRegExpValidator(QRegExp("[0-9]*"), ui->e1_weight));
    ui->e2_weight->setValidator(new QRegExpValidator(QRegExp("[0-9]*"), ui->e2_weight));
    //connect weight lineEdit with slots
    //read weight when editing finished or return pressed
    QSignalMapper* weightEditMapper = new QSignalMapper(this);
    connect(ui->e1_weight, SIGNAL(editingFinished()), weightEditMapper, SLOT(map()));
    connect(ui->e2_weight, SIGNAL(editingFinished()), weightEditMapper, SLOT(map()));
    weightEditMapper->setMapping(ui->e1_weight, 0);
    weightEditMapper->setMapping(ui->e2_weight, 1);
    connect(weightEditMapper, SIGNAL(mapped(int)), this, SLOT(overLoad(int)));


    //connect fire button with slot
    connect(ui->fire_button, SIGNAL(pressed()), this, SLOT(fire()));
    //connect power out button with slot
    connect(ui->powerout_button, SIGNAL(pressed()), this, SLOT(powerOut()));

}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::informUp(int fn)
{

    qInfo("up floor#%d", fn);

    //change button color
    QPalette p = upButtonGroup->button(fn)->palette();
    p.setColor(QPalette::Button, "#74b323");//set color green
    upButtonGroup->button(fn)->setPalette(p);
    ecs->getFloor(fn-1)->inform(ECS::UP_DIRECTION);
    //QColor color = upButtonGroup->button(fn)->palette().window().color().name();
    //qInfo()<<color.name();//default color: #efefef
    //p.setColor(QPalette::Button, "#efefef");//set button to default color

}
void MainWindow::informDown(int fn)
{

    qInfo("down floor#%d", fn);
    //change button color
    QPalette p = downButtonGroup->button(fn)->palette();
    p.setColor(QPalette::Button, "#74b323");
    downButtonGroup->button(fn)->setPalette(p);
    ecs->getFloor(fn-1)->inform(ECS::DOWN_DIRECTION);


}

void MainWindow::sendCarRequestE1(int fid){
    qInfo("E1 CAR REQUEST floor#%d", fid);
    //change button color
    QPalette p = e1_floorButtonGroup->button(fid)->palette();
    p.setColor(QPalette::Button, "#74b323");
    e1_floorButtonGroup->button(fid)->setPalette(p);
    ecs->carRequest(0, fid-1);
}

void MainWindow::sendCarRequestE2(int fid){
    qInfo("E1 CAR REQUEST floor#%d", fid);
    //change button color
    QPalette p = e2_floorButtonGroup->button(fid)->palette();
    p.setColor(QPalette::Button, "#74b323");
    e2_floorButtonGroup->button(fid)->setPalette(p);
    ecs->carRequest(1, fid-1);
}


void MainWindow::displayFloor(int floor, int eid){

    //qInfo()<<"IN mainwindow display floor: eid#"<<eid;
    if(eid == 0){
        ui->e1_display->setText("Current floor#"+ QString::number(floor) );//
    }else if(eid == 1){
        ui->e2_display->setText("Current floor#"+ QString::number(floor) );//
    }

}

void MainWindow::turnOffFloorButton(int floor, QString direction){
    QPalette p;
    qInfo()<<"turen off floor button fid#"<<floor<<"direction: "<<direction;

    //set button to default color
    if(direction == ECS::UP_DIRECTION){
        p = upButtonGroup->button(floor)->palette();
        p.setColor(QPalette::Button, "#efefef");
        upButtonGroup->button(floor)->setPalette(p);

    }else{//down
        p = downButtonGroup->button(floor)->palette();
        p.setColor(QPalette::Button, "#efefef");
        downButtonGroup->button(floor)->setPalette(p);
    }

}

void MainWindow::openDoor(int eid)
{
    switch(eid){
    case 0:
        ui->e1_display->setText("Ding");
        ui->e1_display->append("OPEN DOOR");
        break;
    case 1:
        ui->e2_display->setText("Ding");
        ui->e2_display->append("OPEN DOOR");
        break;
    default:
        break;
    }
}

void MainWindow::closeDoor(int eid)
{
    //if open door button is not hold, try close door
    if(!ecs->getElevator(eid)->getHoldDoorOpen()){
        switch(eid){
        case 0:
            ui->e1_display->setText("Ding");
            ui->e1_display->append("CLOSE DOOR");
            break;
        case 1:
            ui->e2_display->setText("Ding");
            ui->e2_display->append("CLOSE DOOR");
            break;
        default:
            break;
        }
        //if not overload
        if(!ecs->getElevator(eid)->getOverload()){
            //handle door obstacle
            if(!ecs->getElevator(eid)->getObstacle()){//if don't exist door obstacle
                ecs->readyToMove(eid);
            }else{//if exist door obstacle
                qInfo("close door detect door obstacle");
                ecs->getElevator(eid)->handleDoorObstacle();
            }
        }else{
            qInfo("close door detect overload");
        }

    }

}

void MainWindow::openDoorButtonPressed(int eid){
    QPushButton *button;
    switch(eid){
    case 0:
        button = ui->e1_open;
        break;
    case 1:
        button = ui->e2_open;
        break;
    default:
        break;
    }
    if(!button->isChecked()){//open door pressed
        ecs->getElevator(eid)->setHoldDoorOpen(true);
        if(ecs->getElevator(eid)->isIdle() || ecs->getElevator(eid)->getStopState()){
            openDoor(eid);
        }
    }else{
        //open door released
        ecs->getElevator(eid)->setHoldDoorOpen(false);
        closeDoor(eid);
    }

}

void MainWindow::closeDoorButtonPressed(int eid){
    if(!ecs->getElevator(eid)->isIdle()){
       ecs->getElevator(eid)->cancelCloseDoorTimer();
    }
    closeDoor(eid);

}



void MainWindow::turnOffCarFloorButton(int eid, int floor){
    QPalette p;
    qInfo()<<"turn off car floor button. eid:"<<eid<<"floor"<<floor;
    switch(eid){
    case 0:
        p = e1_floorButtonGroup->button(floor)->palette();
        p.setColor(QPalette::Button, "#efefef");
        e1_floorButtonGroup->button(floor)->setPalette(p);
        break;
    case 1:
        p = e2_floorButtonGroup->button(floor)->palette();
        p.setColor(QPalette::Button, "#efefef");
        e2_floorButtonGroup->button(floor)->setPalette(p);
        break;
    default:
        break;
    }
}

void MainWindow::callHelp(int eid){
    switch(eid){
    case 0:
        ui->e1_display->setText("Calling Building Security...");
        QTimer::singleShot(5000, [this](){ui->e1_display->setText("Calling 911...");});
        break;
    case 1:
        ui->e2_display->setText("Calling Building Security...");
        QTimer::singleShot(5000, [this](){ui->e2_display->setText("Calling 911...");});
        break;
    default:
        break;
    }
}

void MainWindow::doorObstacle(int eid){
    switch(eid){
    case 0:
        if(!ui->e1_obstacle->isChecked()){//obstacle button pressed
            qInfo()<<"detect door obstacle"<<eid;
            if(ecs->getElevator(eid)->getStopState()){//if the elevator is in stop mode, display warning message
                ecs->getElevator(eid)->setDoorObstacle();
            }
        }else{
            ecs->getElevator(eid)->removeObstacle();
            closeDoor(eid);

        }
        break;
    case 1:
        if(!ui->e2_obstacle->isChecked()){//obstacle button pressed
            qInfo()<<"detect door obstacle"<<eid;
            if(ecs->getElevator(eid)->getStopState()){//if the elevator is in stop mode, display warning message
                ecs->getElevator(eid)->setDoorObstacle();
            }
        }else{
            ecs->getElevator(eid)->removeObstacle();
            closeDoor(eid);
        }
        break;
    default:
        break;
    }


}

void MainWindow::displayDoorObstacleMsg(int eid){
    switch(eid){
    case 0:
        ui->e1_display->setText("WARNING: Please remove door obstacle");
        break;
    case 1:
        ui->e2_display->setText("WARNING: Please remove door obstacle");
        break;
    default:
        break;
    }
}


void MainWindow::overLoad(int eid){
    double weight;
    switch(eid){
    case 0:
        weight = ui->e1_weight->text().toDouble();
        ecs->getElevator(eid)->setCurrentWeight(weight);

        if(ecs->getElevator(eid)->getOverload()){
            ui->e1_display->setText("WARNING: Elevator overload. Please reduce load.");
            //cancel close door timer
            if(ecs->getElevator(eid)->getStopState() && !ecs->getElevator(eid)->isIdle()){
                ecs->getElevator(eid)->cancelCloseDoorTimer();
            }
        }else{
            ui->e1_display->setText("");
            //reset close door timer
            if(ecs->getElevator(eid)->getStopState() && !ecs->getElevator(eid)->isIdle()){
                ecs->getElevator(eid)->setCloseDoorTimer(1000, eid);
            }
        }
        break;
    case 1:
        weight = ui->e2_weight->text().toDouble();
        ecs->getElevator(eid)->setCurrentWeight(weight);

        if(ecs->getElevator(eid)->getOverload()){
            ui->e2_display->setText("WARNING: Elevator overload. Please reduce load.");
            if(ecs->getElevator(eid)->getStopState()  && !ecs->getElevator(eid)->isIdle()){
                //cancel close door timer
                ecs->getElevator(eid)->cancelCloseDoorTimer();
            }
        }else{
            ui->e2_display->setText("");
            //if elevaot is in stop mode, reset close door timer
            if(ecs->getElevator(eid)->getStopState() && !ecs->getElevator(eid)->isIdle()){
                ecs->getElevator(eid)->setCloseDoorTimer(1000, eid);
            }

        }
        break;
    default:
        break;
    }
}

void MainWindow::fire(){
    if(!ui->fire_button->isChecked()){//fire button pressed
        ui->e1_display->setText("FIRE EMERGENCY!!!  Moving to the safe floor...");
        ui->e1_display->append("Please disembark once the safe floor is reached");
        ui->e2_display->setText("FIRE EMERGENCY!!!  Moving to the safe floor...");
        ui->e2_display->append("Please disembark once the safe floor is reached.");
        qInfo("Fire button pressed");
        ecs->fireEmergency(true);
    }else{
        //fire button released
        ecs->fireEmergency(false);
        ui->e1_display->setText("");
        ui->e2_display->setText("");

    }

}

void MainWindow::powerOut(){
    if(!ui->powerout_button->isChecked()){
        ui->e1_display->setText("POWER OUTAGE!!!  Moving to the safe floor...");
        ui->e1_display->append("Please disembark once the safe floor is reached");
        ui->e2_display->setText("POWER OUTAGE!!!  Moving to the safe floor...");
        ui->e2_display->append("Please disembark once the safe floor is reached.");
        qInfo("Power Out button pressed");
        ecs->powerOut(true);
    }else{
        ecs->powerOut(false);
        ui->e1_display->setText("");
        ui->e2_display->setText("");
    }

}


void MainWindow::handleFireEmergency(int eid){
    displayFloor(1, eid);
    switch(eid){
    case 0:
        ui->e1_display->append("OPEN DOOR");
        ui->e1_display->append("FIRE EMERGENCY!!!");
        ui->e1_display->append("Arrive at the safe floor. Please disembark from the elevator.");
        break;
    case 1:
        ui->e2_display->append("OPEN DOOR");
        ui->e2_display->append("FIRE EMERGENCY!!!");
        ui->e2_display->append("Arrive at the safe floor. Please disembark from the elevator.");
        break;
    default:
        break;
    }
}

void MainWindow::handlePowerOutage(int eid){
    displayFloor(1, eid);
    switch(eid){
    case 0:
        ui->e1_display->append("OPEN DOOR");
        ui->e1_display->append("POWER OUTAGE!!!");
        ui->e1_display->append("Arrive at the safe floor. Please disembark from the elevator.");
        break;
    case 1:
        ui->e2_display->append("OPEN DOOR");
        ui->e2_display->append("POWER OUTAGE!!!");
        ui->e2_display->append("Arrive at the safe floor. Please disembark from the elevator.");
        break;
    default:
        break;
    }
}



