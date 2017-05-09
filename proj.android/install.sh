#!/usr/bin/env sh
adb install -r bin/2048-debug.apk
adb shell am start -n org.oxygine.game2048/org.oxygine.game2048.MainActivity
