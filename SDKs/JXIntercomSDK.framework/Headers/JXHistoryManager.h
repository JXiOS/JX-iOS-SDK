//
//  JXHistoryManager.h
//  JXIntercomSDK
//
//  Created by Nansen on 2020/4/21.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import <Foundation/Foundation.h>

@class JXCallRecordModel;
@class JXVideoRecordModel;
@class CATiledLayer;
@class PHAsset;

NS_ASSUME_NONNULL_BEGIN

@interface JXHistoryManager : NSObject

/// 删除一个家庭的所有通话记录
- (void)deleteRecordWithHomeId:(NSString *)homeId;

/// 保存一条通话记录
- (void)saveRecord:(JXCallRecordModel *)callRecord;


/// 获取指定家庭的所有通话记录, 会按照 startTime 倒序,
- (NSArray<JXCallRecordModel *> *)getRecordsInHome:(NSString *)homeId;

/// 更新通话记录
- (void)updateRecord:(JXCallRecordModel *)callRecord;

/// 获取家庭的所有录像, 按照录像的创建时间逆序
- (NSArray<JXVideoRecordModel *> *)getVideoRecordsInHome:(NSString *)homeId;

/// 删除一个视频记录
- (void)deleteVideoRecord:(JXVideoRecordModel *)videoRecord;

/// 删除一组视频记录
- (void)deleteVideoRecords:(NSArray<JXVideoRecordModel *> *)videoRecords;

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


@end

NS_ASSUME_NONNULL_END
