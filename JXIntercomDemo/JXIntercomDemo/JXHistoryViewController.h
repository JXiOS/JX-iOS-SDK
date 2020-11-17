//
//  JXHistoryViewController.h
//  JXIntercomDemo
//
//  Created by Nansen on 2020/4/30.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import <UIKit/UIKit.h>

@class JXCallRecordModel;


NS_ASSUME_NONNULL_BEGIN

@interface JXHistoryViewController : UIViewController

- (instancetype)initWithCallRecord:(JXCallRecordModel *)record;


@end

NS_ASSUME_NONNULL_END
