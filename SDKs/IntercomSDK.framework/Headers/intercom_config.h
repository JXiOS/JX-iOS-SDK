#ifndef SDK_INTERCOM_CONFIG_H_
#define SDK_INTERCOM_CONFIG_H_

#import <Foundation/Foundation.h>

/**
 * 系统配置参数集
 */

//应用程序基础配置
@interface AppConf : NSObject

@property(nonatomic, strong) NSString *app_version;

@property(nonatomic, strong) NSString *channel;
/** 保存日志和client_id的工作目录，可以设置为Document目录 */
@property(nonatomic, strong) NSString *work_dir;

/** 保存访客记录的位置，可以设置为cache目录*/
@property(nonatomic, strong) NSString *media_dir;

/** 日志是否记录到文件 */
@property(nonatomic, assign) BOOL log_to_file;

/** 日志水平 */
@property(nonatomic, assign) int log_level;  // LogLevel

@property(nonatomic, strong) NSString *timezone;

- (id)initWithParam:(NSString *)app_version
            channel:(NSString *)channel
           work_dir:(NSString *)work_dir
          media_dir:(NSString *)media_dir
        log_to_file:(BOOL)log_to_file
          log_level:(int)log_level;

- (NSString *)toJson;

@end

@interface ClientConf : NSObject
// NetClientType , IOS: NetClientTypeClient
@property(nonatomic, assign) int type;

// NetClientSubType ,IOS:NetClientSubTypePad
@property(nonatomic, assign) int sub_type;

/** 家庭id */
@property(nonatomic, strong) NSString *family_id;

/** 终端别名，用于室内通，可以设置为:客厅，卧室...*/
@property(nonatomic, strong) NSString *alias;

/** 对应于底座的标识*/
@property(nonatomic, strong) NSString *button_key;

/** 云端推送账号，用于云端推送*/
@property(nonatomic, strong) NSString *pushid;

/**sn 移动端可以用自己的账号 */
@property(nonatomic, strong) NSString *sn;

- (id)initWithParam:(int)type
           sub_type:(int)sub_type
          family_id:(NSString *)family_id
         button_key:(NSString *)button_key
              alias:(NSString *)alias
             pushid:(NSString *)pushid
                 sn:(NSString *)sn;

- (NSString *)toJson;

@end

@interface PortConf : NSObject
/**
 * 系统使用的端口范围：
 * 比如: 30000,35000,40000: [min,max,ice_port_base]
 * 系统中某些端口要留给室外机，所以，我们约定一个范围
 */
@property(nonatomic, strong) NSString *port_range;

/**
 * tcp保留端口，留给室外机的和系统自身使用的，比如10000
 * 格式：11111,1211,1121212
 */
@property(nonatomic, strong) NSString *tcp_reserver;

/**
 * udp保留端口，留给室外机的
 * 格式：11111,1211,1121212
 */
@property(nonatomic, strong) NSString *udp_reserver;

- (id)initWithParam:(NSString *)port_range
       tcp_reserver:(NSString *)tcp_reserver
       udp_reserver:(NSString *)udp_reserver;

- (NSString *)toJson;

@end

/**
 *代理配置
 */

@interface ProxyConf : NSObject

/** server 的UDP侦听端口,当前系统为: 10000 */
@property(nonatomic, assign) int lan_proxy_broadcast_port;

/** server 侦听的组播地址,当前系统为: 238.18.18.1*/
@property(nonatomic, strong) NSString *multicast_address;

/** 客户端的心跳周期,默认为 60s */
@property(nonatomic, assign) int heartbeat_interval;

/** 广播域的掩码定义: 255.255.255.0 */
@property(nonatomic, assign) BOOL enable_lan_broadcast;

/** 是否开启局域网发现*/
@property(nonatomic, assign) BOOL lan_discovery_enable;

/** 中转服务器的地址: http://ip[url]:port*/
@property(nonatomic, strong) NSString *proxy_server;

/** 中转服务器的加密key，用于加密传输内容,可以为空*/
@property(nonatomic, strong) NSString *ssl_cert_path;

@property(nonatomic, strong) NSString *ssl_cert_private_key;

- (id)initWithParam:(int)lan_proxy_broadcast_port
  multicast_address:(NSString *)multicast_address
 heartbeat_interval:(int)heartbeat_interval
enable_lan_broadcast:(BOOL)enable_lan_broadcast
lan_discovery_enable:(BOOL)lan_discovery_enable
       proxy_server:(NSString *)proxy_server
      ssl_cert_path:(NSString *)ssl_cert_path
ssl_cert_private_key:(NSString *)ssl_cert_private_key;

- (NSString *)toJson;

@end

