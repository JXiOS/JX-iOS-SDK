#ifndef SDK_SMARTHOME_MANAGER_H_
#define SDK_SMARTHOME_MANAGER_H_

#import <Foundation/Foundation.h>

@class NetClient;
@class SmartHomeMessage;
@class IntercomProxy;
@class SecurityMessage;
@class Intercom;

typedef NS_ENUM(NSInteger, DeviceOnlineState) {
  SecurityOffline = 0,
  SecurityLanOnline,
  SecurityWanOnline,
};

/**
 * 安防设备的配置
 */
@interface DeviceConfig : NSObject
/** 是否禁用嵌入式设备的安防功能*/
@property(nonatomic, assign, readonly) BOOL disable_embedded_function;

/** 当前布防／撤防状态 */
@property(nonatomic, assign, readonly) int security_state;

/** 安防设备是否连接上了 */
@property(nonatomic, assign, readonly) int device_state;

/** 房间号 */
@property(nonatomic, strong, readonly) NSString* room;

/** 8路防区的别名，在中文语言下是中文 */
@property(nonatomic, strong, readonly) NSString* device_alias;

/** 8路防区的标示，用于准确识别设备 */
@property(nonatomic, strong, readonly) NSString* device_type;

@end

@interface SecurityDevice : NSObject
/** 防区设备的标示，用于给它发消息*/
@property(nonatomic, strong, readonly) NSString* device_name;
/** 防区所在的代理信息*/
@property(nonatomic, strong, readonly) NetClient* net_client;
/** 防区的配置 */
@property(nonatomic, strong, readonly) DeviceConfig* config;

- (BOOL)ready;

- (BOOL)alarming;

- (BOOL)isDefenceEnable;

- (int)router;

- (void)enableAlarm:(BOOL)enable;

- (void)switchDefence;
@end

@interface SmartHomeManager : NSObject

@property(nonatomic, strong, readonly) NetClient* net_client;

@property(nonatomic, weak) Intercom *intercom;

- (id)initWithParam:(Intercom*)intercom
         net_client:(NetClient*)net_client;

- (NSArray<SecurityDevice*>*)devices;

- (void)onProxyStateChanged:(int)router
                     online:(BOOL)online
                      proxy:(IntercomProxy*)proxy;

- (void)onMessageArrival:(int)router
              net_client:(NetClient*)net_client
                 message:(SmartHomeMessage*)message;

- (BOOL)turnOffAlarm:(SecurityDevice*)device;

- (BOOL)switchDefence:(SecurityDevice*)device;

- (BOOL)isDeviceEnable:(SecurityDevice*)device;

- (SmartHomeMessage*)createMessage:(SecurityDevice*)device
                           command:(NSString*)cmmand
                         broadcast:(BOOL)broadcast;

- (BOOL)sendMessage:(int)router
         net_client:(NetClient*)net_client
            message:(SmartHomeMessage*)message;

- (SecurityDevice*)findSecurityDevice:(NSString *)proxy_id
                          device_name:(NSString*)device_name;

@end

#endif //SDK_SMARTHOME_MANAGER_H_
