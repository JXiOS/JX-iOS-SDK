#ifndef SDK_SECURITY_MESSAGE_H_
#define SDK_SECURITY_MESSAGE_H_

#import <Foundation/Foundation.h>

/**
 * 8路防区的报警消息，从SmartHomeMessage.content中解析出来
 * 8路防区报警消息中包含了详细的报警信息，一次可能有一个活着多个
 * 防区发生报警
 防区定时报告活跃状态
 {"cmd":"report","state":[{"area":0,"value":1,"alias":"烟雾探测器","type":"1"},{"area":1,"value":0,"alias":"烟雾探测器","type":"1"}]}
 //防区报警
 {"cmd":"alert","state":[{"area":0,"value":1},{"area":1,"value":0}]}
 */

@interface SecurityDeviceState : NSObject
/** 防区编号 */
@property(nonatomic, readonly) NSInteger area;
/** 防区的值 */
@property(nonatomic, readonly) NSInteger value;

/** 防区的别名，用于显示 */
@property(nonatomic, readonly) NSString *alias;

/** 防区的类型名称，由系统定义 */
@property(nonatomic, readonly) NSString *type;
@end

@interface SecurityMessage : NSObject
/** 防区的报警命令： kSecurityReport；kSecurityAlert */
@property(nonatomic, readonly) NSString *cmd;

/** 房间号 */
@property(nonatomic, readonly) NSString *room;

/** 事件触发事件，微秒为单位 */
@property(nonatomic, readonly) NSString *time;

/** 具体的报警信息 */
@property(nonatomic, readonly) NSArray<SecurityDeviceState *> *states;

- (id)initWithJsonString:(NSString *)json;

@end

#endif //SDK_SECURITY_MESSAGE_H_
