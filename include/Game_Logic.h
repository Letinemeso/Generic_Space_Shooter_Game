#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

namespace GSSG
{

    class Game_Logic
    {
    public:
        Game_Logic();
        virtual ~Game_Logic();

    public:
        virtual void on_activate();
        virtual void on_deactivate();

    public:
        virtual bool can_be_activated() const { return true; }
        virtual bool can_be_deactivated() const { return true; }

    public:
        virtual void update() = 0;

    };

}

#endif // GAME_LOGIC_H
