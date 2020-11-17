#ifndef SDK_INI_PARSER_H_
#define SDK_INI_PARSER_H_

#import <Foundation/Foundation.h>

/**
 * 解析 windows 格式的 INI 文件到字典中
 * [section1]
 * key1=value1
 * [section2]
 * key2=value2
 */

@interface INIParser : NSObject

@property(nonatomic, strong)
    NSDictionary<NSString *, NSDictionary *> *dictionary;

- (id)initWithUTF8String:(NSString *)content;

/**
 *返回一个节点的所有键值对的结合
 */
- (NSDictionary *)getSection:(NSString *)name;

/**
 * 字典输出为INI格式字符串
 */
- (NSString*)toString;

@end

#endif //SDK_INI_PARSER_H_
