#ifndef SDK_RECORD_PLAYER_H_
#define SDK_RECORD_PLAYER_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@protocol RecordPlayerDelegate<NSObject>

- (void)onMediaDuration:(NSString*)session_id duration:(long long)duration;

- (void)onMediaProgress:(NSString*)session_id progress:(long long)progress;

- (void)onMediaCompleted:(NSString*)session_id reason:(NSInteger)reason;

- (void)onMediaVideoDimensionChanged:(NSString*)session_id width:(NSInteger)width height:(NSInteger)height;

@end


@interface RecordPlayer : NSObject
@property(nonatomic, weak) id<RecordPlayerDelegate> delegate;

- (id)init;

- (BOOL)start:(NSString*)media_dir
   session_id:(NSString*)session_id
     filename:(NSString*)filename;

- (void)stop;

- (void)destroy;

- (void)seek:(int)percent;

- (void)switchAudio:(BOOL)local;

- (void)enableAudioTracker:(BOOL)local enable:(BOOL)enable;

- (void)updateSurface:(CALayer *)layer;

@end

#endif //SDK_RECORD_PLAYER_H_
