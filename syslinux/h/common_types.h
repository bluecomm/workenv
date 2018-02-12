#ifndef _SPU_TYPEDEF_H_
#define _SPU_TYPEDEF_H_

typedef signed char INT8;
typedef signed short INT16;
typedef signed int INT32;
typedef signed long long INT64;

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;

typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned int UINT;
typedef unsigned long ULONG;

typedef int BOOL;
typedef int ARGINT;
typedef void VOID;
typedef char CHAR;

typedef struct sockaddr_in ipv4_sock_addr_t;
typedef struct sockaddr_in6 ipv6_sock_addr_t;
typedef struct sockaddr sock_addr_t;

#endif
