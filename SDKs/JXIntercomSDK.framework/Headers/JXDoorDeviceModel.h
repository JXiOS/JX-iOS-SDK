//
//  JXDoorDeviceModel.h
//  JXIntercomSDK
//
//  Created by Nansen on 2020/4/2.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <IntercomSDK/intercom_constants.h>
#import "JXIntercomConstants.h"

@class SubDevice;
@class NetClient;


NS_ASSUME_NONNULL_BEGIN

@interface JXDoorDeviceModel : NSObject

/// 设备的名称
@property (nonatomic, copy) NSString *showName;

@property (nonatomic, copy) NSString *proxyId;

@property (nonatomic, strong) NetClient *netClient;

//@property (nonatomic, strong) SubDevice *device;

/**
 e.g.
    85168b5ca7
    NVR:TDWY:172.16.4.213:1    (NVR开头的就是 NVR 设备)
 */
/// SubDevice.name
@property (nonatomic, copy) NSString *subDeviceName;

/**
 172.16.4.177
 172.16.4.213
 */
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
