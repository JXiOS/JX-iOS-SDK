## 概要介绍

慧管家 SDK 兼容 iOS10.0, Demo 兼容 iOS10.0.

## 结构

`JXManager` 是整个 SDK 功能的管理类, 其中可以进行家庭管理, 设备管理, 通话管理, 历史记录查询, 安防设置等相关功能, 具体可以查看注释.

`JXManagerConfig` 是 SDK 的配置文件, 相关参数可在 SDK 启动之前进行配置

![](https://github.com/JXiOS/JX-iOS-SDK/blob/main/asdk.png)


**特别注意:**

`homeId` 是表示设备的唯一值, 长度必须是 8 的倍数, 不足的要后面加0补足之后才可以使用.


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
JXManagerConfig *jxConfig = [[JXManagerConfig alloc] initWithSipURL:@"sip服务器地址" transitURLString:@"中转服务器地址" channel:@"客户标识"];
```

然后启动 `JXMananger` :

```objective-c
[[JXManager defaultManage] startWithConfig:jxConfig];
```


### 关于推送

客户自己选择 APNS 服务的集成商, 在调用 `startHome:` 之前配置好接收推送所使用的 id,

推送需要自行配置推送的 ID, 需要在启动服务之前配置好此参数
```objective-c
// 这个方法必须在startHome: 调用之前处理, 否则会收不到推送
[[JXManager defaultManage] deployAPNsID:@"推送使用的ID"];
```

在收到对应的推送之后可以直接将对应的Json字符串转成 NSDictionary 提交给 JXManager 处理:
```objective-c
/// Json 转成字典后应该有 key = "message" 的键值对
[[JXManager defaultManage] receivePushNoti:@"Json 转成的字典"];
```



## 登录和登出 [JXManager]

```objective-c
/// 登录
/// @param userId 移动端的账号,代表移动端的唯一值
/// @param alias 用来显示的昵称
- (void)loginWithUserId:(NSString *)userId
                  alias:(NSString *)alias;

/// 登出
- (void)logout;
```





## 服务的启动和停用[JXManager]

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



## 设备的查看[JXConnectingManager]

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





## 呼叫和查看监控

### 主动呼叫

通过 `[[JXManager defaultManage].connectingManager` 可以处理主动呼出的情况,

这里每一个呼叫连接都会拥有一个唯一的 sessionId, 相关方法具体可以查看 demo 

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


/// 查看门禁系统摄像头
/// @param homeId dockSn
/// @param doorDevice KRDoorDeviceModel
/// @param delegate 设置代理
/// 返回的是通话的 sessionId, 如果是 nil, 表示呼叫失败
- (NSString * _Nullable)callDoorMonitorInHome:(NSString *)homeId
                                       device:(JXDoorDeviceModel *)doorDevice
                                videoDelegate:(id<JXConnectingDelegate>)delegate;


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



遵循 `JXConnectingDelegate` 代理用来处理呼叫中的业务

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



### 被呼叫

被呼叫的情况交由 `JXIntercomDelegate` 来处理,

首先是必须实现的方法:

```objective-c
/// 有呼叫进来, 是否需要响应.
- (BOOL)shouldResponseIntercomCall:(JX_IntercomScenes)callScenes;
```

在这里可以处理当前 App 是否可以响应被呼叫的情况, 如果返回 NO, 直接返回用户占线, 呼出方收到消息后可以直接挂断.

如果返回 YES, 在下面两个方法中来处理两种被呼叫的情况:

```objective-c
/// 被门禁呼叫 (shouldResponseIntercomCall 返回 YES 的时候才会有)
- (id<JXConnectingDelegate> _Nullable)calledByDoorWithHomeId:(NSString *)homeId
                                                   sessionId:(NSString *)sessionId
                                                  doorDevice:(JXDoorDeviceModel *)doorDevice;

/// 被室内通呼叫 (shouldResponseIntercomCall 返回 YES 的时候才会有)
- (id<JXConnectingDelegate> _Nullable)calledByExtWithHomeId:(NSString *)homeId
                                                  sessionId:(NSString *)sessionId
                                                  extDevice:(JXExtDeviceModel *)extDevice;
```



具体可以查看 Demo 中的简单处理.





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



