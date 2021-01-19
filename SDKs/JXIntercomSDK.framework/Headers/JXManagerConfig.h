//
//  JXManagerConfig.h
//  JXIntercomSDK
//
//  Created by Nansen on 2020/4/2.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "JXIntercomConstants.h"

//系统日志级别
typedef NS_ENUM(NSInteger, JXMLogLevel) {
    JXMLogLevel_DEBUG = 0,
    JXMLogLevel_RELEASE,
};


NS_ASSUME_NONNULL_BEGIN

@interface JXManagerConfig : NSObject

/// 激活码
@property (nonatomic, copy) NSString *activeCode;
/// Id
@property (nonatomic, copy) NSString *appId;
/// Key
@property (nonatomic, copy) NSString *appKey;


#pragma mark ---- 需要配置的内容 ----
/** 中转服务器的地址: http://ip[url]:port*/
@property (nonatomic, copy) NSString *transitURLString;
/** sip 服务器的地址 */
@property (nonatomic, copy) NSString *sipURLString;
/// 渠道编码
@property (nonatomic, copy) NSString *channel;


/// 用来显示的昵称
@property (nonatomic, copy, nullable) NSString *alias;
/// 移动端用账号
@property (nonatomic, copy, nullable) NSString *userId;

/// APNS 推送所使用的 ID, 配置成开发者对接的推送服务所用的 id. 后台会将推送消息推送至此 id.
@property (nonatomic, copy) NSString *pushId;

/// 通过此方法初始化默认的 Config
- (instancetype)initWithSipURL:(NSString *)sipURLString
              transitURLString:(NSString *)transitURLString
                       channel:(NSString *)channel JX_DEPRECATED("请使用 initWithSipURL:transitURLString:channel:appId:appKey:, 继续使用该方法不会获得授权,云端功能受限");


/// 通过此方法初始化默认的 Config
/// @param sipURLString sip服务地址
/// @param transitURLString 中转服务地址
/// @param channel 标识
/// @param appId 授权的 Id
/// @param appKey 授权的 Key
- (instancetype)initWithSipURL:(NSString *)sipURLString
              transitURLString:(NSString *)transitURLString
                       channel:(NSString *)channel
                         appId:(NSString *)appId
                        appKey:(NSString *)appKey;








/**
 下面为默认配置, 上层开发者可以不用修改, 如果要修改, 请在 JXManager 的 startWithConfig 之前修改.
 */
/// 默认是 JXMLogLevel_ERROR
@property (nonatomic, assign) JXMLogLevel logLevel;
/// 默认存储SDK数据的位置: Document/JXHousekeeper
@property (nonatomic, copy) NSString *dataDir;



#pragma mark ---- 视频相关参数 ----
/// 遮罩视频的图片文件路径. 默认是 JXIntercomBundle 中的 novideo.jpg
@property (nonatomic, copy) NSString *maskImagePath;
/// 黑屏的参数. 默认是 @"128,128,128,0"
@property (nonatomic, copy) NSString *blankColor;
/// 本机视频捕捉帧率. 默认是 25
@property (nonatomic, assign) int videoFps;
/// 本机视频采集的分辨率 宽,高. 默认 640 , 480
@property (nonatomic, assign) int defaultVideoWidth;
@property (nonatomic, assign) int defaultVideoHeight;


#pragma mark ---- 音频相关参数 ----
/// 本机播放的对方音频增益. 参数范围:0~30，0表示不增益. 默认 0
@property (nonatomic, assign) int audio_play_agc_level;
/// 发送给对方的音频增益. 参数范围:0~30，0表示不增益. 默认 0
@property (nonatomic, assign) int audio_send_agc_level;




#pragma mark ---- 网络相关参数 ----
/// 指明SDK使用的端口范围，默认是@"30000,35000,40000", 分别表示: 最小端口，最大端口，ICE起始端口。
@property (nonatomic, copy) NSString *portRange;
/// tcp保留端口，可以有多个，中间用逗号分隔. 默认是 nil
@property (nonatomic, copy) NSString *tcpReserver;
/// udp保留端口，可以有多个，中间用逗号分隔. 默认是 nil
@property (nonatomic, copy) NSString *udpReserver;

/// 本地网络中代理的侦听端口，默认为10000
@property (nonatomic, assign) int lan_proxy_broadcast_port;
/// 代理同时在指定的组播地址上侦听，默认为238.18.18.1
@property (nonatomic, copy) NSString *multicast_address;
/// 代理连接的心跳周期, 默认是 60s
@property (nonatomic, assign) int heartbeat_interval;

/// 如果需要设置ssl通讯，这里提供证书存放的本地路径.
@property(nonatomic, strong) NSString *sslCertPath;
/// ssl证书的口令
@property(nonatomic, strong) NSString *sslCertPrivatekey;






@end

NS_ASSUME_NONNULL_END

