//
//  JXVideoViewController.m
//  JXIntercomDemo
//
//  Created by Nansen on 2020/4/29.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import "JXVideoViewController.h"
#import <JXIntercomSDK/JXIntercomSDK.h>
#import "Masonry.h"
#import <AVFoundation/AVFoundation.h>



@interface JXVideoViewController ()

@property (nonatomic, copy) NSString *homeId;

@property (nonatomic, assign) BOOL isCallout;

@property (nonatomic, strong) JXDoorDeviceModel *doorDevice;
@property (nonatomic, strong) JXExtDeviceModel *extDevice;

@property (nonatomic, strong) UIView *layerView;
@property (nonatomic, strong) CATiledLayer *layer;

@property (nonatomic, strong) UIView *mineLayerView;
@property (nonatomic, strong, nullable) CATiledLayer *mineLayer;

@property (nonatomic, strong) UIView *callInBottomView;
@property (nonatomic, strong) UIView *bottomView;

@property (nonatomic, strong) UIButton *recordBtn;
@property (nonatomic, strong) UIButton *openDoorBtn2;


@property (nonatomic, strong) AVAudioPlayer *player;

@property (nonatomic, copy) NSString *p2pNumber;

@end

@implementation JXVideoViewController


- (instancetype)initWithExtDevice:(JXExtDeviceModel *)extDevice
                         callType:(JX_IntercomCallType)callType
                       callScenes:(JX_IntercomScenes)scenes
                        isCallout:(BOOL)isCallout
                           homeId:(NSString *)homeId
{
    if (self = [super init]) {
        self.callType = callType;
        self.callScenes = scenes;
        self.isCallout = isCallout;
        self.extDevice = extDevice;
        self.homeId = homeId;
    }
    return self;
}

- (instancetype)initWithCallDevice:(JXDoorDeviceModel *)doorDevice
                          callType:(JX_IntercomCallType)callType
                        callScenes:(JX_IntercomScenes)scenes
                         isCallout:(BOOL)isCallout
                            homeId:(NSString *)homeId
{
    if (self = [super init]) {
        self.callType = callType;
        self.callScenes = scenes;
        self.isCallout = isCallout;
        self.doorDevice = doorDevice;
        self.homeId = homeId;
    }
    return self;
}


- (instancetype)initWithCallP2P:(NSString *)callNumber
                         homeId:(NSString *)homeId
                      isCallout:(BOOL)isCallout
{
    if (self = [super init]) {
        self.p2pNumber = callNumber;
        self.homeId = homeId;
        self.isCallout = isCallout;
        self.callType = JX_IntercomCallType_Call;
        self.callScenes = JX_IntercomScenes_P2P;
    }
    return self;
}


- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.view.backgroundColor = [UIColor whiteColor];
    
    [self configSubviews];
    
    [self call];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    
    NSLog(@"VideoViewController viewWillDisappear");
}

- (void)dealloc
{
    NSLog(@"VideoViewController dealloc");
    
    if (self.player) {
        [self.player stop];
        self.player = nil;
    }
}

- (void)call
{
    if (self.isCallout) {
        if (self.callScenes == JX_IntercomScenes_Door && self.callType == JX_IntercomCallType_Monitor) {
            NSString *sessionId = [[JXManager defaultManage].connectingManager callDoorMonitorInHome:self.homeId device:self.doorDevice videoDelegate:self];
            if (sessionId) {
                self.sessionId = sessionId;
            }
            else {
                NSLog(@"门禁监控 呼叫出错!");
                [self dismissViewControllerAnimated:YES completion:nil];
            }
        }
        else if (self.callScenes == JX_IntercomScenes_Ext) {
            NSString *sessionId = [[JXManager defaultManage].connectingManager callExtInHome:self.homeId callType:self.callType extDevice:self.extDevice videoDelegate:self];
            if (sessionId) {
                self.sessionId = sessionId;
                
                [self playMusic];
            }
            else {
                NSLog(@"室内通 呼叫出错!");
                [self dismissViewControllerAnimated:YES completion:nil];
            }
        }
        else if (self.callScenes == JX_IntercomScenes_P2P) {
            NSString *sessionId = [[JXManager defaultManage].connectingManager callP2POutWithHomeId:self.homeId callNumber:self.p2pNumber videoDelegate:self];
            if (sessionId) {
                self.sessionId = sessionId;
                
                [self playMusic];
            }
            else {
                NSLog(@"呼叫户户通 出错!");
                [self dismissViewControllerAnimated:YES completion:nil];
            }
        }
    }
    else {
        [self playMusic];
    }
}

