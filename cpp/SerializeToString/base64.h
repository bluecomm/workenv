/***************************************************************************
 *
 * Copyright (c) 2013 Baidu.com, Inc. All Rights Reserved
 * $Id$
 *
 **************************************************************************/
 
 
 
/**
 * @file base64.h
 * @author liangjianqun(liangjianqun@baidu.com)
 * @date 2013/11/06 13:54:42
 * @version 1.0
 * @brief
 *
 **/


#ifndef  __BASE64_H_
#define  __BASE64_H_


/**
 * @brief 预估Decode需要的buffer大小.
 *        但真正Decode得到的长度由Base64Decode()返回.
 *
 * @param [in] bufcoded : const char*
 * @return  int Decode需要的buffer大小,单位Byte
 * @retval
 * @see
 * @note
 * @author liangjianqun
 * @date 2013/12/23 16:20:43
**/
int Base64DecodeLen(const char *bufcoded);

/**
 * @brief 将Base64的内容解码
 *
 * @param [out] bufplain : char*        解码的输出
 * @param [in] bufcoded : const char*   解码的输入
 * @return  int Decode得到的长度
 * @retval
 * @see
 * @note
 * @author liangjianqun
 * @date 2013/12/23 16:22:31
**/
int Base64Decode(char *bufplain, const char *bufcoded);

/**
 * @brief 预估Encode需要的buffer大小.
 *
 * @param [in] len : int    需要Encode的长度,单位Byte.真正的Encode长度由Base64Encode()返回.
 * @return  int             Encode len长度的内容需要的buffer大小,单位Byte
 * @retval
 * @see
 * @note
 * @author liangjianqun
 * @date 2013/12/23 16:24:54
**/
int Base64EncodeLen(int len);

/**
 * @brief 做Base64编码
 *
 * @param [out] encoded : char*     Base64编码结果
 * @param [in] string : const char* 待编码的内容
 * @param [in] len : int            待编码的数据长度
 * @return  int                     编码后的真正长度
 * @retval
 * @see
 * @note
 * @author liangjianqun
 * @date 2013/12/23 16:26:33
**/
int Base64Encode(char *encoded, const char *string, int len);




#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>
#include <boost/range/iterator.hpp>
#include <boost/algorithm/string.hpp>
//#include <boost/foreach.hpp>
#include <iostream>
#include <string>
#include <string.h>

int base64_encode(const std::string& str, std::string &result); 
int base64_decode(const std::string& str, std::string &result); 

int encode(const std::string& str, std::string &result); 
int decode(const std::string& str, std::string &result); 

#endif  //__BASE64_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 : */
