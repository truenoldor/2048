#include "definitions.h"

#ifdef WIN32
bool g_CheatsOn = true;
#else
bool g_CheatsOn = false;
#endif



oxygine::Resources g_SoundResources;
oxygine::SoundPlayer g_sfxPlayer;
oxygine::SoundPlayer g_sfxEnvirmomentPlayer;
oxygine::SoundPlayer g_musicPlayer;

std::string g_Lang;


int				appleAppId = 1057563636;
std::string		gameCenterTable = "SDE_Score";
std::string		payAds = "RemADS";

//debug:
//  int				appleAppId = 1057572703;
//  std::string		gameCenterTable = "bestscore01";
//  std::string		payAds = "remads1";


 bool g_VideoAds = false;

 bool g_BigAdLoadet = false;

bool	g_VungleInit = false;

bool g_VideoAdsStarted = false;


bool g_IsFirstMusic = true;

int		g_MaxMusicVol = 256;


bool g_StartPreGame = false;

oxygine::Resources	g_GameResources;
const oxygine::Vector2 GAME_SIZE( 1080.f, 1920.f );
oxygine::Point DEVICE_SIZE;

oxygine::Resources g_GameLogoResources;

oxygine::Resources			g_GameResourcesDangerLava;
oxygine::Resources			g_GameResourcesDangerTentacles;


void FreeResFunc( unsigned char * pData )
{
	delete [] pData;
}

void FreeResFunc( void * pData )
{
    FreeResFunc( ( unsigned char * ) pData );
}


unsigned char * GetVfsFileData(const char * szData, int * nLen)
{
	OX_ASSERT(szData);
	if (!szData)
		return NULL;

	oxygine::file::handle fin = NULL;

	fin = oxygine::file::open(szData, "rb", oxygine::error_policy::ep_ignore_error);
	unsigned char*	pData;
	if (fin)
		//return NULL;
	{
		long size;

		size = long(((oxygine::file::fileHandle*)fin)->getSize());

		//oxygine::file::seek(fin, 0, SEEK_END);
		//size = oxygine::file::tell(fin);
		if (size == 0)
		{
			oxygine::file::close(fin);
			return NULL;
		}
		//oxygine::file::seek(fin, 0, SEEK_SET);
		pData = new unsigned char[size + 10]();
		memset(pData, 0, size + 10);
		oxygine::file::read(fin, pData, size);
		if (nLen)
		{
			*nLen = size;
		}
		oxygine::file::close(fin);
		return pData;
	}
	else
	{
		return NULL;
	}
	return NULL;
}
