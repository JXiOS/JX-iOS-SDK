//
//  JXListViewController.m
//  JXIntercomDemo
//
//  Created by Nansen on 2020/4/24.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import "JXListViewController.h"
#import "HomeDetailViewController.h"

#import "Masonry.h"
#import <JXIntercomSDK/JXIntercomSDK.h>
#import "JXVideoViewController.h"

@interface JXListViewController ()
<UITableViewDelegate, UITableViewDataSource,
JXIntercomDelegate>

@property (nonatomic, strong) UITextField *homeIdTF;
@property (nonatomic, strong) UIButton *startButton;

@property (nonatomic, strong) UITableView *tableView;

@property (nonatomic, strong) NSMutableArray<NSString *> *homes;

@end

@implementation JXListViewController

- (NSMutableArray<NSString *> *)homes
{
    if (!_homes) {
        _homes = [NSMutableArray array];
    }
    return _homes;
}

- (UITableView *)tableView
{
    if (!_tableView) {
        UIColor *tableViewBgColor = [UIColor whiteColor];
        CGFloat tableViewRowHeight = 44.0f;
        _tableView = [[UITableView alloc] initWithFrame:CGRectZero style:UITableViewStylePlain];
        _tableView.backgroundColor = tableViewBgColor;
        _tableView.delegate = self;
        _tableView.dataSource = self;
        _tableView.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
        _tableView.rowHeight = tableViewRowHeight;
        
        UIView *tableFooterView = [[UIView alloc] init];
        tableFooterView.backgroundColor = tableViewBgColor;
        _tableView.tableFooterView = tableFooterView;
        
        if (@available(iOS 11.0, *)) {
            _tableView.contentInsetAdjustmentBehavior = UIScrollViewContentInsetAdjustmentNever;
            _tableView.estimatedRowHeight = 0;
            _tableView.estimatedSectionHeaderHeight = 0;
            _tableView.estimatedSectionFooterHeight = 0;
        }
    }
    return _tableView;
}



#pragma mark - ======== TableView ========
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return self.homes.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"UITableViewCell"];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"UITableViewCell"];
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
    }
    
    cell.textLabel.text = self.homes[indexPath.row];
    
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    HomeDetailViewController *hdVC = [[HomeDetailViewController alloc] initWithHomeId:self.homes[indexPath.row]];
    [self.navigationController pushViewController:hdVC animated:YES];
}

- (UITableViewCellEditingStyle)tableView:(UITableView *)tableView editingStyleForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return UITableViewCellEditingStyleDelete;
}

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        NSString *homeId = self.homes[indexPath.row];
        
        [[JXManager defaultManage] stopHome:homeId];
        [self.homes removeObject:homeId];
    }
}

- (NSString *)tableView:(UITableView *)tableView titleForDeleteConfirmationButtonForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return @"停止服务";
}



- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.view.backgroundColor = [UIColor whiteColor];
    self.edgesForExtendedLayout = UIRectEdgeNone;
    self.navigationItem.title = @"Home 列表";
    
    [self configsubviews];
    
    [JXManager defaultManage].appDelegate = self;
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    [self.homes removeAllObjects];
    [self.homes addObjectsFromArray:[[JXManager defaultManage] getConnectingHomes]];
    [self.tableView reloadData];
}

- (void)configsubviews
{
    UILabel *leftL = [[UILabel alloc] init];
    leftL.text = @"homeId :";
    leftL.textAlignment = NSTextAlignmentRight;
    [self.view addSubview:leftL];
    [leftL mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerX.mas_equalTo(@-120);
        make.top.mas_equalTo(@20);
        make.width.mas_equalTo(@120);
        make.height.mas_equalTo(@30);
    }];
    
    self.homeIdTF = [[UITextField alloc] init];
    self.homeIdTF.backgroundColor = [UIColor lightGrayColor];
    [self.view addSubview:self.homeIdTF];
    [self.homeIdTF mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(leftL.mas_right).offset(10);
        make.width.mas_equalTo(@200);
        make.centerY.mas_equalTo(leftL.mas_centerY);
        make.height.mas_equalTo(leftL.mas_height);
    }];
    
    self.startButton = [UIButton buttonWithType:UIButtonTypeSystem];
    [self.startButton setTitle:@"Start Server" forState:UIControlStateNormal];
    [self.view addSubview:self.startButton];
    [self.startButton mas_makeConstraints:^(MASConstraintMaker *make) {
        make.height.mas_equalTo(@40);
        make.width.mas_equalTo(@120);
        make.top.mas_equalTo(leftL.mas_bottom).offset(20);
        make.centerX.mas_equalTo(@0);
    }];
    [self.startButton addTarget:self action:@selector(startAction) forControlEvents:UIControlEventTouchUpInside];
    
    [self.view addSubview:self.tableView];
    [self.tableView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.bottom.mas_equalTo(@0);
        make.top.mas_equalTo(self.startButton.mas_bottom).offset(30);
    }];
}


- (void)startAction
{
    if (self.homeIdTF.text.length > 0) {
        if ([self.homes containsObject:self.homeIdTF.text]) {
            NSLog(@"已添加");
            return;
        }
        
        BOOL result = [[JXManager defaultManage] startHome:self.homeIdTF.text];
        if (result) {
            [self.homes addObject:self.homeIdTF.text];
            [self.tableView reloadData];
            NSLog(@"启动服务成功!");
        }
        else {
            NSLog(@"启动服务失败!");
        }
    }
    else {
        NSLog(@"请先填写 homeId");
    }
}


#pragma mark - ======== JXIntercomDelegate ========
/// 有呼叫进来, 是否需要响应.
- (BOOL)shouldResponseIntercomCall:(JX_IntercomScenes)callScenes
{
    UIViewController *vc = self.navigationController.topViewController;
    if (vc.presentedViewController) {
        return NO;
    }
    return YES;
}


/// 被门禁呼叫 (shouldResponseIntercomCall 返回 YES 的时候才会有)
- (id<JXConnectingDelegate>)calledByDoorWithHomeId:(NSString *)homeId
                                         sessionId:(NSString *)sessionId
                                        doorDevice:(JXDoorDeviceModel *)doorDevice
{
    JXVideoViewController *vc = [[JXVideoViewController alloc] initWithCallDevice:doorDevice callType:JX_IntercomCallType_Monitor callScenes:JX_IntercomScenes_Door isCallout:NO homeId:homeId];
    vc.sessionId = sessionId;
    if (@available(iOS 13.0, *)) {
        vc.modalPresentationStyle = UIModalPresentationFullScreen;
    }
    [self presentViewController:vc animated:YES completion:nil];
    return vc;
}

/// 被室内通呼叫 (shouldResponseIntercomCall 返回 YES 的时候才会有)
- (id<JXConnectingDelegate>)calledByExtWithHomeId:(NSString *)homeId
                                        sessionId:(NSString *)sessionId
                                        extDevice:(JXExtDeviceModel *)extDevice
{
    JXVideoViewController *vc = [[JXVideoViewController alloc] initWithExtDevice:extDevice callType:JX_IntercomCallType_Call callScenes:JX_IntercomScenes_Ext isCallout:NO homeId:homeId];
    vc.sessionId = sessionId;
    if (@available(iOS 13.0, *)) {
        vc.modalPresentationStyle = UIModalPresentationFullScreen;
    }
    [self presentViewController:vc animated:YES completion:nil];
    return vc;
}


@end
