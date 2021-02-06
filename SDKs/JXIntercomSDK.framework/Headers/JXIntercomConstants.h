//
//  JXIntercomConstants.h
//  JXIntercomSDK
//
//  Created by Nansen on 2020/4/13.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#define JX_DEPRECATED(instead) NS_DEPRECATED_IOS(10_0, 10_0, instead)

typedef NS_ENUM(NSUInteger, IntercomType) {
    /// 带有室内机的情况
    IntercomType_Normal = 0,
    /// 纯云端, 无室内机的情况
    IntercomType_Cloud
};



/// 设备类型
typedef NS_ENUM(NSUInteger, JX_DeviceType) {
    /// 未知
    JX_DeviceType_Unknown = 0,
    /// 门口机, 别墅室外机
    JX_DeviceType_Door,
    /// 单元机, outdoor
    JX_DeviceType_DoorUnit,
    /// 物业管理中心
    JX_DeviceType_DoorManager,
    /// 围墙
    JX_DeviceType_DoorWall,
    /// 摄像头 - 一般是 ONVIF IPC
    JX_DeviceType_DoorCamera,
    /// 室内通平板
    JX_DeviceType_ExtPad,
    /// 手机
    JX_DeviceType_ExtMobile,
    /// 智能电视，目前还没有
    JX_DeviceType_ExtTV,
    /// 嵌入式底座
    JX_DeviceType_ExtEmbedded,
    /// 独立的IPC
    JX_DeviceType_IPCCamera,
    /// NVR 连接的 IPC
    JX_DeviceType_NVRIPCCamera,
    /// 户户通设备
    JX_DeviceType_P2P
};


/// 安防的状态
typedef NS_ENUM(NSUInteger, JX_SecurityStatus) {
    /// 撤防状态
    JX_SecurityStatus_UnDefence = 0,
    /// 布防状态
    JX_SecurityStatus_Defence,
    /// 离线
    JX_SecurityStatus_OffLine,
    /// 无设备
    JX_SecurityStatus_NoDevice
};

/// 通话的场景
typedef NS_ENUM(NSUInteger, JX_IntercomScenes) {
    /// 门禁
    JX_IntercomScenes_Door = 0,
    /// 室内通
    JX_IntercomScenes_Ext,
    /// 户户通
    JX_IntercomScenes_P2P,
};

/// 呼叫类型
typedef NS_ENUM(NSUInteger, JX_IntercomCallType) {
    /// 呼叫
    JX_IntercomCallType_Call = 0,
    /// 查看监控
    JX_IntercomCallType_Monitor,
    /// 查看 NVR 历史
    JX_IntercomCallType_NVRHistory
};

/// 结果类型
typedef NS_ENUM(NSUInteger, JX_IntercomResultType) {
    /// 成功
    JX_IntercomResultType_Ok,
    /// 未知错误
    JX_IntercomResultType_Unknown,
    /// 重复操作 | 操作过快 | 当前有操作,无法再次操作
    JX_IntercomResultType_OpRepeat,
    /// 操作失败
    JX_IntercomResultType_OpFailed,
    /// 参数有误
    JX_IntercomResultType_IncorrectParam,
    /// SDK未登录
    JX_IntercomResultType_NoLogin,
    /// 该家庭服务未启动
    JX_IntercomResultType_NoHome,
    
};


typedef NS_ENUM(NSUInteger, JX_IntercomReason) {
    /// OK
    JX_IntercomReason_None = 0,
    /// 未知错误
    JX_IntercomReason_Unknown,
    /// 呼叫设备参数错误
    JX_IntercomReason_DeviceName,
    /// 呼叫 SessionId 错误
    JX_IntercomReason_SessionId,
    /// 用户认证失败
    JX_IntercomReason_NoAuthority,
    /// 非法操作
    JX_IntercomReason_UnSupportCmd,
    /// 连接失败
    JX_IntercomReason_NotConnected,
    /// 对方正忙
    JX_IntercomReason_Busy,
    /// 呼叫参数错误
    JX_IntercomReason_Params,
    /// 网络异常
    JX_IntercomReason_Net,
    /// 数据传输失败
    JX_IntercomReason_Transport,
    
    /// 对方无响应
    JX_IntercomReason_NoResponse = 100,
    /// 验证失败
    JX_IntercomReason_BadCredentials,
    /// 对方拒绝接听
    JX_IntercomReason_Decline,
    /// 对方可能不在线
    JX_IntercomReason_NotFound,
    /// 服务无响应
    JX_IntercomReason_NoAnswered,
    /// 呼叫超时
    JX_IntercomReason_Timeout,
    
    /// 被门禁呼叫打断
    JX_IntercomReason_InterruptByDoor = 200,
    /// 被室内通呼叫打断
    JX_IntercomReason_InterruptByExt,
    /// 被其他人接听
    JX_IntercomReason_PickupByOther,
    /// 设备掉线
    JX_IntercomReason_DeviceOffLine,
    
    /// 对方挂断会话
    JX_IntercomReason_Hungup = 900,
};


NS_ASSUME_NONNULL_BEGIN

// 室内通呼叫
UIKIT_EXTERN NSString * const JX_NameExt;
UIKIT_EXTERN NSString * const JX_ExtMessageKey_ClientID;
UIKIT_EXTERN NSString * const JX_ExtMessageKey_Message;
UIKIT_EXTERN NSString * const JX_ExtMessageKey_IsMonitor;
UIKIT_EXTERN NSString * const JX_ExtMessageKey_isHandHangup;
UIKIT_EXTERN NSString * const JX_ExtMessageKey_CallSession;

/// NVR 查看记录用
UIKIT_EXTERN NSString * const JX_NVRMessageKey_Starttime;
UIKIT_EXTERN NSString * const JX_NVRMessageKey_Endtime;
UIKIT_EXTERN NSString * const JX_NVRMessageKey_Streamtype;

/// 户户通
UIKIT_EXTERN NSString * const JX_NameP2P;

/// 摄像头操作
UIKIT_EXTERN NSString * const JX_Door_IPC;

/// 安防
UIKIT_EXTERN NSString * const JX_SecurityClass;
UIKIT_EXTERN NSString * const JX_MessageKey_Scheme;
UIKIT_EXTERN NSString * const JX_MessageKey_Router;
UIKIT_EXTERN NSString * const JX_MessageKey_Client;
UIKIT_EXTERN NSString * const JX_MessageKey_Message;
UIKIT_EXTERN NSString * const JX_MessageKey_Command;

/// 自定义 message 中的 key
UIKIT_EXTERN NSString * const JX_CustomMessageKey_Type;
UIKIT_EXTERN NSString * const JX_CustomMessageKey_CallSession;
UIKIT_EXTERN NSString * const JX_CustomMessageKey_Content;

/// 自定义 message 的 type 值 : 通知消息
UIKIT_EXTERN NSString * const JX_CustomMessageType_Call;
/// 自定义 message 的 type 值 : 文本消息
UIKIT_EXTERN NSString * const JX_CustomMessageType_Text;


@interface JXIntercomConstants : NSObject

+ (NSString *)reasonMsgByReason:(JX_IntercomReason)reason;


@end

NS_ASSUME_NONNULL_END
