
/*-----------------------------------------------------------------------------------------------*/
/* ipaddr.h                                                                                      */
/*                                                                                               */
/* copyright (c) innovaphone 2001                                                                */
/*-----------------------------------------------------------------------------------------------*/

#ifndef _IPADDR_
#define _IPADDR_

NAMESPACE_BEGIN

typedef union {
    byte    b[16];
    word    w[8];
    dword   d[4];
    ulong64 q[2];
} IPaddr;

typedef struct {
    IPaddr    a;
    word      p;
} IPaddr_prefix;

extern const IPaddr ip_anyaddr;
extern const IPaddr ip_maskall;
extern const IPaddr ip_loopback;
extern const IPaddr_prefix zero_ip_prefix;

extern const IPaddr ip_4_anyaddr;
extern const IPaddr ip_4_maskall;
extern const IPaddr ip_4_loopback;
extern const IPaddr ip_4_loopback_net;
extern const IPaddr ip_4_class_a_mask;
extern const IPaddr ip_4_class_b_mask;
extern const IPaddr ip_4_class_c_mask;

/*-----------------------------------------------------------------------------------------------*/
/* IPaddr operations                                                                             */
/*-----------------------------------------------------------------------------------------------*/

inline void ip_read(IPaddr * a, const byte * b)
{
   	a->b[0] = b[0]; a->b[1] = b[1]; a->b[2] = b[2]; a->b[3] = b[3];
    a->b[4] = b[4]; a->b[5] = b[5]; a->b[6] = b[6]; a->b[7] = b[7];
    a->b[8] = b[8]; a->b[9] = b[9]; a->b[10] = b[10]; a->b[11] = b[11];
    a->b[12] = b[12]; a->b[13] = b[13]; a->b[14] = b[14]; a->b[15] = b[15];
}

inline void ip_write(const IPaddr * a, byte * b)
{
   	b[0] = a->b[0]; b[1] = a->b[1]; b[2] = a->b[2]; b[3] = a->b[3];
    b[4] = a->b[4]; b[5] = a->b[5]; b[6] = a->b[6]; b[7] = a->b[7];
    b[8] = a->b[8]; b[9] = a->b[9]; b[10] = a->b[10]; b[11] = a->b[11];
    b[12] = a->b[12]; b[13] = a->b[13]; b[14] = a->b[14]; b[15] = a->b[15];
}

inline bool is_ip4(const IPaddr & a)
{
    return a.q[0]==0 && a.w[4]==0 && a.w[5]==0xffff;
}

inline bool is_loopback(const IPaddr & a)
{
    if(a.q[0]==0 && a.d[2]==0 && a.w[6]==0 && a.b[14]==0 && a.b[15]==1) return true;
    if(is_ip4(a) && a.b[12]==127) return true;
    return false;
}

inline bool is_private(IPaddr a)
{
    if(is_ip4(a)) {
        if(a.b[12]==10) return true;
        if(a.b[12]==172 && 16<=a.b[13] && a.b[13]<=31) return true;
        if(a.b[12]==192 && a.b[13]==168) return true;
    }
    else {
        if (a.b[0] == 0xfe && (a.b[1] & 0xf0) == 0xc0) return true;
        if ((a.b[0] & 0xfe) == 0xfc) return true;
    }
    return false;
}

inline bool is_multicast(const IPaddr & a)
{
    if(a.b[0] == 0xff) return true;
    if(is_ip4(a) && (a.b[12] &0xf0)==0xe0) return true;
    return false;
}

inline bool ip_match(const IPaddr & a, const IPaddr & b)
{
    return (a.q[1]==b.q[1] && a.q[0]==b.q[0]);
}

inline bool ip_match_mask(const IPaddr & a, const IPaddr & n, const IPaddr & m)
{
    return (a.q[0] & m.q[0])==(n.q[0] & m.q[0]) && (a.q[1] & m.q[1])==(n.q[1] & m.q[1]);
}

inline void ip_mask(IPaddr * a, IPaddr & m)
{
    a->q[0] &= m.q[0];
    a->q[1] &= m.q[1];
}

inline bool ip_match_anyaddr(const IPaddr & a)
{
    return a.q[0]==0 && a.q[1]==0;
}

inline bool is_unspecified(const IPaddr * a)
{
    return (!a->q[0] && !a->q[1]);
}

inline bool is_anyaddr(const IPaddr & a)
{
    if(a.d[3] || a.q[0] || a.w[4]) return false;
    return a.w[5]==0xffff || a.w[5]==0;

}

inline bool ip_match_maskall(IPaddr & a)
{
    return a.q[0]==(ulong64)-1 && a.q[1]==(ulong64)-1;
}

inline bool ip_4_match_anyaddr(IPaddr & a)
{
    return !a.q[0] && !a.w[4] && a.w[5]==0xffff && a.d[3]==0;
}

inline bool ip_4_match_maskall(IPaddr a)
{
    return a.d[3]==(dword)-1;
}

inline bool ip_match_prefix(const IPaddr & a, const IPaddr & prefix, unsigned prefixlen)
{
    if (prefixlen == 128) return ip_match(a, prefix);
    unsigned rest, bytes = prefixlen / 8;
    if (bytes)
        if (memcmp(&a, &prefix, bytes) != 0) return false;
    rest = prefixlen - bytes * 8;
    if (rest) {
        char mask = (0xff << (8 - rest));
        return ((a.b[bytes] & mask) == (prefix.b[bytes] & mask));
    }
    return true;
}

inline bool ip_match_prefix(const IPaddr & a, const IPaddr_prefix & prefix)
{
    return ip_match_prefix(a, prefix.a, prefix.p);
}

NAMESPACE_END

#endif
