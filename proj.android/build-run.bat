call ndk-build NDK_MODULE_PATH=../../oxygine/ %*
call gradlew assembleDebug
call adb install -r build/outputs/apk/proj.android-debug.apk
call adb shell am start -n org.oxygine.game2048/org.oxygine.game2048.MainActivity