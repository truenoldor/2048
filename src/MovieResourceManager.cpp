#include "MovieResourceManager.h"


namespace oxygine
{
	MovieResourceManager * MovieResourceManager::instance = NULL;

	MovieResourceManager::MovieResourceManager()
	{

	}

	MovieResourceManager::~MovieResourceManager()
	{

	}

	void MovieResourceManager::ReleaseResource(const string & path)
	{
		std::map< std::string, MovieResDesc >::iterator it = m_Resources.find(path);
		if (it != m_Resources.end())
			return;

		it->second.m_Ref--;
		
		if (it->second.m_Ref > 0)
			return;

		it->second.m_Resource->clear();
		delete it->second.m_Resource;

		m_Resources.erase(it);
	}
	
	AEMovieResource * MovieResourceManager::AddResource(const string & path)
	{
		std::map< std::string, MovieResDesc >::iterator it = m_Resources.find(path);
		if (it != m_Resources.end())
		{
			it->second.m_Ref++;
			return it->second.m_Resource;
		}
		
		MovieResDesc rd;
		
		rd.m_Ref = 1;
		rd.m_Resource = new AEMovieResource();
		rd.m_Resource->load(path);
		
		m_Resources[path] = rd;

		return rd.m_Resource;
	}
}