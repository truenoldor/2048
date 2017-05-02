#import <Foundation/Foundation.h>
//#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import "iap.h"

#include <assert.h>
#include "core/oxygine.h"
#include "core/Object.h"
#include "core/ThreadDispatcher.h"
#include "core/oxygine.h"


#import "ios_wrapper.h"

#import <StoreKit/StoreKit.h>

#import "constants.h"

// in app

bool areAdsRemoved = false;

bool g_HasBuyProduct = false;

bool g_IAPRequestProcess = false;

NSString * localizedMoneyString = nil;

@interface IPA_PURCHASE : NSObject  <SKProductsRequestDelegate, SKPaymentTransactionObserver >

{
    SKProductsRequest *productsRequest;
}
- (IBAction)restore;
- (IBAction)tapsRemoveAds:( NSString* )prodID;

@end



static IPA_PURCHASE * gTransactor = NULL;

void Purchase_IAP( const char * id )
{
    gTransactor = !gTransactor ? [ [ IPA_PURCHASE alloc ] init ] : gTransactor;
    
    g_IAPRequestProcess = true;
    
    NSString *prId =  [NSString stringWithFormat: @"%s", id];
    
    [gTransactor tapsRemoveAds:prId];
}
void Restore_IAP()
{
    g_IAPRequestProcess = true;
    gTransactor = !gTransactor ? [ [ IPA_PURCHASE alloc ] init ] : gTransactor;
    [gTransactor restore];
}

@implementation IPA_PURCHASE //the name of your view controller (same as above)


- (void)doRemoveAds{
   // [self.view setBackgroundColor:[UIColor blueColor]];
    //g_HasBuyProduct = true;
    areAdsRemoved = YES;
    //set the bool for whether or not they purchased it to YES, you could use your own boolean here, but you would have to declare it in your .h file
    
    g_IAPRequestProcess = false;
    
    [[NSUserDefaults standardUserDefaults] setBool:areAdsRemoved forKey:@"areAdsRemoved"];
    //use NSUserDefaults so that you can load wether or not they bought it
    [[NSUserDefaults standardUserDefaults] synchronize];
}

- (IBAction)tapsRemoveAds:( NSString* )prodID
{
    NSLog(@"User requests to remove ads");
    
    if([SKPaymentQueue canMakePayments]){
        NSLog(@"User can make payments");
        
        //If you have more than one in-app purchase, and would like
        //to have the user purchase a different product, simply define
        //another function and replace kRemoveAdsProductIdentifier with
        //the identifier for the other product
        
        NSString *message =  prodID;
        NSSet *productIdentifiers = [NSSet setWithObject:message];
        productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productIdentifiers];
        productsRequest.delegate = self;
        [productsRequest start];
        
        /*SKProductsRequest *productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:[NSSet setWithObject:@"com.mcdomini.iaptest.iap001"]];
        productsRequest.delegate = self;
        [productsRequest start];*/
        
    }
    else{
        g_IAPRequestProcess = false;
        const char * productID = [prodID UTF8String];		
        oxygine::BillingInternal::CallEventPurchaseFail( productID );
        NSLog(@"User cannot make payments due to parental controls");
        //this is called the user cannot make payments, most likely due to parental controls
    }
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response{
    SKProduct *validProduct = nil;
    int count = [response.products count];
    if(count > 0){
        validProduct = [response.products objectAtIndex:0];
        
        NSNumberFormatter *formatter = [[NSNumberFormatter alloc] init] ;
        [formatter setNumberStyle:NSNumberFormatterCurrencyStyle];
        [formatter setLocale:validProduct.priceLocale];
        
        NSLocale* storeLocale = validProduct.priceLocale;
        NSString *storeCountry = (NSString*)CFLocaleGetValue((CFLocaleRef)storeLocale, kCFLocaleCountryCode);
        
       // localizedMoneyString = [[ validProduct price ] stringValue];
        localizedMoneyString =  [NSString stringWithFormat: @"%@ %@", storeCountry, [[ validProduct price ] stringValue]];
        
        
        
        NSLog(@"Products Available!");
        [self purchase:validProduct];
    }
    else if(!validProduct){
        g_IAPRequestProcess = false;
        NSLog(@"No products available");
        int invcount = [response.invalidProductIdentifiers count];
        if( invcount > 0 )
        {
            NSString * prod = [response.invalidProductIdentifiers objectAtIndex:0];
            oxygine::BillingInternal::CallEventPurchaseFail( [ prod UTF8String ] );
        }
        
        
        //this is called if your product id is not valid, this shouldn't be called unless that happens.
    }
}

