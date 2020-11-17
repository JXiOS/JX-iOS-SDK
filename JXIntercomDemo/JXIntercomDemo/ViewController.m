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
@property (weak, nonatomic) IBOutlet UITextField *systemIdTF;
@property (weak, nonatomic) IBOutlet UITextField *nameTF;
@property (weak, nonatomic) IBOutlet UITextField *snTF;
@property (weak, nonatomic) IBOutlet UITextField *buttonKeyTF;

@property (nonatomic, assign) BOOL isLogin;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.isLogin = NO;
    
    self.versionLabel.text = [NSString stringWithFormat:@"SDK Version: %@", [JXManager version]];
    
//    self.systemIdTF.text = @"xxx";
//    self.nameTF.text = @"xxx";
//    self.snTF.text = @"xxx";
//    self.buttonKeyTF.text = @"xxx";
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [self.view endEditing:YES];
}



- (IBAction)loginAction:(UIButton *)sender {
    
    if (self.isLogin == NO) {
        NSString *systemId = self.systemIdTF.text;
        NSString *myName = self.nameTF.text;
        NSString *mySn = self.snTF.text;
        NSString *buttonKey = self.buttonKeyTF.text;
        
        [[JXManager defaultManage] loginWithSystemId:systemId buttonKey:buttonKey myName:myName mySn:mySn];

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
