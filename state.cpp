#include "state.h"

State::State(Elevator* ele):elevator(ele)
{
    ecs = ECS::getECSInstance();
}
