#include <Effects_Controller.h>

using namespace GSSG;


Effects_Controller::Effects_Controller()
{

}

Effects_Controller::~Effects_Controller()
{
    for(LDS::List<Visual_Effect*>::Iterator it = m_effect_objects.begin(); !it.end_reached() && it.is_ok(); ++it)
        delete *it;
}



void Effects_Controller::add_object(Visual_Effect *_object)
{
//    L_ASSERT(LV::cast_variable<LR::Draw_Module__Animation>(_object->draw_module()));  //  non-animations shall not pass   //

    m_effect_objects.push_back(_object);
}



void Effects_Controller::update()
{
    for(LDS::List<Visual_Effect*>::Iterator it = m_effect_objects.begin(); !it.end_reached() && it.is_ok(); ++it)
        (*it)->update();

    if(m_effect_objects.size() == 0)
        return;

    LDS::List<Visual_Effect*>::Iterator it = m_effect_objects.begin();

    while(true)
    {
        if((*it)->should_be_destroyed())
        {
            LDS::List<Visual_Effect*>::Iterator next = it;
            if(!it.end_reached())
                ++next;

            delete *it;
            m_effect_objects.erase(it);

            it = next;
        }

        if(it.end_reached() || !it.is_ok() || m_effect_objects.size() == 0)
            break;

        ++it;
    }
}

void Effects_Controller::draw()
{
    for(LDS::List<Visual_Effect*>::Iterator it = m_effect_objects.begin(); !it.end_reached() && it.is_ok(); ++it)
        (*it)->draw(*m_renderer);
}
