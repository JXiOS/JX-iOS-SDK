#ifndef SDK_BUTTON_MESSAGE_H_
#define SDK_BUTTON_MESSAGE_H_

#import <Foundation/Foundation.h>

/**
 * 如果代理运行于嵌入式设备，一般都包含物理按键
 * 代理会检测按键事件，并将按键事件打包发送给在线的终端
 * 终端需要处理按键信息，并作出相应的动作
 */
@interface ButtonMessage : NSObject

/** 按键来自于哪里，一般是嵌入式设备的终端id */
@property(nonatomic, strong) NSString* from;

/** 消息接收终端的id，也就是自己的id */
@property(nonatomic, strong) NSString* to;

/** 按键的命令,参考 intercom_constants : kButton... */
@property(nonatomic, strong) NSString* cmd;

/**这个消息从哪个代理发来的 */
@property(nonatomic, strong) NSString* proxy_id;

/** 这个消息关联的会话标示，只有接听挂断才可能有这个值 */
@property(nonatomic, strong) NSString* session_id;

/** 底座的按键标识，用于匹配终端的按键标识*/
@property (nonatomic,strong) NSString* button_key;

/** 消息发生的时间，微妙单位*/
@property (nonatomic,strong) NSString* time;

/** 是否是回复消息 */
@property (nonatomic,assign) BOOL ack;

/** 从json解析消息内容 */
- (id)initWithJsonString:(NSString*)json;

- (id)init;

- (NSString*)toJson;
@end

#endif //SDK_BUTTON_MESSAGE_H_
