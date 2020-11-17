#ifndef SDK_INTERCOM_OBSERVER_H_
#define SDK_INTERCOM_OBSERVER_H_

#import <Foundation/Foundation.h>
#import "intercom_constants.h"
#import "weak_array.h"

@class NetClient;
@class ButtonMessage;
@class IntercomMessage;
@class SmartHomeMessage;
@class SecurityDevice;
@class SecurityMessage;
@class Intercom;

/**
 * App事件
 */
@protocol IntercomAppDelegate<NSObject>

/** App 初始化callback, 理论上要在这个事件之后再创建别的，
 * 但是如果能确保初始化一定可以成功，就无需等待直接进行下一步
 */
- (void)onIntercomAppInitialized:(BOOL)result;

- (void)onIntercomAppUnInitialized;

/** 系统媒体设备的占用状态改变,audio/camera
 * media_type:TransportMediaType
 * state:StreamState
 * 如果state==StreamStateStop,说明当前系统处于空闲状态
 * 系统声卡设备或摄像头设备被释放
 */
- (void)onIntercomMediaDeviceStateChanged:(int)media_type state:(int)state;

- (void)onCameraError:(int)err from:(NSString *)from reason:(NSString *)reason;

- (void)onCameraStateChanged:(int)state;

@end

/** 创建一个新的门禁对象的回调事件，报告创建的结果
 * 如果创建失败，应当通过 IntercomManager
 * removeIntercom:intercom.fid,不能直接销毁
 */
@protocol IntercomClientDelegate<NSObject>

- (void)onIntercomClientInitializeResult:(Intercom *)intercom
                                  result:(BOOL)result;
@end

/**
 * 代理事件回调
 */
@protocol IntercomProxyDelegate<NSObject>

/**
 * 代理上线或者下线通知
 * 在收到这个事件之后，需要通过proxy_id查询代理的在线状态
 * 代理的在线状态有两种,LAN/WAN，同一个代理同时在LAN和WAN在线
 */
- (void)onIntercomProxyStateChanged:(Intercom *)intercom
                           proxy_id:(NSString *)proxy_id
                             online:(BOOL)online;

/**
 * 代理的中间状态发生变化，就是说，代理是有效的，但是LAN／WAN的其中某个
 * 状态发生变化，如果UI层需要分别显示LAN／WAN在线状态，通过这个方法来更新
 */
- (void)onIntercomProxyOnlineStateChanged:(Intercom *)intercom
                                 proxy_id:(NSString *)proxy_id
                                   router:(Router)router
                                   online:(BOOL)online;

/**
 * 终端上线下线通知
 * 终端下线表示这个终端彻底从LAN或者WAN上断开，UI层需要去掉相关的信息
 */
- (void)onIntercomClientStateChanged:(Intercom *)intercom
                           client_id:(NSString *)client_id
                              online:(BOOL)online;

/**
 * 终端的中间状态发生改变，比如从某个代理中掉线，或者WAN上掉线
 * 但终端是可用的，继续保持显示状态
 * proxy_id:可以知道在哪个代理上发生变化，只有router：LAN的时候才有效
 */
- (void)onIntercomClientOnlineStateChanged:(Intercom *)intercom
                                 client_id:(NSString *)client_id
                                  proxy_id:(NSString *)proxy_id
                                    router:(Router)router
                                    online:(BOOL)online;

- (void)onIntercomInternetStateChanged:(Intercom *)intercom online:(BOOL)online;

- (void)onIntercomProxyDeviceChanged:(Intercom *)intercom
                            proxy_id:(NSString *)proxy_id
                              router:(Router)router
                           device_id:(NSString *)device_id;
/**
 * 通过代理收到按键消息
 */
- (void)onIntercomButtonMessageArrival:(Intercom *)intercom
                                router:(Router)router
                            net_client:(NetClient *)net_client
                               message:(ButtonMessage *)message;

/**
 * 通过代理收到其他类型的消息，目前还没有
 */
- (void)onIntercomProxyMessageArrival:(Intercom *)intercom
                              message:(NSDictionary *)message;

@end

/**
 *门禁事件的回调
 */
@protocol IntercomConversationDelegate<NSObject>

/** 收到门禁的消息，我们根据消息来决定如何处理呼叫 */
- (void)onIntercomMessageArrival:(Intercom *)intercom
                          router:(Router)router
                      net_client:(NetClient *)net_client
                         message:(IntercomMessage *)message;

