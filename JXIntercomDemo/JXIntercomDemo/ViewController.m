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
#import "JX_DemoConfig.h"

@import AVFoundation;


@interface ViewController ()

@property (weak, nonatomic) IBOutlet UILabel *versionLabel;

@property (weak, nonatomic) IBOutlet UITextField *userTF;

@property (weak, nonatomic) IBOutlet UITextField *aliasTF;
@property (weak, nonatomic) IBOutlet UITextField *codeTF;



@property (nonatomic, assign) BOOL isLogin;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.isLogin = NO;
    
    self.versionLabel.text = [NSString stringWithFormat:@"SDK Version: %@", [JXManager version]];
    
    self.userTF.text = [JX_DemoConfig userId];
    self.aliasTF.text = [JX_DemoConfig alias];
    self.codeTF.text = [JX_DemoConfig activeCode];
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [self.view endEditing:YES];
}



- (IBAction)loginAction:(UIButton *)sender {
    
    if (self.isLogin == NO) {
        NSString *userId = self.userTF.text;
        NSString *alias = self.aliasTF.text;
        NSString *code = self.codeTF.text;
        
        [[JXManager defaultManage] loginWithUserId:userId alias:alias activecode:code complete:^(BOOL succeed) {
            if (succeed) {
                JXListViewController *listVC = [[JXListViewController alloc] init];
                [self.navigationController pushViewController:listVC animated:YES];
                
                self.isLogin = YES;
                sender.selected = YES;
            }
            else {
                NSLog(@"SDK 配置出错!");
            }
        }];
    }
    else {
        
        [[JXManager defaultManage] logout];
        
        self.isLogin = NO;
        sender.selected = NO;
    }
}




@end
