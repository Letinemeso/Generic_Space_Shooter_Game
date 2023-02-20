#ifndef NODE_BASE_H
#define NODE_BASE_H


namespace GSSG
{

    enum class BT_Execution_Result
    {
        Success,
        Fail,
        In_Progress
    };

    class Node_Base
    {
    public:
        Node_Base();
        virtual ~Node_Base();

    public:
        virtual BT_Execution_Result process() const = 0;

    };

}


#endif // NODE_BASE_H