/** 某个会话已经收到视频数据，此刻，我们可以通知底层生成快照 */
- (void)onIntercomVideoReady:(Intercom *)intercom
                  session_id:(NSString *)session_id;

- (void)onIntercomVideoDimensionChanged:(Intercom *)intercom
                             session_id:(NSString *)session_id
                                  width:(int)width
                                 height:(int)height;

/**
 * 生成了一张视频快照，并保存到filename指定的文件中
 * 一般而言，我们需要生成三个快照，时间间隔一定的周期，比如1s
 */
- (void)onIntercomSnapshotReady:(Intercom *)intercom
                     session_id:(NSString *)session_id
                       filename:(NSString *)filename;

/** sip登陆状态发生变化 */
- (void)onIntercomSipStateChanged:(Intercom *)intercom
                            proxy:(NSString *)proxy
                            state:(SipRegistrationState)state;

/** 媒体流状态发生变化 */
- (void)onIntercomStreamStateChanged:(Intercom *)intercom
                          session_id:(NSString *)session_id
                          media_type:(TransportMediaType)media_type
                               state:(StreamState)state;

/** 媒体传输开始了*/
- (void)onIntercomTransportStart:(Intercom *)intercom
                      session_id:(NSString *)session_id
                      media_type:(TransportMediaType)media_type;

/** 传输定期的统计数据，比如收发了多少字节数据
 * interval：统计的时间周期，秒单位，比如30s*/
- (void)onIntercomTransportStatistics:(Intercom *)intercom
                           session_id:(NSString *)session_id
                           media_type:(TransportMediaType)media_type
                             interval:(NSInteger)interval
                           recv_bytes:(long long)recv_bytes
                           send_bytes:(long long)send_bytes;

/**
 *ICE协商结果，result=0表示成功，其他表示失败
 *ICE协商成功，表示媒体传输将通过P2P的方式传输，不会占用服务器资源
 *每个会话可能包含语音和视频，分别通过各自的通道进行传输
 */
- (void)onIntercomTransportIceNegotiationResult:(Intercom *)intercom
                                     session_id:(NSString *)session_id
                                     media_type:(TransportMediaType)media_type
                                         result:(NSInteger)result;
@end

@protocol IntercomSmartHomeDelegate<NSObject>

- (void)onSmartHomeSecurityDeviceStateChanged:(Intercom *)intercom
                                       device:(SecurityDevice *)device
                                       online:(BOOL)online;

- (void)onSmartHomeSecurityDeviceOnlineStateChanged:(Intercom *)intercom
                                             device:(SecurityDevice *)device
                                             router:(int)router
                                             online:(BOOL)online;

- (void)onSmartHomeMessage:(Intercom *)intercom
                    device:(SecurityDevice *)device
                   message:(SmartHomeMessage *)message;

- (void)onSmartHomeAlarm:(Intercom *)intercom
                  device:(SecurityDevice *)device
       smarthome_message:(SmartHomeMessage *)smarthome_message
        security_message:(SecurityMessage *)security_message;

@end

@interface IntercomAppDelegateHolder : WeakReferenceArray

- (void)onIntercomAppInitialized:(BOOL)result;

- (void)onIntercomAppUnInitialized;

- (void)onIntercomMediaDeviceStateChanged:(int)media_type state:(int)state;

/**
 camera operation failed
 err: ## this error code from chromium
 kMacSetCaptureDeviceFailed = 102,
 kMacCouldNotStartCaptureDevice = 103,
 kMacReceivedFrameWithUnexpectedResolution = 104,
 kMacUpdateCaptureResolutionFailed = 105,
 kMacDeckLinkDeviceIdNotFoundInTheSystem = 106,
 kMacDeckLinkErrorQueryingInputInterface = 107,
 kMacDeckLinkErrorCreatingDisplayModeIterator = 108,
 kMacDeckLinkCouldNotFindADisplayMode = 109,
 kMacDeckLinkCouldNotSelectTheVideoFormatWeLike = 110,
 kMacDeckLinkCouldNotStartCapturing = 111,
 kMacDeckLinkUnsupportedPixelFormat = 112,
 kMacAvFoundationReceivedAVCaptureSessionRuntimeErrorNotification = 113,
 from: error code location
 reason:error description
 */
- (void)onCameraError:(int)err from:(NSString *)from reason:(NSString *)reason;

/**
 camera start : 1 or stop: 0
 */
