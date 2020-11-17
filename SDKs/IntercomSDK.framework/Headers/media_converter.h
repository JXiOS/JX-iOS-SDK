#ifndef SDK_MEDIA_CONVERTER_H_
#define SDK_MEDIA_CONVERTER_H_

#import <Foundation/Foundation.h>

@protocol MediaConverterDelegate<NSObject>

- (void)onMediaConvertReady:(NSDictionary *)info;

- (void)onMediaConvertCompleted:(NSInteger)result
                        outfile:(NSString*)outfile;

- (void)onMediaConvertProgress:(NSInteger)progress;

@end


@interface MediaConverter : NSObject
@property(nonatomic, weak) id<MediaConverterDelegate> delegate;

- (id)init;

- (BOOL)start:(NSString*)infile
      outfile:(NSString*)outfile
    watermark:(BOOL)watermark;

- (void)stop;


@end

#endif //SDK_MEDIA_CONVERTER_H_
