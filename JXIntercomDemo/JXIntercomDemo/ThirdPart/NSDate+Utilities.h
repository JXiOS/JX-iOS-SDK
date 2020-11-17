//
//  NSDate+Utilities.h
//  iOS-Categories (https://github.com/shaojiankui/iOS-Categories)
//
//  Created by Jakey on 14/12/30.
//  Copyright (c) 2014年 www.skyfox.org. All rights reserved.
//

#import <Foundation/Foundation.h>
#define D_MINUTE	60
#define D_HOUR	3600
#define D_DAY	86400
#define D_WEEK	604800
#define D_YEAR	31556926
@interface NSDate (Utilities)

// 0
+ (NSCalendar *) currentCalendar; // avoid bottlenecks

// 1 : 明天
+ (NSDate *) dateTomorrow;
// 2 : 昨天
+ (NSDate *) dateYesterday;
// 3 : 几天之后
+ (NSDate *) dateWithDaysFromNow: (NSInteger) days;
// 4 : 几天之前
+ (NSDate *) dateWithDaysBeforeNow: (NSInteger) days;
// 5 : 几个小时之后
+ (NSDate *) dateWithHoursFromNow: (NSInteger) dHours;
// 6 : 几个小时之前
+ (NSDate *) dateWithHoursBeforeNow: (NSInteger) dHours;
// 7 : xx分钟之后
+ (NSDate *) dateWithMinutesFromNow: (NSInteger) dMinutes;
// 8 : xx分钟之前
+ (NSDate *) dateWithMinutesBeforeNow: (NSInteger) dMinutes;


// 9
/*
 中文如下:
 NSDateFormatterNoStyle     = 无输出
 NSDateFormatterShortStyle  = 2019/5/24 上午10:23
 NSDateFormatterMediumStyle = 2019年5月24日 上午10:23:51
 NSDateFormatterLongStyle   = 2019年5月24日 GMT+8 上午10:23:51
 NSDateFormatterFullStyle   = 2019年5月24日 星期五 中国标准时间 上午10:23:51
 */
- (NSString *)stringWithDateStyle:(NSDateFormatterStyle)dateStyle timeStyle:(NSDateFormatterStyle)timeStyle;

// 10 : 格式化
/*
 @"y" = 2019
 @"yy" = 19
 @"yyy" = 2019
 @"yyyy" = 2019
 @"M" = 5
 @"MM" = 05
 @"MMM" = 5月
 @"MMMM" = 五月
 @"dd" = 24     单个 d 前面不补 0
 @"hh" = 10     12 小时制
 @"aa" = 上午
 @"HH" = 10     24 小时制
 @"mm" = 23
 @"ss" = 45
 
 @"EEE" = 周五
 @"EEEE" = 星期五
 
 @"zzz" = GMT+8
 @"G" = 公元
 */
- (NSString *)stringWithFormat:(NSString *)format;

// 11 : NSDateFormatterShortStyle
@property (nonatomic, readonly) NSString *shortString;
// 12 : NSDateFormatterShortStyle 2019/5/24
@property (nonatomic, readonly) NSString *shortDateString;
// 13 : NSDateFormatterShortStyle 上午10:23
@property (nonatomic, readonly) NSString *shortTimeString;
// 14 : 2019年5月24日 上午10:23:51
@property (nonatomic, readonly) NSString *mediumString;
// 15
@property (nonatomic, readonly) NSString *mediumDateString;
// 16
@property (nonatomic, readonly) NSString *mediumTimeString;
// 17 : 2019年5月24日 GMT+8 上午10:23:51
@property (nonatomic, readonly) NSString *longString;
// 18
@property (nonatomic, readonly) NSString *longDateString;
// 19
@property (nonatomic, readonly) NSString *longTimeString;


// 20 : 是否同一天
// Comparing dates
- (BOOL) isEqualToDateIgnoringTime: (NSDate *) aDate;

// 21 : 是否是今天
- (BOOL) isToday;
// 22 : 是否是明天
- (BOOL) isTomorrow;
// 23 : 是否是昨天
- (BOOL) isYesterday;
// 24 : 是否是前天
- (BOOL) isTheDayBeforeYesterday;

// 25 : 和 aDate 是否同一周
- (BOOL) isSameWeekAsDate: (NSDate *) aDate;
// 26 : 是否是这周
- (BOOL) isThisWeek;
// 27 : 是否是下周
- (BOOL) isNextWeek;
// 28 : 是否是上周
- (BOOL) isLastWeek;

// 29 : 是否是同一月
- (BOOL) isSameMonthAsDate: (NSDate *) aDate;
// 30 : 是否是这个月
- (BOOL) isThisMonth;
// 31 : 是否是下个月
- (BOOL) isNextMonth;
// 32 : 是否是上个月
- (BOOL) isLastMonth;

