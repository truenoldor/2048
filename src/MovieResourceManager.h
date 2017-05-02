#pragma once

#include "definitions.h"
#include "player_ae/AEMovie.h"
#include <map>

namespace oxygine
{
	class MovieResourceManager
	{
	public:
		MovieResourceManager();
		~MovieResourceManager();

	public:

		void									ReleaseResource(const string & path);
		AEMovieResource *						AddResource(const string & path);

		static MovieResourceManager *			getInstance()
		{
			if (!instance)
				instance = new MovieResourceManager();

			return instance;
		}

		static void								free()
		{
			delete instance;
			instance = 0;
		}

	private:
		static MovieResourceManager * instance;

		struct MovieResDesc
		{
			AEMovieResource*	m_Resource;
			int					m_Ref;
			
			MovieResDesc():
				m_Ref( 0 ),
				m_Resource( NULL )
			{}
		};

		std::map< std::string, MovieResDesc >			m_Resources;
	};
}