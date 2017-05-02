LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := main

#SDK_ROOT points to folder with SDL and oxygine-framework
LOCAL_SRC_FILES := ../../../../oxygine//SDL/src/main/android/SDL_android_main.c

src := ../../../src
srcAe := ../../../../aemovie/src
incAe := ../../../../aemovie/include/

LOCAL_SRC_FILES += $(src)/entry_point.cpp\
	$(src)/analytics.cpp\
	$(src)/Android_Wrapper.cpp\
	$(src)/bezier.cpp\
	$(src)/blocking.cpp\
	$(src)/ChangePersScreen.cpp\
	$(src)/coroutines.cpp\
	$(src)/definitions.cpp\
	$(src)/DownPanel.cpp\
	$(src)/GameOverScreen.cpp\
	$(src)/GameScreen.cpp\
	$(src)/Helper.cpp\
	$(src)/ios_wrapper.cpp\
	$(src)/Localization.cpp\
	$(src)/LogoScreen.cpp\
	$(src)/MainScreen.cpp\
	$(src)/Monster.cpp\
	$(src)/MovieResourceManager.cpp\
	$(src)/Player.cpp\
	$(src)/ToolBarDeath.cpp\
	$(src)/UpPanel.cpp\
	$(src)/achievements.cpp\

LOCAL_SRC_FILES += $(srcAe)/movie_data.c\
	$(srcAe)/movie_instance.c\
	$(srcAe)/movie_math.c\
	$(srcAe)/movie_node.c\
	$(srcAe)/movie_transformation.c\
	
LOCAL_SRC_FILES += $(src)/player_ae/AERenderer.cpp\
	$(src)/player_ae/AEMovie.cpp\
	

LOCAL_SRC_FILES += $(src)/tinyxml_p/bind_dynamics.cpp\
	$(src)/tinyxml_p/tinybind.cpp\
	$(src)/tinyxml_p/tinystr.cpp\
	$(src)/tinyxml_p/tinyxml.cpp\
	$(src)/tinyxml_p/tinyxmlerror.cpp\
	$(src)/tinyxml_p/tinyxmlparser.cpp\
	
LOCAL_SRC_FILES += $(src)/ui/Button2.cpp\
	$(src)/ui/Modal.cpp\

	

LOCAL_STATIC_LIBRARIES := oxygine-framework_static
LOCAL_SHARED_LIBRARIES := SDL2


LOCAL_STATIC_LIBRARIES += oxygine-billing_static
LOCAL_STATIC_LIBRARIES += oxygine-admob_static
LOCAL_STATIC_LIBRARIES += oxygine-movie_static
LOCAL_STATIC_LIBRARIES += oxygine-sound_static
LOCAL_STATIC_LIBRARIES += oxygine-framework_static

LOCAL_SHARED_LIBRARIES := SDL2

include $(BUILD_SHARED_LIBRARY)


#import from NDK_MODULE_PATH defined in build.cmd
$(call import-module, oxygine-billing)
$(call import-module, oxygine-admob)
$(call import-module, oxygine-movie)
$(call import-module, oxygine-sound)
$(call import-module, oxygine-framework)

