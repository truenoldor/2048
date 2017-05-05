#pragma once

#include "oxygine-framework.h"
#include <functional>
#include <set>

namespace oxygine
{
	DECLARE_SMART(Player, spPlayer);
	class Player : public ref_counter
	{
	public:
		Player();
		virtual ~Player();

		static spPlayer				instance;


		bool						Load();
		bool						Save();

	public:

		int							m_AllPoints;
		int							m_BestPoints;
		int							m_CurrentPers;
		bool						m_IsRemoveAds;
		
		bool						m_IsViewTutorial;

		bool						m_SoundOn;
        bool                        m_MusicOn;
        
        int                         m_ShowRate;
        bool                        m_WasRate;
        
        int                         m_WaterDeath;
        int                         m_FlowerDeath;
        int                         m_WallDeath;
        int                         m_AllLifesBuy;

		std::string					m_DefaultTwitterUser;
		std::string					m_DefaultTwitterPass;
        
        int                         m_LifesCount;

		std::set< int >				m_PurchasedMonsters;

		int							m_VideoAdsCounter;
		int							m_VideoAdsCounterTotal;
	};
	
};
