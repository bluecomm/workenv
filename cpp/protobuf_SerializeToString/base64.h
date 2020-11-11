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
 * @brief Ԥ��Decode��Ҫ��buffer��С.
 *        ������Decode�õ��ĳ�����Base64Decode()����.
 *
 * @param [in] bufcoded : const char*
 * @return  int Decode��Ҫ��buffer��С,��λByte
 * @retval
 * @see
 * @note
 * @author liangjianqun
 * @date 2013/12/23 16:20:43
**/
int Base64DecodeLen(const char *bufcoded);

/**
 * @brief ��Base64�����ݽ���
 *
 * @param [out] bufplain : char*        ��������
 * @param [in] bufcoded : const char*   ���������
 * @return  int Decode�õ��ĳ���
 * @retval
 * @see
 * @note
 * @author liangjianqun
 * @date 2013/12/23 16:22:31
**/
int Base64Decode(char *bufplain, const char *bufcoded);

/**
 * @brief Ԥ��Encode��Ҫ��buffer��С.
 *
 * @param [in] len : int    ��ҪEncode�ĳ���,��λByte.������Encode������Base64Encode()����.
 * @return  int             Encode len���ȵ�������Ҫ��buffer��С,��λByte
 * @retval
 * @see
 * @note
 * @author liangjianqun
 * @date 2013/12/23 16:24:54
**/
int Base64EncodeLen(int len);

/**
 * @brief ��Base64����
 *
 * @param [out] encoded : char*     Base64������
 * @param [in] string : const char* �����������
 * @param [in] len : int            ����������ݳ���
 * @return  int                     ��������������
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
