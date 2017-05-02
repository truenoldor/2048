#import <Foundation/Foundation.h>


#import "Flurry.h"
#import "AppDelegate.h"
#import "analitycs_wrap.h"

#import "ios_wrapper.h"

#import "constants.h"

#import <Firebase/Firebase.h>
#import <GoogleMobileAds/GoogleMobileAds.h>


//@import GoogleMobileAds;
//@import Firebase


bool g_ADSoundEnable = true;

bool g_AppRateProcess = false;

UIWindow* g_UIwindow = nil;
AppDelegate * g_Delegate = NULL;
UIApplication * g_Application = NULL;

std::string iosGetLanguage()
{
	NSString * language = [[NSLocale preferredLanguages] objectAtIndex:0];
    NSString *textType = [language substringToIndex:2];
    std::string lang = std::string([textType UTF8String]);
	return lang;
}


//flurry
void EventFlurry (const char *e, const char * par1, const char * par2)
{
    NSString *message =  [NSString stringWithFormat: @"%s", e];
    
    NSString *parN1 = [NSString stringWithFormat: @"%s", par1];
    
    NSString *parN2 = [NSString stringWithFormat: @"%s", par2];
    
    NSDictionary *articleParams = [NSDictionary dictionaryWithObjectsAndKeys:
                                   parN2, @"Par 2",
                                   parN1, @"Par 1",
                                   nil];
    
    FlurryEventRecordStatus ss = [Flurry logEvent:message withParameters:articleParams];
    
}

void SDL_ChartBoost()
{
    //[Chartboost showMoreApps:CBLocationDefault];
}

void SDL_RateApp( const char * title, const char * body, const char * yesBtn, const char * cancelBtn )
{
    	
	NSString *ttl =  [NSString stringWithUTF8String: title];
    NSString *message =  [NSString stringWithUTF8String: body];
    
    NSString *y = [NSString stringWithUTF8String: yesBtn];
    
    NSString *cb = [NSString stringWithUTF8String: cancelBtn];
    
    [ g_Delegate rateStart: ttl body: message yesBtnText: y cancelBtnText: cb ];
}

void SDL_ShowFacebookPage ()
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:faceBookLink ]];
}

void SDL_AdMobStart( int h )
{
    UIWindow * win = [ g_Application keyWindow ];
    CGRect r = win.frame;
    g_Delegate.bannerView = [[ GADBannerView alloc ] initWithFrame:CGRectMake(0, r.size.height - h,r.size.width , h) ];
    
    
    g_Delegate.bannerView.adUnitID = addmob_ads_id;
    g_Delegate.bannerView.rootViewController = win.rootViewController;
    [ g_Delegate.bannerView setHidden:true ];
    
    [ win.rootViewController.view addSubview:g_Delegate.bannerView ];
    
    GADRequest *request = [GADRequest request];
    request.testDevices = nil;
    [g_Delegate.bannerView loadRequest:request];
}

void SDL_AdMobHide( bool v )
{
    if( g_Delegate.bannerView )
        [ g_Delegate.bannerView setHidden:v ];
}

NSString * gPushMessage = NULL;
NSString * gDeviceToken = NULL;

bool isPushAllow = false;
bool isPushSend = false;
bool isSendNotiInit = false;

void sendPush()
{
    //[ Chartboost showmoreapps ];
    isPushSend = true;
    
    const char* notificationID = "notification_24";
    
    NSString* objID = [NSString stringWithUTF8String: notificationID ];
    UILocalNotification* notificationToCancel = nil;
    for(UILocalNotification* localNotif in [[UIApplication sharedApplication] scheduledLocalNotifications])
    {
        if([[localNotif.userInfo objectForKey:@"name"] isEqualToString: objID])
        {
            notificationToCancel = localNotif;
            break;
        }
    }
    
    if(notificationToCancel)
    {
        [[UIApplication sharedApplication] cancelLocalNotification: notificationToCancel];
    }
    
    
    NSMutableDictionary* infoDict = nil;
    infoDict = [NSMutableDictionary dictionaryWithCapacity:2];
    
     NSString* nameString = nil;
    
     nameString = [NSString stringWithUTF8String:notificationID];
    
    [infoDict setValue:nameString forKey:@"name"];
    
    UILocalNotification* localNotification = [[UILocalNotification alloc] init];
    int fireTime = notificationsTimeInseconds;
    localNotification.fireDate = [NSDate dateWithTimeIntervalSinceNow:fireTime]; // 60 * 60 * 24
    localNotification.alertBody = gPushMessage;
    localNotification.timeZone = [NSTimeZone defaultTimeZone];
    localNotification.userInfo = infoDict;
   
    [[UIApplication sharedApplication] scheduleLocalNotification:localNotification];
}

void SDL_SendPushNotification( const char * message )
{
    gPushMessage = [[NSString alloc] initWithUTF8String:message];
    isSendNotiInit = true;
    //if( isPushAllow )
        sendPush();
}

bool g_StartProcessVideoAD = false;
bool g_CompleteVideo = false;

void SDL_StartVideoAD()
{
    g_StartProcessVideoAD = true;
    g_ADSoundEnable = false;
    
   
    
    VungleSDK* sdk = [VungleSDK sharedSDK];
    NSError *error;
    
    UIViewController * view = [[[UIApplication sharedApplication]keyWindow]rootViewController];
    
    [sdk playAd:view error:&error];
    
    
    
    
    //
    //[Chartboost showRewardedVideo:CBLocationMainMenu];
}

@implementation SDLUIKitDelegate (AppDelegate)


+ (NSString *)getAppDelegateClassName
{
    return @"AppDelegate";
}

@end

@implementation AppDelegate

