#ifndef ACTION_H_
#define ACTION_H_

enum mg_actions_t {
    DAYSTATE_CHANGED = 1,
};

typedef void (*ActionFunction_t)( void * );

namespace action {
    class Action {
    private:
        static int actionsCounter;
        static ActionFunction_t actions[];

    public:
        static void dispatchAction(mg_actions_t actionID, char *payload);
        static void registerAction(mg_actions_t actionID, ActionFunction_t handler);
    };
}

#endif
