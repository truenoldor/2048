adb shell stop
adb shell setprop log.redirect-stdio false
adb shell start
adb install -r build/outputs/apk/proj.android-release.apk
adb shell am start -n org.oxygine.2048/org.oxygine.2048.MainActivity