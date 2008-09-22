// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ASN_Messages_h_
#define __ASN_Messages_h_

#include "Publisher_ABC.h"

//=============================================================================
// ASN ENCODER WRAPPER MACROS
//=============================================================================
 
#define GENERATE_EMPTY_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )          \
class ASNMSG                                                             \
{                                                                        \
public:                                                                  \
    template< typename P >                                               \
    void Send( P& publisher, int context = 0 )                           \
    {                                                                    \
        ASN1T_Msgs##TARGET asnMsg;                                       \
        asnMsg.context = context;                                        \
        asnMsg.msg.t   = T_Msgs##TARGET##_msg_msg_##ASNVAR;              \
        publisher.Send( asnMsg );                                        \
    }                                                                    \
};

#define GENERATE_NOPTR_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )          \
class ASNMSG                                                             \
{                                                                        \
public:                                                                  \
    ASNMSG() {}                                                          \
    ASNMSG(ASN1T_Msg##ASNMSG& asn):asnTmp(asn) {}                        \
    template< typename P >                                               \
    void Send( P& publisher, int context = 0 )                           \
    {                                                                    \
        ASN1T_Msgs##TARGET asnMsg;                                       \
        asnMsg.context            = context;                             \
        asnMsg.msg.t              = T_Msgs##TARGET##_msg_msg_##ASNVAR;   \
        asnMsg.msg.u.msg_##ASNVAR = asnTmp;                              \
        publisher.Send( asnMsg );                                        \
    }                                                                    \
                                                                         \
    ASN1T_Msg##ASNMSG& operator()() { return asnTmp; }                   \
private:                                                                 \
    ASN1T_Msg##ASNMSG asnTmp;                                            \
                                                                         \
};

#define GENERATE_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )                \
class ASNMSG                                                             \
{                                                                        \
public:                                                                  \
    ASNMSG() {}                                                          \
    ASNMSG(ASN1T_Msg##ASNMSG& asn):asnTmp(asn) {}                        \
    template< typename P >                                               \
    void Send( P& publisher, int context = 0 )                           \
    {                                                                    \
        ASN1T_Msgs##TARGET asnMsg;                                       \
        asnMsg.context            = context;                             \
        asnMsg.msg.t              = T_Msgs##TARGET##_msg_msg_##ASNVAR;   \
        asnMsg.msg.u.msg_##ASNVAR = &asnTmp;                             \
        publisher.Send( asnMsg );                                        \
    }                                                                    \
                                                                         \
    ASN1T_Msg##ASNMSG& operator()() { return asnTmp; }                   \
private:                                                                 \
    ASN1T_Msg##ASNMSG asnTmp;                                            \
                                                                         \
};

#define GENERATE_NOCTX_EMPTY_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )    \
class ASNMSG                                                             \
{                                                                        \
public:                                                                  \
    template< typename P >                                               \
    void Send( P& publisher )                                            \
    {                                                                    \
        ASN1T_Msgs##TARGET asnMsg;                                       \
        asnMsg.t   = T_Msgs##TARGET##_msg_##ASNVAR;                      \
        publisher.Send( asnMsg );                                        \
    }                                                                    \
};

                                          
#define GENERATE_NOCTX_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR  )         \
class ASNMSG                                                             \
{                                                                        \
public:                                                                  \
    ASNMSG() {}                                                          \
    ASNMSG(ASN1T_Msg##ASNMSG& asn):asnTmp(asn) {}                        \
    template< typename P >                                               \
    void Send( P& publisher)                                             \
    {                                                                    \
        ASN1T_Msgs##TARGET asnMsg;                                       \
        asnMsg.t              = T_Msgs##TARGET##_msg_##ASNVAR;           \
        asnMsg.u.msg_##ASNVAR = &asnTmp;                                 \
        publisher.Send( asnMsg );                                        \
    }                                                                    \
                                                                         \
    ASN1T_Msg##ASNMSG& operator()() { return asnTmp; }                   \
private:                                                                 \
    ASN1T_Msg##ASNMSG asnTmp;                                            \
                                                                         \
};

#define GENERATE_NOCTX_NOPTR_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )    \
class ASNMSG                                                             \
{                                                                        \
public:                                                                  \
    ASNMSG() {}                                                          \
    ASNMSG(ASN1T_Msg##ASNMSG& asn):asnTmp(asn) {}                        \
    template< typename P >                                               \
    void Send( P& publisher)                                             \
    {                                                                    \
        ASN1T_Msgs##TARGET asnMsg;                                       \
        asnMsg.t              = T_Msgs##TARGET##_msg_##ASNVAR;           \
        asnMsg.u.msg_##ASNVAR = asnTmp;                                  \
        publisher.Send( asnMsg );                                        \
    }                                                                    \
                                                                         \
    ASN1T_Msg##ASNMSG& operator()() { return asnTmp; }                   \
private:                                                                 \
    ASN1T_Msg##ASNMSG asnTmp;                                            \
                                                                         \
};

#endif // __ASN_Messages_h_
