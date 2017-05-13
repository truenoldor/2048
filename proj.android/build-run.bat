call ndk-build NDK_MODULE_PATH=../../oxygine/ %*
call gradlew assembleDebug
call adb install -r build/outputs/apk/proj.android-debug.apk
call adb shell am start -n com.divol13.magic2048/com.divol13.magic2048.MainActivity