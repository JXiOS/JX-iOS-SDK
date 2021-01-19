//
//  JXDoorDeviceModel.h
//  JXIntercomSDK
//
//  Created by Nansen on 2020/4/2.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <IntercomSDK/IntercomSDK.h>
#import "JXIntercomConstants.h"

@class SubDevice;
@class NetClient;


NS_ASSUME_NONNULL_BEGIN

@interface JXDoorDeviceModel : NSObject

/// 设备的名称
@property (nonatomic, copy) NSString *showName;

@property (nonatomic, copy) NSString *proxyId;

@property (nonatomic, strong) NetClient *netClient;

/// SubDevice.name
@property (nonatomic, copy) NSString *subDeviceName;

/// SubDevice.ip
@property (nonatomic, copy) NSString *subDeviceIp;

/// NetDeviceItem.name
@property (nonatomic, copy) NSString *itemName;

/// 是否支持操作摄像头转向
@property (nonatomic, assign) BOOL ptzSupport;

/// 是否可以开门
@property (nonatomic, assign) BOOL canOpenDoor;

/// 设备类型
@property (nonatomic, assign) JX_DeviceType deviceType;

@property (nonatomic, assign) Router router;

- (instancetype)initWithProxyId:(NSString *)proxyId
                      netClient:(NetClient *)netClient
                         device:(SubDevice *)device
                       itemName:(NSString *)itemName
                         router:(Router)router;

@end

NS_ASSUME_NONNULL_END
