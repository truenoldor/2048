#include "analytics.h"
//#include "Profile.h"
#include "tinyxml_p/bind_dynamics.h"
#include "analitycs_wrap.h"
#include "Android_Wrapper.h"
#include "ios_wrapper.h"

namespace oxygine
{
    
    
	analytics analytics::m_sInstance;

    analytics::analytics():
        m_IsEnabled( true )
	{
	}

	
	void analytics::Create()
	{

	}	

	void analytics::PushEvent( const std::string & message )
	{
		PushEvent( message, "", "" );
	}

	void analytics::PushEvent( const std::string & message, const std::string & param1 )
	{
		PushEvent( message, param1, "" );
	}

	void analytics::PushEvent( const std::string & message, const std::string & param1, const std::string & param2 )
	{
        if( !m_IsEnabled )
            return;
        
        #if TARGET_OS_IPHONE || TARGET_OS_SIMULATOR
        EventFlurry ( message.c_str(), param1.empty() ? "empty" : param1.c_str(), param2.empty() ? "empty" : param2.c_str() );
        #endif

#if __ANDROID__
		log::messageln("pre SDL_FlurryEvent");
		SDL_FlurryEvent( 0, message.c_str(),  param1.empty() ? "" : param1.c_str(), param2.empty() ? "" : param2.c_str() );
#endif

		
	}
}

