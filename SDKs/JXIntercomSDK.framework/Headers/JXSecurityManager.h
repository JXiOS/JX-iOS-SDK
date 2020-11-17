//
//  JXSecurityManager.h
//  JXIntercomSDK
//
//  Created by Nansen on 2020/4/21.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "JXIntercomConstants.h"


typedef NS_ENUM(NSUInteger, JXSecurityCMD) {
    JXSecurityCMD_State,
    JXSecurityCMD_Device,
    JXSecurityCMD_Switch,
    JXSecurityCMD_Query
};


@class IntercomManager;
@class SecurityDevice;
@class SecurityMessage;


NS_ASSUME_NONNULL_BEGIN

@protocol JXSecurityDelegate <NSObject>

@optional
/// 家庭的安防状态
- (void)didSecurityStatusChangedInHome:(NSString *)homeId
                                status:(JX_SecurityStatus)status
                                   cmd:(JXSecurityCMD)cmd;

/// 收到安防报警
- (void)didReceivedAlarmInHome:(NSString *)homeId securityDevice:(SecurityDevice *)device securityMessage:(SecurityMessage *)message;

/// 报警取消
- (void)didCancelAlarmInHome:(NSString *)homeId securityDevice:(SecurityDevice *)device;

@end



@interface JXSecurityManager : NSObject

/// 设置安防监听
- (void)addSecurityDelegate:(id<JXSecurityDelegate>)holder;

/// 移除安防监听
- (void)removeSecurityDelegate:(id<JXSecurityDelegate>)holder;

/// 是否支持安防
- (BOOL)isSupportSecurity:(NSString *)homeId;

/// 查询安防状态:
- (JX_SecurityStatus)querySecurityStatus:(NSString *)homeId;

/// 切换安防状态
- (void)switchSecurityStatus:(NSString *)homeId;

/// 关闭安防报警
- (void)cancelSecurityWarning:(NSString *)homeId;












/// 外部不需要调用的方法
- (void)loginWithIntercomManager:(IntercomManager *)intercomManager;

- (void)logoutClear;

@end

NS_ASSUME_NONNULL_END
