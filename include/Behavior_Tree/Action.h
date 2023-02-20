#ifndef ACTION_H
#define ACTION_H

#include <L_Debug/L_Debug.h>
#include <Stuff/Function_Wrapper.h>

#include <Behavior_Tree/Node_Base.h>


namespace GSSG
{

    class Action : public Node_Base
    {
    private:
        LST::Function_Wrapper<BT_Execution_Result>* m_action = nullptr;

    public:
        Action(LST::Function_Wrapper<BT_Execution_Result>* _action);
        ~Action();

    public:
        void set_action(LST::Function_Wrapper<BT_Execution_Result>* _action);

    public:
        BT_Execution_Result process() const override;

    };

}


#endif // ACTION_H
