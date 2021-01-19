//
//  AppDelegate.m
//  JXIntercomDemo
//
//  Created by Nansen on 2020/3/23.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import "AppDelegate.h"

#import <JXIntercomSDK/JXIntercomSDK.h>

@import Photos;
@import AVFoundation;


@interface AppDelegate ()

@end

@implementation AppDelegate



- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    
    /// 配置两个服务器地址
    #warning TODO : 配置基本的参数
    /// 测试
    NSString *sipURL = @"<#sipurl#>";
    NSString *transitURL = @"<#transiturl#>";
    NSString *channel = @"<#channel#>";
    
    NSString *appid = @"<#appId#>";
    NSString *appkey = @"<#appKey#>";
    
    JXManagerConfig *jxConfig = [[JXManagerConfig alloc] initWithSipURL:sipURL transitURLString:transitURL channel:channel appId:appid appKey:appkey];
    
    [[JXManager defaultManage] startWithConfig:jxConfig];
    
    [self requestAuth];
    
    return YES;
}

- (void)requestAuth
{
    PHAuthorizationStatus authorizationStatus = [PHPhotoLibrary authorizationStatus];
    if (authorizationStatus == PHAuthorizationStatusNotDetermined) {
        [PHPhotoLibrary requestAuthorization:^(PHAuthorizationStatus status) {
            NSLog(@"Photos Auth = %zd", status);
        }];
    }
    
    AVAuthorizationStatus authStatus = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
    if (authStatus == AVAuthorizationStatusNotDetermined) {
        [AVCaptureDevice requestAccessForMediaType:AVMediaTypeVideo completionHandler:^(BOOL granted) {
            NSLog(@"Camera Auth : %d", granted);
        }];
    }
    
    AVAuthorizationStatus audioStatus = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeAudio];
    if (audioStatus == AVAuthorizationStatusNotDetermined) {
        [AVCaptureDevice requestAccessForMediaType:AVMediaTypeAudio completionHandler:^(BOOL granted) {
            NSLog(@"Audio Auth : %d", granted);
        }];
    }
}


@end
