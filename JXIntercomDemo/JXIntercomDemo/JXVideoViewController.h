//
//  JXVideoViewController.h
//  JXIntercomDemo
//
//  Created by Nansen on 2020/4/29.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import <UIKit/UIKit.h>

#import <JXIntercomSDK/JXIntercomSDK.h>

NS_ASSUME_NONNULL_BEGIN

@interface JXVideoViewController : UIViewController <JXConnectingDelegate>

@property (nonatomic, copy, nullable) NSString *sessionId;

@property (nonatomic, assign) JX_IntercomCallType callType;
@property (nonatomic, assign) JX_IntercomScenes callScenes;

- (instancetype)initWithExtDevice:(JXExtDeviceModel *)extDevice
                         callType:(JX_IntercomCallType)callType
                       callScenes:(JX_IntercomScenes)scenes
                        isCallout:(BOOL)isCallout
                           homeId:(NSString *)homeId;

- (instancetype)initWithCallDevice:(JXDoorDeviceModel *)doorDevice
                          callType:(JX_IntercomCallType)callType
                        callScenes:(JX_IntercomScenes)scenes
                         isCallout:(BOOL)isCallout
                            homeId:(NSString *)homeId;


- (instancetype)initWithCallP2P:(NSString *)callNumber
                         homeId:(NSString *)homeId
                      isCallout:(BOOL)isCallout;


@end

NS_ASSUME_NONNULL_END
