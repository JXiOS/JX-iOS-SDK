#ifndef SDK_WEAK_ARRAY_H_
#define SDK_WEAK_ARRAY_H_

#import <Foundation/Foundation.h>

/**
 *NSPointerArray 封装，可用于一对多的代理应用场景
 */
@interface WeakReferenceArray : NSObject

@property(nonatomic, strong, readonly) NSPointerArray *delegates;

/** 添加一个代理对象，不增加引用计数 */
- (void)add:(id)object;

/** 删除一个代理对象 */
- (void)remove:(id)object;

/** 是否包含某个代理 */
- (BOOL)contains:(id)object;

/** 释放无效的代理 */
- (void)compact;

@end

#endif //SDK_WEAK_ARRAY_H_
