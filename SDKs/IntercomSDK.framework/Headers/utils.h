#ifndef SDK_UTILS_H_
#define SDK_UTILS_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

UIKIT_EXTERN NSString* DictionaryToJSON(NSDictionary* dictionary);

UIKIT_EXTERN NSDictionary* JSONToDictionary(NSString* json);

#endif  // SDK_UTILS_H_
