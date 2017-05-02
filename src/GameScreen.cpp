#include "GameScreen.h"
#include "Helper.h"
#include "MainScreen.h"
#include "ios_wrapper.h"
#include "achievements.h"
#include "Android_Wrapper.h"

#if __ANDROID__
#include "billing.h"
#endif

namespace oxygine
{
    GameScreen::GameScreen()
    {

    }

    GameScreen::~GameScreen()
    {

    }

  
    void GameScreen::Init()
    {

    }

    void GameScreen::update(const UpdateState& us)
    {
        Modal::update(us);
    }

    void GameScreen::doLoop()
    {
        while (1)
        {
            std::string action = waitAction();

            if (action == "close")
            {
                g_SessionEnd = true;

                return;
            }
        }
    }
}
