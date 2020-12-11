//
//  ViewController.m
//  JXIntercomDemo
//
//  Created by Nansen on 2020/3/23.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import "ViewController.h"
#import <JXIntercomSDK/JXIntercomSDK.h>
#import "JXListViewController.h"
#import "JXVideoViewController.h"

@import AVFoundation;


@interface ViewController ()

@property (weak, nonatomic) IBOutlet UILabel *versionLabel;

@property (weak, nonatomic) IBOutlet UITextField *userTF;

@property (weak, nonatomic) IBOutlet UITextField *aliasTF;



@property (nonatomic, assign) BOOL isLogin;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.isLogin = NO;
    
    self.versionLabel.text = [NSString stringWithFormat:@"SDK Version: %@", [JXManager version]];
    
    // 这里的参数需要填写
//    self.userIdTF.text = @"xxx";
//    self.aliasTF.text = @"xxx";
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [self.view endEditing:YES];
}



- (IBAction)loginAction:(UIButton *)sender {
    
    if (self.isLogin == NO) {
        NSString *userId = self.userTF.text;
        NSString *alias = self.aliasTF.text;
        
        [[JXManager defaultManage] loginWithUserId:userId alias:alias];

        JXListViewController *listVC = [[JXListViewController alloc] init];
        [self.navigationController pushViewController:listVC animated:YES];
        
        self.isLogin = YES;
        sender.selected = YES;
    }
    else {
        
        [[JXManager defaultManage] logout];
        
        self.isLogin = NO;
        sender.selected = NO;
    }
}




@end