- (void)playMusic
{
    if (self.player) {
        [self.player stop];
        self.player = nil;
    }
    
    AVAudioSession *session = [AVAudioSession sharedInstance];
    [session setCategory:AVAudioSessionCategorySoloAmbient error:nil];
    [session setActive:YES error:nil];
    
    NSString *path = [[NSBundle mainBundle] pathForResource:@"JXIntercomBundle" ofType:@"bundle"];
    NSBundle *jxBundle = [NSBundle bundleWithPath:path];
    NSURL *url = [jxBundle URLForResource:@"video_chat_tip_receiver.aac" withExtension:nil];
    if (url) {
        self.player = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:nil];
        [self.player setNumberOfLoops:20];
        [self.player prepareToPlay];
        [self.player play];
    }
}

- (void)configSubviews
{
    CGFloat sw = [UIScreen mainScreen].bounds.size.width;
    
    UILabel *titleLabel = [[UILabel alloc] init];
    titleLabel.textColor = [UIColor whiteColor];
    titleLabel.textAlignment = NSTextAlignmentCenter;
    titleLabel.font = [UIFont boldSystemFontOfSize:24];
    [self.view addSubview:titleLabel];
    [titleLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerX.mas_equalTo(0);
        make.top.mas_equalTo(@20);
        make.height.mas_equalTo(@40);
    }];
    
    CGFloat layerH = sw * 3.0 / 4.0;
    self.layerView = [[UIView alloc] init];
    self.layerView.backgroundColor = [UIColor greenColor];
    [self.view addSubview:self.layerView];
    [self.layerView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.mas_equalTo(@0);
        make.centerY.mas_equalTo(@0);
        make.height.mas_equalTo(@(layerH));
    }];
    
    self.layer = [CATiledLayer layer];
    [self.layerView.layer addSublayer:self.layer];
    self.layer.frame = CGRectMake(0, 0, sw, layerH);
    self.layer.backgroundColor = [UIColor blackColor].CGColor;
    
    self.mineLayerView = [[UIView alloc] init];
    self.mineLayerView.backgroundColor = [UIColor greenColor];
    [self.view addSubview:self.mineLayerView];
    [self.mineLayerView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_equalTo(titleLabel.mas_bottom).offset(20);
        make.right.mas_equalTo(@-10);
        make.width.mas_equalTo(@120);
        make.height.mas_equalTo(@90);
    }];
    
    self.mineLayer = [CATiledLayer layer];
    [self.mineLayerView.layer addSublayer:self.mineLayer];
    self.mineLayer.frame = CGRectMake(0, 0, 120, 90);
    self.mineLayer.backgroundColor = [UIColor greenColor].CGColor;
    
    [self configBottomView];
    
    [self configCallInBottomView];
    
    
    if (self.callType == JX_IntercomCallType_Monitor) {
        self.mineLayerView.hidden = YES;
    }
    
    if (self.isCallout) {
        self.callInBottomView.hidden = YES;
    }
}

- (void)configCallInBottomView
{
    CGFloat bottomM = [self isFullScreen] ? -34 : 0;
    
    self.callInBottomView = [[UIView alloc] init];
    self.callInBottomView.backgroundColor = [UIColor whiteColor];
    [self.view addSubview:self.callInBottomView];
    [self.callInBottomView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.mas_equalTo(@0);
        make.bottom.mas_equalTo(@(bottomM));
        make.height.mas_equalTo(@60);
    }];
    
    UIButton *hangupBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    [hangupBtn setImage:[UIImage imageNamed:@"icons_calloff"] forState:UIControlStateNormal];
    [hangupBtn addTarget:self action:@selector(hangup) forControlEvents:UIControlEventTouchUpInside];
    [self.callInBottomView addSubview:hangupBtn];
    [hangupBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerY.mas_equalTo(@0);
        make.height.width.mas_equalTo(@55);
        make.right.mas_equalTo(@-60);
    }];
    
    UIButton *pickupBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    [pickupBtn setImage:[UIImage imageNamed:@"icons_jieting"] forState:UIControlStateNormal];
    [pickupBtn addTarget:self action:@selector(pickup) forControlEvents:UIControlEventTouchUpInside];
    [self.callInBottomView addSubview:pickupBtn];
    [pickupBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerY.mas_equalTo(@0);
        make.height.width.mas_equalTo(@55);
        make.left.mas_equalTo(@60);
    }];
    
    if (self.doorDevice && self.doorDevice.canOpenDoor) {
        UIButton *doorBtn = [UIButton buttonWithType:UIButtonTypeCustom];
        [doorBtn setImage:[UIImage imageNamed:@"monitor_icon_opendoor"] forState:UIControlStateNormal];
        [doorBtn addTarget:self action:@selector(openDoor) forControlEvents:UIControlEventTouchUpInside];
        [self.callInBottomView addSubview:doorBtn];
        [doorBtn mas_makeConstraints:^(MASConstraintMaker *make) {
            make.centerY.mas_equalTo(@0);
            make.height.width.mas_equalTo(@55);
            make.centerX.mas_equalTo(@0);
        }];
    }
}

