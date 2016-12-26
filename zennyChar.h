//
//  zennyChar.h
//  UTF_trans
//
//  Created by Zenny Chen on 14/12/8.
//  Copyright (c) 2014年 Adwo. All rights reserved.
//

#ifndef UTF_trans_zennyChar_h
#define UTF_trans_zennyChar_h

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * 将UTF16字符串转为UTF8字符串
 * @param pUTF16 指向目的存放UTF16字符串的缓存地址
 * @param pUTF8 指向源UTF8字符串缓存首地址
 * @param pUTF16Length 指向存放目的UTF16字符串长度的变量首地址；如果转换成功，且此指针不空，那么将最终UTF16字符串的长度存放进去
 * @return 若转换成功返回true，否则返回false
*/
extern bool ZennyUTF8ToUTF16(uint16_t pUTF16[], const char *pUTF8, size_t *pUTF16Length);

/**
 * 从UTF8字符串获得相应UTF16字符串的长度
 * @param utf8Str 指向UTF8字符串首地址
 * @return 相应UTF16字符串长度
*/
extern size_t ZennyGetUTF16LengthFromUTF8(const char *utf8Str);

/**
 * 将UTF16字符串转为UTF8字符串
 * @param pUTF8 指向目的存放UTF8字符串的缓存首地址
 * @param pUTF16 指向源存放UTF16字符串的缓存首地址
 * @param pUTF8Length 指向存放目的UTF8字符串长度的变量指针；当转换成功时，若pUTF8Length不空，则将转换后的UTF8字符串的长度存放进去
 * @return 若转换成功返回true，否则返回false
*/
extern bool ZennyUTF16ToUTF8(char pUTF8[], const uint16_t *pUTF16, size_t *pUTF8Length);

/**
 * 从UTF16字符串获得相应UTF8字符串的长度
 * @param utf16Str 指向源UTF16字符串的首地址
 * @return 相应UTF8字符串的长度
*/
extern size_t ZennyGetUTF8LengthFromUTF16(const uint16_t *utf16Str);

/**
 * 获得指定UTF16字符串的长度
 * @param s UTF16字符串首地址
 * @return UTF16字符串长度
*/
extern size_t ZennyUTF16StrLen(const uint16_t *s);

/**
 * 将dst所存放的字符串内容与src所存放的字符串内容拼接（dst内容为头，src内容为尾），然后将结果存放入dst的缓存中
 * @warning dst必须有足够的存储空间来存放结果字符串内容
 * @param dst 指向目的UTF16字符串以及作为源UTF16字符串的头部
 * @param src 指向源UTF16字符串尾部内容
 * @return 如果拼接成功，指向dst；否则指向空
*/
extern const uint16_t* ZennyUTF16StrCat(uint16_t dst[], const uint16_t src[]);

#endif

