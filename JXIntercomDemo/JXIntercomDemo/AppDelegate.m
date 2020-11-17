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
    #warning TODO : 服务器配置
    /// 测试
    NSString *sipURL = @"sipurl";
    NSString *transitURL = @"transiturl";
    
    JXManagerConfig *jxConfig = [[JXManagerConfig alloc] initWithSipURL:sipURL transitURLString:transitURL];
    
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



//#pragma mark - UISceneSession lifecycle
//
//
//- (UISceneConfiguration *)application:(UIApplication *)application configurationForConnectingSceneSession:(UISceneSession *)connectingSceneSession options:(UISceneConnectionOptions *)options {
//    // Called when a new scene session is being created.
//    // Use this method to select a configuration to create the new scene with.
//    return [[UISceneConfiguration alloc] initWithName:@"Default Configuration" sessionRole:connectingSceneSession.role];
//}
//
//
//- (void)application:(UIApplication *)application didDiscardSceneSessions:(NSSet<UISceneSession *> *)sceneSessions {
//    // Called when the user discards a scene session.
//    // If any sessions were discarded while the application was not running, this will be called shortly after application:didFinishLaunchingWithOptions.
//    // Use this method to release any resources that were specific to the discarded scenes, as they will not return.
//}


@end
