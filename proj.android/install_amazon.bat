adb shell stop
adb shell setprop log.redirect-stdio false
adb shell start
adb install -r build/outputs/apk/proj.android-release.apk
adb shell am start -n com.divol13.magic2048/com.divol13.magic2048.MainActivity