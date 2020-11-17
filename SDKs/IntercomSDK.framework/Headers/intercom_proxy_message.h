#ifndef SDK_INTERCOM_PROXY_MESSAGE_H_
#define SDK_INTERCOM_PROXY_MESSAGE_H_

#import <Foundation/Foundation.h>
#import "net_client.h"

/**
 * 解码从云端推送过来的message
 *
 * scheme: kIntercomScheme,kSmarthomeScheme,其他的直接抛弃
 * cmd: 根据scheme，而不同
 * to:目标client_id，应该就是当前设备
 * to_type:目标设备类型
 * ack:是否是回复消息
 * version:协议版本
 * errcode:错误代码
 * net_client:发送者的client 信息
 * payload:消息正文，根据scheme 而不同
 * 如果scheme: kIntercomScheme, IntercomMessage* message = [IntercomMessage initWithJsonString:payload];
 * 如果是:kSmarthomeScheme:可以解析成SmartHomeMessage消息 SmarthomeMesage* message = [SmarthomeMessage initWithJsonString:payload];
 */

@interface IntercomProxyMessage : NSObject

@property(nonatomic, strong) NSString *scheme;

@property(nonatomic, strong) NSString *cmd;

@property(nonatomic, strong) NSString *to;

@property(nonatomic, assign) NSInteger to_type;

@property(nonatomic, assign) BOOL ack;

@property(nonatomic, assign) NSInteger version;

@property(nonatomic, assign) NSInteger errcode;

@property(nonatomic, strong) NSString *payload;

@property(nonatomic, readonly) NetClient *net_client;

- (id)initWithString:(NSString *)json;

@end

#endif //SDK_INTERCOM_MESSAGE_H_
