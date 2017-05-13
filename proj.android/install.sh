#!/usr/bin/env sh
adb install -r bin/2048-debug.apk
adb shell am start -n com.divol13.magic2048/com.divol13.magic2048.MainActivity
