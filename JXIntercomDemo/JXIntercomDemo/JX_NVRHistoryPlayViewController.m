//
//  JX_NVRHistoryPlayViewController.m
//  JXIntercomDemo
//
//  Created by Nansen on 2020/6/15.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import "JX_NVRHistoryPlayViewController.h"

#import <JXIntercomSDK/JXIntercomSDK.h>
#import "Masonry.h"

#import "NSDate+Utilities.h"

// 底部按钮到屏幕边缘的距离
static CGFloat const bottomM = 20.0f;
// 底部按钮的宽,高
static CGFloat const bottomBtnH = 24.0f;
// 滑动条和按钮的距离
static CGFloat const sliderLRM = 20.0f;


@interface JX_NVRHistoryPlayViewController ()<JXConnectingDelegate>

@property (nonatomic, strong) UIView *videoView;
@property (nonatomic, strong) CATiledLayer *videoLayer;
@property (nonatomic, strong) UILabel *dateLabel;

@property (nonatomic, strong) UIButton *playButton;
@property (nonatomic, strong) UISlider *slider;


@property (nonatomic, strong) NSDate *startDate;
@property (nonatomic, strong) NSDate *endDate;

/// 播放的偏移
@property (nonatomic, assign) NSInteger npt;

/// 实际总时长
@property (nonatomic, assign) NSInteger videoDuration;

@property (nonatomic, assign) BOOL getDuration;

@property (nonatomic, strong) dispatch_source_t timer;

@property (nonatomic, assign) BOOL timerSuspend;

@end

@implementation JX_NVRHistoryPlayViewController

- (instancetype)initWithNVRDevice:(JXDoorDeviceModel *)nvrDevice homeId:(NSString *)homeId startDate:(NSDate *)startDate endDate:(NSDate *)endDate
{
    if (self = [super init]) {
        self.homeId = homeId;
        self.nvrDevice = nvrDevice;
        self.startDate = startDate;
        self.endDate = endDate;
    }
    return self;
}


- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.view.backgroundColor = [UIColor blackColor];
    self.edgesForExtendedLayout = UIRectEdgeNone;
    
    self.npt = 0;
    self.videoDuration = (NSInteger)[self.endDate timeIntervalSinceDate:self.startDate];

    CGFloat sw = [UIScreen mainScreen].bounds.size.width;
    CGFloat vh = sw * 3.0 / 4.0;
    
    self.videoView = [[UIView alloc] init];
    self.videoView.backgroundColor = [UIColor greenColor];
    [self.view addSubview:self.videoView];
    [self.videoView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.mas_equalTo(@0);
        make.centerY.mas_equalTo(@0);
        make.height.mas_equalTo(@(vh));
    }];
    
    self.dateLabel = [[UILabel alloc] init];
    [self.view addSubview:self.dateLabel];
    self.dateLabel.textColor = [UIColor whiteColor];
    self.dateLabel.text = [self.startDate stringWithFormat:@"yyyy-MM-dd"];
    [self.dateLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerX.mas_equalTo(0);
        make.bottom.mas_equalTo(self.videoView.mas_top).offset(-10);
    }];
    
    
    self.videoLayer = [CATiledLayer layer];
    self.videoLayer.backgroundColor = [UIColor greenColor].CGColor;
    self.videoLayer.frame = CGRectMake(0, 0, sw, vh);
    [self.videoView.layer addSublayer:self.videoLayer];
    
    self.playButton = [UIButton buttonWithType:UIButtonTypeCustom];
    [self.playButton setImage:[UIImage imageNamed:@"start"] forState:UIControlStateNormal];
    [self.playButton setImage:[UIImage imageNamed:@"stop"] forState:UIControlStateSelected];
    [self.view addSubview:self.playButton];
    [self.playButton mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(bottomM);
        make.bottom.mas_equalTo(-bottomM-34);
        make.width.height.mas_equalTo(bottomBtnH);
    }];
    [self.playButton addTarget:self action:@selector(playClicked:) forControlEvents:UIControlEventTouchUpInside];
    
    
    UISlider *slider = [[UISlider alloc] init];
    [self.view addSubview:slider];
    [slider mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(self.playButton.mas_right).offset(sliderLRM);
        make.centerY.mas_equalTo(self.playButton.mas_centerY);
        make.right.mas_equalTo(-bottomM);
    }];
    slider.minimumValue = self.npt;
    slider.maximumValue = self.videoDuration;
    slider.value = self.npt;
    slider.continuous = YES;
    slider.minimumTrackTintColor = [UIColor greenColor];
    slider.maximumTrackTintColor = [UIColor whiteColor];
