//
//  JXDeviceManager.h
//  JXIntercomSDK
//
//  Created by Nansen on 2020/4/2.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "JXIntercomConstants.h"

@class JXDoorDeviceModel;
@class JXExtDeviceModel;
@class IntercomManager;

/// 可控摄像头的操作方向
typedef NS_ENUM(NSUInteger, JX_PTZDirection) {
    /// 上
    JX_PTZDirection_Up = 0,
    /// 左
    JX_PTZDirection_Left,
    /// 下
    JX_PTZDirection_Down,
    /// 右
    JX_PTZDirection_Right
};


NS_ASSUME_NONNULL_BEGIN

@protocol JXDeviceManagerDelegate <NSObject>

@optional
/// 门禁设备列表改变
- (void)updateDoorDevicesInHome:(NSString *)homeId;

/// 室内通列表改变
- (void)updateExtDevicesInHome:(NSString *)homeId;

@end


@interface JXDeviceManager : NSObject

/// 添加代理, 用来处理设备列表的更新
- (void)addDeviceDelegateHolder:(id<JXDeviceManagerDelegate>)holder;

/// 移除代理
- (void)removeDeviceDelegateHolder:(id<JXDeviceManagerDelegate>)holder;

/// 获取到指定家庭的门禁设备
- (NSMutableArray<JXDoorDeviceModel *> *)getDoorDeviceInHome:(NSString *)homeId;

/// 获取指定家庭的 NVR 设备
- (NSMutableArray<JXDoorDeviceModel *> *)getNvrDeviceInHome:(NSString *)homeId;

/// 获取到指定家庭的室内通设备
- (NSMutableArray<JXExtDeviceModel *> *)getExtDeviceInHome:(NSString *)homeId;

/// 控制摄像头的方向
- (void)cameraControlWithDirection:(JX_PTZDirection)direction
                            homeId:(NSString *)homeId
                         sessionId:(NSString *)sessionId
                            isStop:(BOOL)isStop;

















/// 外部不需要调用的方法
- (void)loginWithIntercomManager:(IntercomManager *)intercomManager;

- (void)logoutClear;


@end

NS_ASSUME_NONNULL_END
