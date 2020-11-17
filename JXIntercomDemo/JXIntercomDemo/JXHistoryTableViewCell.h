//
//  JXHistoryTableViewCell.h
//  JXIntercomDemo
//
//  Created by Nansen on 2020/4/30.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import <UIKit/UIKit.h>

@class JXVideoRecordModel;

NS_ASSUME_NONNULL_BEGIN

@interface JXHistoryTableViewCell : UITableViewCell

@property (nonatomic, strong) JXVideoRecordModel *videoRecord;

@end

NS_ASSUME_NONNULL_END
