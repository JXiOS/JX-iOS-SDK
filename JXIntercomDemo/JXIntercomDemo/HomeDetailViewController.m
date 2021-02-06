//
//  HomeDetailViewController.m
//  JXIntercomDemo
//
//  Created by Nansen on 2020/4/26.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import "HomeDetailViewController.h"
#import <JXIntercomSDK/JXIntercomSDK.h>
#import "Masonry.h"
#import "JXVideoViewController.h"
#import "JXHistoryViewController.h"
#import "JX_NVRHistoryVideoController.h"
#import "NSDate+Utilities.h"
#import "JX_DemoConfig.h"



@interface HomeDetailViewController ()
<UITableViewDelegate, UITableViewDataSource,
JXDeviceManagerDelegate,
JXSecurityDelegate>

@property (nonatomic, strong) NSMutableArray *details;
@property (nonatomic, strong) NSArray *sectionTitles;


@property (nonatomic, strong) UITableView *tableView;

@property (nonatomic, assign) BOOL isSupportSecurity;
@property (nonatomic, assign) JX_SecurityStatus securityStatus;

@property (nonatomic, strong) NSMutableArray<JXDoorDeviceModel *> *doorDevices;
@property (nonatomic, strong) NSMutableArray<JXExtDeviceModel *> *extDevices;

@property (nonatomic, strong) NSMutableArray<JXDoorDeviceModel *> *nvrDevices;


@property (nonatomic, strong) NSMutableArray<JXCallRecordModel *> *historyArray;

@property (nonatomic, strong) UIButton *openDoorBtn;
@property (nonatomic, assign) BOOL canOpenDoor;

@property (nonatomic, strong) UITextField *p2pNumberTF;
@property (nonatomic, strong) UIButton *callP2PBtn;


@end

@implementation HomeDetailViewController

- (NSArray *)sectionTitles
{
    return @[@"门禁设备", @"室内通设备", @"安防状态", @"历史记录", @"NVR回放"];
}

- (NSMutableArray<JXDoorDeviceModel *> *)doorDevices
{
    if (!_doorDevices) {
        _doorDevices = [NSMutableArray array];
    }
    return _doorDevices;
}

- (NSMutableArray<JXExtDeviceModel *> *)extDevices
{
    if (!_extDevices) {
        _extDevices = [NSMutableArray array];
    }
    return _extDevices;
}

- (NSMutableArray<JXCallRecordModel *> *)historyArray
{
    if (!_historyArray) {
        _historyArray = [NSMutableArray array];
    }
    return _historyArray;
}

