#ifndef STATE_H
#define STATE_H

#include "elevator.h"
#include "ecs.h"
/* Class Purpose: The parent class of NormalState, FireStae and PowerOutState. Implements State Pattern.
 *
 * Data Members:
 * -Elevator* elevator: hold an instance of elevator;
 * -ECS* ecs: hold an instance of ECS.
 *
 * Class Functions:
 * -void move(QString): abstract method, will be implement in child class.
 *
 */

class State
{
public:
    State(Elevator*);
    virtual void move(QString) = 0;
protected:
    Elevator* elevator;
    ECS* ecs;
};

#endif // STATE_H
