#pragma once

#include "definitions.h"
#include <map>

#define GetLocalize( szToken, pscale ) g_Localization.GetText( szToken, pscale )

class crc32_noldor{
protected:
	unsigned table[256];
public:
	unsigned m_crc32;
	crc32_noldor();
	unsigned ProcessCRC(void* pData, int nLen);
};

class CLocalization
{
public:
	CLocalization();
	void					Init( const char * szPathFile );
	std::string					GetText( const char * szToken, float * pfScale = NULL );
	unsigned				GetHashCRC32( const char * szData, int len );

	std::map< unsigned, std::string >		m_Texts;
	std::map< unsigned, float >		m_TextsScale;
	

	crc32_noldor			m_Crc32Help;
};


extern CLocalization g_Localization;
extern crc32_noldor g_HelpCRC;
