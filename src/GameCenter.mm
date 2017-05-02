
#import <GameKit/GameKit.h>

#import "AppDelegate.h"
#import "ios_wrapper.h"
#import "constants.h"

bool g_GCAuth = false;

@interface GCViewController : UIViewController <GKGameCenterControllerDelegate>

@end


@interface GCViewController ()
/// A flag indicating whether the Game Center features can be used after a user has been authenticated.
@property (nonatomic) BOOL gameCenterEnabled;

// This property stores the default leaderboard's identifier.
@property (nonatomic, strong) NSString *leaderboardIdentifier;

// It's used to authenticate a player, and display the login view controller if not authenticated.
-(void)authenticateLocalPlayer;

// It's used to display either the leaderboard or the achievements to the player.
-(void)showLeaderboardAndAchievements:(BOOL)shouldShowLeaderboard;


// It updates the achievements by calculating any required progress percentage and then reporting it.
-(void)updateAchievements;


// It resets the achievements.
-(void)resetAchievements;

- (void)removeAllSubviewsFromUIView:(UIView *)parentView;

@end

@implementation GCViewController

- (void)removeAllSubviewsFromUIView:(UIView *)parentView
{
    for (id child in [parentView subviews])
    {
        if ([child isMemberOfClass:[UIView class]])
        {
            [child removeFromSuperview];
        }
    }
}




- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


-(void)authenticateLocalPlayer{
    // Instantiate a GKLocalPlayer object to use for authenticating a player.
    GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
    
    localPlayer.authenticateHandler = ^(UIViewController *viewController, NSError *error){
        if (viewController != nil)
        {
            UIViewController * activeController = [ UIApplication sharedApplication ].keyWindow.rootViewController;
            
            if( [ activeController isKindOfClass:[ UINavigationController class ] ] )
            {
                activeController = [ ( UINavigationController* )activeController visibleViewController ];
            }
            
            [ activeController presentViewController:viewController animated:YES completion: ^(){} ];
            
            // If it's needed display the login view controller.
            // UIWindow *uiwindow = ((SDL_WindowData *) mMainWindow->driverdata)->uiwindow;
            // [uiwindow addSubview:viewController.view];
            //[self presentViewController:viewController animated:YES completion:nil];
        }
        else{
            if ([GKLocalPlayer localPlayer].authenticated) {
                // If the player is already authenticated then indicate that the Game Center features can be used.
                _gameCenterEnabled = YES;
                g_GCAuth = true;
                
                // Get the default leaderboard identifier.
                [[GKLocalPlayer localPlayer] loadDefaultLeaderboardIdentifierWithCompletionHandler:^(NSString *leaderboardIdentifier, NSError *error) {
                    
                    if (error != nil) {
                        NSLog(@"%@", [error localizedDescription]);
                    }
                    else{
                        _leaderboardIdentifier = leaderboardIdentifier;
                    }
                }];
            }
            
            else{
                _gameCenterEnabled = NO;
                g_GCAuth = false;
            }
        }
    };
}

- (IBAction)handleAnswer:(id)sender {
    
    int jjj = 0;
}

- (IBAction)showGCOptions:(id)sender {
    
    
        
}

-(void)showLeaderboardAndAchievements:(BOOL)shouldShowLeaderboard{
    // Init the following view controller object.
    GKGameCenterViewController *gcViewController = [[GKGameCenterViewController alloc] init];
    
    // Set self as its delegate.
    gcViewController.gameCenterDelegate = self;
    
    // Depending on the parameter, show either the leaderboard or the achievements.
    if (shouldShowLeaderboard) {
        gcViewController.viewState = GKGameCenterViewControllerStateLeaderboards;
        gcViewController.leaderboardIdentifier = gameCenterTable;
    }
    else{
        gcViewController.viewState = GKGameCenterViewControllerStateAchievements;
    }
    
    // Finally present the view controller.
    //UIWindow *uiwindow = ((SDL_WindowData *) mMainWindow->driverdata)->uiwindow;
    // [uiwindow addSubview:gcViewController.view];
    
    [self.view.window.rootViewController presentViewController:gcViewController animated:YES completion:nil];
}