- (void)purchase:(SKProduct *)product{
    
    SKMutablePayment *payment = [SKMutablePayment paymentWithProduct:product];
    
    //Populate applicationUsername with your customer's username on your server
    payment.applicationUsername = @"userdata";//[self hashedValueForAccountName:@"userNameOnYourServer"];
    
    // Submit payment request
    //[[SKPaymentQueue defaultQueue] addPayment:payment];
    
   // SKPayment *payment = [SKPayment paymentWithProduct:product];
    
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    
   // int cnt = [[[ SKPaymentQueue defaultQueue] transactions ] count];
    
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

- (IBAction) restore{
    //this is called when the user restores purchases, you should hook this up to a button
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

- (void) paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue
{
    NSLog(@"received restored transactions: %i", queue.transactions.count);
    for(SKPaymentTransaction *transaction in queue.transactions){
        if(transaction.transactionState == SKPaymentTransactionStateRestored){
           
            NSString *productID = transaction.payment.productIdentifier;
            //called when the user successfully restores a purchase
            NSLog(@"Transaction state -> Restored");
            //g_HasBuyProduct = true;
            //if you have more than one in-app purchase product,
            //you restore the correct product for the identifier.
            //For example, you could use
            //if(productID == kRemoveAdsProductIdentifier)
            //to get the product identifier for the
            //restored purchases, you can use
            //
            //NSString *productID = transaction.payment.productIdentifier;
            //[self doRemoveAds];
            
           /* const char * trID = transaction.transactionIdentifier.UTF8String;
            
            const char * productIDStr = [transaction.payment.productIdentifier UTF8String];
            
            
            const char * price = localizedMoneyString.UTF8String;
            oxygine::BillingInternal::CallEventPurchase( productIDStr, price );*/
            
            [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
            g_IAPRequestProcess = false;
            break;
        }
    }
    g_IAPRequestProcess = false;
}

-( void )request:( SKRequest* )request didFailWithError:(NSError *)error
{
    g_IAPRequestProcess = false;
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions{
    for(SKPaymentTransaction *transaction in transactions){
        switch(transaction.transactionState){
          /*  case SKPaymentTransactionStatePurchasing:
                [self showTransactionAsInProgress:transaction deferred:NO];
                break;
            */
            case SKPaymentTransactionStatePurchasing: NSLog(@"Transaction state -> Purchasing");
                //called when the user is in the process of purchasing, do not add any of your own code here.
                break;
            case SKPaymentTransactionStatePurchased:
            {
                //this is called when the user has successfully purchased the package (Cha-Ching!)
                [self doRemoveAds]; //you can add your code for what you want to happen when the user buys the purchase here, for this tutorial we use removing ads
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                
                NSLog(@"Transaction state -> Purchased");
                g_IAPRequestProcess = false;
                
                const char * trID = transaction.transactionIdentifier.UTF8String;
                
                const char * productID = [transaction.payment.productIdentifier UTF8String];
                
                
				const char * price = localizedMoneyString.UTF8String;
                oxygine::BillingInternal::CallEventPurchase( productID, price );
                localizedMoneyString = nil;
                
               // [self completeTransaction:transaction];
                
               // PurchasedEvent ev(data, sign);
                //_dispatcher->dispatchEvent(&ev);
                
            }
                break;
            case SKPaymentTransactionStateRestored:
            {
                NSLog(@"Transaction state -> Restored");
                //add the same code as you did from SKPaymentTransactionStatePurchased here
                
                
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                const char * productID = [transaction.payment.productIdentifier UTF8String];
				const char * price = "n";
                oxygine::BillingInternal::CallEventPurchase( productID, price );
                g_IAPRequestProcess = false;
               // [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
                
            }
                break;
            case SKPaymentTransactionStateFailed:
            {
                //called when the transaction does not finish
                int code = transaction.error.code;
                
                const char * errorM = [ transaction.error.domain UTF8String ];
                if(transaction.error.code == SKErrorPaymentCancelled)
                {
                    NSLog(@"Transaction state -> Cancelled");
                    //the user cancelled the payment ;(
                }
                
                [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                g_IAPRequestProcess = false;
                const char * productID = [transaction.payment.productIdentifier UTF8String];
                oxygine::BillingInternal::CallEventPurchaseFail( productID );
                // [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
            }
                break;
        }
    }
}

@end



