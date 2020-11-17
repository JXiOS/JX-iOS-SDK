//
//  JX_NVRHistoryVideoController.m
//  smartlifebell
//
//  Created by Nansen on 2020/6/17.
//  Copyright © 2020 Jingxi. All rights reserved.
//

#import "JX_NVRHistoryVideoController.h"
#import "NSDate+Utilities.h"
#import "QiSlider.h"
#import <JXIntercomSDK/JXIntercomSDK.h>
#import "Masonry.h"

// 滑动条和按钮的距离
static CGFloat const sliderLRM = 10.0f;


@interface JX_NVRHistoryVideoController ()<JXConnectingDelegate>

@property (nonatomic, copy, nullable) NSString *sessionId;

@property (nonatomic, strong) JXDoorDeviceModel *nvrDevice;

@property (nonatomic, copy) NSString *homeId;
@property (nonatomic, strong) NSDate *startDate;
@property (nonatomic, strong) NSDate *endDate;

@property (nonatomic, strong) UIView *videoView;
@property (nonatomic, strong) CATiledLayer *videoLayer;
@property (nonatomic, strong) UILabel *dateLabel;

//@property (nonatomic, strong) UIButton *playButton;
@property (nonatomic, strong) QiSlider *slider;

/// 播放的偏移
@property (nonatomic, assign) NSInteger npt;

/// 实际总时长
@property (nonatomic, assign) NSInteger videoDuration;

@property (nonatomic, assign) BOOL getDuration;

@property (nonatomic, strong) dispatch_source_t timer;

@property (nonatomic, assign) BOOL timerSuspend;


@end

@implementation JX_NVRHistoryVideoController

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
    CGFloat y = ([UIScreen mainScreen].bounds.size.height - vh) / 2.0;
    
    NSLog(@"y1 = %.0f", y);
    self.videoView = [[UIView alloc] initWithFrame:CGRectMake(0, y, sw, vh)];
    self.videoView.backgroundColor = [UIColor greenColor];
    [self.view addSubview:self.videoView];

    self.videoLayer = [CATiledLayer layer];
    self.videoLayer.backgroundColor = [UIColor greenColor].CGColor;
    self.videoLayer.frame = CGRectMake(0, 0, sw, vh);
    [self.videoView.layer addSublayer:self.videoLayer];
    
    
    
    UIButton *closeButton = [UIButton buttonWithType:UIButtonTypeCustom];
    closeButton.backgroundColor = [UIColor clearColor];
    [closeButton setImage:[UIImage imageNamed:@"icons_back"] forState:UIControlStateNormal];
    closeButton.imageView.contentMode = UIViewContentModeCenter;
    [self.view addSubview:closeButton];
    [closeButton addTarget:self action:@selector(close) forControlEvents:UIControlEventTouchUpInside];
    [closeButton mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(@16);
        make.height.width.mas_equalTo(44);
        make.top.mas_equalTo([[UIApplication sharedApplication] statusBarFrame].size.height);
    }];
    
    self.dateLabel = [[UILabel alloc] init];
    self.dateLabel.font = [UIFont boldSystemFontOfSize:20];
    self.dateLabel.text = [self.startDate stringWithFormat:@"yyyy-MM-dd"];
    self.dateLabel.textColor = [UIColor whiteColor];
    self.dateLabel.textAlignment = NSTextAlignmentCenter;
    [self.view addSubview:self.dateLabel];
    [self.dateLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerX.mas_equalTo(@0);
        make.centerY.mas_equalTo(closeButton.mas_centerY);
    }];
    
    QiSlider *slider = [[QiSlider alloc] init];
    [self.view addSubview:slider];
    [slider mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(sliderLRM);
        make.bottom.mas_equalTo(-sliderLRM);
        make.right.mas_equalTo(-sliderLRM);
        make.height.mas_equalTo(24);
    }];
    slider.minimumValue = self.npt;
    slider.maximumValue = self.videoDuration;
    slider.value = self.npt;
    slider.continuous = YES;
    slider.thumbTintColor = [UIColor whiteColor];
    slider.minimumTrackTintColor = [UIColor whiteColor];
    slider.maximumTrackTintColor = [UIColor lightGrayColor];
    
    __weak typeof(self) weakSelf = self;
    [slider setSeek:^(NSInteger npt) {
        [weakSelf seek:npt];
    }];
    
    self.slider = slider;
    
    self.getDuration = NO;
    
    [self connectSession];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];

}

- (void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
    
    if (self.sessionId) {
        [self stopPlay];
    }
    
    [self stopTimer];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
}

- (void)close
{
    [self stopPlay];
    [self stopTimer];
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)stopPlay
{
    if (self.sessionId) {
        [[JXManager defaultManage].connectingManager hangUpInHome:self.homeId sessionId:self.sessionId];
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
        
        if (self.timer) {
            [self restartTimer];
        }
        else {
            [self startTimer];
        }
    }
    else {
        NSLog(@"查看 NVR 记录出错");
    }
}

//- (void)sliderValueChanged:(UISlider *)slider forEvent:(UIEvent *)event
//{
//    UITouch * touchEvent = [[event allTouches] anyObject];
//    switch (touchEvent.phase) {
//        case UITouchPhaseBegan:
//        {
//            slider.continuous = NO;
//            break;
//        }
//        case UITouchPhaseEnded:
//        {
//            slider.continuous = YES;
//            [self seek:ceil(slider.value)];
//            break;
//        }
//        case UITouchPhaseCancelled:
//        {
//            slider.continuous = YES;
//            break;
//        }
//        default:
//            break;
//    }
//}

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
    
    if (self.slider.continuous) {
        [self.slider setValue:self.npt animated:YES];
    }
}


#pragma mark - ======== JXConnectingDelegate ========
/// 视频连接被挂断
- (void)didHangUpWithReason:(JX_IntercomReason)reason animated:(BOOL)animated
{
    if (reason != JX_IntercomReason_None) {
        NSString *reasonStr = [JXIntercomConstants reasonMsgByReason:reason];
        [self stopTimer];
        self.sessionId = nil;
        NSLog(@"%@", reasonStr);
    }
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
    if (self.getDuration == NO) {
        
        self.startDate = [self.startDate dateByAddingSeconds:startNpt];
        self.endDate = [self.startDate dateByAddingSeconds:endNpt];
        self.slider.maximumValue = endNpt;
        
        self.videoDuration = endNpt;
        self.getDuration = YES;
    }
}

- (void)didVideoSizeChangeNewWidth:(int)width newHeight:(int)height
{
    CGFloat w = self.videoView.bounds.size.width;
    CGFloat newH = w * height / width;
    
    if (newH > [UIScreen mainScreen].bounds.size.height) {
        newH = [UIScreen mainScreen].bounds.size.height;
    }
    
    [self updateVideoSize:newH];
}

- (void)updateVideoSize:(CGFloat)videoH
{
    if (videoH > 0) {        
        CGFloat sw = [UIScreen mainScreen].bounds.size.width;
        CGFloat y = ([UIScreen mainScreen].bounds.size.height - videoH) / 2.0;
        
        self.videoView.frame = CGRectMake(0, y, sw, videoH);
        self.videoLayer.frame = CGRectMake(0, 0, sw, videoH);
        
        [[JXManager defaultManage].connectingManager updateVideoLayer:self.videoLayer sessionId:self.sessionId homeId:self.homeId];
    }
}

@end
