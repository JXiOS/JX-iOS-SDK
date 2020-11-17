#ifndef SDK_NET_CLIENT_H_
#define SDK_NET_CLIENT_H_

#import <Foundation/Foundation.h>
#import "intercom_constants.h"

/**
 *门禁系统中代理和客户端的信息描述
 */

@interface NetClient : NSObject

/** 代理或者客户端的网络地址 */
@property(nonatomic, strong) NSString *address;

/** 客户端的别名，比如[客厅]，用于UI端展示 */
@property(nonatomic, strong) NSString *alias;

/** 代理或者客户端的键值，用于匹配客户端和底座的对应关系 */
@property(nonatomic, strong) NSString *button_key;

/** 代理或者客户端的ID，新版本由mac地址hash生成，旧版本用时间戳生成 */
@property(nonatomic, strong) NSString *client_id;

/** 家庭的ID，由服务器分配 */
@property(nonatomic, strong) NSString *family_id;

/** 目前没有使用，设想的是室外机的编号 */
@property(nonatomic, strong) NSString *number;

/** 代理的TCP侦听端口，对客户端无效 */
@property(nonatomic, assign) NSInteger port;

/** 代理的随机数，用于分辨代理是否重启过 */
@property(nonatomic, strong) NSString *rand;

/** 设备类型，包含代理和客户端以及室外机 */
@property(nonatomic, assign) NetClientType type;

/** 客户端类型,参考 intercom_define.h */
@property(nonatomic, assign) NetClientSubType sub_type;

/** NetClientOSPlatformType,我们可以知道这个终端的 OS 标识 */
@property(nonatomic, assign) NSInteger platform;

/**
 * 如果是移动客户端，这里提供推送的ID
 * 用于通过推送接受呼叫消息
 */
@property(nonatomic, strong) NSString *pushid;

@property(nonatomic,strong) NSString *sn;

/** 协议版本 可能有1和2两个版本，1版本表示是以前的版本，2版本一定是此刻的新版本*/
@property(nonatomic, assign) NSInteger version;

- (id)initWithJsonString:(NSString *)json;

- (id)init;

- (NSString *)toJson;

- (NSInteger)osType;
@end

#endif //SDK_NET_CLIENT_H_
