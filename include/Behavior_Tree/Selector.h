#ifndef SELECTOR_H
#define SELECTOR_H

#include <Data_Structures/List.h>

#include <Behavior_Tree/Node_Base.h>


namespace GSSG
{

    class Selector : public Node_Base
    {
    private:
        LDS::List<Node_Base*> m_childs;

    public:
        Selector();
        ~Selector();

    public:
        void add_child(Node_Base* _node);   //  this class becomes _node's owner
        void remove_child(Node_Base* _node);    //  deletes specified child
        void remove_childs();

    public:
        inline const LDS::List<Node_Base*>& childs() const { return m_childs; }

    public:
        BT_Execution_Result process() const override;

    };

}


#endif // SELECTOR_H
