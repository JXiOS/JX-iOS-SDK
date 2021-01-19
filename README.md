[更新说明](#更新说明)

- [2.0.66302](#2.0.66302)



## 概要介绍

慧管家 SDK 兼容 iOS10.0, Demo 兼容 iOS10.0.

## 结构

`JXManager` 是整个 SDK 功能的管理类, 其中可以进行家庭管理, 设备管理, 通话管理, 历史记录查询, 安防设置等相关功能, 具体可以查看注释.

`JXManagerConfig` 是 SDK 的配置文件, 相关参数可在 SDK 启动之前进行配置

![](https://github.com/JXiOS/JX-iOS-SDK/blob/main/asdk.png)




## 手动集成

将`IntercomSDK.framework`, `JXIntercomBundle.bundle`, `JXIntercomSDK.framework`  拖入工程中

设置 `General` -> `Frameworks, Libraries, and Embedded Content` -> `IntercomSDK.framework` 和  `JXIntercomSDK.framework` 为 `Embed & Sign`

在工程的 `Info.plist` 中设置好权限字段: 

​	NSCameraUsageDescription

​	NSMicrophoneUsageDescription

​	NSPhotoLibraryAddUsageDescription

​	App Transport Security Settings -> Allow Arbitrary Loads : YES

给工程添加如下依赖库:

- AudioToolbox.framework

- AVFoundation.framework
- CoreMedia.framework

- CoreAudio.framework
- CoreVideo.framework
- VideoToolbox.framework

- CoreGraphics.framework
- OpenGLES.framework
- CoreTelephony.framework
- QuartzCore.framework
- libbz2.1.0.tbd
- libiconv.2.4.0.tbd
- libresolv.9.tbd
- libz.1.2.5.tbd



设置 `Build Settings` -> `Enable Bitcode` 为 `NO`  

设置 `Build Settings` -> `Other Linker Flags` 添加 `-Objc`

由于库包含模拟器版本，会导致打包失败。所以需要在打包之前将模拟器版本剥去:

- 将 `jx_strip_archs.sh` 文件复制到工程目录下面
- 在 `Build Phases` 中增加过程，类型为 `New Run Script Phase`
- 输入路径: `/bin/sh "${SRCROOT}/工程文件夹/jx_strip_archs.sh"`

具体可以参照 Demo 中.



## 初始化[JXManager]

在使用门禁功能之前需要先将配置项设置好: 

```objective-c
NSString *sipURL = @"sip服务器地址";
NSString *transitURL = @"中转服务器地址";
NSString *channel = @"客户标识";
NSString *appid = @"注册的 AppId";
NSString *appkey = @"注册的 AppKey";

JXManagerConfig *jxConfig = [[JXManagerConfig alloc] initWithSipURL:sipURL transitURLString:transitURL channel:channel appId:appid appKey:appkey];
```

然后启动 `JXMananger` :

```objective-c
[[JXManager defaultManage] startWithConfig:jxConfig];
```


### 关于推送

客户自己选择 APNS 服务的集成商, 在调用 `startHome:` 之前配置好接收推送所使用的 id,
```objective-c
// 这个方法必须在startHome: 调用之前处理, 否则会收不到推送
[[JXManager defaultManage] deployAPNsID:@"推送使用的ID"];
```

在收到对应的推送之后可以直接将对应的Json字符串转成 NSDictionary 提交给 JXManager 处理:
```objective-c
/// Json 转成字典后应该有 key = "message" 的键值对
[[JXManager defaultManage] receivePushNoti:@"Json 转成的字典"];
```



## 登录和登出

```objective-c
/// 登录
/// @param userId 移动端的账号,代表移动端的唯一值
/// @param alias 用来显示的昵称. 传空值则默认为"iPhone"
/// @param activecode 和 userId 绑定的激活码
/// @param completeBlock 指示登录是否成功. 返回 YES 后才可以正常使用 SDK 中的功能. 否则可能发生异常
- (void)loginWithUserId:(NSString *)userId
                  alias:(NSString *)alias
             activecode:(NSString *)activecode
               complete:(void(^)(BOOL succeed))completeBlock;

/// 登出
- (void)logout;
```

鉴权的结果会在 `JXIntercomDelegate` 的方法中告知:

```objective-c
/// 鉴权的结果
/// @param result 参照 DeviceActiveResult 说明
/// @param message message
- (void)onDeviceActivated:(DeviceActiveResult)result message:(NSString * _Nullable)message;
```







## 服务的启动和停用

```objective-c
/// 启动服务
/// @param homeId 家庭参数
- (BOOL)startHome:(NSString *)homeId;

/// 停止服务
/// @param homeId 家庭参数
- (void)stopHome:(NSString *)homeId;

/// 获得当前已经连接上的家庭
- (NSArray<NSString *> *)getConnectingHomes;
```



## 设备的查看[JXDeviceManager]

通过 `[[JXManager defaultManage].deviceManager` 来处理

通过添加 `JXDeviceManagerDelegate` 代理, 可以实时获得设备离线上线的更新

```objective-c
/// 添加代理, 用来处理设备列表的更新
- (void)addDeviceDelegateHolder:(id<JXDeviceManagerDelegate>)holder;
/// 移除代理
- (void)removeDeviceDelegateHolder:(id<JXDeviceManagerDelegate>)holder;
```

获取到具体的设备:

```objective-c
/// 获取到指定家庭的门禁设备
- (NSMutableArray<JXDoorDeviceModel *> *)getDoorDeviceInHome:(NSString *)homeId;

/// 获取到指定家庭的室内通设备
- (NSMutableArray<JXExtDeviceModel *> *)getExtDeviceInHome:(NSString *)homeId;
```



### 远程开门

通过 `getDoorDeviceInHome:` 方法获取到门禁设备后, 判断 `JXDoorDeviceModel` 的 `canOpenDoor` 属性来确认是否支持远程开门.

```objective-c
/// 远程开门
/// @param homeId 家庭参数
/// @param deviceName JXDoorDeviceModel.subDeviceName
- (BOOL)openDoor:(NSString *)homeId
      deviceName:(NSString *)deviceName;
```





## 设备的呼叫[JXConnectingManager]

通过 `[[JXManager defaultManage].connectingManager` 可以处理设备的呼叫业务, 包含有户户通, 室内通, 门禁系统摄像头的查看. 每一个成功连接的会话都会用一个唯一的 sessionId 来标识, 相关业务处理如下:

### 户户通呼叫

```objective-c
/// 是否支持户户通通话
- (BOOL)isSupportP2PInHome:(NSString *)homeId;

/// 呼叫户户通
/// @param homeId homeId
/// @param callNumber 呼叫的房号
/// @param delegate 会话相关的代理
/// 返回的是会话的 sessionId, 如果是 nil, 表示呼叫失败.
- (NSString * _Nullable)callP2POutWithHomeId:(NSString *)homeId
                                  callNumber:(NSString *)callNumber
                               videoDelegate:(id<JXConnectingDelegate>)delegate;
```

### 室内通呼叫

```objective-c
/// 是否支持室内通通话
- (BOOL)isSupportExtInHome:(NSString *)homeId;

/// 呼叫室内通设备, 呼叫成功返回 sessionId, 呼叫失败返回 nil
/// @param homeId homeId
/// @param callType 区分是呼叫还是查看监控
/// @param extDevice 室内通设备
/// @param delegate 视频连接的代理
- (NSString * _Nullable)callExtInHome:(NSString *)homeId
                             callType:(JX_IntercomCallType)callType
                            extDevice:(JXExtDeviceModel *)extDevice
                        videoDelegate:(id<JXConnectingDelegate>)delegate;
```

### 门禁系统摄像头的查看

```objective-c
/// 查看门禁系统摄像头
/// @param homeId dockSn
/// @param doorDevice KRDoorDeviceModel
/// @param delegate 设置代理
/// 返回的是通话的 sessionId, 如果是 nil, 表示呼叫失败
- (NSString * _Nullable)callDoorMonitorInHome:(NSString *)homeId
                                       device:(JXDoorDeviceModel *)doorDevice
                                videoDelegate:(id<JXConnectingDelegate>)delegate;
```



### 会话的公共方法

```objective-c
/// 主动接听, 不会有回调
- (BOOL)pickUpInHome:(NSString *)homeId sessionId:(NSString *)sessionId;

/// 主动挂断, 不会有回调
- (void)hangUpInHome:(NSString *)homeId sessionId:(NSString *)sessionId;

/// 开启/禁用 视频
- (BOOL)enableVideo:(BOOL)enable;

/// 切换前后镜头: front:YES-前置 NO-后置
- (BOOL)switchCamera:(BOOL)front;

/// 开启/禁用 麦克风
- (BOOL)enableVoice:(BOOL)enable inHome:(NSString *)homeId sessionId:(NSString *)sessionId;

/// 开启/禁用 扬声器
- (BOOL)enableSpeaker:(BOOL)enable inHome:(NSString *)homeId sessionId:(NSString *)sessionId;

/// 开门
- (void)unlockInHome:(NSString *)homeId sessionId:(NSString *)sessionId;

/// 获取快照 (保存到相册需要外部自己处理)
- (void)takeSnapshotInHome:(NSString *)homeId
                 sessionId:(NSString *)sessionId
             completeBlock:(getSnapshotBlock)completeBlock;

/// 开始录制视频
- (BOOL)startRecordVideoInHome:(NSString *)homeId
                     sessionId:(NSString *)sessionId
                    deviceName:(NSString *)deviceName;

/// 停止录制视频
- (void)stopRecordVideoInHome:(NSString *)homeId
                    sessionId:(NSString *)sessionId;
```



遵循 `JXConnectingDelegate` 代理来处理会话中的交互

```objective-c
/// 视频连接被挂断
- (void)didHangUpWithReason:(JX_IntercomReason)reason animated:(BOOL)animated;

/// 通话被对方接听
- (void)didPickup;

/// 提供用来展示对方视频的 Layer, 必须是 CATiledLayer
- (CATiledLayer *)showOtherVideoLayer;

/// 提供用来展示自己视频的 Layer, 必须是 CATiledLayer
- (CATiledLayer * _Nullable)showMineVideoLayer;

/// 门锁已打开
- (void)didDoorLockOpen;

/// 开始录制视频
- (void)didStartRecord;
```



### 被其他设备呼叫

我们将被呼叫的业务交由 `JXManager` 的  `JXIntercomDelegate` 来统一处理, 方便开发者管理此 SDK 的会话在整个 App 中的优先级. 要实现的方法如下:

```objective-c
/*
有呼叫进来, 是否需要响应. 

返回 NO, 会向呼叫方响应当前用户正忙, 无法响应呼叫.
返回 YES, 会区分通话场景触发响应的被呼回调.
*/
- (BOOL)shouldResponseIntercomCall:(JX_IntercomScenes)callScenes;
```



响应被呼叫: 

```objective-c
/// 被门禁呼叫 (shouldResponseIntercomCall 返回 YES 的时候才会有)
- (id<JXConnectingDelegate> _Nullable)calledByDoorWithHomeId:(NSString *)homeId
                                                   sessionId:(NSString *)sessionId
                                                  doorDevice:(JXDoorDeviceModel *)doorDevice;

/// 被室内通呼叫 (shouldResponseIntercomCall 返回 YES 的时候才会有)
- (id<JXConnectingDelegate> _Nullable)calledByExtWithHomeId:(NSString *)homeId
                                                  sessionId:(NSString *)sessionId
                                                  extDevice:(JXExtDeviceModel *)extDevice;

/// 被户户通呼叫 (shouldResponseIntercomCall 返回 YES 的时候才会有)
/// @param homeId homeId
/// @param sessionId 会话的 sessionId
/// @param callNumber 呼叫的房号
- (id<JXConnectingDelegate> _Nullable)calledByP2PWithHomeId:(NSString *)homeId
                                                  sessionId:(NSString *)sessionId
                                                 callNumber:(NSString *)callNumber;
```







## 历史记录

所有的呼叫都会自动保存到记录中, 通过 `[[JXManager defaultManage].historyManager` 来获取

提供如下的功能:

```objective-c
/// 获取指定家庭的所有通话记录, 会按照 startTime 倒序,
- (NSArray<JXCallRecordModel *> *)getRecordsInHome:(NSString *)homeId;

/// 更新通话记录
- (void)updateRecord:(JXCallRecordModel *)callRecord;

/// 播放录像 - 每次只能播放一个录像, 如果当前正在播放其他的录像, 会直接返回 NO, 不会有任何block被执行
/// @param videoRecord videoRecord
/// @param videoLayer 必须是 CATiledLayer
/// @param getDurationBlock 获得录像总时长, msDuration 单位是毫秒
/// @param playProgressBlock 录像已经播放的市场, playingMs 单位是毫秒
/// @param playCompleteBlock 录像播放完成
- (BOOL)playVideoRecord:(JXVideoRecordModel *)videoRecord
                onLayer:(CATiledLayer *)videoLayer
       getDurationBlock:(void(^)(long long msDuration))getDurationBlock
      playProgressBlock:(void(^)(long long playingMs))playProgressBlock
          completeBlock:(void(^)(void))playCompleteBlock;

/// 停止播放录像, 不是暂停
- (void)stopPlayVideoRecord:(JXVideoRecordModel *)videoRecord;

/// 调整播放的进度
/// @param progress 百分比进度, 比如 80%, progress=80
- (void)seekProgress:(int)progress;

/// 将视频保存到相册中, 视频格式会转换成 mp4, 如果当前有正在转码的任务, 返回 NO
/// @param videoRecord videoRecord
/// @param progressBlock 进度
- (BOOL)exportVideoToAlbum:(JXVideoRecordModel *)videoRecord
             progressBlock:(void(^)(NSInteger progress))progressBlock
             completeBlock:(void(^)(PHAsset *__nullable asset, NSString *__nullable errorMessage))completeBlock;
```



## 安防

当拥有支持安防功能的时候, 可以通过  `[[JXManager defaultManage].securityManager` 来处理

提供如下功能:

```objective-c
/// 设置安防监听
- (void)addSecurityDelegate:(id<JXSecurityDelegate>)holder;

/// 移除安防监听
- (void)removeSecurityDelegate:(id<JXSecurityDelegate>)holder;

/// 是否支持安防
- (BOOL)isSupportSecurity:(NSString *)homeId;

/// 查询安防状态:
- (JX_SecurityStatus)querySecurityStatus:(NSString *)homeId;

/// 切换安防状态
- (void)switchSecurityStatus:(NSString *)homeId;

/// 关闭安防报警
- (void)cancelSecurityWarning:(NSString *)homeId;
```









## 更新说明

### 2.0.66302

新增设备激活, 需要在初始化 `JXManagerConfig` 的时候提供注册过的 `appId` 和  `appKey` .  在登录的时候提供和 `userId` 绑定的激活码 `activecode` , 如果该 `userId` 还未绑定过激活码, 请提供新的激活码. 登录过程会自动激活和验证. 

变更的接口:

JXManagerConfig:

```objective-c
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
```

JXManager:

```objective-c
/// 登录
/// @param userId 移动端的账号,代表移动端的唯一值
/// @param alias 用来显示的昵称. 传空值则默认为"iPhone"
/// @param activecode 和 userId 绑定的激活码
/// @param completeBlock 指示登录是否成功. 返回 YES 后才可以正常使用 SDK 中的功能. 否则可能发生异常
- (void)loginWithUserId:(NSString *)userId
                  alias:(NSString *)alias
             activecode:(NSString *)activecode
               complete:(void(^)(BOOL succeed))completeBlock;
```

JXIntercomDelegate 新增回调告知激活结果:

```objective-c
/// 鉴权的结果
/// @param result 参照 DeviceActiveResult 说明
/// @param message message
- (void)onDeviceActivated:(DeviceActiveResult)result message:(NSString * _Nullable)message;
```

若设备激活失败, 云端功能不开启, 但若和设备在同一局域网内, 相关服务依然可用; 激活成功, 所有功能可用.