- (void)configBottomView
{
    CGFloat bottomM = [self isFullScreen] ? -34 : 0;
    
    self.bottomView = [[UIView alloc] init];
    self.bottomView.backgroundColor = [UIColor whiteColor];
    [self.view addSubview:self.bottomView];
    [self.bottomView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.mas_equalTo(@0);
        make.bottom.mas_equalTo(@(bottomM));
        make.height.mas_equalTo(@60);
    }];
    
    UIButton *hangupBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    [hangupBtn setImage:[UIImage imageNamed:@"icons_calloff"] forState:UIControlStateNormal];
    [hangupBtn addTarget:self action:@selector(hangup) forControlEvents:UIControlEventTouchUpInside];
    [self.bottomView addSubview:hangupBtn];
    [hangupBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerY.mas_equalTo(@0);
        make.height.width.mas_equalTo(@55);
        make.centerX.mas_equalTo(@0);
    }];
    
    UIButton *voiceBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    [voiceBtn setImage:[UIImage imageNamed:@"icons_voice_off"] forState:UIControlStateNormal];
    [voiceBtn setImage:[UIImage imageNamed:@"icons_voice_on"] forState:UIControlStateSelected];
    [voiceBtn addTarget:self action:@selector(voice:) forControlEvents:UIControlEventTouchUpInside];
    [self.bottomView addSubview:voiceBtn];
    [voiceBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerY.mas_equalTo(@0);
        make.height.width.mas_equalTo(@55);
        make.left.mas_equalTo(hangupBtn.mas_right).offset(10);
    }];
    
    UIButton *soundBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    [soundBtn setImage:[UIImage imageNamed:@"monitor_icon_voice"] forState:UIControlStateNormal];
    [soundBtn setImage:[UIImage imageNamed:@"monitor_icon_voiceoff"] forState:UIControlStateSelected];
    [soundBtn addTarget:self action:@selector(sound:) forControlEvents:UIControlEventTouchUpInside];
    [self.bottomView addSubview:soundBtn];
    [soundBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerY.mas_equalTo(@0);
        make.height.width.mas_equalTo(@55);
        make.right.mas_equalTo(hangupBtn.mas_left).offset(-10);
    }];
    
    UIButton *recordBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    [recordBtn setImage:[UIImage imageNamed:@"monitor_icon_video"] forState:UIControlStateNormal];
    [recordBtn setImage:[UIImage imageNamed:@"monitor_icon_video_work"] forState:UIControlStateSelected];
    [recordBtn addTarget:self action:@selector(record:) forControlEvents:UIControlEventTouchUpInside];
    [self.bottomView addSubview:recordBtn];
    [recordBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerY.mas_equalTo(@0);
        make.height.width.mas_equalTo(@55);
        make.right.mas_equalTo(soundBtn.mas_left).offset(-10);
    }];
    
    if (self.doorDevice && self.doorDevice.canOpenDoor) {
        UIButton *doorBtn = [UIButton buttonWithType:UIButtonTypeCustom];
        [doorBtn setImage:[UIImage imageNamed:@"monitor_icon_opendoor"] forState:UIControlStateNormal];
        [doorBtn addTarget:self action:@selector(openDoor) forControlEvents:UIControlEventTouchUpInside];
        [self.bottomView addSubview:doorBtn];
        [doorBtn mas_makeConstraints:^(MASConstraintMaker *make) {
            make.centerY.mas_equalTo(@0);
            make.height.width.mas_equalTo(@55);
            make.left.mas_equalTo(voiceBtn.mas_right).offset(10);
        }];
    }
    else {
        if (self.callType == JX_IntercomCallType_Call) {
            UIButton *cameraBtn = [UIButton buttonWithType:UIButtonTypeCustom];
            [cameraBtn setImage:[UIImage imageNamed:@"monitor_icon_takephoto"] forState:UIControlStateNormal];
            [cameraBtn addTarget:self action:@selector(camera:) forControlEvents:UIControlEventTouchUpInside];
            [self.bottomView addSubview:cameraBtn];
            [cameraBtn mas_makeConstraints:^(MASConstraintMaker *make) {
                make.centerY.mas_equalTo(@0);
                make.height.width.mas_equalTo(@55);
                make.left.mas_equalTo(voiceBtn.mas_right).offset(10);
            }];
        }
    }
}



