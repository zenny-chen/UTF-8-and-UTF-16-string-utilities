//
//  main.m
//  UTF_trans
//
//  Created by Zenny Chen on 14/12/4.
//  Copyright (c) 2014年 Adwo. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <string.h>

#include "zennyChar.h"

int main(int argc, const char * argv[])
{
    // 我们声明一个s字符数组，用于存放一个UTF-8字符串
    const char s[] = u8"你好，世界！αβγ😁Hello, world!😄";
    
    printf("The length is: %zu, and the content is: %s\n", strlen(s), s);
    
    // 我们使用NSString对象来存放这个字符串，并且str对象引用所包含的字符编码已经转为了UTF-16
    NSString *str = [NSString stringWithUTF8String:s];
    NSLog(@"The length is: %zu, and the content is: %@", [str length], str);
    
    // 下面用我们自己实现的UTF-8转UTF-16的方法进行对比测试
    unichar buffer[64];
    size_t length;
    if(ZennyUTF8ToUTF16(buffer, s, &length))
        NSLog(@"\nThe transformed UTF16 length is: %zu, and the string is: %@", length, [NSString stringWithCharacters:buffer length:length]);
    
    NSLog(@"The UTF16 string length from UTF8 is: %zu, and the original UTF16 string length is: %zu", ZennyGetUTF16LengthFromUTF8(s), ZennyUTF16StrLen(buffer));
    
    // 我们再将UTF-16编码的字符串再转回UTF-8编码的字符串
    char chBuffer[64];
    ZennyUTF16ToUTF8(chBuffer, buffer, &length);
    printf("\nThe transformed UTF8 length is: %zu, and the string is: %s\n", length, chBuffer);
    
    length = ZennyGetUTF8LengthFromUTF16(buffer);
    printf("The UTF8 length from UTF16 is: %zu\n\n", length);
    
    // 最后我们测试一下ZennyUTF16StrCat函数
    ZennyUTF16StrCat(buffer, u"👋拜拜～");
    
    length = ZennyUTF16StrLen(buffer);
    NSLog(@"The UTF16 length is: %zu, and the content is: %@", length, [NSString stringWithCharacters:buffer length:length]);
    
    return 0;
}

