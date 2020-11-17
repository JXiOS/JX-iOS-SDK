#ifndef SDK_INTERCOM_MESSAGE_H_
#define SDK_INTERCOM_MESSAGE_H_

#import <Foundation/Foundation.h>

/**
 * 门禁消息
 {
 "from": "1496136497079886",
 "to": "1496136497079881",
 "cmd": "invite",
 "proxy_id": "1496136497079886",
 "result": 0,
 "ack": false,
 "username": "0101101",
 "session_id": "1496789106154438",
 "record": false,
 "version": 1,
 "content": "whoami",
 "duation":120,
 "contact": {
 "stun": "120.25.197.113:5060",
 "account": "test_001@120.25.197.113"
 },
 "sdp": {
 "audio_address":"172.18.18.3:12344",
 "video_address":"172.18.18.3.12346",
 "audio_format":
 "InJ0cCI6IHRydWUsImNvZGVjIjogIlVMQVciLCJmb3JtYXQiOiAiODAwMDoxOjE2IiwidHMiOjMyMCwiZGlyIjoyLCJwdCI6MCxldmVudF9wdDoxMDEsZHRtZjoiKiIsIm1hcmsiOnRydWU=",
 "video_format":
 "InJ0cCI6dHJ1ZSwiY29kZWMiOiJoMjY0IiwidHMiOjM2MDAsInciOjY0MCwiaCI6NDgwLCJleHRyYV9kYXRhIjoieHh4eCIsInBhY2tldGl6ZXIiOjEsImRpciI6MiwgInB0Ijo5NywiZWFybHlfbWVkaWEiOnRydWU="
 }
 }
 */

@interface IntercomMessage : NSObject<NSCopying, NSMutableCopying>
/** 消息来源，如果从代理过来的消息，则是设备的id，回复消息必须在to中设置这个名称
 * 错误的消息id，代理直接丢弃
 */
@property(nonatomic, strong) NSString *from;

/**
 *消息的目标地址，如果网络地址一样，from和to必须指定，否则消息无法到达正确的设备
 */
@property(nonatomic, strong) NSString *to;

/** 消息中的指令,见:
 extern NSString *const kIntercomCommandInvite;
 extern NSString *const kIntercomCommandRinging;
 extern NSString *const kIntercomCommandCall;
 extern NSString *const kIntercomCommandPickup;
 extern NSString *const kIntercomCommandHangup;
 extern NSString *const kIntercomCommandUnlock;
 extern NSString *const kIntercomCommandPickupByOther;
 extern NSString *const kIntercomCommandBeginTalk;
 extern NSString *const kIntercomCommandEndTalk;
 extern NSString *const kIntercomCommandSessionTimeout;
 extern NSString *const kIntercomCommandLeaveSession;
 extern NSString *const kIntercomCommandMessage;
 */
@property(nonatomic, strong) NSString *cmd;

/**
 * 消息的代理id，代理id必须设置正确，否则消息无法发出
 */
@property(nonatomic, strong) NSString *client_id;

/** 消息执行的结果 IntercomMessageResult*/
@property(nonatomic, assign) NSInteger result;
@property(nonatomic, assign) BOOL ack;

/** 用户名，楼宇对讲就是会话的房号*/
@property(nonatomic, strong) NSString *username;

/** 设备别名，可以用于显示*/
@property(nonatomic, strong) NSString *device_name;

/** 会话的id，每个会话的id是不同的*/
@property(nonatomic, strong) NSString *session_id;

/** 会话是否需要录制*/
@property(nonatomic, assign) BOOL record;

/** 会话是否支持语音*/
@property(nonatomic, assign) BOOL audio;

/** 会话是否支持视频*/
@property(nonatomic, assign) BOOL video;

/** 消息版本，目前=2*/
@property(nonatomic, assign) NSInteger version;

/** 消息的附属内容，根据场景不同用所不同
 *content一般为json格式，在室内通或者P2P会话中，content格式 =
 *{"client_id":"xxxxx","message":"xxx"};
 */
@property(nonatomic, strong) NSString *content;

/** ice
 * 描述会话的ICE配置，用于P2P媒体传输，只对Internet呼叫有效，上层不需要关心*/
@property(nonatomic, strong) NSString *ice;

/** 本次会话代理中的sip账号，只对Internet呼叫有效，上层不需要关心*/
@property(nonatomic, strong) NSString *account;

/** 代理的音频流地址*/
@property(nonatomic, strong) NSString *audio_address;

/** 代理中的视频流地址 */
@property(nonatomic, strong) NSString *video_address;

@property(nonatomic, strong) NSString *audio_format;

@property(nonatomic, strong) NSString *video_format;

- (id)initWithJsonString:(NSString *)json;

- (id)init;

- (NSString *)toJson;

@end

#endif //SDK_INTERCOM_MESSAGE_H_
