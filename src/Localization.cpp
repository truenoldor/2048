#include "Localization.h"

#include "tinyxml_p/bind_dynamics.h"


crc32_noldor::crc32_noldor(){
	const unsigned CRC_POLY = 0xEDB88320;
	unsigned i, j, r;
	for (i = 0; i < 256; i++){
		for (r = i, j = 8; j; j--)
			r = r & 1? (r >> 1) ^ CRC_POLY: r >> 1;
		table[i] = r;
	}
	m_crc32 = 0;
}

unsigned crc32_noldor::ProcessCRC(void* pData, register int nLen){
	const unsigned CRC_MASK = 0xD202EF8D;
	register unsigned char* pdata = reinterpret_cast<unsigned char*>(pData);
	register unsigned crc = m_crc32;
	while (nLen--){
		crc = table[(unsigned char)crc ^ *pdata++] ^ crc >> 8;
		crc ^= CRC_MASK;
	}
	m_crc32 = crc;

	return m_crc32;
}

crc32_noldor g_HelpCRC;

CLocalization g_Localization;

CLocalization::CLocalization()
{

}

void CLocalization::Init( const char * szPathFile )
{
	m_Texts.clear();
	m_TextsScale.clear();
	if ( !szPathFile )
		return;	
	void * pData = GetVfsFileData( szPathFile );
	if( !pData )
		return;
	TiXmlDocument doc;
	doc.Parse( ( const char* )pData );
	FreeResFunc( pData );
	TiXmlElement * pLoc = doc.FirstChildElement( "Localization" );
	if ( !pLoc )
		return;
	TiXmlElement * pEl = pLoc->FirstChildElement( "Element" );
	while( pEl )
	{
		TiXmlElement * pToken = pEl->FirstChildElement( "Token" );
		TiXmlElement * pText = pEl->FirstChildElement( "Text" );
		TiXmlElement * pScale = pEl->FirstChildElement( "font_scale" );
		if ( pToken && pText )
		{
			const char * szToken = pToken->GetText();
			const char * szText = pText->GetText();
			if ( szToken && szText )
			{
				crc32_noldor crc_help = g_HelpCRC;
				crc_help.ProcessCRC( ( void* )szToken, strlen( szToken ) );
				//std::wstring srW = utf8tows( szText );
				m_Texts[ crc_help.m_crc32 ] = std::string( szText )/*srW*/;
				if ( pScale )
				{
					if ( const char * szTextScale = pScale->GetText() )
					{
						m_TextsScale[ crc_help.m_crc32 ] = atof( szTextScale );
					}
				}
			}else 
			if ( szToken && !szText )
			{
				crc32_noldor crc_help = g_HelpCRC;
				crc_help.ProcessCRC( ( void* )szToken, strlen( szToken ) );				
				m_Texts[ crc_help.m_crc32 ] = "";
				if ( pScale )
				{
					if ( const char * szTextScale = pScale->GetText() )
					{
						m_TextsScale[ crc_help.m_crc32 ] = atof( szTextScale );
					}
				}
			}			
		}
		pEl = pEl->NextSiblingElement( "Element" );
	}
}

unsigned CLocalization::GetHashCRC32( const char * szData, int len )
{
	if ( !szData )
		return 0;	
	crc32_noldor crc_help = g_HelpCRC;
	unsigned hash = crc_help.ProcessCRC( ( void* )szData, len );
	return hash;
}

std::string CLocalization::GetText( const char * szToken, float * pfScale )
{
	if ( pfScale )
	{
		*pfScale = 1.f;
	}
	if ( !szToken )
		return "";
	crc32_noldor crc_help = g_HelpCRC;
	crc_help.ProcessCRC( ( void* )szToken, strlen( szToken ) );
	std::map< unsigned, std::string >::iterator iter = m_Texts.find( crc_help.m_crc32 );
	if ( iter != m_Texts.end() )
	{
		if ( pfScale )
		{
			std::map< unsigned, float >::iterator iter2 = m_TextsScale.find( crc_help.m_crc32 );
			if ( iter2 != m_TextsScale.end() )
			{
				*pfScale = iter2->second;
			}
		}

		return iter->second;
	}
	
	return std::string( szToken );

// 	std::wstring srW = utf8tows( szToken );
// 	
// 	return srW;
}
