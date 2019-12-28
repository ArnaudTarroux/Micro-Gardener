#include <Arduino.h>
#include "action.h"

namespace action {
    int Action::actionsCounter = 0;
    ActionFunction_t Action::actions[] = {};

    void Action::dispatchAction(mg_actions_t actionID, char *payload)
    {
        for (int i = 0; i < Action::actionsCounter; i++) {
            ActionFunction_t action = Action::actions[i];
            action(payload);
        }
    }

    void Action::registerAction(mg_actions_t actionID, ActionFunction_t handler)
    {
        Serial.printf("New handler registered for action : %d\n", actionID);
        Action::actions[Action::actionsCounter] = handler;
        Action::actionsCounter++;
    }
}