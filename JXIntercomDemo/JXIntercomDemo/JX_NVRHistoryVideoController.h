//
//  JX_NVRHistoryVideoController.h
//  smartlifebell
//
//  Created by Nansen on 2020/6/17.
//  Copyright © 2020 Jingxi. All rights reserved.
//

#import <UIKit/UIKit.h>

@class JXDoorDeviceModel;

NS_ASSUME_NONNULL_BEGIN

@interface JX_NVRHistoryVideoController : UIViewController

- (instancetype)initWithNVRDevice:(JXDoorDeviceModel *)nvrDevice
                           homeId:(NSString *)homeId
                        startDate:(NSDate *)startDate
                          endDate:(NSDate *)endDate;


@end

NS_ASSUME_NONNULL_END
