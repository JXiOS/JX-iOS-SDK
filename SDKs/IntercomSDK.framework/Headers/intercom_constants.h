#ifndef SDK_INTERCOM_CONSTANTS_H_
#define SDK_INTERCOM_CONSTANTS_H_

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, LogLevel) { Info = 0, Warning, Error, Fatal };

/**
 * 客户端类型
 */
typedef NS_ENUM(NSInteger, NetClientType) {
  NetClientTypeUndefined = 0,
  NetClientTypeProxy = 1,   //代理客户端
  NetClientTypeClient = 2,  //终端，比如PAD，mobile
};

/**
 * 终端的类型
 */
typedef NS_ENUM(NSInteger, NetClientSubType) {
  NetClientSubTypeUndefined = -1,
  NetClientSubTypePad,       //平板
  NetClientSubTypeMobile,    //手机
  NetClientSubTypeTV,        //智能电视，目前还没有
  NetClientSubTypeEmbedded,  //嵌入式底座
};

typedef NS_ENUM(NSInteger, NetProxytSubType) {
  NetProxySubTypeUndefined = -1,
  NetProxySubTypeIndoor = 0,
  NetProxySubTypeIntercom = 1
};

typedef NS_ENUM(NSInteger, NetClientOSPlatformType) {
  NetClientOSPlatformUndefined = 0,
  NetClientOSPlatformOPENWRT = 1,
  NetClientOSPlatformLINUX = 2,
  NetClientOSPlatformANDROID = 3,
  NetClientOSPlatformIOS = 4,
  NetClientOSPlatformMAC = 5,
  NetClientOSPlatformWIN = 6
};

typedef NS_ENUM(NSInteger, Router) { LAN = 0, PROXY };

typedef NS_ENUM(NSInteger, SubDeviceType) {
  IntercomDeviceTypeIndoor = 0,     //室内机
  IntercomDeviceTypeOutdoor,        //室外机
  IntercomDeviceTypeAdministrator,  //管理中心
  IntercomDeviceTypeWall,           //围墙机
  IntercomDeviceTypeVillaOutdoor,   //别墅室外机
  IntercomDeviceTypeCamera          //摄像头
};

/**
 * 代理的事件类型
 */
typedef NS_ENUM(NSInteger, IntercomProxyEvent) {
  IntercomProxyEventMessage = 0,
  IntercomProxyEventSendResult,
  IntercomProxyEventProxyStateChanged,
  IntercomProxyEventClientStateChanged,
  IntercomProxyEventInternetProxyStateChanged,
  IntercomProxyEventDeviceChanged
};

typedef NS_ENUM(NSInteger, IntercomEvent) {
  IntercomEventBase = 0,
  IntercomEventMessage = IntercomEventBase,
  IntercomVideoReady = IntercomEventBase + 1,
  IntercomSnapshotReady = IntercomEventBase + 2,
  IntercomVideoDimensionChanged = IntercomEventBase + 3,
  IntercomSipStateChanged = IntercomEventBase + 4,
  IntercomDtmfReady = IntercomEventBase + 5,
  IntercomStreamStateChanged = IntercomEventBase + 6,
  IntercomTransportStarted = IntercomEventBase + 7,
  IntercomTransportStatistics = IntercomEventBase + 8,
  IntercomTransportIceNegotiation = IntercomEventBase + 9
};

typedef NS_ENUM(NSInteger, AppEvent) {
  AppInitialized = 0,
  MediaDeviceStateChanged,
  AppUnInitialized,
  CameraStateChanged,
  CameraError,
  DeviceActive,
};

typedef NS_ENUM(NSInteger, SipRegistrationState) {
  SipRegistrationNone = 0,
  SipRegistrationProgress,
  SipRegistrationOk,
  SipRegistrationCleared,
  SipRegistrationFailed,
};

typedef NS_ENUM(NSInteger, TransportMediaType) {
  TransportMediaTypeNone = 0,
  TransportMediaTypeAudio,
  TransportMediaTypeVideo,
};

typedef NS_ENUM(NSInteger, StreamState) {
  StreamStateStart = 0,
  StreamStateStop,
};

