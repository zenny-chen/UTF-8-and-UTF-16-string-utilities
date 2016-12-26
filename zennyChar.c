//
//  zennyChar.c
//  UTF_trans
//
//  Created by Zenny Chen on 2016/12/26.
//  Copyright © 2016年 Adwo. All rights reserved.
//

#include "zennyChar.h"

bool ZennyUTF8ToUTF16(uint16_t pUTF16[], const char *pUTF8, size_t *pUTF16Length)
{
    if(pUTF16 == NULL || pUTF8 == NULL)
        return false;
    
    size_t orgIndex = 0, dstIndex = 0;
    char ch;
    
    while((ch = pUTF8[orgIndex]) != '\0')
    {
        uint32_t result = 0;
        int length = 0;
        
        // counting leading '1' for number of UTF-8 bytes
        uint32_t firstByteFlag = (uint32_t)ch & 0xfc;
        while((firstByteFlag & 0x80) != 0)
        {
            firstByteFlag <<= 1;
            length++;
        }
        
        // 若长度为0，则直接为兼容的ASCII码
        if(length == 0)
        {
            pUTF16[dstIndex++] = ch;
            orgIndex++;
            continue;
        }
        
        // 对于macOS系统，采用的是大端的Unicode
        
        // 先拼接第一个字节的剩余有效比特
        result = (uint32_t)ch & (0xffU >> (length + 1));
        
        // 对于UTF-8字节数小于4的，说明是基本多语言平面（BMP），对应于Unicode一定为两个字节
        // 对于大于3字节数的UTF-8则被扩充到21位的Unicode
        int base;
        switch(length)
        {
            case 2:
                base = 11;
                break;
                
            case 3:
                base = 16;
                break;
                
            case 4:
            default:
                base = 21;
                break;
        }
        
        int shiftedBitPosition = base - (8 - (length + 1));
        
        result <<= shiftedBitPosition;
        
        int i = 1;
        
        // 再拼接其余字节的比特位
        do
        {
            i++;
            
            shiftedBitPosition -= 6;
            
            result |= ((uint32_t)pUTF8[++orgIndex] & 0x3f) << shiftedBitPosition;
        }
        while(i < length);
        
        // 对于UTF-8字节数小于4的，说明是基本多语言平面（BMP），对应于Unicode一定为两个字节
        if(length < 4)
            pUTF16[dstIndex++] = result;
        else
        {
            // 对于大于3字节数的UTF-8，则采用高低交替对码点格式
            result -= 0x00010000;
            uint16_t high = result >> 10;
            uint16_t low = result - (high << 10);
            pUTF16[dstIndex++] = high + 0xd800;
            pUTF16[dstIndex++] = low + 0xdc00;
        }
        
        orgIndex++;
    }
    
    pUTF16[dstIndex] = u'\0';
    
    if(pUTF16Length != NULL)
        *pUTF16Length = dstIndex;
    
    return true;
}

size_t ZennyGetUTF16LengthFromUTF8(const char *utf8Str)
{
    if(utf8Str == NULL)
        return 0;
    
    size_t orgIndex = 0, dstLength = 0;
    char ch;
    
    while((ch = utf8Str[orgIndex]) != '\0')
    {
        int length = 0;
        
        // 对UTF-8字节序列计算前导1的个数，有多少个前导1说明该字符由多少个字节构成
        uint32_t firstByteFlag = (uint32_t)ch & 0xfc;
        while((firstByteFlag & 0x80) != 0)
        {
            firstByteFlag <<= 1;
            length++;
        }
        
        if(length == 0)
            length = 1;
        
        orgIndex += length;
        
        int addition = length > 3? 2 : 1;
        dstLength += addition;
    }
    
    return dstLength;
}

bool ZennyUTF16ToUTF8(char pUTF8[], const uint16_t *pUTF16, size_t *pUTF8Length)
{
    if(pUTF8 == NULL || pUTF16 == NULL)
        return false;
    
    size_t orgIndex = 0, dstIndex = 0;
    uint16_t ch;
    
    while((ch = pUTF16[orgIndex]) != u'\0')
    {
        // 处理ASCII码兼容情况
        if(ch < 0x80)
        {
            pUTF8[dstIndex++] = ch;
            orgIndex++;
            continue;
        }
        
        // 处理16位Unicode的情况（最多3字节UTF-8）
        if(ch < 0xd800 || ch >= 0xe000)
        {
            if((ch & 0xf800) == 0)
            {
                // 高5位为0，说明是2字节UTF-8
                uint8_t value = (ch >> 6) | 0xc0;
                pUTF8[dstIndex++] = value;
                
                value = (ch & 0x3f) | 0x80;
                pUTF8[dstIndex++] = value;
            }
            else
            {
                // 否则为3字节UTF-8
                uint8_t value = (ch >> 12) | 0xe0;
                pUTF8[dstIndex++] = value;
                
                value = (ch >> 6) & 0x3f;
                value |= 0x80;
                pUTF8[dstIndex++] = value;
                
                value = ch & 0x3f;
                value |= 0x80;
                pUTF8[dstIndex++] = value;
            }
        }
        else
        {
            // 处理21位Unicode的情况
            uint32_t high = ch - 0xd800;
            uint32_t low = pUTF16[++orgIndex] - 0xdc00;
            uint32_t result = low + (high << 10);
            result += 0x00010000;
            
            uint8_t value = (result >> 18) | 0xf0;
            pUTF8[dstIndex++] = value;
            
            value = (result >> 12) & 0x3f;
            value |= 0x80;
            pUTF8[dstIndex++] = value;
            
            value = (result >> 6) & 0x3f;
            value |= 0x80;
            pUTF8[dstIndex++] = value;
            
            value = (result & 0x3f) | 0x80;
            pUTF8[dstIndex++] = value;
        }
        
        orgIndex++;
    }
    
    pUTF8[dstIndex] = '\0';
    
    if(pUTF8Length != NULL)
        *pUTF8Length = dstIndex;
    
    return true;
}

size_t ZennyGetUTF8LengthFromUTF16(const uint16_t *utf16Str)
{
    if(utf16Str == NULL)
        return 0;
    
    size_t orgIndex = 0, dstLength = 0;
    uint16_t ch;
    
    while((ch = utf16Str[orgIndex]) != u'\0')
    {
        // 处理ASCII码兼容情况
        if(ch < 0x80)
        {
            dstLength++;
            orgIndex++;
            continue;
        }
        
        // 处理16位Unicode的情况（最多3字节UTF-8）
        if(ch < 0xd800 || ch >= 0xe000)
        {
            if((ch & 0xf800) == 0)
            {
                // 高5位为0，说明是2字节UTF-8
                dstLength += 2;
            }
            else
            {
                dstLength += 3;
            }
        }
        else
        {
            // 否则为4字节UTF-8
            dstLength += 4;
            orgIndex++;
        }
        
        orgIndex++;
    }
    
    return dstLength;
}

size_t ZennyUTF16StrLen(const uint16_t *s)
{
    if(s == NULL)
        return 0;
    
    size_t index;
    
    for(index = 0; s[index] != u'\0'; index++);
    
    return index;
}

const uint16_t* ZennyUTF16StrCat(uint16_t dst[], const uint16_t src[])
{
    if(dst == NULL || src == NULL)
        return NULL;
    
    size_t index = ZennyUTF16StrLen(dst);
    
    for(size_t i = 0; src[i] != u'\0'; i++)
        dst[index++] = src[i];
    
    dst[index] = u'\0';
    
    return dst;
}

