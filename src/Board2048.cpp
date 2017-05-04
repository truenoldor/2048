#include "Board2048.h"
#include "Helper.h"

namespace oxygine
{
    const Point MATRIX_SIZE(4, 4);
	const Vector2 DELTA(24.f, 24.f);

    Tile2048::Tile2048(const oxygine::Resources & res):
        m_MatrixPos( 0, 0 ),
		m_Denomination( 0 ),
		m_DenominationBuffer( 0 ),
		m_WillKill( false ),
		m_Lock( false )
    {
    }

	void Tile2048::updateColor()
	{
		if (!m_Score)
			return;

		Color cl(0xffffffff);

		switch ( m_Denomination )
		{
		case 2: cl = Color::MidnightBlue; break;
		case 4: cl = Color::Red; break;
		case 8: cl = Color::Aquamarine; break;
		case 16: cl = Color::Purple; break;
		case 32: cl = Color::Salmon; break;
		case 64: cl = Color::Teal; break;
		case 128: cl = Color::Brown; break;
		case 256: cl = Color::Coral; break;
		case 512: cl = Color::Peru; break;
		case 1024: cl = Color::Green; break;
		case 2048: cl = Color::Orchid; break;
		case 4096: cl = Color::Lime; break;
		case 8192: cl = Color::Maroon; break;
		case 16384: cl = Color::RosyBrown; break;
		case 32768: cl = Color::Aqua; break;
		}

		m_Score->setColor(cl);
	}

	void Tile2048::doUpdate(const UpdateState& us)
	{
		Sprite::doUpdate(us);
		if ( m_Score )
		{
			char out[256] = "";
			sprintf(out, "%d", m_Denomination);
			m_Score->setText(out);
			updateColor();
		}
	}


    Board2048::Board2048():
		m_Resources( 0 ),
		m_TouchPoint( 0.f, 0.f ),
		m_State( ebsIdle ),
		m_MoveCounter(0),
		m_Scores(0)
    {

    }

    Board2048::~Board2048()
    {

    }

    void Board2048::Create( const oxygine::Resources & res )
    {
		m_Resources = &res;
        setResAnim(res.getResAnim("border"));

		addEventListener(TouchEvent::TOUCH_DOWN, [=](Event* e) {
			m_TouchPoint = ((TouchEvent *)e)->localPosition;
			m_State = ebsSlide;
		});

		addEventListener(TouchEvent::TOUCH_UP, [=](Event* e) {
			m_State = ebsIdle;
		});

		EventCallback cb = CLOSURE(this, &Board2048::objectSlide);
		addEventListener(TouchEvent::MOVE, cb);

		time_t ltime = time(NULL);
		unsigned int stime = (unsigned int)ltime / 2;
		srand( stime );
//  		Point p1(0, 0);
//  		Point p2(1,0);
//  		Point p3(2, 0);
//  		Point p4(3, 0);
//         Spawn( &p1, 2 );
//         Spawn( &p2, 4 );
//  		Spawn( &p3, 2 );
//  		Spawn( &p4, 2 );

		Spawn();
		Spawn();
    }

	bool Board2048::checkTweens()
	{
		for (spTile2048 tile : m_Tiles )
		{
			if (tile->getFirstTween() && ( tile->getFirstTween()->getName() == "move" ))
				return true;
		}

		return false;
	}

	void Board2048::objectSlide(Event * e)
	{
		if (m_State == ebsMoveProcess)
			return;

		TouchEvent * te = (TouchEvent *)e;
		if (!te->getPointer()->isPressed())
			return;

		if (checkTweens())
			return;

		Vector2 dir( te->localPosition - m_TouchPoint );

		float len = 60.f;
		float lenDiff = 21.f;

		float l = fabs(fabs(dir.x) - fabs(dir.y));
		if (dir.length() > len && l > lenDiff)
		{
			m_MoveCounter = 0;
			float angle = atan2f(dir.y, dir.x);
			if (fabs(dir.x) >= fabs(dir.y))
			{
				if (dir.x >= 0.f)
				{
					move(enmdRight);
				}
				else
				{
					move(enmdLeft);
				}
			}
			else
			{
				if (dir.y >= 0.f)
				{
					move(enmdDown);
				}
				else
				{
					move(enmdUp);
				}
			}
		}
	}

