#include <Behavior_Tree/Selector.h>

using namespace GSSG;


Selector::Selector()
{

}

Selector::~Selector()
{
    remove_childs();
}



void Selector::add_child(Node_Base *_node)
{
    m_childs.push_back(_node);
}

void Selector::remove_child(Node_Base *_node)
{
    LDS::List<Node_Base*>::Iterator it = m_childs.begin();
    for(; !it.end_reached(); ++it)
    {
        if(*it != _node)
            continue;

        delete _node;
        m_childs.erase(it);
        break;
    }
    L_ASSERT(!it.end_reached());
}

void Selector::remove_childs()
{
    for(LDS::List<Node_Base*>::Iterator it = m_childs.begin(); !it.end_reached(); ++it)
        delete *it;

    m_childs.clear();
}



BT_Execution_Result Selector::process() const
{
    L_ASSERT(m_childs.size() > 0);
    BT_Execution_Result result = BT_Execution_Result::Success;
    for(LDS::List<Node_Base*>::Const_Iterator it = m_childs.begin(); !it.end_reached(); ++it)
    {
        result = (*it)->process();

        if(result == BT_Execution_Result::Fail)
            continue;

        return result;
    }

    return result;
}
