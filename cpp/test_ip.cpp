#include <arpa/inet.h>  
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <string>

// struct sockaddr {
//       unsigned short sa_family;     /* address family, AF_xxx */
//       char sa_data[14];                  /* 14 bytes of protocol address */
// };
// struct sockaddr_in {
//       short int sin_family;                /* Address family AF_INET */
//       unsigned short int sin_port;    /* Port number */
//       struct in_addr sin_addr;         /* Internet address */
//       unsigned char sin_zero[8];     /* Same size as struct sockaddr */
// };
// struct in_addr {
//       unsigned long s_addr;           /* Internet address */
// };
// struct sockaddr_in6 {
//       sa_family_t sin6_family;         /* AF_INET6 */
//       in_port_t sin6_port;               /* transport layer port # */
//       uint32_t sin6_flowinfo;           /* IPv6 traffic class & flow info */
//       struct in6_addr sin6_addr;    /* IPv6 address */
//       uint32_t sin6_scope_id;        /* set of interfaces for a scope */
// };
// struct in6_addr {
//       uint8_t s6_addr[16];            /* IPv6 address */
// };
// struct addrinfo{
//       int ai_flags;                         /* AI_PASSIVE,AI_CANONNAME,AI_NUMERICHOST */
//       int ai_family;                        /* AF_INET,AF_INET6 */
//       int ai_socktype;                   /* SOCK_STREAM,SOCK_DGRAM */
//       int ai_protocol;                   /* IPPROTO_IP, IPPROTO_IPV4, IPPROTO_IPV6 */
//       size_t ai_addrlen;               /* Length */
//       char *ai_cannoname;         /* */
//       struct sockaddr *ai_addr;   /* struct sockaddr */
//       struct addrinfo *ai_next;      /* pNext */
// }

uint64_t htonll(uint64_t val) {
    return (((uint64_t) htonl(val)) << 32) + htonl(val >> 32);
}

uint64_t ntohll(uint64_t val) {
    return (((uint64_t) ntohl(val)) << 32) + ntohl(val >> 32);
}

bool trans_ipstr_to_value(const std::string& ip_str, uint64_t& ip_value) {
    struct in_addr addr;
    bzero(&addr, sizeof(addr));
    if (inet_pton(AF_INET, ip_str.c_str(), &addr) <= 0) {
        std::cout << "trans_ipstr_to_value failed: " <<  ip_str << std::endl;
        return false;
    }
    ip_value = (uint64_t)ntohl(addr.s_addr);
    return true;
}

bool trans_ip6str_to_value(const std::string& ip6_str,
        uint64_t& ip_value_high, uint64_t& ip_value_low) {
    struct in6_addr addr;
    bzero(&addr, sizeof(addr));
    if (inet_pton(AF_INET6, ip6_str.c_str(), &addr) == 0) {
        std::cout << "trans_ip6str_to_value failed: " <<  ip6_str << std::endl;
        return false;
    }
    ip_value_high = ntohll(*((uint64_t*)addr.s6_addr));
    ip_value_low = ntohll(*((uint64_t*)(addr.s6_addr + 8)));
    return true;
}

int main () {
    uint64_t ip_value = 0;
    uint64_t ip_value_high = 0;
    uint64_t ip_value_low = 0;

    std::string ipv4_addrs[] = {
        "10.0.0.1",
        "8.8.8.8",
        "0.0.0.1",
        "0.0.0.255",
        "0.0.0.256",
        "0.0.0.0"
    };
    for (size_t i = 0; i < 6U; ++i) {
        if (trans_ipstr_to_value(ipv4_addrs[i], ip_value)) {
            std::cout << ipv4_addrs[i] << " -> " << ip_value << std::endl;
        }
    }

    std::cout << std::endl;

    std::string ipv6_addrs[] = {
        "2001:0DB8:0000:0000:0000:0000:1428:57ab",
        "2001:0DB8:0000:0000:0000::1428:57ab",
        "2001:0DB8:0:0:0:0:1428:57ab",
        "2001:0DB8:0::0:1428:57ab",
        "2001:0DB8::1428:57ab",
        "2001:0DB8:02de::0e13",
        "2001:DB8:2de::e13", // "2001:DB8:2de::e13" == "2001:0DB8:02de::0e13",
        "0000:0000:0000:0000:0000:0000:874B:2B34", // "135.75.43.52"
        "::874B:2B34", // "135.75.43.52"
        "::135.75.43.52", // "135.75.43.52"
        "::",
        "2001::25de::cade" // invalid
    };
    for (size_t i = 0; i < 12U; ++i) {
        if (trans_ip6str_to_value(ipv6_addrs[i], ip_value_high, ip_value_low)) {
            std::cout << ipv6_addrs[i] << " -> " << ip_value_high << " " << ip_value_low << std::endl;
        }
    }
    return 0;
}
