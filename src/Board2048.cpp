#include "Board2048.h"

namespace oxygine
{
    const Point MATRIX_SIZE(4, 4);

    Tile2048::Tile2048(const oxygine::Resources & res):
        m_MatrixPos( 0, 0 )
    {
        setResAnim(res.getResAnim("chip"));
    }

    Board2048::Board2048()
    {

    }

    Board2048::~Board2048()
    {

    }

    void Board2048::Create( const oxygine::Resources & res )
    {
        setResAnim(res.getResAnim("border"));
        Spawn();
        Spawn();
    }

    void Board2048::getFreeCells(std::vector< Point > & cells)
    {
        for ( int x = 0; x < MATRIX_SIZE.x; x++ )
            for (int y = 0; y < MATRIX_SIZE.y; y++)
            {
                bool isFind = false;
                for (spTile2048 tile : m_Tiles)
                {
                    if (tile->m_MatrixPos.x == x &&
                        tile->m_MatrixPos.y == y)
                    {
                        isFind = true;
                        break;
                    }
                }
                if (isFind)
                    continue;

                cells.push_back(Point( x, y ));
            }
    }

    void Board2048::Spawn()
    {
        std::vector< Point > cells;
        getFreeCells(cells);
    }
}