//
//  JXHistoryTableViewCell.m
//  JXIntercomDemo
//
//  Created by Nansen on 2020/4/30.
//  Copyright © 2020 jingxi. All rights reserved.
//

#import "JXHistoryTableViewCell.h"
#import "Masonry.h"
#import <JXIntercomSDK/JXIntercomSDK.h>

@interface JXHistoryTableViewCell ()

@property (nonatomic, strong) UIImageView *bgImageView;

@property (nonatomic, strong) UILabel *timeLabel;

@end


@implementation JXHistoryTableViewCell

- (void)awakeFromNib {
    [super awakeFromNib];
    // Initialization code
}

- (instancetype)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier]) {
        self.selectionStyle = UITableViewCellSelectionStyleNone;
        
        self.bgImageView = [[UIImageView alloc] init];
        self.bgImageView.backgroundColor = [UIColor blackColor];
        self.bgImageView.contentMode = UIViewContentModeScaleAspectFit;
        [self.contentView addSubview:self.bgImageView];
        [self.bgImageView mas_makeConstraints:^(MASConstraintMaker *make) {
            make.left.right.mas_equalTo(@0);
            make.top.mas_equalTo(@10);
            make.bottom.mas_equalTo(@-10);
        }];
        
        
        self.timeLabel = [[UILabel alloc] init];
        self.timeLabel.font = [UIFont boldSystemFontOfSize:14];
        self.timeLabel.textAlignment = NSTextAlignmentLeft;
        self.timeLabel.textColor = [UIColor whiteColor];
        [self.contentView addSubview:self.timeLabel];
        [self.timeLabel mas_makeConstraints:^(MASConstraintMaker *make) {
            make.left.mas_equalTo(@15);
            make.top.mas_equalTo(@20);
            make.right.mas_equalTo(@-15);
        }];
    }
    return self;
}


- (void)setVideoRecord:(JXVideoRecordModel *)videoRecord
{
    _videoRecord = videoRecord;
    
    NSString *imagePath = [videoRecord fullImagePath];
    if (imagePath != nil && [[NSFileManager defaultManager] fileExistsAtPath:imagePath]) {
        self.bgImageView.image = [UIImage imageWithContentsOfFile:imagePath];
    }
    else {
        self.bgImageView.image = nil;
    }
    
    self.timeLabel.text = [videoRecord hourAndMinute];
}


- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

@end
