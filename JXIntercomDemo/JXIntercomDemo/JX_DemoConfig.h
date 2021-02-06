//
//  JX_DemoConfig.h
//  JXIntercomDemo
//
//  Created by Nansen on 2021/2/6.
//  Copyright © 2021 jingxi. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface JX_DemoConfig : NSObject

+ (NSString *)sipURL;
+ (NSString *)transitURL;
+ (NSString *)channel;

+ (NSString *)appid;
+ (NSString *)appkey;

+ (NSString *)userId;
+ (NSString *)alias;
+ (NSString *)activeCode;

+ (NSString *)homeId;
+ (NSString *)callNumber;


@end

NS_ASSUME_NONNULL_END
