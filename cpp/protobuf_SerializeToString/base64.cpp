/***************************************************************************
 *
 * Copyright (c) 2013 Baidu.com, Inc. All Rights Reserved
 * $Id$
 *
 **************************************************************************/



/**
 * @file base64.cpp
 * @author liangjianqun(liangjianqun@baidu.com)
 * @date 2013/11/06 13:55:49
 * @version 1.0
 * @brief
 *
 **/

#include "base64.h"


/* aaaack but it's fast and const should make it shared text page. */
static const unsigned char pr2six[256] = {
    /* ASCII table */
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
    64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
    64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};

int Base64DecodeLen(const char *bufcoded)
{
    int nbytesdecoded;
    register const unsigned char *bufin;
    register int nprbytes;

    bufin = (const unsigned char *) bufcoded;
    while (pr2six[*(bufin++)] <= 63);

    nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;
    nbytesdecoded = ((nprbytes + 3) / 4) * 3;

    return nbytesdecoded + 1;
}

int Base64Decode(char *bufplain, const char *bufcoded)
{
    int nbytesdecoded;
    register const unsigned char *bufin;
    register unsigned char *bufout;
    register int nprbytes;

    bufin = (const unsigned char *) bufcoded;
    while (pr2six[*(bufin++)] <= 63);
    nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;
    nbytesdecoded = ((nprbytes + 3) / 4) * 3;

    bufout = (unsigned char *) bufplain;
    bufin = (const unsigned char *) bufcoded;

    while (nprbytes > 4) {
        *(bufout++) =
            (unsigned char)(pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
        *(bufout++) =
            (unsigned char)(pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
        *(bufout++) =
            (unsigned char)(pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
        bufin += 4;
        nprbytes -= 4;
    }

    /* Note: (nprbytes == 1) would be an error, so just
     * ingore that case */
    if (nprbytes > 1) {
        *(bufout++) =
            (unsigned char)(pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
    }
    if (nprbytes > 2) {
        *(bufout++) =
            (unsigned char)(pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
    }
    if (nprbytes > 3) {
        *(bufout++) =
            (unsigned char)(pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
    }

    *(bufout++) = '\0';
    nbytesdecoded -= (4 - nprbytes) & 3;
    return nbytesdecoded;
}

static const char basis_64[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int Base64EncodeLen(int len)
{
    return ((len + 2) / 3 * 4) + 1;
}

int Base64Encode(char *encoded, const char *string, int len)
{
    int i;
    char *p;

    p = encoded;
    for (i = 0; i < len - 2; i += 3) {
        *p++ = basis_64[(string[i] >> 2) & 0x3F];
        *p++ = basis_64[((string[i] & 0x3) << 4) |
                ((int)(string[i + 1] & 0xF0) >> 4)];
        *p++ = basis_64[((string[i + 1] & 0xF) << 2) |
                ((int)(string[i + 2] & 0xC0) >> 6)];
        *p++ = basis_64[string[i + 2] & 0x3F];
    }
    if (i < len) {
        *p++ = basis_64[(string[i] >> 2) & 0x3F];
        if (i == (len - 1)) {
            *p++ = basis_64[((string[i] & 0x3) << 4)];
            *p++ = '=';
        } else {
            *p++ = basis_64[((string[i] & 0x3) << 4) |
                    ((int)(string[i + 1] & 0xF0) >> 4)];
            *p++ = basis_64[((string[i + 1] & 0xF) << 2)];
        }
        *p++ = '=';
    }

    *p++ = '\0';
    return p - encoded;
}


int32_t encode(const std::string& src, std::string& dst)
{
    int encode_len = Base64EncodeLen(src.length());
    char* encode_buf = new(std::nothrow) char[encode_len];
    if (encode_buf == NULL) {
        return -1;
    }
    memset(encode_buf, 0, encode_len);
    Base64Encode(encode_buf, const_cast<char*>(src.data()), src.length());
    dst.assign(encode_buf);
    delete[] encode_buf;
    return 0;
}

int32_t decode(const std::string& src, std::string& dst)
{
    int decode_len = Base64DecodeLen(src.c_str());
    char* decode_buf = new(std::nothrow) char[decode_len];
    if (decode_buf == NULL) {
        return -1;
    }
    memset(decode_buf, 0, decode_len);
    int decoded_len = Base64Decode(decode_buf, src.c_str());
    dst.assign(decode_buf);
    delete[] decode_buf;

    std::cout << "decoded_len: " << decoded_len << " decode_len: " << decode_len << std::endl;
    return 0;
}


// ********************************************************

//char* BAIDU_BASE64_ALPHABET =    "0KajD7AZcF2QnPr5fwiHRNygmupUTIXx69BWb-hMCGJo_V8Eskz1YdvL34letqSO";
//char* STANDARD_BASE64_ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
char BASE64_PADDLE_CHAR = '=';

int base64_encode(const std::string& str, std::string &result)
{
    //if (result == NULL) {
    //    return -1;
    //}

    typedef boost::archive::iterators::base64_from_binary <
    boost::archive::iterators::transform_width <
    std::string::const_iterator, 6, 8 > > it_base64_t;

    try {
        // Base64 Encode
        uint32_t paddleChars = (3 - str.length() % 3) % 3;
        result.assign(it_base64_t(str.begin()), it_base64_t(str.end()));
        result.append(paddleChars, BASE64_PADDLE_CHAR);
    } catch (...) {
        return -1;
    }
    return 0;
}


int base64_decode(const std::string& str, std::string &result)
{
    //if (result == NULL) {
    //    return -1;
    //}

    typedef boost::archive::iterators::transform_width <
    boost::archive::iterators::binary_from_base64 <
    boost::archive::iterators::remove_whitespace <
    std::string::const_iterator > > , 8, 6 > it_binary_t;

    try {
        // Base64 Decode
        uint32_t paddleChars = count(str.begin(), str.end(), BASE64_PADDLE_CHAR);
        result.assign(str);

        // replace '=' by base64 encoding of '\0'
        std::replace(result.begin(), result.end(), BASE64_PADDLE_CHAR, 'A');

        // Base64 decode
        result.assign(it_binary_t(result.begin()), it_binary_t(result.end()));

        // erase padding '\0' characters
        result.erase(result.end() - paddleChars, result.end());
    } catch (...) {
        return -1;
    }

    return 0;
}



/* vim: set ts=4 sw=4 sts=4 tw=100 : */
