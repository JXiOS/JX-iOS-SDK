//
//  JXManagerConfig.h
//  JXIntercomSDK
//
//  Created by Nansen on 2020/4/2.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <IntercomSDK/intercom_constants.h>

//系统日志级别
typedef NS_ENUM(NSInteger, JXMLogLevel) {
    JXMLogLevel_DEBUG = 0,
    JXMLogLevel_RELEASE,
};


NS_ASSUME_NONNULL_BEGIN

@interface JXManagerConfig : NSObject

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
                       channel:(NSString *)channel;


/**
 下面为默认配置, 上层开发者可以不用修改, 如果要修改, 请在 JXManager 的 startWithConfig 之前修改
 */
/// 默认是 JXMLogLevel_ERROR
@property (nonatomic, assign) JXMLogLevel logLevel;
/// 默认存储SDK数据的位置: Document/JXHousekeeper
@property (nonatomic, copy) NSString *dataDir;
/// 遮罩视频的图片文件路径. 默认是 JXIntercomBundle 中的 novideo.jpg
@property (nonatomic, copy) NSString *maskImagePath;
/// 黑屏的参数. 默认是 @"128,128,128,0"
@property (nonatomic, copy) NSString *blankColor;

/// 本机视频采集的分辨率 宽,高. 默认 640 , 480
@property (nonatomic, assign) int defaultVideoWidth;
@property (nonatomic, assign) int defaultVideoHeight;


@end

NS_ASSUME_NONNULL_END

