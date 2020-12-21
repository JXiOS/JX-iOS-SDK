//
//  JXVideoRecordModel.h
//  JXIntercomSDK
//
//  Created by Nansen on 2020/4/16.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface JXVideoRecordModel : NSObject

/// 该条视频记录对应的 id, 使用 uuid, 初始化的时候自动赋值
@property (nonatomic, copy) NSString *recordId;

@property (nonatomic, copy) NSString *homeId;

@property (nonatomic, copy) NSString *sessionId;

/// 设备别名
@property (nonatomic, copy) NSString *deviceAlias;

/// 文件夹路径:一般是时间戳 e.g. 13203138231365510
@property (nonatomic, copy) NSString *folderName;

/// 封面截图的文件名 e.g. 13203394960184379.jpeg
@property (nonatomic, copy, nullable) NSString *jpegFileName;

/// 视频文件的文件名 e.g. 13203394960184379.rec
@property (nonatomic, copy, nullable) NSString *recFileName;

/// 通话时长(秒)
@property (nonatomic, assign) NSInteger durationSecond;

// 初始化
- (instancetype)initWithHomeId:(NSString *)homeId
                     sessionId:(NSString *)sessionId
                    deviceName:(NSString *)deviceName;

#pragma mark - ======== func ========
/** 图片的完整路径 */
- (NSString * _Nullable)fullImagePath;

/// 图片文件是否还存在
- (BOOL)imageIsExsit;

/** 视频的完整路径 */
- (NSString * _Nullable)fullRecPath;

/// 视频文件是否还存在
- (BOOL)videoIsExsit;

/** 用来展示的时长 xx:xx */
- (NSString * _Nullable)hourAndMinute;

/// 创建时间
- (NSDate * _Nullable)createDate;

/// 开始计时
- (void)start;

/// 结束计时, 返回计时的秒数
- (NSInteger)stop;




@end

NS_ASSUME_NONNULL_END
