//
//  JXHistoryViewController.m
//  JXIntercomDemo
//
//  Created by Nansen on 2020/4/30.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import "JXHistoryViewController.h"
#import <JXIntercomSDK/JXIntercomSDK.h>
#import "JXHistoryTableViewCell.h"
#import "Masonry.h"
#import "JXVideoRecordViewController.h"

@interface JXHistoryViewController ()<UITableViewDelegate, UITableViewDataSource>

@property (nonatomic, strong) UITableView *tableView;

@property (nonatomic, strong) JXCallRecordModel *callRecord;

@end

@implementation JXHistoryViewController

- (instancetype)initWithCallRecord:(JXCallRecordModel *)record
{
    if (self = [super init]) {
        self.callRecord = record;
    }
    return self;
}


- (UITableView *)tableView
{
    if (!_tableView) {
        CGFloat sw = [UIScreen mainScreen].bounds.size.width;
        
        UIColor *tableViewBgColor = [UIColor whiteColor];
        CGFloat tableViewRowHeight = sw * 3.0 / 4.0 + 20;
        _tableView = [[UITableView alloc] initWithFrame:CGRectZero style:UITableViewStylePlain];
        _tableView.backgroundColor = tableViewBgColor;
        _tableView.delegate = self;
        _tableView.dataSource = self;
        _tableView.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
        _tableView.rowHeight = tableViewRowHeight;
        
        UIView *tableFooterView = [[UIView alloc] init];
        tableFooterView.backgroundColor = tableViewBgColor;
        _tableView.tableFooterView = tableFooterView;
        
        [_tableView registerClass:[JXHistoryTableViewCell class] forCellReuseIdentifier:@"JXHistoryTableViewCell"];
        
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
    return self.callRecord.videoRecordsArray.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    JXHistoryTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"JXHistoryTableViewCell"];
    cell.videoRecord = self.callRecord.videoRecordsArray[indexPath.row];
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    JXVideoRecordModel *m = self.callRecord.videoRecordsArray[indexPath.row];
    JXVideoRecordViewController *vc = [[JXVideoRecordViewController alloc] initWithVideo:m];
    [self.navigationController pushViewController:vc animated:YES];
}


- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.view.backgroundColor = [UIColor whiteColor];
    self.edgesForExtendedLayout = UIRectEdgeNone;
    self.title = @"通话录像";
    
    [self.view addSubview:self.tableView];
    [self.tableView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.top.bottom.mas_equalTo(@0);
    }];
}



@end
