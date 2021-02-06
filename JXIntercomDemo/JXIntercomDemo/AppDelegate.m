//
//  AppDelegate.m
//  JXIntercomDemo
//
//  Created by Nansen on 2020/3/23.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import "AppDelegate.h"

#import <JXIntercomSDK/JXIntercomSDK.h>

#import "JX_DemoConfig.h"

@import Photos;
@import AVFoundation;


@interface AppDelegate ()

@end

@implementation AppDelegate



- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.

#warning TODO : 在 JX_DemoConfig 中先配置相关参数
    
    JXManagerConfig *jxConfig = [[JXManagerConfig alloc] initWithSipURL:[JX_DemoConfig sipURL] transitURLString:[JX_DemoConfig transitURL] channel:[JX_DemoConfig channel] appId:[JX_DemoConfig appid] appKey:[JX_DemoConfig appkey]];
    
    [[JXManager defaultManage] startWithConfig:jxConfig];
    
    [self requestAuth];
    
    return YES;
}

- (void)requestAuth
{
    if (@available(iOS 14.0, *)) {
        [PHPhotoLibrary requestAuthorizationForAccessLevel:PHAccessLevelAddOnly handler:^(PHAuthorizationStatus status) {
            NSLog(@"Photos Add Auth = %zd", status);
        }];
    }
    else {
        PHAuthorizationStatus authorizationStatus = [PHPhotoLibrary authorizationStatus];
        if (authorizationStatus == PHAuthorizationStatusNotDetermined) {
            [PHPhotoLibrary requestAuthorization:^(PHAuthorizationStatus status) {
                NSLog(@"Photos Auth = %zd", status);
            }];
        }
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
