#pragma once

#include "definitions.h"

namespace oxygine
{
    DECLARE_SMART(Tile2048, spTile2048);
    class Tile2048 : public Sprite
    {
    public:
        Tile2048(const oxygine::Resources & res);

        Point                    m_MatrixPos;
    };

    DECLARE_SMART(Board2048, spBoard2048);
    class Board2048 : public Sprite
    {
    public:
        Board2048();
        virtual ~Board2048();

        void                    Create(const oxygine::Resources & res);

    private:

        void                    Spawn();
        void                    getFreeCells(std::vector< Point > & cells);

    private:

        std::vector< spTile2048 >       m_Tiles;
    };
};

