//
//  JXPicRecordModel.h
//  JXIntercomSDK
//
//  Created by Nansen on 2020/4/16.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface JXPicRecordModel : NSObject
/// uuid
@property (nonatomic, copy) NSString *picId;

@property (nonatomic, copy) NSString *homeId;

@property (nonatomic, copy) NSString *sessionId;


/// 截图的文件名 e.g. 13203394960184379.jpeg
@property (nonatomic, copy, nullable) NSString *picName;
/// 文件夹路径:一般是时间戳 e.g. 13203138231365510
@property (nonatomic, copy, nullable) NSString *folderName;

// 初始化
- (instancetype)initWithHomeId:(NSString *)homeId
                     sessionId:(NSString *)sessionId;

/// 图片的完整本地路径
- (NSString * _Nullable)fullImagePath;

/// 图片的创建时间
- (NSDate * _Nullable)createDate;

/// 获得图片对象
- (UIImage * _Nullable)imageFromFile;

@end

NS_ASSUME_NONNULL_END
