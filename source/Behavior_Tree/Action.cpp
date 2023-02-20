#include <Behavior_Tree/Action.h>

using namespace GSSG;


Action::Action(LST::Function_Wrapper<BT_Execution_Result>* _action)
    : m_action(_action)
{
    L_ASSERT(_action);
}

Action::~Action()
{
    delete m_action;
}



void Action::set_action(LST::Function_Wrapper<BT_Execution_Result> *_action)
{
    L_ASSERT(_action);
    delete m_action;
    m_action = _action;
}



BT_Execution_Result Action::process() const
{
    return m_action->invoke();
}
