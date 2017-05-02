#include "achievements.h"
#include "ios_wrapper.h"
#include "Android_Wrapper.h"

namespace oxygine
{
	achievements achievements::m_sInstance;

	achievements::achievements()
	{
	}

	std::string achievements::m_sAchievements[ max_achievements ]=
	{
		"id_session_1",
		"id_session_2",
		"id_session_3",
		"id_all_1",
		"id_all_2",
		"id_all_3",
		"id_open_bear",
		"id_open_parrot",
		"id_open_wild_cat",
		"id_open_koala",
        "id_open_crocodile",
        "id_open_lizard",
        "id_open_rhinoceros",
        "id_open_snake",
        "id_food_self_1",
        "id_food_self_2",
        "id_food_self_3",
        "id_food_self_4",
        "id_diver_1",
        "id_diver_2",
        "id_diver_3",
        "id_diver_4",
        "id_lifes1",
        "id_lifes2",
        "id_ads",
        "id_wall_breaker"
        
	};

	void achievements::Create()
	{
				
	}

	void achievements::Clear()
	{
		m_Applies.clear();
	}

	void achievements::serialize()
	{
        file::handle h = file::open("achi.xml", "w");
        if (!h)
            return;
        
       pugi::xml_document doc;
       pugi::xml_node node = doc.append_child( "achievements" );
		
		for ( std::set< ETypes >::iterator iter = m_Applies.begin(), iter_end = m_Applies.end(); iter != iter_end; ++iter )
		{
			pugi::xml_node element = node.append_child( "ach" );
			setAttr( element, "id", int( *iter ), 0 );			
		}
        
        XmlFileWriter writer(h);
        doc.save(writer, "\t", pugi::format_default/* | pugi::format_write_bom*/, pugi::encoding_utf8);
        
        file::close(h);
    }
	
	void achievements::deserialize()
	{
        file::handle h = file::open("achi.xml", "r", ep_ignore_error);
        if (!h)
            return;
        
        file::buffer b;
        file::read(h, b);
        if (b.getSize() == 0)
        {
            file::close(h);
            return ;
        }
        
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_buffer_inplace(reinterpret_cast<void *>(&b.data[0]), b.getSize());
        if (!result)
        {
            file::close(h);
            return;
        }
        
        
        m_Applies.clear();

		pugi::xml_node anode = doc.child( "achievements" );
		if( anode.empty() )
        {
            file::close(h);
            return;
        }

		pugi::xml_node element = anode.child( "ach" );
		while( element )
		{
			int type = element.attribute( "id" ).as_int();

			m_Applies.insert( ETypes( type ) );
			
			element = element.next_sibling();
		}
        
        file::close(h);
	}

	void achievements::Apply( const std::string & achievement )
	{
		if ( achievement.empty() )
			return;

		for( int i = 0; i < max_achievements; i++ ) 
		{
			if ( m_sAchievements[ i ] == achievement )
			{
				Apply( ETypes( i ) );
				break;
			}
		}
	}

	void achievements::AddAchievements( const std::string & ach )
	{
		#ifdef __APPLE__
				SDL_SetAchievement( ach.c_str(), 100 );
		#endif

		#ifdef __ANDROID__
				Andy_UnlockAchi( ach.c_str(), 100 );
		#endif
        

		
		//add achievements here
	}
    
    void achievements::ApplyAll()
    {
        for( ETypes type : m_Applies )
        {
            AddAchievements( m_sAchievements[ ( int )type ] );
        }
    }

	void achievements::Apply( ETypes type )
	{
		if ( m_Applies.find( type ) != m_Applies.end() )
			return;		

		m_Applies.insert( type );

        AddAchievements( m_sAchievements[ ( int )type ] );
        
        serialize();
	}
}

