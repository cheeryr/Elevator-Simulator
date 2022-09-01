#ifndef POWEROUTSTATE_H
#define POWEROUTSTATE_H
#include "state.h"
#include <QTimer>
/* Class Purpose: PowerOUTState class inherits State Class, which is the state when there is a power outage. Implements State Pattern.
 *
 * Data Members:
 *
 * Class Functions:
 * -void move(QString):over write the move method in State class, implement the move logic during power outage
 * -void handlePowerOut(int): set a timer to send signal to MainWindow to perform fire emergency process(open door, display message, etc)
 */
class PowerOutState : public State
{
public:
    PowerOutState(Elevator*);
    void move(QString) override;
private:
    void handlePowerOut(int);//floor
};

#endif // POWEROUTSTATE_H
