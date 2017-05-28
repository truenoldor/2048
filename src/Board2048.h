#pragma once

#include "definitions.h"

namespace oxygine
{
    DECLARE_SMART(Tile2048, spTile2048);
    class Tile2048 : public Sprite
    {
    public:
        Tile2048(const oxygine::Resources & res);

		void						doUpdate(const UpdateState& us) override;

        Point						m_MatrixPos;
		int							m_Denomination;
		int							m_DenominationBuffer;
		spTextField					m_Score;
		bool						m_WillKill;
		bool						m_Lock;
	private:
		void						updateColor();
    };

    DECLARE_SMART(Board2048, spBoard2048);
    class Board2048 : public Sprite
    {
    public:
        Board2048();
        virtual ~Board2048();

        void                    Create(const oxygine::Resources & res);

		int						getScores() { return m_Scores; }

	protected:
		
		void					doUpdate(const UpdateState& us) override;

    private:

        spTile2048              Spawn(Point * pDebugPoint = 0, int debugVal = 0);
		void					RemoveTile(Tile2048 * pTileTrace);

        void                    getFreeCells(std::vector< Point > & cells);
		void					objectSlide(Event * e);		
		Tile2048 *				getTile(const Point & p);		
		void					moveTile(Tile2048 * pTile, const Point & p, Tile2048 * pTileTrace = 0);		
		void					checkLeftTurns();
		bool					IsEmptyNearCells(spTile2048 tile);
		bool					IsDenominationNearCells(spTile2048 tile);
		bool					belongArea(const Point & p);
        const ResAnim *         getResWithScore(int den);

    private:

        std::vector< spTile2048 >       m_Tiles;
		const oxygine::Resources	*	m_Resources;
		Vector2							m_TouchPoint;
		int								m_MoveCounter;		
		int								m_Scores;

        bool                            m_Finish;

        bool                            m_WithReward;

		enum EBoardMoveDir
		{
			enmdLeft,
			enmdUp,
			enmdRight,
			enmdDown
		};

		void					move(EBoardMoveDir dir);
		Tile2048 *				traceTile(const Point & p, EBoardMoveDir dir);
		bool					checkTweens( bool all = false );

		enum EBoardState
		{
			ebsIdle,
			ebsSlide,
			ebsMoveProcess,
            ebsWinProcess,
            ebsGameOver
		}m_State;
    };
};

