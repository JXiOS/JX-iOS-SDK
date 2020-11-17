//
//  JX_NVRHistoryPlayViewController.h
//  JXIntercomDemo
//
//  Created by Nansen on 2020/6/15.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import <UIKit/UIKit.h>

@class JXDoorDeviceModel;

NS_ASSUME_NONNULL_BEGIN

@interface JX_NVRHistoryPlayViewController : UIViewController

@property (nonatomic, copy, nullable) NSString *sessionId;

@property (nonatomic, strong) JXDoorDeviceModel *nvrDevice;
@property (nonatomic, copy) NSString *homeId;

- (instancetype)initWithNVRDevice:(JXDoorDeviceModel *)nvrDevice
                           homeId:(NSString *)homeId
                        startDate:(NSDate *)startDate
                          endDate:(NSDate *)endDate;

@end

NS_ASSUME_NONNULL_END
