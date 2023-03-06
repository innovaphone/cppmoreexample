
NAMESPACE_BEGIN

typedef struct { byte b[16]; } OS_GUID;
typedef struct { word tag; word mode; word mki; word mki_len; byte b[46]; word flags; } SRTP_KEY;

/*---------------------------------------------------------------------------*/
/* IP Address                                                                */
/*---------------------------------------------------------------------------*/

#define IP4_ALEN 4  // IP4 address length in bytes (octets)

typedef union {
    byte    b[4];
    word    w[2];
    dword   d[1];
} IP4addr;

#define IPaddr2IP4addr(x) (x?((IP4addr *)&(x)->d[3]):0)
#define IPaddr2IP4addr_local(x) (((IP4addr *)&(x)->d[3]))
#define IP4addr2IPaddr(x,d) IPaddr d = {{ 0,0,0,0,0,0,0,0,0,0,0xff,0xff,(x).b[0],(x).b[1],(x).b[2],(x).b[3] }};

extern const IP4addr ip4_anyaddr;
extern const IP4addr ip4_maskall;
extern const IP4addr ip4_loopback;
extern const IP4addr ip4_loopback_net;
extern const IP4addr ip4_multicast_net;
extern const IP4addr ip4_multicast_all_router;
extern const IP4addr ip4_class_a_mask;
extern const IP4addr ip4_class_b_mask;
extern const IP4addr ip4_class_c_mask;

/*---------------------------------------------------------------------------*/
/* legacy macros                                                             */
/*---------------------------------------------------------------------------*/

#define IP_MATCH(x,y) ip_match(x,y)
#define IP_MATCH_MASK(a,n,m) ip_match_mask(a,n,m)
#define IP_ANYADDR(x) is_anyaddr(x)
#define IP_MASKALL(x) ip_match_maskall(x)
#define IP_PRIVATE(x) is_private(x)

/*---------------------------------------------------------------------------*/
/* IP4addr operations                                                        */
/*---------------------------------------------------------------------------*/

inline void ip4_read(IP4addr * a, const byte * b)
{
   	a->b[0] = b[0]; a->b[1] = b[1]; a->b[2] = b[2]; a->b[3] = b[3];
}

inline void ip4_write(const IP4addr * a, byte * b)
{
   	b[0] = a->b[0]; b[1] = a->b[1]; b[2] = a->b[2]; b[3] = a->b[3];
}

inline bool ip4_match(IP4addr a, IP4addr b)
{
    return a.d[0] == b.d[0];
}

inline bool ip4_match_mask(IP4addr a, IP4addr n, IP4addr m)
{
    return (a.d[0] & m.d[0]) == n.d[0];
}

inline void ip4_mask(IP4addr * a, IP4addr m)
{
    a->d[0] &= m.d[0];
}

inline bool ip4_match_anyaddr(IP4addr a)
{
    return a.d[0]==0;
}

inline bool ip4_match_maskall(IP4addr a)
{
    return a.d[0]==(dword)-1;
}

inline bool ip4_match_class_a_net(IP4addr a)
{
    return ((a).b[0] & 0x80) == 0x00;
}

inline bool ip4_match_class_b_net(IP4addr a)
{
    return ((a).b[0] & 0xc0) == 0x80;
}

inline bool ip4_match_class_c_net(IP4addr a)
{
    return ((a).b[0] & 0xe0) == 0xc0;
}

inline bool ip4_match_multicast_net(IP4addr a)
{
    return (a.b[0] &0xf0)==0xe0;
}

/*---------------------------------------------------------------------------*/
/* Ethernet Address                                                          */
/*---------------------------------------------------------------------------*/

#define E_ALEN 6        // Ethernet address length in bytes (octets)

typedef struct {        // internet address
    byte b[E_ALEN];
} Eaddr;

const Eaddr   * mcast_eaddr(const IPaddr * addr);

inline bool operator==(const Eaddr & a, const Eaddr & b) { return (memcmp(&a, &b, sizeof(Eaddr)) == 0); };
inline bool operator!=(const Eaddr & a, const Eaddr & b) { return !(a == b); };

extern const Eaddr bcast_eaddr;
extern const Eaddr null_eaddr;

/*---------------------------------------------------------------------------*/
/* ObjectIdentifier definition                                               */
/*---------------------------------------------------------------------------*/

class objectIdentifier {
    byte b[32];
public:
    objectIdentifier();
    objectIdentifier(byte * asn);
    objectIdentifier(word len, ...);

    byte len();
    byte * get();
    word get(dword * buffer);
    word get(class objectIdentifier & base, word len, dword * buffer);

    void set(word len, ...);
    void set(byte * asn);
    void set(word len, dword * buffer);
    void set(class objectIdentifier & base, word len, dword * buffer);
};

bool operator==(class objectIdentifier x, class objectIdentifier y);

NAMESPACE_END