/** 门禁对讲的sip配置策略
 * 我们需要从服务器获取sip的配置信息，如果设置 auth_user/pwd
 * 服务器可以使用此账号生成sip账号，并且返回详细的配置
 * sip配置将缓存到本地，在这里指定文件名
 * 每次启动的时候，首先从本地加载sip配置，然后再从服务器请求
 * 如果配置有更新，则重新启动sip服务
 */
@interface ServerConfig : NSObject
/** sip配置服务器请求地址 */
@property(nonatomic, strong) NSString *url;
/** 认证用户 */
@property(nonatomic, strong) NSString *auth_user;
/** 认证密码 */
@property(nonatomic, strong) NSString *auth_pwd;
/** 本地缓存的配置文件名 */
@property(nonatomic, strong) NSString *cache_filename;

/** 失败重试的时间间隔 ,秒单位,默认5s*/
@property(nonatomic, assign) int retry_interval;

- (id)initWithParam:(NSString *)url
          auth_user:(NSString *)auth_user
           auth_pwd:(NSString *)auth_pwd
     cache_filename:(NSString *)cache_filename
     retry_interval:(int)retry_interval;

- (NSString *)toJson;

@end

@interface CaptureConfig : NSObject

/**
 * 本机视频捕捉的参数，参数参考各个平台自身的定义
 * 未必所有分辨率都支持
 */

/** 视频宽度: 640 */
@property(nonatomic, assign) int capture_video_width;
/** 视频高度: 480 */
@property(nonatomic, assign) int capture_video_height;
/** 捕捉帧率: 25*/
@property(nonatomic, assign) int capture_video_fps;
/** 移动端，底层输出的格式未必符合此处的设定
 */
@property(nonatomic, assign) int capture_video_format;

/**
 * 摄像头选择模式: 移动设备默认为:1，表示使用前置后置来设置
 * 其他平台只能通过名称来设置
 */
@property(nonatomic, assign) int camera_type;

/** kFrontCameraName or kBackCameraName*/
@property(nonatomic, strong) NSString *camera_name;

@property(nonatomic,assign) int max_cache;

- (id)initWithParam:(int)width
             height:(int)height
                fps:(int)fps
             format:(int)format
               type:(int)type
               name:(NSString*)name
              cache:(int)cache;

- (NSString *)toJson;

- (NSDictionary*)toDictionary;

@end

@interface VideoCodecConfig : NSObject
/**
 * 摄像头视频编码参数，采用默认值即可
 * 如果想要提高编码质量，可以自行设置以下参数
 */
@property(nonatomic, assign) int video_encode_profile;
@property(nonatomic, assign) int video_encode_bitrate;
@property(nonatomic, assign) int video_encode_gotsize;
@property(nonatomic, assign) int video_encode_avg_bps;
@property(nonatomic, assign) int video_encode_max_bps;
@property(nonatomic, assign) double video_encode_qcompress;
@property(nonatomic, assign) double video_encode_qblur;
@property(nonatomic, assign) int video_encode_qmin;
@property(nonatomic, assign) int video_encode_qmax;
@property(nonatomic, assign) int video_encode_crf;
@property(nonatomic, assign) int video_encode_codec;
@property(nonatomic, strong) NSString *video_encode_preset;
@property(nonatomic, strong) NSString *video_encode_tune;
@property(nonatomic, assign) int video_frame_mtu;
@property(nonatomic, assign) int video_encode_codec_width;
@property(nonatomic, assign) int video_encode_codec_height;
@property(nonatomic, assign) int frame_rate;
@property(nonatomic, assign) int video_decode_engine;
@property(nonatomic, assign) int video_encode_engine;

- (id)init;

- (NSString *)toJson;

- (NSDictionary*)toDictionary;
@end

/** 对讲功能的配置 */
@interface IntercomConfig : NSObject

@property(nonatomic,strong) CaptureConfig* capture;
@property(nonatomic,strong) VideoCodecConfig* video_codec_conf;


/**
 * 本机通用的音频格式，目前最佳设置为8000/1/16
 */
@property(nonatomic, assign) int audio_samplerate;
@property(nonatomic, assign) int audio_channels;
@property(nonatomic, assign) int bits_per_sample;

/**
 *关于音频媒体流的一些参数
 */

/** play remote audio agc [1~30?],0:do not agc*/
@property(nonatomic, assign) int audio_play_agc_level;
/** do agc before send to peer [1~30?],0:do not agc*/
@property(nonatomic, assign) int audio_send_agc_level;

//please see webrtc: WebRtcAgc_config: default is 9
@property(nonatomic, assign) int audio_play_agc_compression_gain_db;
//please see webrtc: WebRtcAgc_config: default is 9
@property(nonatomic, assign) int audio_send_agc_compression_gain_db;

