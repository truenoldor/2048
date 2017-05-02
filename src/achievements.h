#pragma once

#include "definitions.h"

namespace oxygine
{
	class achievements
	{
	public:
		achievements();

		enum ETypes
		{
			id_session_1,
			id_session_2,
			id_session_3,
			id_all_1,
			id_all_2,
			id_all_3,
			id_open_bear,
			id_open_parrot,
			id_open_wild_cat,
			id_open_koala,
            id_open_crocodile,
            id_open_lizard,
            id_open_rhinoceros,
            id_open_snake,
            id_food_self_1,
            id_food_self_2,
            id_food_self_3,
            id_food_self_4,
            id_diver_1,
            id_diver_2,
            id_diver_3,
            id_diver_4,
            id_lifes1,
            id_lifes2,
            id_ads,
            id_wall_breaker,
			max_achievements
			
		};
		
	private:

		static std::string m_sAchievements[ max_achievements ];
		
		static achievements m_sInstance;
		
	public:
		static achievements * GetInstance()
		{
			return &m_sInstance;
		}

		void								serialize();
		void								deserialize();

		void								Apply( ETypes type );
		void								Apply( const std::string & achievement );

		void								Clear();

		void								Create();
        
        void                                ApplyAll();
	private:
		void								AddAchievements( const std::string & ach );
	private:

		std::set< ETypes >					m_Applies;

		
	};
}
