
#import <MessageUI/MessageUI.h>
//#include "SDL_uikitwindow.h"
#import <GameKit/GameKit.h>

#import "AppDelegate.h"

#import "constants.h"

@interface MailViewController : UIViewController <MFMailComposeViewControllerDelegate>



@end

@implementation MailViewController
@end



UIWindow *mail_window = nil;

MFMailComposeViewController *mailer = nil;

@interface Mail_ViewDelegate : NSObject <MFMailComposeViewControllerDelegate>
{
    
    
@public
    bool bFinished;
}
//- (void)mailComposeController:(MFMailComposeViewController *)controller didFinishWithResult:(MFMailComposeResult)res;
//- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex;

@end

@implementation Mail_ViewDelegate

- (void)mailComposeController:(MFMailComposeViewController *)controller didFinishWithResult:(MFMailComposeResult)result error:(NSError *)error
{
    bFinished = true;
    // [mail_window makeKeyAndVisible];
   // [mailer.mailComposeDelegate release];
   // [mailer release];
    mailer = nil;
    
   // [mail_window release];
    mail_window = nil;
    //SDL_ShowWindow(mMainWindow);
    
    
}


@end

int SDL_emailResult = 0;



@interface SimpleEmailViewController : UIViewController <MFMailComposeViewControllerDelegate> // Add the delegate
{
    MFMailComposeViewController *mc;
}

- (void)showEmail:(NSString *)mHeader mBody:(NSString *)mBody ;
- (void)removeAllSubviewsFromUIView:(UIView *)parentView;



@end

@implementation SimpleEmailViewController

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


- (void)showEmail:(NSString *)mHeader mBody:(NSString *)mBody  {
    
    if (![MFMailComposeViewController canSendMail])
    {
        UIAlertView *alert=[[ UIAlertView alloc]
                            initWithTitle:@"Cannot send email"
                            message: @"Please check internet connection and email set up"
                            delegate: self
                            cancelButtonTitle:@"Ok"
                            otherButtonTitles: nil];
        
        [alert show];
        return;
    }
    
    // Email Subject
    NSString *emailTitle = mailTitle;
    // Email Content
     NSString *messageBody = mailBody;
    // To address
    
    mc = [[MFMailComposeViewController alloc] init];
    mc.mailComposeDelegate = self;
    //[mc setSubject:mHeader];
    //[mc setMessageBody:mBody isHTML:YES];
    [mc setSubject:emailTitle];
    [mc setMessageBody:messageBody isHTML:YES];
    
    
    [mc setToRecipients:[ NSArray arrayWithObject:mailRecipients ]];
    
    // Present mail view controller on screen
    [self.view.window.rootViewController presentViewController:mc animated:YES completion:NULL];
    
    
}

SimpleEmailViewController *mViewController = NULL;


- (void) mailComposeController:(MFMailComposeViewController *)controller didFinishWithResult:(MFMailComposeResult)result error:(NSError *)error
{
    
    
    SDL_emailResult = 1;
    
    switch (result)
    {
        case MFMailComposeResultCancelled:
            NSLog(@"Mail cancelled");
            SDL_emailResult = 1;
            break;
        case MFMailComposeResultSaved:
            NSLog(@"Mail saved");
            SDL_emailResult = 2;
            break;
        case MFMailComposeResultSent:
            NSLog(@"Mail sent");
            SDL_emailResult = 3;
            break;
        case MFMailComposeResultFailed:
            NSLog(@"Mail sent failure: %@", [error localizedDescription]);
            SDL_emailResult = 4;
            break;
        default:
            break;
    }
    
    
    // Close the Mail Interface
    [mc dismissViewControllerAnimated:YES completion:NULL];
    
    // [mc release];
    
     g_UIwindow = [ g_Application keyWindow ];
    UIWindow *uiwindow = g_UIwindow;
    //[uiwindow makeKeyAndVisible];
    
    [self removeAllSubviewsFromUIView:uiwindow];
    
    UIViewController *uicontroller = [ uiwindow rootViewController ];
    //  [self presentViewController:uicontroller animated:YES completion:NULL];
    [mc.view removeFromSuperview];
    uiwindow.rootViewController = uicontroller;
    
    [mc resignFirstResponder];
    // [[self view] removeFromSuperview];
    
    //[self.navigationController popViewControllerAnimated:YES];
    
    // [self removeFromParentViewController];
    
    mViewController = NULL;
}
@end

int SDL_SendEmailResult ()
{
    return SDL_emailResult;
}



void SDL_SendEmail (const char * a)
{
    if (SDL_emailResult == -1) return;
    
    SDL_emailResult = -1;
    //if (mail_window) return;
    NSString *header = [[NSString alloc] initWithUTF8String:a];
    NSString *emailBody = [[NSString alloc] initWithUTF8String:a];
    
    
    mViewController = [[SimpleEmailViewController alloc] init];
    
    g_UIwindow = [ g_Application keyWindow ];
    UIWindow *uiwindow = g_UIwindow;
   
    [uiwindow addSubview:mViewController.view];
    [mViewController showEmail:header mBody:emailBody];
    return;
    
    mail_window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    
    
    
    
    
    if ([MFMailComposeViewController canSendMail])
    {
        mailer = [[MFMailComposeViewController alloc] init];
        Mail_ViewDelegate *mDelegate = [[Mail_ViewDelegate alloc] init];
        mailer.mailComposeDelegate = mDelegate;
        
        [mailer setSubject:header];
        
        //SDL_HideWindow(mMainWindow);
        
        //NSString *emailBody = @"Have you seen the MobileTuts+ web site?";
        [mailer setMessageBody:emailBody isHTML:YES];
        
        mail_window.rootViewController = mailer;
        [mail_window addSubview:mailer.view];
        [mail_window makeKeyAndVisible];
//         [mail_window setToRecipients:recipients];
        
        mDelegate->bFinished = false;
        /*   while (!mDelegate->bFinished ) {
         [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
         }
         
         [mailer.mailComposeDelegate release];
         [mailer release];
         [mail_window release];*/
    }
    
    // MailViewController * mailer = [[MailViewController alloc] init];
    //[mailer openMail:[[UIApplication sharedApplication] delegate]];
    
    //NSString *textToLoad = [prefs stringForKey:input];
}