#pragma mark - ======== Actions ========
- (void)pickup
{
    if (self.player) {
        [self.player stop];
        self.player = nil;
    }
    
    [[JXManager defaultManage].connectingManager pickUpInHome:self.homeId sessionId:self.sessionId];
    self.callInBottomView.hidden = YES;
}

- (void)hangup
{
    if (self.player) {
        [self.player stop];
        self.player = nil;
    }
    
    [[JXManager defaultManage].connectingManager hangUpInHome:self.homeId sessionId:self.sessionId];
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)record:(UIButton *)button
{
    if (button.selected == NO) {
        NSString *deviceName = nil;
        if (self.extDevice) {
            deviceName = self.extDevice.showName;
        }
        else {
            deviceName = self.doorDevice.showName;
        }
        BOOL result = [[JXManager defaultManage].connectingManager startRecordVideoInHome:self.homeId sessionId:self.sessionId deviceName:deviceName];
        if (result) {
            button.selected = YES;
        }
    }
    else {
        button.selected = NO;
        [[JXManager defaultManage].connectingManager stopRecordVideoInHome:self.homeId sessionId:self.sessionId];
    }
}

- (void)voice:(UIButton *)button
{
    BOOL enable = button.selected == YES;
    BOOL result = [[JXManager defaultManage].connectingManager enableVoice:enable inHome:self.homeId sessionId:self.sessionId];
    if (result) {
        button.selected = !button.selected;
    }
}

- (void)sound:(UIButton *)button
{
    BOOL enable = button.selected == YES;
    BOOL result = [[JXManager defaultManage].connectingManager enableSpeaker:enable inHome:self.homeId sessionId:self.sessionId];
    if (result) {
        button.selected = !button.selected;
    }
}

- (void)openDoor
{
    [[JXManager defaultManage].connectingManager unlockInHome:self.homeId sessionId:self.sessionId device:self.doorDevice];
}

// 切换摄像头
- (void)camera:(UIButton *)button
{
    BOOL front = button.selected == NO;
    BOOL result = [[JXManager defaultManage].connectingManager switchCamera:front];
    if (result) {
        button.selected = !button.selected;
    }
}

#pragma mark - ======== JXConnectingDelegate ========
/// 视频连接被挂断
- (void)didHangUpWithReason:(JX_IntercomReason)reason animated:(BOOL)animated
{
    if (self.player) {
        [self.player stop];
        self.player = nil;
    }
    
    [self dismissViewControllerAnimated:animated completion:nil];
}

/// 通话被对方接听
- (void)didPickup
{
    if (self.player) {
        [self.player stop];
        self.player = nil;
    }
}

/// 提供用来展示对方视频的 Layer, 必须是 CATiledLayer
- (CATiledLayer *)showOtherVideoLayer
{
    return self.layer;
}

/// 提供用来展示自己视频的 Layer, 必须是 CATiledLayer
- (CATiledLayer * _Nullable)showMineVideoLayer;
{
    if (self.mineLayerView.hidden == NO) {
        return self.mineLayer;
    }
    else {
        return nil;
    }
}

/// 门锁已打开
- (void)didDoorLockOpen
{
    NSLog(@"门已打开!");
}

/// 开始录制视频
- (void)didStartRecord
{
    NSLog(@"开始录制");
}

/// 结束录制视频
- (void)didEndRecord
{
    NSLog(@"录制结束");
}

- (BOOL)isFullScreen
{
    if ([UIDevice currentDevice].userInterfaceIdiom == UIUserInterfaceIdiomPhone) {
        
        CGSize size = [UIScreen mainScreen].bounds.size;
        NSInteger notchValue = size.width / size.height * 100;
        
        if (216 == notchValue || 46 == notchValue) {
            return YES;
        }
        
        return NO;
    }
    else {
        return NO;
    }
}

@end