// 33 : 是否是同一年
- (BOOL) isSameYearAsDate: (NSDate *) aDate;
// 34 : 是否是今年
- (BOOL) isThisYear;
// 35 : 是否是明年
- (BOOL) isNextYear;
// 36 : 是否是去年
- (BOOL) isLastYear;

// 37 : 是否在 aDate 之前
- (BOOL) isEarlierThanDate: (NSDate *) aDate;
// 38 : 是否在 aDate 之后
- (BOOL) isLaterThanDate: (NSDate *) aDate;

// 39 : 是否是未来
- (BOOL) isInFuture;
// 40 : 是否是过去
- (BOOL) isInPast;

// 41 : 是否是工作日
// Date roles
- (BOOL) isTypicallyWorkday;
// 42 : 是否是周末
- (BOOL) isTypicallyWeekend;


// 43 : 加 xx 年
- (NSDate *) dateByAddingYears: (NSInteger) dYears;
// 44 : 减 xx 年
- (NSDate *) dateBySubtractingYears: (NSInteger) dYears;
// 45 : 加 xx 月
- (NSDate *) dateByAddingMonths: (NSInteger) dMonths;
// 46 : 减 xx 月
- (NSDate *) dateBySubtractingMonths: (NSInteger) dMonths;
// 47 : 加 xx 天
- (NSDate *) dateByAddingDays: (NSInteger) dDays;
// 48 : 减 xx 天
- (NSDate *) dateBySubtractingDays: (NSInteger) dDays;
// 49 : 加 xx 小时
- (NSDate *) dateByAddingHours: (NSInteger) dHours;
// 50 : 减 xx 小时
- (NSDate *) dateBySubtractingHours: (NSInteger) dHours;
// 51 : 加 xx 分钟
- (NSDate *) dateByAddingMinutes: (NSInteger) dMinutes;
// 52 : 减 xx 分钟
- (NSDate *) dateBySubtractingMinutes: (NSInteger) dMinutes;

// 53 : 这一天的开始时间
- (NSDate *) dateAtStartOfDay;
// 54 : 这一天的结束时间
- (NSDate *) dateAtEndOfDay;

// 55 : 在 xx 之后多少分钟
- (NSInteger) minutesAfterDate: (NSDate *) aDate;
// 56
- (NSInteger) minutesBeforeDate: (NSDate *) aDate;
// 57
- (NSInteger) hoursAfterDate: (NSDate *) aDate;
// 58
- (NSInteger) hoursBeforeDate: (NSDate *) aDate;
// 59
- (NSInteger) daysAfterDate: (NSDate *) aDate;
// 60
- (NSInteger) daysBeforeDate: (NSDate *) aDate;
// 61
- (NSInteger) distanceInDaysToDate:(NSDate *)anotherDate;

// 62 : 下个小时
@property (readonly) NSInteger nearestHour;
// 63 : 当前小时
@property (readonly) NSInteger hour;
// 64
@property (readonly) NSInteger minute;
// 65
@property (readonly) NSInteger seconds;
// 66
@property (readonly) NSInteger day;
// 67
@property (readonly) NSInteger month;
// 68 : 当年的第几周
@property (readonly) NSInteger weekOfYear;
// 69 : 周几, 周日是 1, 周一是 2
@property (readonly) NSInteger weekday;
// 70 : 这个月的第几个星期几, 例如: 2019-08-15 是 2019 年 8 月的第三个周四
@property (readonly) NSInteger nthWeekday; // e.g. 2nd Tuesday of the month == 2
// 71 : 年份
@property (readonly) NSInteger year;
// 72 : 是否是闰月
@property (readonly) BOOL leapMonth;
// 73 : 当月的第几周
@property (readonly) NSInteger weekOfMonth;

// 74 : 10 位时间戳 (1970 -> 秒)
+ (NSString *)second_timestamp;
// 75 : 13 位时间戳 (1970 -> 毫秒)
+ (NSString *)millisecond_timestamp;


// 76 : 加 xx 秒, xx 可以为负数
- (NSDate *)dateByAddingSeconds:(NSInteger)seconds;

/// 77 : ISO 8601 格式的时间 yyyy-MM-dd'T'HH:mm:ss'Z'
/// 2020-06-16T00:00:00Z
- (NSString *)iso8601TimeString;

/// 78 : 当天的零点时间 2020-06-16 00:00:00
- (NSDate *)theDayBeginDate;
/// 79 : 当天的结束时间 2020-06-16 23:59:59
- (NSDate *)theDayEndDate;

/// 80 : 通过指定的 年-月-日-时-分-秒 获得 NSDate 格式的时间, 如果给的值不是合理的值,会自动换算, 比如给了 day=32, 那么月份会相应加+1
+ (NSDate *)getDateWithYear:(NSUInteger)year month:(NSUInteger)month day:(NSUInteger)day hour:(NSUInteger)hour minute:(NSUInteger)minute second:(NSUInteger)second;


@end