- (NSMutableArray<JXDoorDeviceModel *> *)nvrDevices
{
    if (!_nvrDevices) {
        _nvrDevices = [NSMutableArray array];
    }
    return _nvrDevices;
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
        _tableView.sectionHeaderHeight = tableViewRowHeight;
        
        UIView *tableHeaderView = [[UIView alloc] init];
        tableHeaderView.frame = CGRectMake(0, 0, self.view.bounds.size.width, 60);
        tableHeaderView.backgroundColor = tableViewBgColor;
        
        self.p2pNumberTF = [[UITextField alloc] init];
        self.p2pNumberTF.placeholder = @"请输入户户通房号";
        self.p2pNumberTF.textAlignment = NSTextAlignmentCenter;
        [tableHeaderView addSubview:self.p2pNumberTF];
        [self.p2pNumberTF mas_makeConstraints:^(MASConstraintMaker *make) {
            make.top.left.right.mas_equalTo(0);
            make.height.mas_equalTo(30);
        }];
        
        self.p2pNumberTF.text = [JX_DemoConfig callNumber];
        
        UIButton *callP2pBtn = [UIButton buttonWithType:UIButtonTypeSystem];
        [callP2pBtn setTitle:@"呼叫户户通" forState:UIControlStateNormal];
        [callP2pBtn setTitle:@"不支持户户通" forState:UIControlStateDisabled];
        [callP2pBtn addTarget:self action:@selector(callP2pAction:) forControlEvents:UIControlEventTouchUpInside];
        [tableHeaderView addSubview:callP2pBtn];
        [callP2pBtn mas_makeConstraints:^(MASConstraintMaker *make) {
            make.bottom.mas_equalTo(0);
            make.height.mas_equalTo(30);
            make.centerX.mas_equalTo(0);
            make.width.mas_equalTo(150);
        }];
        self.callP2PBtn = callP2pBtn;
        
        _tableView.tableHeaderView = tableHeaderView;
        
        UIView *tableFooterView = [[UIView alloc] init];
        tableFooterView.frame = CGRectMake(0, 0, self.view.bounds.size.width, 40);
        tableFooterView.backgroundColor = tableViewBgColor;
        
        self.openDoorBtn = [UIButton buttonWithType:UIButtonTypeSystem];
        [self.openDoorBtn setTitle:@"远程开锁" forState:UIControlStateNormal];
        [tableFooterView addSubview:self.openDoorBtn];
        [self.openDoorBtn mas_makeConstraints:^(MASConstraintMaker *make) {
            make.centerX.mas_equalTo(0);
            make.centerY.mas_equalTo(0);
            make.width.mas_equalTo(100);
            make.height.mas_equalTo(30);
        }];
        [self.openDoorBtn addTarget:self action:@selector(openDoor:) forControlEvents:UIControlEventTouchUpInside];
        self.openDoorBtn.hidden = !self.canOpenDoor;
        
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
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return self.sectionTitles.count;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    return self.sectionTitles[section];
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if (section == 0) {
        // 门禁设备
        return self.doorDevices.count;
    }
    else if (section == 1) {
        // 室内通设备
        return self.extDevices.count;
    }
    else if (section == 2) {
        // 安防状态
        return 2;
    }
    else if (section == 3) {
        // 历史记录
        return self.historyArray.count;
    }
    else if (section == 4) {
        // NVR 回放
        return self.nvrDevices.count;
    }
    else {
        return 0;
    }
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"UITableViewHomeCell"];
    if (!cell) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:@"UITableViewHomeCell"];
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
    }
    
    if (indexPath.section == 0) {
        // 门禁设备
        JXDoorDeviceModel *doorDeviceModel = self.doorDevices[indexPath.row];
        cell.textLabel.text = doorDeviceModel.showName;
        cell.detailTextLabel.text = nil;
    }
    else if (indexPath.section == 1) {
        // 室内通设备
        JXExtDeviceModel *extDeviceModel = self.extDevices[indexPath.row];
        cell.textLabel.text = extDeviceModel.showName;
        cell.detailTextLabel.text = nil;
    }
    else if (indexPath.section == 2) {
        // 安防状态
        if (indexPath.row == 0) {
            cell.textLabel.text = @"当前安防状态:";
            cell.detailTextLabel.text = [self securityStatusString];
        }
        else {
            cell.textLabel.text = @"切换安防状态";
            cell.detailTextLabel.text = nil;
        }
    }
    else if (indexPath.section == 3) {
        // 历史记录
        JXCallRecordModel *callRecordModel = self.historyArray[indexPath.row];
        
        NSString *scenes = @"门禁";
        if (callRecordModel.scenes == JX_IntercomScenes_Ext) {
            scenes = @"室内通";
        }
        else if (callRecordModel.scenes == JX_IntercomScenes_P2P) {
            scenes = @"户户通";
        }
        
        NSString *type = callRecordModel.callType == JX_IntercomCallType_Call ? @"呼叫" : @"监控";
        NSString *iscallout = callRecordModel.isCallout ? @"呼出" : @"被呼";
        cell.textLabel.text = [NSString stringWithFormat:@"%@-%@-%@", scenes, type, iscallout];
        
        if (callRecordModel.videoRecordsArray.count > 0) {
            cell.detailTextLabel.text = @"有录像";
        }
        else {
            cell.detailTextLabel.text = @"无录像";
        }
    }
    else if (indexPath.section == 4) {
        // nvr 回放
        JXDoorDeviceModel *nvrDevice = self.nvrDevices[indexPath.row];
        cell.textLabel.text = nvrDevice.showName;
        cell.detailTextLabel.text = nil;
    }
    return cell;
}

