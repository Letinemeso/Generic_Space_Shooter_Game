#ifndef VISUAL_EFFECT_H
#define VISUAL_EFFECT_H

#include <Stuff/Function_Wrapper.h>

#include <Object_System/Object_2D.h>

#include <Draw_Modules/Draw_Module__Animation.h>


namespace GSSG
{

    class Visual_Effect_Stub : public LEti::Object_2D_Stub
    {
    public:
        DECLARE_VARIABLE;

    public:
        LR::Draw_Module__Animation__Stub* draw_module = nullptr;

    public:
        LV::Variable_Base* M_construct_product() const override;
        void M_init_constructed_product(LV::Variable_Base* _product) const override;

    };



    class Visual_Effect : public LEti::Object_2D
    {
    public:
        DECLARE_VARIABLE;

    public:
        LST::Function<bool()> should_be_destroyed;

    };

}

#endif // VISUAL_EFFECT_H