@synthesize bannerView;

@synthesize window=_window;

-(void)rateStart:( NSString* )title body:( NSString* )message yesBtnText:( NSString* )y cancelBtnText:( NSString* )c
{
    UIAlertView *deleteAlarmAlert = [[UIAlertView alloc] initWithTitle:title
                                                               message:message
                                                              delegate:self
                                                     cancelButtonTitle:y
                                                     otherButtonTitles:c, nil];
    [deleteAlarmAlert show];

}

-(void)productViewControllerDidFinish:(SKStoreProductViewController *)viewController
{
    if( storeController )
        [storeController dismissViewControllerAnimated:YES completion:nil];
    
    storeController = nil;
}

- (void)presentAppStoreForID:(NSNumber *)appStoreID withDelegate:(id<SKStoreProductViewControllerDelegate>)delegate
{
    if(NSClassFromString(@"SKStoreProductViewController")) { // Checks for iOS 6 feature.
        
        storeController = [[SKStoreProductViewController alloc] init];
        storeController.delegate = delegate; // productViewControllerDidFinish
        
        // Example App Store ID (e.g. for Words With Friends)
        // @322852954
        
        UIViewController * view = [[[UIApplication sharedApplication]keyWindow]rootViewController];
        
        [storeController loadProductWithParameters:@{ SKStoreProductParameterITunesItemIdentifier: appStoreID }
                                   completionBlock:^(BOOL result, NSError *error) {
                                       if (result) {
                                           [view presentViewController:storeController animated:YES completion:nil];
                                       } else {
                                           [[[UIAlertView alloc] initWithTitle:@"Uh oh!" message:@"There was a problem opening the app store" delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles: nil] show];
                                       }
                                   }];
        
    } else { // Before iOS 6, we can only open the App Store URL
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"http://itunes.apple.com/app/id%@",appStoreID]]];
    }
}

- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    if(buttonIndex == 0)
    {
        NSNumber * num = [NSNumber numberWithInt:appRateLinkID];
        [ self presentAppStoreForID:num withDelegate:self ];
        //[[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"https://itunes.apple.com/app///id512939461" ]];
    }
    else{
        //do nothing
    }
    g_AppRateProcess = false;
}

- (void)vungleSDKwillShowAd
{
    
}

- (void)vungleSDKwillCloseAdWithViewInfo:(NSDictionary *)viewInfo willPresentProductSheet:(BOOL)willPresentProductSheet
{
    g_StartProcessVideoAD = false;
    g_ADSoundEnable = true;
    g_CompleteVideo = true;
}

- (void)vungleSDKwillCloseProductSheet:(id)productSheet __attribute__((deprecated))
{
    
}

- (void)vungleSDKAdPlayableChanged:(BOOL)isAdPlayable
{
    
}

/*
- (void)didFailToLoadRewardedVideo:(NSString *)location withError:(CBLoadError)error
{
    g_StartProcessVideoAD = false;
    g_CompleteVideo = true;
    g_ADSoundEnable = true;
}

- (void)didCloseRewardedVideo:(CBLocation)location
{
    g_StartProcessVideoAD = false;
    g_ADSoundEnable = true;
}

- (void)didCompleteRewardedVideo:(CBLocation)location withReward:(int)reward
{
    g_StartProcessVideoAD = false;
    g_CompleteVideo = true;
    g_ADSoundEnable = true;
}*/

- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
    NSLog(@"Did Register for Remote Notifications with Device Token (%@)", deviceToken);
    gDeviceToken = [[NSString alloc] initWithData:deviceToken encoding:NSUTF8StringEncoding];
    isPushAllow = true;
   // if( !isPushSend && isSendNotiInit )
    //    sendPush();
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
    NSLog(@"Did Fail to Register for Remote Notifications");
    NSLog(@"%@, %@", error, error.localizedDescription);
    
}








- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
   
    g_Delegate = self;
   //busal return [super application:application didFinishLaunchingWithOptions:launchOptions];
    
    //[Chartboost startWithAppId:chartboost_app_id appSignature:chartboost_signature delegate:self];
    
    [Flurry startSession:flurryApiKey];
    
    
    [FIRApp configure];
    // Initialize Google Mobile Ads SDK
    [GADMobileAds configureWithApplicationID:admob_app_id];
    
   
    
    
    
    
if ([[[ UIDevice currentDevice] systemVersion] floatValue] >= 8.0)
{
    [[UIApplication sharedApplication] registerUserNotificationSettings:[UIUserNotificationSettings settingsForTypes:(UIUserNotificationTypeSound | UIUserNotificationTypeAlert | UIUserNotificationTypeBadge) categories:nil]];
}
else
{
     [application registerForRemoteNotificationTypes:(UIRemoteNotificationTypeAlert | UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound)];
}
    
   
 



    /*[[GCHelper defaultHelper] authenticateLocalUserOnViewController:self setCallbackObject:self withPauseSelector:@selector(authenticationRequired)];
    [[GCHelper defaultHelper] registerListener:self];*/
    
   // GameCenterAuth();
    
    
    
  //  g_UIwindow = [ self window ];
    
   // [ Flurry logEvent:@"test flurry from my code message after start session"];
    
    g_Application = application;
    
     NSString* appID = vungle_api;
    VungleSDK* sdk = [VungleSDK sharedSDK];
    [ sdk setDelegate: self];
    // start vungle publisher library
    [sdk startWithAppId:appID];
    
    return [super application:application didFinishLaunchingWithOptions:launchOptions];
}



void SDL_StartFB()
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:faceBookLink ]];
}


- (void)applicationDidBecomeActive:(UIApplication *)application
{
    [super applicationDidBecomeActive:application];
    
   
}


@end