	Tile2048 * Board2048::getTile(const Point & p)
	{
		for (spTile2048 tile : m_Tiles)
		{
			if( tile->m_WillKill)
				continue;

			if (tile->m_MatrixPos.x == p.x &&
				tile->m_MatrixPos.y == p.y)
				return tile.get();
		}
		return 0;
	}

	Tile2048 *Board2048::traceTile(const Point & p, EBoardMoveDir dir)
	{
		switch ( dir )
		{
			case enmdRight:
			{
				for (int x = p.x; x < MATRIX_SIZE.x; x++)
				{
					Tile2048 * tile = getTile(Point(x, p.y));
					if (tile)
						return tile;
				}
			}
			break;
			case enmdLeft:
			{
				for (int x = p.x; x >= 0; x--)
				{
					Tile2048 * tile = getTile(Point(x, p.y));
					if (tile)
						return tile;
				}
			}
			break;
			case enmdDown:
			{
				for (int y = p.y; y < MATRIX_SIZE.y; y++)
				{
					Tile2048 * tile = getTile(Point(p.x, y));
					if (tile)
						return tile;
				}
			}
			break;
			case enmdUp:
			{
				for (int y = p.y; y >= 0 ; y--)
				{
					Tile2048 * tile = getTile(Point(p.x, y));
					if (tile)
						return tile;
				}
			}
			break;
		}

		return 0;
	}

	void Board2048::RemoveTile(Tile2048 * pTileTrace)
	{
		for (std::vector< spTile2048 >::iterator it = m_Tiles.begin(), it_end = m_Tiles.end(); it != it_end; ++it)
		{
			if ( (*it).get() == pTileTrace )
			{
				pTileTrace->detach();
				m_Tiles.erase(it);
				return;
			}
		}
	}

	void Board2048::moveTile(Tile2048 * pTile, const Point & p, Tile2048 * pTileTrace)
	{
		OX_ASSERT(pTile);
		if (!pTile)
			return;

		if (pTile->m_MatrixPos.x == p.x &&
			pTile->m_MatrixPos.y == p.y)
			return;

		if (pTileTrace)
		{
			pTile->m_DenominationBuffer = pTile->m_Denomination;
			pTile->m_Lock = true;
			pTileTrace->m_WillKill = true;
		}

		m_MoveCounter++;

		pTile->m_MatrixPos = p;
		Vector2 size(pTile->getSize());
		Vector2 dstPos(DELTA + Vector2(size.x * float(p.x) + size.x / 2.f, size.y * float(p.y) + size.y / 2.f));

		if (pTile->getFirstTween())
		{
			int j = 0;
		}


		spTween tw = pTile->addTween(Actor::TweenPosition(dstPos), 100, 1, false, 0, Tween::ease_linear);
		tw->setName("move");
		tw->addDoneCallback([=](Event * e)->void
		{
			if ( pTileTrace )
			{
				pTile->m_Denomination *= 2;
				pTile->m_DenominationBuffer = 0;
				m_Scores += pTile->m_Denomination;
			//	RemoveTile(pTileTrace);
			}
		}
		);
	}

