//
//  JXManagerConfig.h
//  JXIntercomSDK
//
//  Created by Nansen on 2020/4/2.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <IntercomSDK/intercom_constants.h>

//系统日志级别
typedef NS_ENUM(NSInteger, JXMLogLevel) {
    JXMLogLevel_DEBUG = 0,
    JXMLogLevel_VERBOSE,
    JXMLogLevel_INFO,
    JXMLogLevel_WARNING,
    JXMLogLevel_ERROR,
};


NS_ASSUME_NONNULL_BEGIN

@interface JXManagerConfig : NSObject

#pragma mark ---- 需要配置的内容 ----
/** 中转服务器的地址: http://ip[url]:port*/
@property (nonatomic, copy) NSString *transitURLString;
/** sip 服务器的地址 */
@property (nonatomic, copy) NSString *sipURLString;

/// 通过此方法初始化默认的 Config
- (instancetype)initWithSipURL:(NSString *)sipURLString
              transitURLString:(NSString *)transitURLString;

#pragma mark - ======== 下面为默认配置,开发者可以不用修改 ========
- (void)deploySystemId:(NSString *)systemId
             buttonKey:(NSString *)buttonKey
                myName:(NSString *)myName
                  mySN:(NSString *)mySN;

/// 退出账号, 清理相关内容
- (void)logOutClear;

/// 默认是 JXMLogLevel_ERROR
@property (nonatomic, assign) JXMLogLevel logLevel;

/// 设备及用户唯一标识, 即同一设备和同一用户
@property (nonatomic, copy, nullable) NSString *systemId;

@property (nonatomic, copy, nullable) NSString *buttonKey;

@property (nonatomic, copy, nullable) NSString *myName;
/// 移动端用账号就可以
@property (nonatomic, copy, nullable) NSString *mySN;



#pragma mark ---- Appconf ----
@property (nonatomic, copy) NSString *appVersion;

@property (nonatomic, copy) NSString *channel;
/// 默认存储位置-document/housekeeper
@property (nonatomic, copy) NSString *workDir;
/// 默认的数据库存储位置-document/housekeeper
@property (nonatomic, copy) NSString *dbPath;
/// 保存音频,视频的路径
@property (nonatomic, copy) NSString *mediaDir;
/// 遮罩视频的图片
@property (nonatomic, copy) NSString *maskImagePath;



#pragma mark ---- CaptureConfig 本机视频捕捉的参数 ----
/// 视频宽度: 640
@property (nonatomic, assign) int defaultVideoWidth;
/// 视频高度: 480
@property (nonatomic, assign) int defaultVideoHeight;
/// 视频捕捉帧率: 25
@property (nonatomic, assign) int videoFps;



#pragma mark ---- IntercomConf ----
/// 黑屏参数 @"128,128,128,0"
@property (nonatomic, copy) NSString *blankColor;
/// 音频延迟参数: 允许缓冲远端音频数据的时间长度(微秒单位)  V64458: 80000; V65002:120000
@property (nonatomic, assign) NSInteger maxAudioDelayUs;
///// 音频延迟参数: 音频播放缓冲区欠载最长等待多少微秒. 这个数值不要超过10000(10毫秒)，V64268 8000
//@property (nonatomic, assign) NSInteger maxAudioPlayoutWaitingUs;
/// 声音采样频率: 8000
@property (nonatomic, assign) int audioSamplerate;

/// V65002 --> webrtc 3A模块(增益，去噪，回声消除)
/// 0:do not agc
@property (nonatomic, assign) int audio_play_agc_level;
/// 0
@property (nonatomic, assign) int audio_send_agc_level;
/// 增益 20
@property (nonatomic, assign) int audio_play_agc_compression_gain_db;
/// 20
@property (nonatomic, assign) int audio_send_agc_compression_gain_db;
/// 降噪 false
@property (nonatomic, assign) BOOL audio_play_denoise;
/// false
@property (nonatomic, assign) BOOL audio_send_denoise;
/// V65002 <--


#pragma mark ---- ProxyConf ----
/** server 的UDP侦听端口,当前系统为: 10000 */
@property (nonatomic, assign) int lan_proxy_broadcast_port;
/** server 侦听的组播地址,当前系统为: 238.18.18.1*/
@property (nonatomic, copy) NSString *multicast_address;
/** 客户端的心跳周期,默认为 60s */
@property (nonatomic, assign) int heartbeat_interval;
/** 中转服务器的加密key，用于加密传输内容,可以为空*/
@property(nonatomic, strong) NSString *certPath;

@property(nonatomic, strong) NSString *certPrivatekey;



#pragma mark ---- ClientConf ----
/// 推送的 id
@property (nonatomic, copy) NSString *pushId;
/// NetClientTypeClient - 2
@property (nonatomic, assign) int clientType;
/// NetClientSubTypeMobile - 1
@property (nonatomic, assign) int clientSubType;


#pragma mark ---- PortConf ----
/// 系统使用的端口范围 @"30000,35000,40000"
@property (nonatomic, copy) NSString *portRange;

#pragma mark ---- ServerConf ----
/// 本地缓存的配置文件名: client_sip.json
@property (nonatomic, copy) NSString *cacheFileName;
/// 失败重试的时间间隔 ,秒单位,默认5s
@property (nonatomic, assign) int retryInterval;

#pragma mark - ======== 配置文件 ========
- (NSString *)getAppConfJsonString;

- (NSString *)getIntercom_confJsonString;

- (NSString *)getClientConfJsonStringWithHome:(NSString *)homeId;

- (NSString *)getPortConfJsonString;

- (NSString *)getServerConfigJsonString;

- (NSString *)getProxyConfJsonString;

@end

NS_ASSUME_NONNULL_END

