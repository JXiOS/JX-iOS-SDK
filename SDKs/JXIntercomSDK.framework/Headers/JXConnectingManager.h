//
//  JXConnectingManager.h
//  JXIntercomSDK
//
//  Created by Nansen on 2020/4/22.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "JXIntercomConstants.h"
#import <IntercomSDK/IntercomSDK.h>


@class CATiledLayer;

@class IntercomManager;
@class JXPicRecordModel;
@class JXDoorDeviceModel;
@class JXExtDeviceModel;

/// 截图成功 picModel 会有值, 失败为 nil
typedef void (^getSnapshotBlock)(JXPicRecordModel * __nullable picModel, JX_IntercomResultType resultType);


NS_ASSUME_NONNULL_BEGIN

@protocol JXConnectingDelegate <NSObject>

@optional
/// 通话连接被挂断
- (void)didHangUpWithReason:(JX_IntercomReason)reason animated:(BOOL)animated;

/// 通话被对方接听
- (void)didPickup;

/// 提供用来展示对方视频的 Layer, 必须是 CATiledLayer
- (CATiledLayer *)showOtherVideoLayer;

/// 提供用来展示自己视频的 Layer, 必须是 CATiledLayer
- (CATiledLayer * _Nullable)showMineVideoLayer;

/// 门锁已打开
- (void)didDoorLockOpen;

/// 开始录制视频
- (void)didStartRecord;

/// 结束录制视频
- (void)didEndRecord;

/// 视频流开始
- (void)didVideoAppears;

/// 视频尺寸改变
- (void)didVideoSizeChangeNewWidth:(int)width newHeight:(int)height;

/// 收到 NPT 播放偏移
/// @param startNpt 相对于起始时间的偏移
/// @param endNpt 结束时间的偏移
- (void)didReceiveNPT:(NSInteger)startNpt endNpt:(NSInteger)endNpt;

@end




@interface JXConnectingManager : NSObject<IntercomConversationDelegate>

/// 主动接听 sessionId 对应的会话, 不会有回调
- (BOOL)pickUpInHome:(NSString *)homeId sessionId:(NSString *)sessionId;

/// 主动挂断 sessionId 对应的会话, 不会有回调
- (void)hangUpInHome:(NSString *)homeId sessionId:(NSString *)sessionId;

/// 主动挂断掉所有的会话
- (void)hangUpAllSession;

/// 开启/禁用 视频
- (BOOL)enableVideo:(BOOL)enable inHome:(NSString *)homeId sessionId:(NSString *)sessionId;

/// 切换前后镜头: front:YES-前置 NO-后置.
- (BOOL)switchCamera:(BOOL)front;

/// 开启/禁用 麦克风
- (BOOL)enableVoice:(BOOL)enable inHome:(NSString *)homeId sessionId:(NSString *)sessionId;

/// 开启/禁用 扬声器
- (BOOL)enableSpeaker:(BOOL)enable inHome:(NSString *)homeId sessionId:(NSString *)sessionId;

/// 通话中开锁
- (BOOL)unlockInHome:(NSString *)homeId
           sessionId:(NSString *)sessionId
              device:(JXDoorDeviceModel *)device;

/// 获取会话的快照 (保存到相册需要外部自己处理)
- (void)takeSnapshotInHome:(NSString *)homeId
                 sessionId:(NSString *)sessionId
             completeBlock:(getSnapshotBlock)completeBlock;

/// 开始录制视频
- (BOOL)startRecordVideoInHome:(NSString *)homeId
                     sessionId:(NSString *)sessionId
                    deviceName:(NSString *)deviceName;

/// 停止录制视频
- (void)stopRecordVideoInHome:(NSString *)homeId
                    sessionId:(NSString *)sessionId;

/// 更新视频的 layer
- (void)updateVideoLayer:(CATiledLayer *)layer sessionId:(NSString *)sessionId homeId:(NSString *)homeId;




#pragma mark ---- 室内通 ----
/// 是否支持室内通通话
- (BOOL)isSupportExtInHome:(NSString *)homeId;

/// 呼叫室内通设备, 呼叫成功返回 sessionId, 呼叫失败返回 nil
/// @param homeId homeId
/// @param callType 区分是呼叫还是查看监控
/// @param extDevice 室内通设备
/// @param delegate 视频连接的代理
- (NSString * _Nullable)callExtInHome:(NSString *)homeId
                             callType:(JX_IntercomCallType)callType
                            extDevice:(JXExtDeviceModel *)extDevice
                        videoDelegate:(id<JXConnectingDelegate>)delegate;


#pragma mark ---- 门禁 ----
/// 查看门禁系统摄像头
/// @param homeId homeId
/// @param doorDevice KRDoorDeviceModel
/// @param delegate 设置代理
/// 返回的是通话的 sessionId, 如果是 nil, 表示呼叫失败
- (NSString * _Nullable)callDoorMonitorInHome:(NSString *)homeId
                                       device:(JXDoorDeviceModel *)doorDevice
                                videoDelegate:(id<JXConnectingDelegate>)delegate;


#pragma mark ---- 户户通 ----
/// 是否支持户户通通话
- (BOOL)isSupportP2PInHome:(NSString *)homeId;

/// 呼叫户户通
/// @param homeId homeId
/// @param callNumber 呼叫的房号
/// @param delegate 会话相关的代理
/// 返回的是会话的 sessionId, 如果是 nil, 表示呼叫失败.
- (NSString * _Nullable)callP2POutWithHomeId:(NSString *)homeId
                                  callNumber:(NSString *)callNumber
                               videoDelegate:(id<JXConnectingDelegate>)delegate;


#pragma mark ---- NVR ----
/// 查看 NVR 的历史记录, 呼叫成功返回 sessionId, 呼叫失败返回 nil
/// @param homeId homeId
/// @param nvrDevice nvr设备, deviceType == JX_DeviceType_NVRIPCCamera
/// @param startDate 开始时间
/// @param endDate 结束时间
/// @param delegate 播放视频的代理
- (NSString * _Nullable)playNVRHistoryWithHomeId:(NSString *)homeId
                                       nvrDevice:(JXDoorDeviceModel *)nvrDevice
                                       startDate:(NSDate *)startDate
                                         endDate:(NSDate *)endDate
                                   videoDelegate:(id<JXConnectingDelegate>)delegate;

/// nvr 历史快进
- (BOOL)seekNVR:(NSString *)homeId
      sessionId:(NSString *)sessionId
      nvrDevice:(JXDoorDeviceModel *)nvrDevice
        starNpt:(NSInteger)startNpt;



















@end

NS_ASSUME_NONNULL_END