//denoise before play
@property(nonatomic, assign) BOOL audio_play_denoise;
//denoise before send to peer
@property(nonatomic, assign) BOOL audio_send_denoise;

/**
 * 控制音频延迟,一般情况可以设置80000~120000,表示音频延迟缓冲区最大时间长度
 * 如果为了保证最低延迟，可以设置为0,此刻需要保证是局域网连接室外机或者直连
 */
@property(nonatomic, assign) long max_audio_delay_us;


/** 黑屏的参数(R,G,B,A):@"128,128,128,0 */
@property(nonatomic, strong) NSString *blank_color;

/** 会话初始化最长等待时间，默认为60s即可*/
@property(nonatomic, assign) int max_session_wait_time;

/** 楼宇对讲会话最长持续时间，默认为200s即可*/
@property(nonatomic, assign) int intercom_max_session_connect_time;

//户户通设备，[名称,LAN最长通话时间(秒),WAN最长通话时间(秒)]
@property(nonatomic, strong) NSString *special_p2p_device;

//室内通设备，[名称,LAN最长通话时间(秒),WAN最长通话时间(秒)]
//考虑到云端资源的占用，可以对云端通话时长进行不一样的限制
// LAN通话，最长可以允许24小时
@property(nonatomic, strong) NSString *speical_ext_device;

- (id)initWithDefault:(CaptureConfig*)capture_conf
     video_codec_conf:(VideoCodecConfig*) codec_conf
          blank_color:(NSString *)blank_color;

- (NSString *)toJson;
@end

/**
 * sip账号
 */
@interface SipAuth : NSObject
@property(nonatomic, strong) NSString *username;
@property(nonatomic, strong) NSString *userid;
@property(nonatomic, strong) NSString *passwd;
@end

/**
 * sip代理
 */
@interface SipProxy : NSObject
@property(nonatomic, strong) NSString *proxy;
@property(nonatomic, strong) NSString *identity;
@property(nonatomic, strong) NSString *route;
@property(nonatomic, strong) NSString *contact_param;
@property(nonatomic, assign) int expires;
@end

@interface SipConfig : NSObject
/** sip配置*/
@property(nonatomic, assign) int session_expires;
@property(nonatomic, assign) BOOL use_rport;
@property(nonatomic, assign) BOOL reuse_authorization;
@property(nonatomic, assign) BOOL expire_old_registration_contacts;
@property(nonatomic, assign) int dscp;
@property(nonatomic, assign) BOOL use_ipv6;
@property(nonatomic, assign) int sip_port_range;
@property(nonatomic, assign) int sip_listen_port;
@property(nonatomic, strong) NSString *transport;
@property(nonatomic, assign) BOOL one_matching_codec;
@property(nonatomic, assign) BOOL use_double_registrations;
@property(nonatomic, assign) BOOL add_dates;
@property(nonatomic, assign) int keepalive_period;
@property(nonatomic, assign) BOOL tcp_tls_keepalive;
@property(nonatomic, strong) NSString *user_agent;
@property(nonatomic, assign) BOOL use_exosip2_version;
@property(nonatomic, strong) NSString *default_contact;
@property(nonatomic, assign) int max_calls;
@property(nonatomic, strong) NSString *answer_options;
@property(nonatomic, assign) BOOL prevent_colliding_calls;
@property(nonatomic, assign) NSString *cert_password;

/** 支持多个账号 */
@property(nonatomic, strong) NSMutableArray<SipAuth *> *auths;
@property(nonatomic, strong) NSMutableArray<SipProxy *> *proxies;

/** ICE 配置，一般只需要配置 stun_server*/
@property(nonatomic, strong) NSString *turn_server;
@property(nonatomic, strong) NSString *turn_username;
@property(nonatomic, strong) NSString *turn_pwd;
@property(nonatomic, strong) NSString *turn_auth_type;
@property(nonatomic, strong) NSString *turn_pwd_type;
@property(nonatomic, strong) NSString *turn_conn_type;
@property(nonatomic, strong) NSString *turn_auth_realm;
@property(nonatomic, strong) NSString *stun_server;
@property(nonatomic, assign) BOOL loopaddr;
@property(nonatomic, assign) BOOL aggressive;

/** 默认设置只支持一个账号登陆 */
- (id)initWithDefault:(NSString *)user_id
             password:(NSString *)password
            transport:(NSString *)transport
           sip_server:(NSString *)sip_server
          server_port:(int)server_port
            stun_port:(int)stun_port
              use_ice:(BOOL)use_ice;

- (NSString *)toJson;
@end

#endif //SDK_INTERCOM_CONFIG_H_
