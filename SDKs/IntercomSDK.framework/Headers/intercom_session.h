#ifndef SDK_INTERCOM_SESSION_H_
#define SDK_INTERCOM_SESSION_H_

#import <Foundation/Foundation.h>
#import "intercom_constants.h"

typedef NS_ENUM(NSInteger, SessionState) {
  SessionStateIdle = 0,
  SessionStateReady,
  SessionStateRinging,
  SessionStateConnected
};

typedef NS_ENUM(NSInteger, SessionDir) {
  SessionDirCallout = 0,
  SessionDirCallin,
};

@class IntercomMessage;
@class NetClient;
@class Intercom;

@interface IntercomSession : NSObject

/** 当前会话状态 */
@property(nonatomic, assign, readonly) SessionState state;

/** 会话方向，呼入或者呼出 */
@property(nonatomic, assign, readonly) SessionDir dir;

/** 本次会话的路由，是LAN／WAN*/
@property(nonatomic, assign, readonly) Router router;

/** 本次会话的唯一ID */
@property(nonatomic, strong, readonly) NSString *session_id;

/** 本次会话的对方信息，室外机呼叫就是代理信息，室内通就是对方客户端信息*/
@property(nonatomic, strong, readonly) NetClient *netClient;

/** 请求的消息，可以提供后续回复的参考 */
@property(nonatomic, strong, readonly) IntercomMessage *requestMessage;

/** 是否允许传输视频，在传输过程中可以改变这个状态不让对方看到我的视频*/
@property(nonatomic, assign, readonly) BOOL videoEnable;

/** 是否允许对方听到我的声音*/
@property(nonatomic, assign, readonly) BOOL localAudioEnable;

/** 是否播放对方的声音 */
@property(nonatomic, assign, readonly) BOOL remoteAudioEnable;

@property(nonatomic, strong, readonly) NSString *peerClientId;

@property(nonatomic, strong, readonly) NSString *peerDeviceName;

- (void)ringing;

- (void)connected;
@end

@interface IntercomSessionManager : NSObject

@property(nonatomic, strong, readonly) NSString *localClientId;

@property(nonatomic, weak) Intercom *intercom;

- (id)initWithParam:(Intercom*)intercom
      localClientId:(NSString *)localClientId;

- (NSDictionary<NSString *, IntercomSession *> *)sessions;

- (IntercomSession *)findSession:(NSString *)session_id;

- (void)addSession:(IntercomSession *)session;

- (void)removeSession:(NSString *)session_id;

/** 指定客户端是否在会话中，室内通时不能与一个室内机同时发起多起会话 */
- (BOOL)clientBusy:(NSString *)client_id;

/** 判断指定代理、指定设备、指定房号是否处于呼叫中 ，对门禁和户户通有效*/
- (BOOL)deviceBusy:(NSString *)proxy_id
       device_name:(NSString *)device_name
          username:(NSString *)username;

- (BOOL)callout:(Router)router
         client:(NetClient *)client
        message:(IntercomMessage *)message;

/** 接收新的呼叫*/
- (BOOL)accpet:(Router)router
        client:(NetClient *)client
       message:(IntercomMessage *)message;

/** 拒绝新的呼叫,reason: IntercomMessageResult 不能=0*/
- (BOOL)decline:(Router)router
         client:(NetClient *)client
        message:(IntercomMessage *)message
         reason:(int)reason;

/** 挂断某个会话，如果挂断成功，则从会话列表中删除此会话 */
- (BOOL)hangUp:(NSString *)session_id
        reason:(int)reason;

/** callin会话，我们需要主动摘机，摘机之后可以语音对讲*/
- (BOOL)pickUp:(NSString *)session_id;

/** 挂断所有会话*/
- (void)hangUpAll;

- (BOOL)unlock:(NSString *)session_id;

- (BOOL)unlock:(Router)router
    net_client:(NetClient *)net_client
    session_id:(NSString *)session_id
      username:(NSString *)username
        device:(NSString *)device;

- (BOOL)sendDTMF:(NSString*)session_id
            dtmf:(NSString*)dtmf;

/** 设置是否允许对方看到的我的视频，对户户通和室内通有效
 * 楼宇对讲终端不发视频给对方
 */
- (BOOL)enableVideo:(NSString*)session_id
             enable:(BOOL)enable;

/** 设置本地或远端声音的使能状态*/
- (BOOL)enableAudio:(NSString*)session_id
              local:(BOOL)local
             enable:(BOOL)enable;

/**
 * 通知底层拍个快照，快照通过callback返回文件路径，快照保存到指定的缓存目录
 */
- (BOOL)takeSnapshot:(NSString*)session_id;

/** 设置门禁的SIP/ICE参数，config= nil，表示注销sip
 * 底层支持自动获取sip账号，所以，上层不需要调用此接口
 */
- (BOOL)setupSip:(NSString *)config;

/** 指定设备是否支持录像 ，默认门禁设备才支持，在我们发起呼叫的时候有用*/
+ (BOOL)supportRecord:(NSString *)deviceId;

/** 开启录制，需要提供session_id和一个文件名
 文件名不能包含路径，也不能包含扩展名，可以使用[IntercomManager createSessionId]
 生成一个时间戳就可以了。
 call这个方法，会在record目录生成一个以filename为名称的filename.rec文件
 在回放的时候需要指定这个文件名称，所以，要保存到缓存中，或者数据库中
 */
- (BOOL)startRecord:(NSString*)session_id
           filename:(NSString*)filename;

/** 停止录制*/
- (BOOL)stopRecord:(NSString*)session_id;

@end

#endif //SDK_INTERCOM_SESSION_H_
