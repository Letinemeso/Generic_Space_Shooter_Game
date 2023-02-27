#include <Behavior_Tree/Action.h>

using namespace GSSG;


Action::Action(const LST::Function<BT_Execution_Result()>& _action)
    : m_action(_action)
{
    L_ASSERT(_action);
}

Action::~Action()
{

}



void Action::set_action(const LST::Function<BT_Execution_Result()>& _action)
{
    L_ASSERT(_action);
    m_action = _action;
}



BT_Execution_Result Action::process() const
{
    return m_action();
}