//    [slider setThumbImage:[UIImage imageNamed:@"sp_cicle"] forState:UIControlStateNormal];
    [slider addTarget:self action:@selector(sliderValueChanged:forEvent:) forControlEvents:UIControlEventValueChanged];
    self.slider = slider;
    
    self.getDuration = NO;
    
    [self connectSession];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    
    if (self.playButton.selected) {
        [self stopPlay];
    }
    
    [self stopTimer];
}

- (void)dealloc
{
    NSLog(@"NVR dealloc");
}


- (void)stopPlay
{
    NSLog(@"停止播放!");
    if (self.sessionId) {
        [[JXManager defaultManage].connectingManager hangUpInHome:self.homeId sessionId:self.sessionId];
        self.playButton.selected = NO;
        
        [self suspendTimer];
    }
    else {
        NSLog(@"stopPlay no sessionId");
    }
}

- (void)connectSession
{
    NSDate *startDate = [self.startDate dateByAddingSeconds:self.npt];
    NSLog(@"创建 NVR 连接: %@-%@", [startDate stringWithFormat:@"HH:mm:ss"], [self.endDate stringWithFormat:@"HH:mm:ss"]);
    
    NSString *sessionId = [[JXManager defaultManage].connectingManager playNVRHistoryWithHomeId:self.homeId nvrDevice:self.nvrDevice startDate:startDate endDate:self.endDate videoDelegate:self];
    
    if (sessionId) {
        NSLog(@"开始播放 : %@", sessionId);
        self.sessionId = sessionId;
        self.playButton.selected = YES;
        
        if (self.timer) {
            [self restartTimer];
        }
        else {
            [self startTimer];
        }
    }
    else {
        NSLog(@"查看 NVR 记录出错");
        self.playButton.selected = NO;
    }
}


- (void)playClicked:(UIButton *)button
{
    if (button.selected == NO) {
        // 播放
        [self connectSession];
    }
    else {
        [self stopPlay];
    }
}


- (void)sliderValueChanged:(UISlider *)slider forEvent:(UIEvent *)event
{
    UITouch * touchEvent = [[event allTouches] anyObject];
    switch (touchEvent.phase) {
        case UITouchPhaseBegan:
        {
            slider.continuous = NO;
            break;
        }
        case UITouchPhaseEnded:
        {
            slider.continuous = YES;
            
            [self seek:ceil(slider.value)];
//            int progress = ceil(slider.value * 100 / slider.maximumValue);
//            [[JXManager defaultManage].historyManager seekProgress:progress];
            break;
        }
        case UITouchPhaseCancelled:
        {
            slider.continuous = YES;
            break;
        }
        default:
            break;
    }
}

#pragma mark ---- 快进 ----
- (void)seek:(NSInteger)npt
{
    if (npt == 0) {
        return;
    }
    
    NSInteger h = npt / 3600;
    NSInteger m = npt / 60 % 60;
    NSInteger s = npt % 60;
    NSLog(@"-----> seek : %zd:%zd:%zd", h, m, s);
    
    if (self.sessionId) {
        BOOL result = [[JXManager defaultManage].connectingManager seekNVR:self.homeId sessionId:self.sessionId nvrDevice:self.nvrDevice starNpt:npt];
        if (result) {
            [self.slider setValue:npt animated:YES];
            self.npt = npt;
            NSLog(@"seek OK!");
        }
        else {
            NSLog(@"seek NVR Error : NO");
        }
    }
    else {
        NSLog(@"seek error: 无 sessionId");
    }
}


