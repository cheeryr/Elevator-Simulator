#ifndef FIRESTATE_H
#define FIRESTATE_H
#include "state.h"
#include <QTimer>
/* Class Purpose: FireState class inherits State Class, which is the state when there is a fire emergency. Implements State Pattern.
 *
 * Data Members:
 *
 * Class Functions:
 * -void move(QString):over write the move method in State class, implement the move logic during fire emergency
 * -void handleFire(int): set a timer to send signal to MainWindow to perform fire emergency process(open door, display message, etc)
 */
class FireState : public State
{
public:
    FireState(Elevator*);
    void move(QString) override;
private:
    void handleFire(int);//floor
};

#endif // FIRESTATE_H