typedef NS_ENUM(NSInteger, IntercomMessageResult) {
  IntercomMessageResultOK = 0,
  IntercomMessageResultErr,
  IntercomMessageResultDeviceName,
  IntercomMessageResultSessionId,
  IntercomMessageResultNOAuthority,
  IntercomMessageResultUnsupportCommand,
  IntercomMessageResultNotConnected,
  IntercomMessageResultDeviceBusy,
  IntercomMessageResultUserName,
  IntercomMessageResultNet,
  IntercomMessageResultTransport,

  IntercomMessageResultSipReasonNoResponse = 100,
  IntercomMessageResultSipReasonBadCredentials,
  IntercomMessageResultSipReasonDecline,
  IntercomMessageResultSipReasonNotFound,
  IntercomMessageResultSipReasonNotAnswered,
  IntercomMessageResultSipReasonBusy,
  IntercomMessageResultSipReasonTimeout,
};

typedef NS_ENUM(NSInteger, DeviceActiveResult) {
  DeviceActive_ErrorLib = -2, //SDK库有误
  DeviceActive_ErrorNet = -1, //网络不通
  DeviceActive_Success = 200, //激活成功
  DeviceActive_CodeUsedInOtherDevice = 301, //当前提供的激活码已经在其他设备上激活
  DeviceActive_Expired = 302, //激活码已经过期
  DeviceActive_Invalid = 303, //激活码无效
  DeviceActive_AlreadyActivated = 304 //该设备已经使用其他激活码激活了
};

extern NSString *const kAppScheme;
extern NSString *const kClientScheme;
extern NSString *const kMCUScheme;

extern NSString *const kIntercomScheme;
extern NSString *const kIntercomCommandInvite;
extern NSString *const kIntercomCommandRinging;
extern NSString *const kIntercomCommandCall;
extern NSString *const kIntercomCommandPickup;
extern NSString *const kIntercomCommandHangup;
extern NSString *const kIntercomCommandUnlock;
extern NSString *const kIntercomCommandPickupByOther;
extern NSString *const kIntercomCommandBeginTalk;
extern NSString *const kIntercomCommandEndTalk;
extern NSString *const kIntercomCommandSessionTimeout;
extern NSString *const kIntercomCommandLeaveSession;
extern NSString *const kIntercomCommandMessage;
extern NSString *const kIntercomCommandSnapshot;
/** config or control intercom device , use this command*/
extern NSString *const kIntercomCommandConfigure;

/** if proxy send this command to us , must be a warning or urgent event occur
 from intercom device
 like camera obscured and so on...
 */
extern NSString *const kIntercomCommandNotify;

extern NSString *const kProxyScheme;

extern NSString *const kMediaPlayScheme;

extern NSString *const kSmarthomeScheme;
extern NSString *const kSmarthomeQuery;
extern NSString *const kSmarthomeSwitch;
extern NSString *const kSmarthomeEvent;
extern NSString *const kSmarthomeCancel;
extern NSString *const kSmarthomeState;
extern NSString *const kSmarthomeDevice;
extern NSString *const kSmarthomeButton;
extern NSString *const kSmarthomeEmbed;
extern NSString *const kSmarthomeMCUDevice;
extern NSString *const kSmarthomeMCUClient;

extern NSString *const kSecurityReport;
extern NSString *const kSecurityAlert;

// button message scheme
extern NSString *const kButtonScheme;
//用户按下SOS键
extern NSString *const kButtonSOS;
//用户按下User键
extern NSString *const kButtonUser;
//用户按下monitor键
extern NSString *const kButtonMonitor;
//用户按下摘机／挂断键，由上层来处理接听还是挂断
extern NSString *const kButtonPickup;
//用户按下开锁键
extern NSString *const kButtonUnlock;
//平板放置在底座上
extern NSString *const kButtonPadNear;
//平板从底座拿开
extern NSString *const kButtonPadLeave;
//感应到人体靠近
extern NSString *const kButtonHumanNear;
//感应到人体离开
extern NSString *const kButtonHumanLeav;

extern NSString *const kFrontCameraName;

extern NSString *const kBackCameraName;

#endif //SDK_INTERCOM_CONSTANTS_H_