- (void)scrollViewWillBeginDragging:(UIScrollView *)scrollView
{
    if ([self.p2pNumberTF isFirstResponder]) {
        [self.p2pNumberTF resignFirstResponder];
    }
}

- (NSString *)securityStatusString
{
    if (self.isSupportSecurity == NO) {
        return @"不支持安防";
    }
    else {
        if (self.securityStatus == JX_SecurityStatus_UnDefence) {
            return @"撤防";
        }
        else if (self.securityStatus == JX_SecurityStatus_Defence) {
            return @"布防";
        }
        else if (self.securityStatus == JX_SecurityStatus_OffLine) {
            return @"离线";
        }
        else {
            return @"无设备";
        }
    }
}



- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.section == 0) {
        // 门禁设备 -> 查看门禁摄像头
        JXDoorDeviceModel *doorDeviceModel = self.doorDevices[indexPath.row];
        [self callDoorMonitor:doorDeviceModel];
    }
    else if (indexPath.section == 1) {
        // 室内通设备
        JXExtDeviceModel *extDeviceModel = self.extDevices[indexPath.row];
        [self callExt:extDeviceModel];
    }
    else if (indexPath.section == 2) {
        // 安防状态
        if (indexPath.row == 1) {
            [self switchSecurity];
        }
    }
    else if (indexPath.section == 3) {
        // 历史记录
        JXCallRecordModel *callRecordModel = self.historyArray[indexPath.row];
        if (callRecordModel.videoRecordsArray.count > 0) {
            [self showHistoryVideos:callRecordModel];
        }
    }
    else if (indexPath.section == 4) {
        // nvr 回放
        JXDoorDeviceModel *nvrDevice = self.nvrDevices[indexPath.row];
        [self playNvrHistory:nvrDevice];
    }
}


- (instancetype)initWithHomeId:(NSString *)homeId
{
    if (self = [super init]) {
        self.homeId = homeId;
    }
    return self;
}


- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.title = self.homeId;
    self.view.backgroundColor = [UIColor whiteColor];
    self.edgesForExtendedLayout = UIRectEdgeNone;
    
    [[JXManager defaultManage].deviceManager addDeviceDelegateHolder:self];
    [[JXManager defaultManage].securityManager addSecurityDelegate:self];
    
    self.isSupportSecurity = [[JXManager defaultManage].securityManager isSupportSecurity:self.homeId];
    self.securityStatus = [[JXManager defaultManage].securityManager querySecurityStatus:self.homeId];
    
    [self.doorDevices addObjectsFromArray:[[JXManager defaultManage].deviceManager getDoorDeviceInHome:self.homeId]];
    [self.extDevices addObjectsFromArray:[[JXManager defaultManage].deviceManager getExtDeviceInHome:self.homeId]];
    
    for (JXDoorDeviceModel *tmpDoorDevice in self.doorDevices) {
        if (tmpDoorDevice.deviceType == JX_DeviceType_NVRIPCCamera) {
            [self.nvrDevices addObject:tmpDoorDevice];
        }
        
        /// 展示开门
        if (tmpDoorDevice.canOpenDoor == YES) {
            self.canOpenDoor = YES;
            
            if (self.openDoorBtn) {
                self.openDoorBtn.hidden = !self.canOpenDoor;
            }
        }
    }
    
    [self.historyArray addObjectsFromArray:[[JXManager defaultManage].historyManager getRecordsInHome:self.homeId]];
    
    [self.view addSubview:self.tableView];
    [self.tableView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.bottom.top.mas_equalTo(@0);
    }];
    
    self.callP2PBtn.enabled = [[JXManager defaultManage].connectingManager isSupportP2PInHome:self.homeId];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    
    if ([self.p2pNumberTF isFirstResponder]) {
        [self.p2pNumberTF resignFirstResponder];
    }
}


