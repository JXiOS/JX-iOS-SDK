//
//  JXCallRecordModel.h
//  JXIntercomSDK
//
//  Created by Nansen on 2020/4/16.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "JXIntercomConstants.h"
#import "JX_GDataObjectProtocol.h"
#import <IntercomSDK/IntercomSDK.h>

@class JXPicRecordModel;
@class JXVideoRecordModel;

NS_ASSUME_NONNULL_BEGIN

@interface JXCallRecordModel : NSObject<JX_GDataObjectProtocol>

@property (nonatomic, copy) NSString *homeId;

@property (nonatomic, copy) NSString *sessionId;

@property (nonatomic, assign) Router router;

@property (nonatomic, copy) NSString *clientId;

/// 通话场景
@property (nonatomic, assign) JX_IntercomScenes scenes;

/// 对方名字
@property (nonatomic, copy) NSString *otherName;
/// 对方设备类型
@property (nonatomic, assign) JX_DeviceType deviceType;
/// 呼叫类型
@property (nonatomic, assign) JX_IntercomCallType callType;
/// 是否是主动呼出
@property (nonatomic, assign) BOOL isCallout;


/// 时间戳 13位(毫秒)
@property (nonatomic, copy) NSString *startTime;
/// 时间戳 13位(毫秒)
@property (nonatomic, copy) NSString *endTime;
/// 是否开过门
@property (nonatomic, assign) BOOL openDoor;
/// 是否接通
@property (nonatomic, assign) BOOL isConnect;
/// 该通话的所有截图
@property (nonatomic, strong) NSMutableArray<JXPicRecordModel *> *picArray;
/// 该通话的所有录制视频
@property (nonatomic, strong) NSMutableArray<JXVideoRecordModel *> *videoRecordsArray;

- (instancetype)initWithHomeId:(NSString *)homeId
                     sessionId:(NSString *)sessionId
                        router:(Router)router
                      clientId:(NSString *)clientId
                        scenes:(JX_IntercomScenes)scenes
                     otherName:(NSString *)otherName
                    deviceType:(JX_DeviceType)deviceType
                      callType:(JX_IntercomCallType)callType
                     isCallout:(BOOL)isCallout;

- (void)callEndAndSaveToDB;


@end

NS_ASSUME_NONNULL_END
