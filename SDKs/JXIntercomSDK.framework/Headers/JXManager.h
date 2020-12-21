//
//  JXManager.h
//  JXIntercomSDK
//
//  Created by Nansen on 2020/3/26.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "JXIntercomConstants.h"
#import "JXConnectingManager.h"

NS_ASSUME_NONNULL_BEGIN

@class JXManagerConfig;
@class JXDoorDeviceModel;
@class JXExtDeviceModel;

@class JXHistoryManager;
@class JXSecurityManager;
@class JXDeviceManager;

@class CATiledLayer;

@protocol JXIntercomDelegate <NSObject>

@required
/// 有呼叫进来, 是否需要响应.
- (BOOL)shouldResponseIntercomCall:(JX_IntercomScenes)callScenes;

@optional
/// 被门禁呼叫 (shouldResponseIntercomCall 返回 YES 的时候才会有)
- (id<JXConnectingDelegate> _Nullable)calledByDoorWithHomeId:(NSString *)homeId
                                                   sessionId:(NSString *)sessionId
                                                  doorDevice:(JXDoorDeviceModel *)doorDevice;

/// 被室内通呼叫 (shouldResponseIntercomCall 返回 YES 的时候才会有)
- (id<JXConnectingDelegate> _Nullable)calledByExtWithHomeId:(NSString *)homeId
                                                  sessionId:(NSString *)sessionId
                                                  extDevice:(JXExtDeviceModel *)extDevice;

/// 被户户通呼叫 (shouldResponseIntercomCall 返回 YES 的时候才会有)
/// @param homeId homeId
/// @param sessionId 会话的 sessionId
/// @param callNumber 呼叫的房号
- (id<JXConnectingDelegate> _Nullable)calledByP2PWithHomeId:(NSString *)homeId
                                                  sessionId:(NSString *)sessionId
                                                 callNumber:(NSString *)callNumber;


@end


@interface JXManager : NSObject

+ (instancetype)defaultManage;

+ (NSString *)version;

@property (nonatomic, weak, nullable) id<JXIntercomDelegate> appDelegate;

/// 默认配置
@property (nonatomic, strong, nullable) JXManagerConfig *config;

/// 历史记录相关
@property (nonatomic, strong, readonly) JXHistoryManager *historyManager;

/// 安防相关
@property (nonatomic, strong, readonly) JXSecurityManager *securityManager;

/// 设备管理相关
@property (nonatomic, strong, readonly) JXDeviceManager *deviceManager;

/// 通话相关
@property (nonatomic, strong, readonly) JXConnectingManager *connectingManager;

/// SDK初始化配置, 全局只需要调用一次
- (void)startWithConfig:(JXManagerConfig *)config;

/// 配置推送的 ID, 建议在 startHome: 之前进行配置完成
- (void)deployAPNsID:(NSString *)pushId;

/// 登录
/// @param userId 移动端的账号,代表移动端的唯一值
/// @param alias 用来显示的昵称. 传空值则默认为"iPhone"
/// @param completeBlock 指示登录是否成功. 返回 YES 后才可以正常使用 SDK 中的功能. 否则可能发生异常
- (void)loginWithUserId:(NSString *)userId
                  alias:(NSString *)alias
               complete:(void(^)(BOOL succeed))completeBlock;


/// 退出账号的时候调用.
- (void)logout;

/// 收到推送的消息
/// @param pushInfo 推送的消息体
- (void)receivePushNoti:(NSDictionary *)pushInfo;


#pragma mark - ======== 家庭管理 ========
/// 启动一个家庭的服务
/// @param homeId 家庭参数
- (BOOL)startHome:(NSString *)homeId;

/// 启动一组家庭的服务, 不在该数组中的家庭将会停止服务.
- (void)startHomes:(NSArray <NSString *> *)homeIds;

/// 纯云端, 无室内机情况下的启动服务
/// @param homeId 家庭参数
- (BOOL)startCloudHome:(NSString *)homeId;

/// 停止服务,保留相关记录
/// @param homeId 家庭参数
- (void)stopHome:(NSString *)homeId;

/// 停止所有家庭的服务
- (void)stopAllHome;

/// 移除家庭服务, 停止服务并删除此家庭的所有记录
- (void)removeHome:(NSString *)homeId;

/// 获得当前已有服务的 homeId
/// 返回值: 一个包含 homeId 的数组.
- (NSArray<NSString *> *)getConnectingHomes;

@end

NS_ASSUME_NONNULL_END
