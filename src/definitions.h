#pragma once
#include "oxygine-framework.h"
#include <functional>
#include "Player.h"

#include "oxygine-sound.h"


void ShowAD();
void HideAD();

void InitBigAD();
void ShowBigAD();
void HideBigAD();

 extern bool			g_BigAdLoadet;

 extern oxygine::spSprite g_blackSpt;

extern  bool				g_CheatsOn;

 extern int				appleAppId;
 extern std::string		gameCenterTable;
 extern std::string		payAds;
 
 extern oxygine::Resources g_SoundResources;

 extern oxygine::SoundPlayer g_sfxPlayer;
 extern oxygine::SoundPlayer g_sfxEnvirmomentPlayer;
 extern oxygine::SoundPlayer g_musicPlayer;

extern std::string g_Lang;

  

 extern bool g_VideoAds;

extern bool				g_VungleInit;

void ShowVideoAD();
void HideVideoAD();

void InitVideoAD();


extern bool g_VideoAdsStarted;

extern void*   g_AudioData;
extern int     g_AudioSize;


extern void*   g_AudioData2;
extern int     g_AudioSize2;

void		   PlayMusic1();
void		   PlayMusic2();

extern bool		g_IsFirstMusic;

extern int		g_MaxMusicVol;

extern bool g_StartPreGame;


extern const oxygine::Vector2 GAME_SIZE;

extern oxygine::Point DEVICE_SIZE;

extern oxygine::Resources			g_GameResourcesDangerLava;
extern oxygine::Resources			g_GameResourcesDangerTentacles;

extern oxygine::Resources	g_GameResources;
extern oxygine::Resources	g_GameLogoResources;



namespace oxygine
{
    class BillingPurchaseEvent : public Event
    {
    public:
        enum { EVENT = sysEventID('b', 'p', 'e') };
        BillingPurchaseEvent(const std::string & id, const std::string & price) : Event(EVENT), _id(id), _price(price){}
        
		std::string	   _price;
        std::string    _id;
    };
    
    class BillingCancelPurchaseEvent : public Event
    {
    public:
        enum { EVENT = sysEventID('b', 'p', 'c') };
        BillingCancelPurchaseEvent(const std::string & id) : Event(EVENT), _id(id) {}
        
        std::string    _id;
    };

}



struct XmlFileWriter : pugi::xml_writer
{
	XmlFileWriter(oxygine::file::handle h) : _handle(h)
	{
	}

	virtual void write(const void* data, size_t size)
	{
		oxygine::file::write(_handle, data, size);
	}

	oxygine::file::handle _handle;
};

void FreeResFunc( void * pData );
void FreeResFunc( unsigned char * pData );
unsigned char * GetVfsFileData( const char * szData, int * nLen = NULL );

#define DEVELOPER_MODE
