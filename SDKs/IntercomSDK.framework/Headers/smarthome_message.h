#ifndef SDK_SMARTHOME_MESSAGE_H_
#define SDK_SMARTHOME_MESSAGE_H_

#import <Foundation/Foundation.h>

/**
 * 智能家居的消息，具体的内容在content中
 */
@interface SmartHomeMessage : NSObject

typedef NS_ENUM(NSInteger, MessageSource) {
  MessageSourceSecurityCenter = 0,  //消息来自报警中心
  MessageSourceDevice,              //消息来自设备
  MessageSourceProxy,               //消息来自代理
  MessageSourceClientPad,           //消息来自移动终端
  MessageSourceClientEmbedded       //消息来自嵌入式底座
};

/** 消息来源，智能家居设备名称 securtiy,knx,bus...*/
@property(nonatomic, strong) NSString *from;

/** 接收方的id */
@property(nonatomic, strong) NSString *to;

/** 消息的命令标识 */
@property(nonatomic, strong) NSString *cmd;

/** 代理id */
@property(nonatomic, strong) NSString *proxy_id;

/** 消息正文，是json格式，根据command不同，格式也不同*/
@property(nonatomic, strong) NSString *content;

/** 设备序列号 一般用mac地址表示*/
@property(nonatomic, strong) NSString *sn;

/** 消息状态 */
@property(nonatomic, assign) NSInteger result;

@property(nonatomic, assign) BOOL ack;

/** 是否是广播消息 */
@property(nonatomic, assign) BOOL broadcast;

/** 消息真正的来源 */
@property(nonatomic, assign) MessageSource source;

@property(nonatomic, strong) NSString* time;

@property(nonatomic, strong) NSString* source_id;

- (id)initWithJsonString:(NSString *)json;

- (id)init;

- (NSString *)toJson;
@end

#endif //SDK_SMARTHOME_MESSAGE_H_