- (void)startTimer
{
    if (self.timer) {
        return;
    }
    
    self.timerSuspend = NO;
    
    // 全局队列
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);

    // 创建一个 timer 类型定时器 （ DISPATCH_SOURCE_TYPE_TIMER）
    self.timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, queue);
    
    // 设置立即执行事件, 间隔 1 秒
    dispatch_source_set_timer(self.timer, DISPATCH_TIME_NOW, NSEC_PER_SEC, 0.1 * NSEC_PER_SEC);
    
    // 任务回调
    __weak typeof(self) weakSelf = self;
    dispatch_source_set_event_handler(self.timer, ^{
        if ([NSThread isMainThread]) {
            [weakSelf addNpt];
        }
        else {
            dispatch_async(dispatch_get_main_queue(), ^{
                [weakSelf addNpt];
            });
        }
    });
    
    // 开始定时器任务（定时器默认开始是暂停的，需要复位开启）
    dispatch_resume(self.timer);
}

/// 暂停计时
- (void)suspendTimer
{
    if (self.timer) {
        dispatch_suspend(self.timer);
        self.timerSuspend = YES;
    }
}

/// 继续计时
- (void)restartTimer
{
    if (self.timer && self.timerSuspend) {
        self.timerSuspend = NO;
        dispatch_resume(self.timer);
    }
}

/// 停止计时, 返回当前显示的描述
- (void)stopTimer
{
    if (self.timer && self.timerSuspend == NO) {
        dispatch_source_cancel(self.timer);
        self.timer = nil;
    }
    else if (self.timerSuspend) {
        [self restartTimer];
        [self stopTimer];
    }
    
    self.npt = 0;
    [self.slider setValue:self.npt animated:YES];
}


- (void)addNpt
{
    self.npt++;
    
    if (self.npt > self.videoDuration) {
        self.npt = 0;
        [self stopPlay];
    }
    
    [self.slider setValue:self.npt animated:YES];
}


#pragma mark - ======== JXConnectingDelegate ========
/// 视频连接被挂断
- (void)didHangUpWithReason:(JX_IntercomReason)reason animated:(BOOL)animated
{
    NSLog(@"连接被挂断 reason = %@", [JXIntercomConstants reasonMsgByReason:reason]);
    
    [self stopTimer];
    self.sessionId = nil;
}

/// 提供用来展示对方视频的 Layer, 必须是 CATiledLayer
- (CATiledLayer *)showOtherVideoLayer
{
    return self.videoLayer;
}

/// 视频流开始
- (void)didVideoAppears
{
    [self startTimer];
}

/// 收到 NPT 播放偏移
/// @param startNpt 相对于起始时间的偏移
/// @param endNpt 结束时间的偏移
- (void)didReceiveNPT:(NSInteger)startNpt endNpt:(NSInteger)endNpt
{
    NSLog(@"收到 NPT 播放偏移 : %zd - %zd", startNpt, endNpt);
    
    if (self.getDuration == NO) {
        
        self.startDate = [self.startDate dateByAddingSeconds:startNpt];
        self.endDate = [self.startDate dateByAddingSeconds:endNpt];
        self.slider.maximumValue = endNpt;
        
        self.videoDuration = endNpt;
        
        self.getDuration = YES;
    }
}


/// 视频尺寸改变
- (void)didVideoSizeChangeNewWidth:(int)width newHeight:(int)height
{
    CGFloat w = self.videoView.bounds.size.width;
    CGFloat newH = w * height / width;
    
    [self updateVideoSize:newH];
}

- (void)updateVideoSize:(CGFloat)videoH
{
    if (videoH > 0) {
        NSLog(@"更新视频尺寸: [%.0f, %.0f] -> [%.0f, %.0f]", self.videoLayer.frame.size
        .width, self.videoLayer.frame.size.height, self.videoLayer.frame.size.width, videoH);
        
        [self.videoView mas_updateConstraints:^(MASConstraintMaker *make) {
            make.height.mas_equalTo(videoH);
        }];
        self.videoLayer.frame = CGRectMake(0, 0, self.view.bounds.size.width, videoH);
    }
}


@end
