#ifndef SDK_NET_DEVICE_H_
#define SDK_NET_DEVICE_H_

#import <Foundation/Foundation.h>

/**
 [device]
 audio_duplex=1
 multi_session=1
 number_mask=2,2,1,2,2,2,0
 p2p_number_mask=0,2,2,2,2,0,0
 [ip]
 0=01011050101@192.168.250.50,室内机,0,0,0
 1=01@192.168.250.8,管理中心,1,1,2
 2=02@192.168.250.9,围墙机,1,0,3
 3=0101101@192.168.250.10,室外机,1,0,1
 */

/**
 * [device]解析
 */
@interface DeviceProperty : NSObject
/** 这个类型的设备是否支持双工语音对讲，目前都是支持的，不支持的设备应该不予考虑
 */
@property(nonatomic, readonly) BOOL audioDuplex;

/** 这个类型的设备是否支持多路会话，sip类型的设备一般都支持，其他类型的未必支持
 */
@property(nonatomic, readonly) BOOL multiSession;

/** 房号表的掩码  number_mask=2,2,1,2,2,2,0 */
@property(nonatomic, readonly) NSArray<NSNumber *> *numberMask;

/** 户户通的房号表掩码 p2p_number_mask=0,2,2,2,2,0,0*/
@property(nonatomic, readonly) NSArray<NSNumber *> *p2pNumberMask;

/** 如果 dynamicTable=true，表示室外机是通过动态发现的，也就是说，一开始代理不会给我们IP表
 只有室外机上线之后，代理才会通知我们，我们可以在onIntercomProxyDeviceChanged
 中更新UI。dynamicTable=true也意味着室外机一开始没有alias，所以，UI上可以改名字
 然后通知代理保存这个名字，其他终端同时被同步。
 */
@property(nonatomic,readonly) BOOL dynamicTable;
@end

/**
 * [ip]解析
 */
@interface SubDevice : NSObject

@property(nonatomic, readonly) NSString *index;
/** 房号，比如 01031020601 */
@property(nonatomic, readonly) NSString *name;

/** 室外机的ip地址 192.168.250.29 */
@property(nonatomic, readonly) NSString *ip;

/** 室外机的别名，用来显示给用户 */
@property(nonatomic, readonly) NSString *alias;

/** 室外机是否支持双工,目前都支持的 */
@property(nonatomic, readonly) BOOL duplex;

/** 室外机是否支持语音，目前都支持 */
@property(nonatomic, readonly) BOOL hasVoice;

/** 室外机类型，由系统约定 */
@property(nonatomic, readonly) NSInteger deviceType;

/** if devieType == IntercomDeviceTypeCamera , IPC is support PTZ?
 if support, we can control it!
 */
@property(nonatomic, readonly) BOOL ptzSupport;
@end

/*
 *一个设备类型可以连接多个室外机设备，由[ip]节点来描述
 [alias] = [scheme],[device_class],[name]
 quanshitong=icom,commonsip,quanshitong
 p2p=icom,commonsip,p2p
 ext=icom,ext,ext
 knx=gateway,bus,knx
 security=gateway,security,security
 dahua=icom,camera,camera
 */
@interface NetDeviceItem : NSObject
/** 设备别名 */
@property(nonatomic, readonly) NSString *alias;

/** 设备类，应该不会使用到*/
@property(nonatomic, readonly) NSString *device_class;

@property(nonatomic, readonly) NSString *name;

/** 设备命名空间 参考intercom_constants 定义*/
@property(nonatomic, readonly) NSString *scheme;

/**
 * 我们从configure中解析出 subDevices和 deviceProperty，如果需要其他信息，可以在
 * configure字典中查找
 */
@property(nonatomic, readonly) NSDictionary *configure;
@property(nonatomic, readonly) DeviceProperty *deviceProperty;

/**
 * 每个设备可以有多个子设备，子设备比如室外机，围墙机，物业管理机，他们之间的区别在于IP地址的不同
 * 其他的如协议等完全一致，在UI上需要展示所有的SubDevice
 */
@property(nonatomic, readonly) NSArray<SubDevice *> *subDevices;

/** 当前子设备是否是门禁设备 */
- (BOOL)icomDevice;

/** 当前子设备是否是智能家居设备 */
- (BOOL)smarthomeDevice;

- (BOOL)defenceDevice;

- (void)updateConfigure:(NSString*)config;

-(SubDevice*) findSubDeviceWithIndex:(NSString*)index;

-(SubDevice*) findSubDeviceWithName:(NSString*)name;

/** 是否支持召梯功能*/
- (BOOL)supportElevator;

@end

@interface NetDevice : NSObject
@property(nonatomic, readonly) NSDictionary<NSString *, NSDictionary *> *firmware;
/** 这一组设备所在的代理id */
@property(nonatomic, readonly) NSString *proxy_id;

/** 设备集合，可能包含门禁设备，安防设备等*/
@property(nonatomic, readonly) NSArray<NetDeviceItem *> *devices;

- (id)initWithJsonString:(NSString *)json;

- (void)udpateSubDevice:(NSString*)device_id config:(NSString*)config;

- (NetDeviceItem*) findNetDeviceItem:(NSString*)device_id;

@end

#endif //SDK_NET_DEVICE_H_
