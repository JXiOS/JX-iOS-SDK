//
//  HomeDetailViewController.h
//  JXIntercomDemo
//
//  Created by Nansen on 2020/4/26.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface HomeDetailViewController : UIViewController

@property (nonatomic, copy) NSString *homeId;

- (instancetype)initWithHomeId:(NSString *)homeId;

@end

NS_ASSUME_NONNULL_END
