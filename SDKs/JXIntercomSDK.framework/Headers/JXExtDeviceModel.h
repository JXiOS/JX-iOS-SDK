//
//  JXExtDeviceModel.h
//  JXIntercomSDK
//
//  Created by Nansen on 2020/4/2.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <IntercomSDK/intercom_constants.h>
#import "JXIntercomConstants.h"


@class NetClient;


NS_ASSUME_NONNULL_BEGIN

@interface JXExtDeviceModel : NSObject

@property (nonatomic, copy) NSString *showName;

@property (nonatomic, assign) Router router;

@property (nonatomic, strong) NetClient *netClient;

@property (nonatomic, assign) JX_DeviceType deviceType;

- (instancetype)initWithNetClient:(NetClient *)netClient isLan:(BOOL)isLan;


@end

NS_ASSUME_NONNULL_END
