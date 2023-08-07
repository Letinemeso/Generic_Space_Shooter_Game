#ifndef POLYGON__SPACE_SHIP_H
#define POLYGON__SPACE_SHIP_H

#include <Physical_Models/Rigid_Body_Polygon.h>


namespace GSSG
{

    class Polygon__Space_Ship : public LPhys::Rigid_Body_Polygon
    {
    private:
        unsigned int m_block_index_x = 0;
        unsigned int m_block_index_y = 0;

    public:
        Polygon__Space_Ship();
        Polygon__Space_Ship(const Polygon__Space_Ship& _other);
        ~Polygon__Space_Ship();

        void setup(const Polygon& _other) override;

    public:
        inline void set_block_index(unsigned int _x, unsigned int _y) { m_block_index_x = _x; m_block_index_y = _y; }

    public:
        inline unsigned int block_index_x() const { return m_block_index_x; }
        inline unsigned int block_index_y() const { return m_block_index_y; }

    };

}


#endif // POLYGON__SPACE_SHIP_H
