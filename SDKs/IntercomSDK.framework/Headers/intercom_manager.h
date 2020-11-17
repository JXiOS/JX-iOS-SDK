#ifndef SDK_INTERCOM_MANAGER_H_
#define SDK_INTERCOM_MANAGER_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "intercom_message.h"
#import "net_client.h"
#import "net_device.h"
#import "intercom_constants.h"
#import "button_message.h"
#import "smarthome_message.h"

@class IntercomAppDelegateHolder;
@class IntercomClientDelegateHolder;
@class IntercomConversationDelegateHolder;
@class IntercomProxyDelegateHolder;
@class IntercomSmartHomeDelegateHolder;

@class IntercomMediaPlayer;
@class SmartHomeManager;
@class IntercomClientManager;
@class IntercomSessionManager;
@class CaptureConfig;

/**
 * 门禁对象，可以有多个，如果有多个家庭的话
 * 每个对象关联一个family ID
 * 不允许直接创建此对方，必须通过IntercomManager addIntercom
 * 也不允许直接销毁，必须通过IntercomManager removeIntercom
 */
@interface Intercom : NSObject

@property(nonatomic, assign) int intercom_type;
// family id
@property(nonatomic, strong, readonly) NSString *fid;

//本机client的信息，每个家庭不一样的，但clientid允许相同
@property(nonatomic, readonly) NetClient *netClient;

//此家庭中所有终端和代理的信息
@property(nonatomic, strong) IntercomClientManager *clientManager;

//此家庭中当前活跃会话
@property(nonatomic, strong) IntercomSessionManager *sessionManager;

//此家庭智能家居管理
@property(nonatomic, strong) SmartHomeManager *smarthome;

@property(nonatomic, strong, readonly)
IntercomClientDelegateHolder *client_delegate;

@property(nonatomic, strong, readonly)
IntercomProxyDelegateHolder *proxy_delegate;

@property(nonatomic, strong, readonly)
IntercomConversationDelegateHolder *intercom_delegate;

@property(nonatomic, strong, readonly)
IntercomSmartHomeDelegateHolder *smarthome_delegate;

//启动代理
- (BOOL)startProxy:(NSString *)prox_conf;

//停止代理
- (void)stopProxy;

//启动门禁系统
- (BOOL)startIntercom:(NSString *)server_conf;

//停止门禁系统
- (void)stopIntercom;

//向这个Intercom对象发消息
- (BOOL)sendIntercomMessage:(NSDictionary *)message;

//向这个Intercom对象发命令
- (BOOL)sendIntercomCommand:(NSDictionary *)command;

/**
 * 终端允许远程配置代理，可以配置的项目有：
 * delay_alert_mobile:(int),当有新的呼叫，代理延迟多久通知云端的移动端，代理在部署之后，默认为10s
 * do_not_alert_mobile_if_pickup:(boolean),当延迟通知移动端之前，如果有终端已经接听，是否继续通知移动端，默认为不通知(true)
 * enable_multi_client:(boolean)，是否支持多终端接听，这里只是为了测试，默认是不允许的
 * disable_indoor_client_unlock:(boolean)，是否禁止室内机（平板和底座）开锁，默认不禁止。使用场景是：如果只是针对某些特定人群独自在家，家长
 * 可以通过手机/平板之类的禁止室内机开门，确保安全性
 *
 * 这些配置在代理重启之后就会恢复默认值，并不会持久化
 * 上述各项可以单独设置，最后打包成json通过下述方法发送即可
 */

- (BOOL)sendIntercomConfigure:(Router)router
                   net_client:(NetClient *)net_client
                           to:(NSString *)to
                   session_id:(NSString *)session_id
                    configure:(NSString *)configure;

//更新某个会话的视频显示图层，当需要更换图层的时候，再次调用此方法通知底层
- (BOOL)updateSurface:(NSString *)session_id layer:(CALayer *)layer;

@end

/**
 * 门禁系统的管理,在初始化之前，至少要通过IntercomAppDelegateHolder 添加
 * IntercomAppDelegate代理
 */
@interface IntercomManager : NSObject

@property(nonatomic, strong, readonly) IntercomAppDelegateHolder *app_delegate;

//全局初始化，只需要调用一次，必须在调用其他任何方法之前初始化
+ (void)globalInitialize:(NSString *)app_conf;

/**
 *启动门禁系统
 * system_id:系统唯一标示
 * intercom_conf:门禁配置(IntercomConfig)
 * arrays:二维数组，第一维保存一张jpeg图像，第二维暂时没有使用，后期扩展使用
 */
- (BOOL)start:(NSString *)system_id
    port_conf:(NSString *)port_conf
intercom_conf:(NSString *)intercom_conf
       arrays:(NSArray *)arrays;

/**
 *停止门禁系统，如果切换账号的话，先停止，再从新创建Intercomanager。
 *因为门禁系统销毁的时候需要用到UI线程，所以，必须等到
 *onIntercomAppUnInitialized callback
 *才能将对象设置为nil
 */
- (void)stop;

/**
 在收到AppUnInitialize callback之后才能destroy
 */
- (void)destroy;

/**
 *添加一个家庭的门禁系统
 * client_conf: 终端配置(ClientConf)
 */
- (Intercom*)addIntercom:(NSString *)fid
             client_conf:(NSString *)client_conf
           intercom_type:(int)intercom_type;

/**
 *删除一个家庭的门禁系统
 */
- (void)removeIntercom:(NSString *)fid;

/**
 *根据家庭ID查询对应的门禁系统
 */
- (Intercom*)getIntercom:(NSString*)fid;

/**
 *更新摄像头预览显示图层
 */
- (BOOL)updatePreviewSurface:(CALayer*)layer;

/**必须使能才能看到摄像头视频*/
- (BOOL)enablePreview:(BOOL)enable;


/** 从推送服务器收到的消息调用此方法传给底层处理 */
- (BOOL)sendPushMessage:(NSString*)message;

/** 系统级的命令，playback控制通过此接口*/
- (BOOL)sendAppCommand:(NSDictionary*)message;

/** 系统级的消息 */
- (BOOL)sendAppMessage:(NSDictionary*)message;

/** 切换摄像头：kFrontCameraName:前置 kBackCameraName:后置 */
- (BOOL)switchCamera:(CaptureConfig*)conf;

/** 查询当前系统中保存的录像名称列表 */
+ (NSArray<NSString *> *)enumeratorRecord;

/** 当前sdk版本*/
+ (NSString *)getSDKVersion;

/** 我们外呼的时候产生一个新的sessionid*/
+ (NSString *)createSessionId;

/** base64 编解码 */
+ (NSString *)base64:(BOOL)encode
             content:(NSData *)content;

/** des 编解码 */
+ (NSData *)encode:(BOOL)encode
               key:(NSString *)key
           content:(NSData *)content;

@end

#endif //SDK_INTERCOM_MANAGER_H_