- (void)dealloc
{
    [[JXManager defaultManage].deviceManager removeDeviceDelegateHolder:self];
    [[JXManager defaultManage].securityManager removeSecurityDelegate:self];
}

#pragma mark - ======== Actions ========

- (void)callP2pAction:(UIButton *)button
{
    NSString *callNumber = self.p2pNumberTF.text;
    if (callNumber == nil || callNumber.length == 0) {
        NSLog(@"输入房号");
        return;
    }
    
    self.callP2PBtn.userInteractionEnabled = NO;
    
    JXVideoViewController *vc = [[JXVideoViewController alloc] initWithCallP2P:callNumber homeId:self.homeId isCallout:YES];
    if (@available(iOS 13.0, *)) {
        vc.modalPresentationStyle = UIModalPresentationFullScreen;
    }
    [self presentViewController:vc animated:YES completion:^{
        self.callP2PBtn.userInteractionEnabled = YES;
    }];
}

// 查看门禁的摄像头
- (void)callDoorMonitor:(JXDoorDeviceModel *)doorDevice
{
    JXVideoViewController *vc = [[JXVideoViewController alloc] initWithCallDevice:doorDevice callType:JX_IntercomCallType_Monitor callScenes:JX_IntercomScenes_Door isCallout:YES homeId:self.homeId];
    
    if (@available(iOS 13.0, *)) {
        vc.modalPresentationStyle = UIModalPresentationFullScreen;
    }
    [self presentViewController:vc animated:YES completion:nil];
}

// 查看室内通
- (void)callExt:(JXExtDeviceModel *)extDevice
{
    // 呼叫/查看监控
    UIAlertController *sheetController = [UIAlertController alertControllerWithTitle:@"室内通" message:@"选择模式" preferredStyle:UIAlertControllerStyleActionSheet];
    
    __weak typeof(self) weakSelf = self;
    UIAlertAction *action = [UIAlertAction actionWithTitle:@"查看监控" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [weakSelf extMonitor:extDevice];
    }];
    
    [sheetController addAction:action];
    
    UIAlertAction *action2 = [UIAlertAction actionWithTitle:@"呼叫" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [weakSelf extCall:extDevice];
    }];
    
    [sheetController addAction:action2];
    

    UIAlertAction *cancelAction = [UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:nil];
    [sheetController addAction:cancelAction];
    
    [self presentViewController:sheetController animated:YES completion:nil];
}

- (void)extMonitor:(JXExtDeviceModel *)extDevice
{
    JXVideoViewController *vc = [[JXVideoViewController alloc] initWithExtDevice:extDevice callType:JX_IntercomCallType_Monitor callScenes:JX_IntercomScenes_Ext isCallout:YES homeId:self.homeId];
    
    if (@available(iOS 13.0, *)) {
        vc.modalPresentationStyle = UIModalPresentationFullScreen;
    }
    [self presentViewController:vc animated:YES completion:nil];
}

- (void)extCall:(JXExtDeviceModel *)extDevice
{
    JXVideoViewController *vc = [[JXVideoViewController alloc] initWithExtDevice:extDevice callType:JX_IntercomCallType_Call callScenes:JX_IntercomScenes_Ext isCallout:YES homeId:self.homeId];
    if (@available(iOS 13.0, *)) {
        vc.modalPresentationStyle = UIModalPresentationFullScreen;
    }
    [self presentViewController:vc animated:YES completion:nil];
}



// 切换安防状态
- (void)switchSecurity
{
    if (self.isSupportSecurity) {
        [[JXManager defaultManage].securityManager switchSecurityStatus:self.homeId];
    }
}

