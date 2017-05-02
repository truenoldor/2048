adb shell stop
adb shell setprop log.redirect-stdio true
adb shell start
adb logcat MyApp:V SDL:V *:S FlurryAgent:* AdmobAdapter:*
rem adb logcat

