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

@end


@interface JXManager : NSObject

+ (instancetype)defaultManage;

+ (NSString *)version;

@property (nonatomic, weak, nullable) id<JXIntercomDelegate> appDelegate;

/// 默认配置
@property (nonatomic, strong) JXManagerConfig *config;

/// 历史记录相关
@property (nonatomic, strong, readonly) JXHistoryManager *historyManager;

/// 安防相关
@property (nonatomic, strong, readonly) JXSecurityManager *securityManager;

/// 设备管理相关
@property (nonatomic, strong, readonly) JXDeviceManager *deviceManager;

/// 通话相关
@property (nonatomic, strong, readonly) JXConnectingManager *connectingManager;

/// App 启动的时候启动服务
- (void)startWithConfig:(JXManagerConfig *)config;

/// 配置推送的 ID, 在 startWithConfig 之后再配置
- (void)deployAPNsID:(NSString *)pushId;

/// 登录
/// @param userId 移动端的账号,代表移动端的唯一值
/// @param alias 用来显示的昵称
- (void)loginWithUserId:(NSString *)userId
                  alias:(NSString *)alias;


/// 退出账号的时候必须调用
- (void)logout;

/// 收到推送的消息
/// @param pushInfo 推送的消息体
- (void)receivePushNoti:(NSDictionary *)pushInfo;


#pragma mark - ======== 家庭管理 ========
/// 启动服务
/// @param homeId 家庭参数
- (BOOL)startHome:(NSString *)homeId;

/// 纯云端, 无室内机情况下的启动服务
/// @param homeId 家庭参数
- (BOOL)startCloudHome:(NSString *)homeId;

/// 停止服务-不会删除数据库
/// @param homeId 家庭参数
- (void)stopHome:(NSString *)homeId;

/// 移除家庭服务-停止服务并删除数据库数据
- (void)removeHome:(NSString *)homeId;

/// 获得当前已经连接上的家庭
- (NSArray<NSString *> *)getConnectingHomes;

/// 启动一组家庭的服务, 会自动去重并停止不在该列表的服务
- (void)startHomes:(NSArray <NSString *> *)homeIds;

/// 停止所有家庭的服务-不会删除数据库数据
- (void)stopAllHome;

@end

NS_ASSUME_NONNULL_END