/// 展示录像
- (void)showHistoryVideos:(JXCallRecordModel *)callRecord
{
    JXHistoryViewController *vc = [[JXHistoryViewController alloc] initWithCallRecord:callRecord];
    [self.navigationController pushViewController:vc animated:YES];
}

/// 展示 nvr 的录像
- (void)playNvrHistory:(JXDoorDeviceModel *)nvrDevice
{
    NSLog(@"name = %@", nvrDevice.subDeviceName);
    NSDate *date = [[NSDate date] dateBySubtractingDays:1];

    JX_NVRHistoryVideoController *vc = [[JX_NVRHistoryVideoController alloc] initWithNVRDevice:nvrDevice homeId:self.homeId startDate:[date theDayBeginDate] endDate:[date theDayEndDate]];
    if (@available(iOS 13.0, *)) {
        vc.modalPresentationStyle = UIModalPresentationFullScreen;
    }
    [self presentViewController:vc animated:YES completion:nil];
}

#pragma mark - ======== JXSecurityDelegate ========
- (void)didSecurityStatusChangedInHome:(NSString *)homeId status:(JX_SecurityStatus)status isFromQuery:(BOOL)isFromQuery
{
    if ([homeId isEqualToString:self.homeId]) {
        self.securityStatus = status;
        [self.tableView reloadSections:[NSIndexSet indexSetWithIndex:2] withRowAnimation:UITableViewRowAnimationAutomatic];
    }
}

/// 收到安防报警
- (void)didReceivedAlarmInHome:(NSString *)homeId
{
    
}

/// 报警取消
- (void)didCancelAlarmInHome:(NSString *)homeId
{
    
}



#pragma mark - ======== JXDeviceManagerDelegate ========
/// 门禁设备列表改变
- (void)updateDoorDevicesInHome:(NSString *)homeId
{
    self.canOpenDoor = NO;
    [self.doorDevices removeAllObjects];
    
    [self.doorDevices addObjectsFromArray:[[JXManager defaultManage].deviceManager getDoorDeviceInHome:self.homeId]];
    [self.tableView reloadSections:[NSIndexSet indexSetWithIndex:0] withRowAnimation:UITableViewRowAnimationAutomatic];
    
    [self.nvrDevices removeAllObjects];
    for (JXDoorDeviceModel *tmpDoorDevice in self.doorDevices) {
        if (tmpDoorDevice.deviceType == JX_DeviceType_NVRIPCCamera) {
            [self.nvrDevices addObject:tmpDoorDevice];
        }
        
        if (tmpDoorDevice.canOpenDoor == YES) {
            self.canOpenDoor = YES;
        }
    }
    
    [self.tableView reloadSections:[NSIndexSet indexSetWithIndex:4] withRowAnimation:UITableViewRowAnimationAutomatic];
    
    if (self.openDoorBtn) {
        self.openDoorBtn.hidden = !self.canOpenDoor;
    }
}

/// 室内通列表改变
- (void)updateExtDevicesInHome:(NSString *)homeId
{
    [self.extDevices removeAllObjects];
    [self.extDevices addObjectsFromArray:[[JXManager defaultManage].deviceManager getExtDeviceInHome:self.homeId]];
    [self.tableView reloadSections:[NSIndexSet indexSetWithIndex:1] withRowAnimation:UITableViewRowAnimationAutomatic];
}




- (void)openDoor:(UIButton *)button
{
    button.enabled = NO;
    
    for (JXDoorDeviceModel *tmpDoorDevice in self.doorDevices) {
        // 开门
        if (tmpDoorDevice.canOpenDoor == YES) {
            NSLog(@"开门!");
            [[JXManager defaultManage].deviceManager openDoor:self.homeId deviceName:tmpDoorDevice.subDeviceName];
        }
    }
    
    
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        button.enabled = YES;
    });
}




@end
