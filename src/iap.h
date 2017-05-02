//
//  iap.h
//  MysteryChronicles3_ios
//
//  Created by Мак on 22.09.16.
//  Copyright © 2016 Mac. All rights reserved.
//

#ifndef iap_h
#define iap_h

void Purchase_IAP( const char * id );
void Restore_IAP();

extern bool areAdsRemoved;
extern bool g_HasBuyProduct;
extern bool g_IAPRequestProcess;

#endif /* iap_h */
