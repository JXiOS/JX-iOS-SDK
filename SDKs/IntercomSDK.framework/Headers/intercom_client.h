#ifndef SDK_INTERCOM_CLIENT_H_
#define SDK_INTERCOM_CLIENT_H_

#import <Foundation/Foundation.h>
#import "net_client.h"
#import "net_device.h"
#import "intercom_constants.h"

@class Intercom;

/**
 *每个代理至少包含两个重要信息
 * NetClient:描述代理本身的信息
 * NetDevice:描述该代理下的设备信息，可以包含多个设备
 */
@interface IntercomProxyDescription : NSObject
//代理信息
@property(nonatomic, readonly) NetClient *netClient;
//代理中包含的设备信息以及其他信息
@property(nonatomic, readonly) NetDevice *netDevice;

@end

/**
 * 代理信息,包含局域网代理和internet代理
 */
@interface IntercomProxy : NSObject
// LAN 代理
@property(nonatomic, readonly) IntercomProxyDescription *lanProxy;
// WAN 代理
@property(nonatomic, readonly) IntercomProxyDescription *wanProxy;
//代理id
@property(nonatomic, readonly) NSString *proxy_id;

- (id)initWithProxy:(IntercomProxy *)proxy;

/**
 *从json中创建代理
 */
- (id)initWithParam:(Router)router
      client_string:(NSString *)client_string
      device_config:(NSString *)device_config;

/**
 *代理状态发生变化，我们要更新一下
 */
- (void)update:(IntercomProxy *)proxy;

/** 如果LAN和WAN都是空，则为空 */
- (BOOL)empty;

- (BOOL)lanOnline;

- (BOOL)wanOnline;

- (void)remove:(Router)router;
@end

/**
 *客户端信息，包含局域网客户端和Internet上的客户端
 *对于局域网客户端，可以出现在多个代理中，所以放在字典里，key=proxy_id
 *无论从LAN／WAN上线，代理信息都是一样的
 */
@interface IntercomClient : NSObject

// LAN终端，终端可能从不同的代理上线，比如室内有多个底座，可能就有几个上线途径
// lanClients: <代理id,终端信息>
@property(nonatomic, readonly)
NSDictionary<NSString *, NetClient *> *lanClients;

//从WAN上线的代理，每个终端只有一个
@property(nonatomic, readonly) NetClient *wanClient;

//终端 id
@property(nonatomic, readonly) NSString *client_id;

- (BOOL)empty;

- (void)add:(Router)router
   proxy_id:(NSString *)proxy_id
 net_client:(NetClient *)net_client;

- (void)remove:(Router)router proxy_id:(NSString *)proxy_id;

- (BOOL)padOnline:(NSString *)proxy_id button_key:(NSString *)button_key;

- (BOOL)lanOnline;

- (BOOL)wanOnline;

- (NetClient *)findLanClientWithProxyId:(NSString *)proxy_id;
@end

@protocol InternalIntercomClientDelegate<NSObject>

- (void)onIntercomProxyStateChanged:(NSString *)proxy_id online:(BOOL)online;

- (void)onIntercomProxyOnlineStateChanged:(NSString *)proxy_id
                                   router:(Router)router
                                   online:(BOOL)online;

- (void)onIntercomClientStateChanged:(NSString *)client_id online:(BOOL)online;

- (void)onIntercomClientOnlineStateChanged:(NSString *)client_id
                                  proxy_id:(NSString *)proxy_id
                                    router:(Router)router
                                    online:(BOOL)online;
@end

/**
 * 代理和终端的管理接口
 */
@interface IntercomClientManager : NSObject

//[proxy_id,IntercomProxy]
@property(nonatomic, readonly)
NSDictionary<NSString *, IntercomProxy *> *proxies;

//[client_id, IntercomClient]
@property(nonatomic, readonly)
NSDictionary<NSString *, IntercomClient *> *clients;

@property(nonatomic, assign, readonly) BOOL internetProxyOnline;

@property(nonatomic, weak) Intercom *intercom;

- (id)initWithParam:(Intercom*)intercom;

- (void)updateProxy:(Router)router
             online:(BOOL)online
              proxy:(IntercomProxy *)proxy;

- (void)updateClient:(Router)router
              online:(BOOL)online
           client_id:(NSString *)client_id
       client_string:(NSString *)client_string;

- (void)changeInternetState:(BOOL)online;

- (BOOL)padOnline:(NSString *)proxy_id
       button_key:(NSString *)button_key;

- (IntercomProxy *)findProxy:(NSString *)proxy_id;

- (IntercomClient *)findClient:(NSString *)client_id;

- (NSString*)findDeviceConfigure:(NSString*)proxy_id
                     device_name:(NSString*)device_name
                             key:(NSString*)key;

- (void)cleanup;

@end

#endif //SDK_INTERCOM_CLIENT_H_