	void Board2048::move(EBoardMoveDir dir)
	{
		switch ( dir )
		{
			case enmdRight:
			{
				for (int y = 0; y < MATRIX_SIZE.y; y++)
				{
					for (int x = MATRIX_SIZE.x - 2; x >= 0; x--)
					{
						Tile2048 * tile = getTile(Point(x, y));
						if (!tile)
							continue;

						Tile2048 * trace = traceTile(Point(x + 1, y), enmdRight);
						if (!trace)
						{
							Point destPoint(MATRIX_SIZE.x - 1, y);
							moveTile(tile, destPoint);
						}
						else
						{
							if (trace->m_Lock || (trace->m_Denomination + trace->m_DenominationBuffer) != tile->m_Denomination)
							{
								Point destPoint(trace->m_MatrixPos.x - 1, y);
								moveTile(tile, destPoint);
							}
							else
							{
								Point destPoint(trace->m_MatrixPos.x, y);
								moveTile(tile, destPoint, trace);
							}
						}
					}
				}

			}
			break;
			case enmdLeft:
			{
				for (int y = 0; y < MATRIX_SIZE.y; y++)
				{
					for (int x = 1; x < MATRIX_SIZE.x; x++)
					{
						Tile2048 * tile = getTile(Point(x, y));
						if (!tile)
							continue;

						Tile2048 * trace = traceTile(Point(x - 1, y), enmdLeft);
						if (!trace)
						{
							Point destPoint(0, y);
							moveTile(tile, destPoint);
						}
						else
						{
							if (trace->m_Lock || (trace->m_Denomination + trace->m_DenominationBuffer) != tile->m_Denomination)
							{
								Point destPoint(trace->m_MatrixPos.x + 1, y);
								moveTile(tile, destPoint);
							}
							else
							{
								Point destPoint(trace->m_MatrixPos.x, y);
								moveTile(tile, destPoint, trace);
							}
						}
					}
				}
			}
			break;
			case enmdDown:
			{
				for (int x = 0; x < MATRIX_SIZE.x; x++)
				{
					for (int y = MATRIX_SIZE.y - 2; y >= 0; y--)
					{
						Tile2048 * tile = getTile(Point(x, y));
						if (!tile)
							continue;

						Tile2048 * trace = traceTile(Point(x, y + 1), enmdDown);
						if (!trace)
						{
							Point destPoint(x, MATRIX_SIZE.y - 1);
							moveTile(tile, destPoint);
						}
						else
						{
							if (trace->m_Lock || (trace->m_Denomination + trace->m_DenominationBuffer) != tile->m_Denomination)
							{
								Point destPoint(x, trace->m_MatrixPos.y - 1);
								moveTile(tile, destPoint);
							}
							else
							{
								Point destPoint(x, trace->m_MatrixPos.y);
								moveTile(tile, destPoint, trace);
							}
						}
					}
				}
			}
			break;
			case enmdUp:
			{
				for (int x = 0; x < MATRIX_SIZE.x; x++)
				{
					for (int y = 1; y < MATRIX_SIZE.y; y++)
					{
						Tile2048 * tile = getTile(Point(x, y));
						if (!tile)
							continue;

						Tile2048 * trace = traceTile(Point(x, y - 1), enmdUp);
						if (!trace)
						{
							Point destPoint(x, 0);
							moveTile(tile, destPoint);
						}
						else
						{
							if (trace->m_Lock || (trace->m_Denomination + trace->m_DenominationBuffer) != tile->m_Denomination)
							{
								Point destPoint(x, trace->m_MatrixPos.y + 1);
								moveTile(tile, destPoint);
							}
							else
							{
								Point destPoint(x, trace->m_MatrixPos.y);
								moveTile(tile, destPoint, trace);
							}
						}
					}
				}
			}
			break;
		}


		if (m_MoveCounter)
		{
			spTile2048 tile = Spawn();
			if (tile)
			{
				tile->setScale(0.f);
				tile->addTween(Actor::TweenScale(1.1f), 250, 1, false, 100, Tween::ease_inSin);
				tile->addTween(Actor::TweenScale(1.f), 100, 1, false, 351, Tween::ease_outSin);
			}
		}

		m_State = ebsMoveProcess;

		//checkLeftTurns();
	}

	bool Board2048::belongArea(const Point & p)
	{
		return p.x >= 0 && p.y >= 0 && p.x <= MATRIX_SIZE.x - 1 && p.y <= MATRIX_SIZE.y - 1;
	}

	bool Board2048::IsEmptyNearCells(spTile2048 tile)
	{
		Point p1(tile->m_MatrixPos + Point( 1, 0 ));
		Point p2(tile->m_MatrixPos + Point(-1, 0));
		Point p3(tile->m_MatrixPos + Point(0, 1));
		Point p4(tile->m_MatrixPos + Point(0, -1));

		if (belongArea(p1) && !getTile(p1))
			return true;
		if (belongArea(p2) && !getTile(p2))
			return true;
		if (belongArea(p3) && !getTile(p3))
			return true;
		if (belongArea(p4) && !getTile(p4))
			return true;

		return false;
	}

