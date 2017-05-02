#pragma once

#include "definitions.h"


namespace oxygine
{
	class analytics
	{
	public:
		analytics();

		void						Create();

	private:

		static analytics m_sInstance;

	public:
		static analytics * GetInstance()
		{
			return &m_sInstance;
		}

		void						PushEvent( const std::string & message );
		void						PushEvent( const std::string & message, const std::string & param1 );
		void						PushEvent( const std::string & message, const std::string & param1, const std::string & param2 );
        
        void                        SetEnabled( bool val ) { m_IsEnabled = val; }
        
    private:
        
        bool                        m_IsEnabled;
	};
}
