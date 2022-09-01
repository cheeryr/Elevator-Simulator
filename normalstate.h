#ifndef NORMALSTATE_H
#define NORMALSTATE_H

#include "state.h"

/* Class Purpose: NormalState class inherits State Class, which represent the elevator operate under normal situation.
 * Implements State Pattern.
 *
 * Data Members:
 *
 * Class Functions:
 * -void move(QString):over write the move method in State class, implement the move logic during normal operation
 *
 */
class NormalState : public State
{
public:
    NormalState(Elevator*);
    void move(QString) override;

};

#endif // NORMALSTATE_H