	bool Board2048::IsDenominationNearCells(spTile2048 tile)
	{
		Point p1(tile->m_MatrixPos + Point(1, 0));
		Point p2(tile->m_MatrixPos + Point(-1, 0));
		Point p3(tile->m_MatrixPos + Point(0, 1));
		Point p4(tile->m_MatrixPos + Point(0, -1));

		if ( Tile2048 * pTile = traceTile(p1, enmdRight))
		{
			if (pTile->m_Denomination + pTile->m_Denomination ==
				tile->m_Denomination + tile->m_Denomination)
				return true;
		}

		if (Tile2048 * pTile = traceTile(p2, enmdLeft))
		{
			if (pTile->m_Denomination + pTile->m_Denomination ==
				tile->m_Denomination + tile->m_Denomination)
				return true;
		}

		if (Tile2048 * pTile = traceTile(p3, enmdDown))
		{
			if (pTile->m_Denomination + pTile->m_Denomination ==
				tile->m_Denomination + tile->m_Denomination)
				return true;
		}

		if (Tile2048 * pTile = traceTile(p4, enmdUp))
		{
			if (pTile->m_Denomination + pTile->m_Denomination ==
				tile->m_Denomination + tile->m_Denomination)
				return true;
		}


		return false;
	}

	void Board2048::doUpdate(const UpdateState& us)
	{
		Sprite::doUpdate(us);
		if ( !checkTweens() )
		{
// 			if (m_State == ebsMoveProcess)
// 			{
//
// 				m_State = ebsIdle;
// 			}

			checkLeftTurns();

			for (spTile2048 tile : m_Tiles)
			{
				tile->m_Lock = false;
			}


			bool isKill = true;

			while ( isKill )
			{
				isKill = false;
				for (spTile2048 tile : m_Tiles)
				{
					if (tile->m_WillKill)
					{
						RemoveTile(tile.get());
						isKill = true;
						break;
					}
				}
			}

		}
	}

	void Board2048::checkLeftTurns()
	{
		for (spTile2048 tile : m_Tiles )
		{
			if (IsEmptyNearCells(tile))
				return;

			if (IsDenominationNearCells(tile))
				return;
		}
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

    spTile2048 Board2048::Spawn(Point * pDebugPoint, int debugVal)
    {
		OX_ASSERT(m_Resources);
		if (!m_Resources)
			return 0;

		const ResAnim * pRes = m_Resources->getResAnim("chip");
		if (!pRes)
			return 0;

		Vector2 size( pRes->getWidth(), pRes->getHeight() );

		std::vector< Point > cells;
        getFreeCells(cells);

		if (cells.empty())
			return 0;

		int index = rand() % cells.size();
		const Point & p = cells[index];

		spTile2048 tile = new Tile2048( *m_Resources );
		tile->setTouchEnabled(false);
		tile->setResAnim(pRes);
		tile->setTouchChildrenEnabled(false);
		tile->m_MatrixPos = pDebugPoint ? *pDebugPoint : p;
		int prob = rand() % 100;
		tile->m_Denomination = prob <= 90 ? 2 : 4;
		if (debugVal)
			tile->m_Denomination = debugVal;

		tile->m_Score = Helper::makeMeTextField("bip-2", 70, TextStyle::HALIGN_MIDDLE, TextStyle::VALIGN_MIDDLE);
		Helper::linkTextField(tile, tile->m_Score, Vector2(0.f, -6.f));

		tile->setAnchor(0.5f, 0.5f);
		tile->setPosition(DELTA + Vector2(size.x * float(tile->m_MatrixPos.x) + size.x / 2.f, size.y * float(tile->m_MatrixPos.y) + size.y / 2.f));
		tile->attachTo(this);

		m_Tiles.push_back(tile);

		return tile;
    }
}