#pragma once

#include "definitions.h"
#include "ui/Button2.h"
#include "ui/Modal.h"

namespace oxygine
{
	DECLARE_SMART(LogoScreen, spLogoScreen);
	class LogoScreen: public Modal
	{
	public:
		//DECLARE_COPYCLONE_NEW(LogoScreen);
		LogoScreen();
		virtual ~LogoScreen();

		virtual void			update(const UpdateState& us)OVERRIDE;
		virtual void			doLoop()OVERRIDE;
		void					PreInit();
		void					Init();
		void					Release();

	private:
		void					OnPlay( Event * pEvent );

	private:

		float					m_LoadingTime;
		bool					m_IsLoading;

	};
};
