//
//  JXVideoRecordViewController.m
//  JXIntercomDemo
//
//  Created by Nansen on 2020/4/30.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import "JXVideoRecordViewController.h"
#import <JXIntercomSDK/JXIntercomSDK.h>
#import "Masonry.h"

// 底部按钮到屏幕边缘的距离
static CGFloat const bottomM = 20.0f;
// 底部按钮的宽,高
static CGFloat const bottomBtnH = 24.0f;
// 滑动条和按钮的距离
static CGFloat const sliderLRM = 20.0f;


@interface JXVideoRecordViewController ()

@property (nonatomic, strong) JXVideoRecordModel *videoRecord;

@property (nonatomic, strong) UIButton *playButton;

@property (nonatomic, strong) UIView *videoView;

@property (nonatomic, strong) UISlider *slider;
@property (nonatomic, strong) CATiledLayer *videoLayer;
@property (nonatomic, strong) UIButton *downloadButton;

@end

@implementation JXVideoRecordViewController

- (instancetype)initWithVideo:(JXVideoRecordModel *)videoRecordModel
{
    if (self = [super init]) {
        self.videoRecord = videoRecordModel;
    }
    return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.view.backgroundColor = [UIColor whiteColor];
    self.title = @"录像";
    
    CGFloat sw = [UIScreen mainScreen].bounds.size.width;
    CGFloat vh = sw * 3.0 / 4.0;
    
    self.videoView = [[UIView alloc] init];
    self.videoView.backgroundColor = [UIColor blackColor];
    [self.view addSubview:self.videoView];
    [self.videoView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.mas_equalTo(@0);
        make.centerY.mas_equalTo(@0);
        make.height.mas_equalTo(@(vh));
    }];
    
    self.videoLayer = [CATiledLayer layer];
    self.videoLayer.backgroundColor = [UIColor blackColor].CGColor;
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
        make.right.mas_equalTo(-sliderLRM-bottomM-bottomBtnH);
    }];
    slider.minimumValue = 0;
    slider.maximumValue = 100;
    slider.value = 0;
    slider.continuous = YES;
    slider.minimumTrackTintColor = [UIColor blueColor];
    slider.maximumTrackTintColor = [UIColor redColor];
    [slider setThumbImage:[UIImage imageNamed:@"sp_cicle"] forState:UIControlStateNormal];
    [slider addTarget:self action:@selector(sliderValueChanged:forEvent:) forControlEvents:UIControlEventValueChanged];
    self.slider = slider;
    
    UIButton *button = [UIButton buttonWithType:UIButtonTypeCustom];
    [button setImage:[UIImage imageNamed:@"sp_download"] forState:UIControlStateNormal];
    [button setImage:[UIImage imageNamed:@"sp_downloading"] forState:UIControlStateHighlighted];
    [self.view addSubview:button];
    [button mas_makeConstraints:^(MASConstraintMaker *make) {
        make.bottom.mas_equalTo(-bottomM-34);
        make.right.mas_equalTo(-bottomM);
        make.height.width.mas_equalTo(bottomBtnH);
    }];
    [button addTarget:self action:@selector(downloadClicked:) forControlEvents:UIControlEventTouchUpInside];
    self.downloadButton = button;
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
 
    
    if (self.playButton.selected) {
        [[JXManager defaultManage].historyManager stopPlayVideoRecord:self.videoRecord];
        self.playButton.selected = NO;
    }
}

- (void)playClicked:(UIButton *)button
{
    if (button.selected == NO) {
        // 播放
        __weak typeof(self) weakSelf = self;
        BOOL result = [[JXManager defaultManage].historyManager playVideoRecord:self.videoRecord onLayer:self.videoLayer getDurationBlock:^(long long msDuration) {
            weakSelf.slider.maximumValue = msDuration;
        } playProgressBlock:^(long long playingMs) {
            [weakSelf.slider setValue:playingMs animated:YES];
        } completeBlock:^{
            weakSelf.playButton.selected = NO;
            [weakSelf.slider setValue:0 animated:YES];
        }];
        
        if (result) {
            button.selected = YES;
        }
        
    }
    else {
        [[JXManager defaultManage].historyManager stopPlayVideoRecord:self.videoRecord];
        button.selected = NO;
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
            int progress = ceil(slider.value * 100 / slider.maximumValue);
            [[JXManager defaultManage].historyManager seekProgress:progress];
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


- (void)downloadClicked:(UIButton *)button
{
    BOOL result = [[JXManager defaultManage].historyManager exportVideoToAlbum:self.videoRecord progressBlock:^(NSInteger progress) {
        NSLog(@"转码进度 : %zd/100", progress);
    } completeBlock:^(PHAsset * _Nullable asset, NSString * _Nullable errorMessage) {
        if (errorMessage) {
            NSLog(@"转码失败 : %@", errorMessage);
        }
        else {
            button.selected = YES;
        }
        
        button.enabled = YES;
    }];
    if (result) {
        button.enabled = NO;
    }
}


@end