- (void)onCameraStateChanged:(int)state;

@end

@interface IntercomClientDelegateHolder : WeakReferenceArray

- (void)onIntercomClientInitializeResult:(Intercom *)intercom
                                  result:(BOOL)result;
@end

@interface IntercomProxyDelegateHolder : WeakReferenceArray

- (void)onIntercomProxyStateChanged:(Intercom *)intercom
                           proxy_id:(NSString *)proxy_id
                             online:(BOOL)online;

- (void)onIntercomProxyOnlineStateChanged:(Intercom *)intercom
                                 proxy_id:(NSString *)proxy_id
                                   router:(Router)router
                                   online:(BOOL)online;

- (void)onIntercomClientStateChanged:(Intercom *)intercom
                           client_id:(NSString *)client_id
                              online:(BOOL)online;

- (void)onIntercomClientOnlineStateChanged:(Intercom *)intercom
                                 client_id:(NSString *)client_id
                                  proxy_id:(NSString *)proxy_id
                                    router:(Router)router
                                    online:(BOOL)online;

- (void)onIntercomInternetStateChanged:(Intercom *)intercom online:(BOOL)online;

/**
 只有动态发现室外机的设备才有这个回调。上层只需要根据设备列表重新绘制UI
 */
- (void)onIntercomProxyDeviceChanged:(Intercom *)intercom
                            proxy_id:(NSString *)proxy_id
                              router:(Router)router
                           device_id:(NSString *)device_id;

- (void)onIntercomButtonMessageArrival:(Intercom *)intercom
                                router:(Router)router
                            net_client:(NetClient *)net_client
                               message:(ButtonMessage *)message;

- (void)onIntercomProxyMessageArrival:(Intercom *)intercom
                              message:(NSMutableDictionary *)message;
@end

@interface IntercomConversationDelegateHolder : WeakReferenceArray

- (void)onIntercomMessageArrival:(Intercom *)intercom
                          router:(Router)router
                      net_client:(NetClient *)net_client
                         message:(IntercomMessage *)message;

- (void)onIntercomVideoReady:(Intercom *)intercom
                  session_id:(NSString *)session_id;

- (void)onIntercomVideoDimensionChanged:(Intercom *)intercom
                             session_id:(NSString *)session_id
                                  width:(int)width
                                 height:(int)height;

- (void)onIntercomSnapshotReady:(Intercom *)intercom
                     session_id:(NSString *)session_id
                       filename:(NSString *)filename;

- (void)onIntercomSipStateChanged:(Intercom *)intercom
                            proxy:(NSString *)proxy
                            state:(SipRegistrationState)state;

- (void)onIntercomStreamStateChanged:(Intercom *)intercom
                          session_id:(NSString *)session_id
                          media_type:(TransportMediaType)media_type
                               state:(StreamState)state;

- (void)onIntercomTransportStart:(Intercom *)intercom
                      session_id:(NSString *)session_id
                      media_type:(TransportMediaType)media_type;

- (void)onIntercomTransportStatistics:(Intercom *)intercom
                           session_id:(NSString *)session_id
                           media_type:(TransportMediaType)media_type
                             interval:(NSInteger)interval
                           recv_bytes:(long long)recv_bytes
                           send_bytes:(long long)send_bytes;

- (void)onIntercomTransportIceNegotiationResult:(Intercom*)intercom
                                     session_id:(NSString *)session_id
                                     media_type:(TransportMediaType)media_type
                                         result:(NSInteger)result;

@end

@interface IntercomSmartHomeDelegateHolder : WeakReferenceArray

- (void)onSmartHomeSecurityDeviceStateChanged:(Intercom*)intercom
                                       device:(SecurityDevice*) device
                                       online:(BOOL)online;

- (void)onSmartHomeSecurityDeviceOnlineStateChanged:(Intercom*)intercom
                                             device:(SecurityDevice*) device
                                             router:(int)router
                                             online:(BOOL)online;

- (void)onSmartHomeMessage:(Intercom*)intercom
                    device:(SecurityDevice*)device
                   message:(SmartHomeMessage*)message;

- (void)onSmartHomeAlarm:(Intercom*)intercom
                  device:(SecurityDevice*)device
       smarthome_message:(SmartHomeMessage*)smarthome_message
        security_message:(SecurityMessage*)security_message;
@end

#endif //SDK_INTERCOM_OBSERVER_H_
