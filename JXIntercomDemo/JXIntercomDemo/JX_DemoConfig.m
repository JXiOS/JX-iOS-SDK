//
//  JX_DemoConfig.m
//  JXIntercomDemo
//
//  Created by Nansen on 2021/2/6.
//  Copyright © 2021 jingxi. All rights reserved.
//

#warning TODO : 配置相关参数

#import "JX_DemoConfig.h"

@implementation JX_DemoConfig

+ (NSString *)sipURL
{
    ///
    return @"<#sip服务器地址#>";
}

+ (NSString *)transitURL
{
    ///
    return @"<#中转服务器地址#>";
}

+ (NSString *)channel
{
    return @"<#客户标识#>";
}


+ (NSString *)appid
{
    return @"<#注册的 AppId#>";
}

+ (NSString *)appkey
{
    return @"<#注册的 AppKey#>";
}

+ (NSString *)userId
{
    /// userId
    return @"";
}

+ (NSString *)alias
{
    /// alias
    return @"";
}

+ (NSString *)activeCode
{
    /// activeCode
    return @"";
}


+ (NSString *)homeId
{
    /// homeId
    return @"";
}

+ (NSString *)callNumber
{
    /// 户户通呼叫号
    return @"";
}

@end
