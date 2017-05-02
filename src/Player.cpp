#include "Player.h"
#include "definitions.h"
#include "MainScreen.h"

namespace oxygine
{
	spPlayer Player::instance;
	Player::Player():
		m_AllPoints( 0 ),
		m_CurrentPers( 0 ),
		m_IsRemoveAds( false ),
		m_SoundOn( true ),
		m_BestPoints( 0 ),
		m_IsViewTutorial( false ),
        m_ShowRate( 0 ),
        m_LifesCount( 1 ),
        m_WaterDeath( 0 ),
        m_FlowerDeath( 0 ),
        m_WallDeath( 0 ),
        m_AllLifesBuy( 0 ),
		m_VideoAdsCounter( 0 ),
		m_VideoAdsCounterTotal( 0 )
	{

	}

	Player::~Player()
	{

	}

	bool Player::Save()
	{
		file::handle h = file::open("player.xml", "w");
		if (!h)
			return false;

		pugi::xml_document doc;
		pugi::xml_node root_node = doc.append_child("level");
		pugi::xml_attribute attrAllP = root_node.append_attribute("all_points");
		attrAllP.set_value( m_AllPoints );
		pugi::xml_attribute attrCurPers = root_node.append_attribute("current_pers");
		attrCurPers.set_value( m_CurrentPers );
		pugi::xml_attribute attrRemoveAds = root_node.append_attribute("is_remove_ads");
		attrRemoveAds.set_value( ( int )m_IsRemoveAds );
		pugi::xml_attribute attrSoundOn = root_node.append_attribute("sound_on");
		attrSoundOn.set_value( ( int )m_SoundOn );
		pugi::xml_attribute bestPointsAt = root_node.append_attribute("best_points");
		bestPointsAt.set_value( ( int )m_BestPoints );

		pugi::xml_attribute dtu = root_node.append_attribute("def_twit_user");
		dtu.set_value( m_DefaultTwitterUser.c_str() );
		pugi::xml_attribute dtp = root_node.append_attribute("def_twit_pass");
		dtp.set_value( m_DefaultTwitterPass.c_str() );

		pugi::xml_attribute is_view_tut = root_node.append_attribute("is_view_tut");
		is_view_tut.set_value(  int( m_IsViewTutorial ) );

        pugi::xml_attribute attrShowRate = root_node.append_attribute("is_show_rate");
        attrShowRate.set_value( ( int )m_ShowRate );
        
        pugi::xml_attribute attrLifes = root_node.append_attribute("lifes");
        attrLifes.set_value( ( int )m_LifesCount );
        
        pugi::xml_attribute attrDeathWater = root_node.append_attribute("water_death");
        attrDeathWater.set_value( ( int )m_WaterDeath );
        
        pugi::xml_attribute attrDeathFlower = root_node.append_attribute("flower_death");
        attrDeathFlower.set_value( ( int )m_FlowerDeath );
        
        pugi::xml_attribute attrDeathWall = root_node.append_attribute("wall_death");
        attrDeathWall.set_value( ( int )m_WallDeath );
        
        pugi::xml_attribute attrAllLifesBuy = root_node.append_attribute("all_lifes_buy");
        attrAllLifesBuy.set_value( ( int )m_AllLifesBuy );

		pugi::xml_attribute videoAdsCounter = root_node.append_attribute("videoAdsCounter");
		videoAdsCounter.set_value((int)m_VideoAdsCounter);

		pugi::xml_attribute videoAdsCounterTotal = root_node.append_attribute("videoAdsCounterTotal");
		videoAdsCounterTotal.set_value((int)m_VideoAdsCounterTotal);

		
        
		pugi::xml_node purchased = root_node.append_child("purchased");
		
		for (std::set< int >::iterator iter = m_PurchasedMonsters.begin(), iter_end = m_PurchasedMonsters.end(); iter != iter_end; ++iter)
		{
			pugi::xml_node po = purchased.append_child("po");
			pugi::xml_attribute attrPers = po.append_attribute("pers");
			attrPers.set_value((int)*iter);
		}
		
		
		
		

		XmlFileWriter writer(h);
		doc.save(writer, "\t", pugi::format_default/* | pugi::format_write_bom*/, pugi::encoding_utf8);

		file::close(h);

		return true;
	}

	bool Player::Load()
	{
		file::handle h = file::open("player.xml", "r", ep_ignore_error);
		if (!h)
			return false;

		file::buffer b;
		file::read(h, b);
		if (b.getSize() == 0)
		{
			file::close(h);
			return false;
		}

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_buffer_inplace(reinterpret_cast<void *>(&b.data[0]), b.getSize());
		if (!result)
		{
			return false;
		}

		pugi::xml_node rootNode = doc.child("level");
		if (rootNode.empty())
		{
			assert(false);
			file::close(h);
			return false;
		}

		m_AllPoints = atoi( rootNode.attribute("all_points").value() );
		m_CurrentPers = atoi( rootNode.attribute("current_pers").value() );
		m_IsRemoveAds = atoi( rootNode.attribute("is_remove_ads").value() ) ? true : false;
		m_SoundOn = atoi( rootNode.attribute("sound_on").value() ) ? true : false;
		m_BestPoints = atoi( rootNode.attribute("best_points").value() );
		m_DefaultTwitterUser =  rootNode.attribute("def_twit_user").value();
		m_DefaultTwitterPass =  rootNode.attribute("def_twit_pass").value();

		m_IsViewTutorial = atoi( rootNode.attribute("is_view_tut").value() ) ? true : false;
        
        m_ShowRate = atoi( rootNode.attribute("is_show_rate").value() );
        
        m_LifesCount = atoi( rootNode.attribute("lifes").value() );
        if( m_LifesCount < 1 )
            m_LifesCount = 1;

        m_WaterDeath = atoi( rootNode.attribute("water_death").value() );
        m_FlowerDeath = atoi( rootNode.attribute("flower_death").value() );
        m_WallDeath = atoi( rootNode.attribute("wall_death").value() );
        m_AllLifesBuy = atoi( rootNode.attribute("all_lifes_buy").value() );
        
        m_VideoAdsCounter = atoi( rootNode.attribute("videoAdsCounter").value() );
        m_VideoAdsCounterTotal = atoi( rootNode.attribute("videoAdsCounterTotal").value() );
        
		m_PurchasedMonsters.clear();

		pugi::xml_node purchased = rootNode.child("purchased");
		if (purchased)
		{
			pugi::xml_node po = purchased.child( "po" );
			while (po)
			{
				m_PurchasedMonsters.insert(po.attribute("pers").as_int());
				po = po.next_sibling();
			}
		}

		g_MainScreen->SetVolume( m_SoundOn ? 100 : 0 );
		////g_SoundSystem.setVolume( m_SoundOn ? 100.f : 0.f );
		
			
		////g_SoundSystem.setVolume( m_SoundOn ? 100.f : 0.f );

		file::close(h);

		return true;
	}
};
