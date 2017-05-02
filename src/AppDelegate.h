#ifndef Match3Quest_Match3QuestDelegate_h
#define Match3Quest_Match3QuestDelegate_h


#import "../../../SDL/src/video/uikit/SDL_uikitappdelegate.h"
#import <VungleSDK/VungleSDK.h>
#import <StoreKit/StoreKit.h>

@interface SDLUIKitDelegate(AppDelegate)

@end

@class GADBannerView;

@interface AppDelegate : SDLUIKitDelegate< VungleSDKDelegate, SKStoreProductViewControllerDelegate >

{
    SKStoreProductViewController *storeController;
}
    @property(strong, nonatomic) UIWindow* window;

    -(IBAction)showAdWithOptions;

-(IBAction)showIncentivizedAd;

@property(nonatomic, strong) IBOutlet GADBannerView *bannerView;

-(void)rateStart:( NSString* )title body:( NSString* )message yesBtnText:( NSString* )y cancelBtnText:( NSString* )c;

@end




extern UIWindow* g_UIwindow;

extern AppDelegate * g_Delegate;

extern UIApplication * g_Application;





#endif