-(void)updateAchievement:(NSString *)achievementIdentifier {
    
    float progressPercentage = 0.0;
    
    
    GKAchievement *mAchievement = nil;
    
    mAchievement = [[GKAchievement alloc] initWithIdentifier:achievementIdentifier];
    mAchievement.percentComplete = 100;
    NSArray *achievements = @[mAchievement];
    
    // Report the achievements.
    [GKAchievement reportAchievements:achievements withCompletionHandler:^(NSError *error) {
        if (error != nil) {
            NSLog(@"%@", [error localizedDescription]);
        }
    }];
}


-(void)resetAchievements{
    // Just call the next method to reset the achievements.
    [GKAchievement resetAchievementsWithCompletionHandler:^(NSError *error) {
        if (error != nil) {
            NSLog(@"%@", [error localizedDescription]);
        }
    }];
}


#pragma mark - GKGameCenterControllerDelegate method implementation

-(void)gameCenterViewControllerDidFinish:(GKGameCenterViewController *)gameCenterViewController
{
    g_UIwindow = [ g_Application keyWindow ];
    [gameCenterViewController dismissViewControllerAnimated:YES completion:nil];
    UIWindow *uiwindow = g_UIwindow;
    [self removeAllSubviewsFromUIView:uiwindow];
}

@end

void SDL_resetAchievements ()
{
    [GKAchievement resetAchievementsWithCompletionHandler:^(NSError *error) {
        if (error != nil) {
            NSLog(@"%@", [error localizedDescription]);
        }
    }];
}

void SDL_SetAchievement (const char *a, int mValue)
{
  //  NSString *achievementIdentifier = [[NSString alloc] initWithUTF8String:a] ;
   
    NSString *achievementIdentifier =  [NSString stringWithFormat: @"%s", a];
    
    
    GKAchievement *mAchievement = nil;
    
    mAchievement = [[GKAchievement alloc] initWithIdentifier:achievementIdentifier];
    mAchievement.percentComplete = mValue;
    mAchievement.showsCompletionBanner = YES;
    
    NSArray *achievements = @[mAchievement];
    
    // Report the achievements.
    [GKAchievement reportAchievements:achievements withCompletionHandler:^(NSError *error) {
        if (error != nil) {
            NSLog(@"%@", [error localizedDescription]);
        }
    }];
    
    //[ achievements release ];
    
}

GCViewController *gViewController = NULL;

void SDL_ShowAchievements (const char * title)
{
    
    g_UIwindow = [ g_Application keyWindow ];
    
    GCViewController *mViewController = gViewController;//[[GCViewController alloc] init];
    UIWindow *uiwindow = g_UIwindow;
    [uiwindow addSubview:mViewController.view];
    
    [mViewController showLeaderboardAndAchievements:FALSE];
    
}

void SDL_AddScores( int v )
{
   /* GKScore *gScore = [[GKScore alloc]
                       initWithLeaderboardIdentifier:gameCenterTable];
    gScore.value = v;
    */
    
    GKScore *reportScore = [[GKScore alloc] initWithLeaderboardIdentifier:gameCenterTable];
    reportScore.value = v;
    
    NSArray *scores = @[reportScore];
    
    [GKScore reportScores:scores withCompletionHandler:nil];
}

void SDL_ShowLeaders()
{
    g_UIwindow = [ g_Application keyWindow ];
    
    GCViewController *mViewController = gViewController;//[[GCViewController alloc] init];
    UIWindow *uiwindow = g_UIwindow;
    [uiwindow addSubview:mViewController.view];
    
    [mViewController showLeaderboardAndAchievements:TRUE];
}

void SDL_authenticateLocalPlayer ()
{    
    
    GCViewController *mViewController = [[GCViewController alloc] init];
    gViewController = mViewController;
    
    [mViewController authenticateLocalPlayer];
}





void SDL_SetString (const char * s, const char *v)
{
    NSString *input = [[NSString alloc] initWithUTF8String:s];
    NSString *value = [[NSString alloc] initWithUTF8String:v];
    
    NSUserDefaults *prefs = [NSUserDefaults standardUserDefaults];
    [prefs setObject:value forKey:input];
    
    //NSString *textToLoad = [prefs stringForKey:input];
}

const char * SDL_GetString (const char * s)
{
    
    NSUserDefaults *prefs = [NSUserDefaults standardUserDefaults];
    
    NSString *input = [[NSString alloc] initWithUTF8String:s];
    NSString *textToLoad = [prefs stringForKey:input];
    const char *cString = [textToLoad cStringUsingEncoding:NSASCIIStringEncoding];
    
    return cString;
}




