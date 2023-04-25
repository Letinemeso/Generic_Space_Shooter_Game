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

    public:
        virtual void update() = 0;

    };

}

#endif // GAME_LOGIC_H
