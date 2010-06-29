// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Message_Senders_h_
#define __Message_Senders_h_

#include "Publisher_ABC.h"

//=============================================================================
// ASN ENCODER WRAPPER MACROS
//=============================================================================

//#define GENERATE_EMPTY_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )          \
//class ASNMSG                                                             \
//{                                                                        \
//public:                                                                  \
//    template< typename P >                                               \
//    void Send( P& publisher, int context = 0 )                           \
//    {                                                                    \
//        Msgs##TARGET asnMsg;                                       \
//        asnMsg.context = context;                                        \
//        asnMsg.msg.t   = T_Msgs##TARGET##_msg_msg_##ASNVAR;              \
//        publisher.Send( asnMsg );                                        \
//    }                                                                    \
//};

#define GENERATE_NOPTR_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )          \
class ASNMSG                                                             \
{                                                                        \
public:                                                                  \
    ASNMSG() {}                                                          \
    ASNMSG(Msg##ASNMSG& asn):asnTmp(asn) {}                        \
    template< typename P >                                               \
    void Send( P& publisher, int context = 0 )                           \
    {                                                                    \
        Msgs##TARGET asnMsg;                                       \
        asnMsg.context            = context;                             \
        asnMsg.msg.t              = T_Msgs##TARGET##_msg_msg_##ASNVAR;   \
        asnMsg.msg().msg_##ASNVAR = asnTmp;                              \
        publisher.Send( asnMsg );                                        \
    }                                                                    \
                                                                         \
    Msg##ASNMSG& operator()() { return asnTmp; }                   \
private:                                                                 \
    Msg##ASNMSG asnTmp;                                            \
                                                                         \
};

#define GENERATE_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )                \
class ASNMSG                                                             \
{                                                                        \
public:                                                                  \
    ASNMSG() {}                                                          \
    ASNMSG(##ASNMSG& asn):asnTmp(asn) {}                        \
    template< typename P >                                               \
    void Send( P& publisher, int context = 0 )                           \
    {                                                                    \
        Msgs##TARGET asnMsg;                                             \
        asnMsg.set_context( context );                                   \
        *(asnMsg.mutable_msg()->mutable_##ASNVAR()) = &asnTmp;           \
        publisher.Send( asnMsg );                                        \
    }                                                                    \
                                                                         \
    ##ASNMSG& operator()() { return asnTmp; }                            \
private:                                                                 \
    Msg##ASNMSG asnTmp;                                                  \
                                                                         \
};

#define GENERATE_NOCTX_EMPTY_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )    \
class ASNMSG                                                             \
{                                                                        \
public:                                                                  \
    template< typename P >                                               \
    void Send( P& publisher )                                            \
    {                                                                    \
        Msgs##TARGET asnMsg;                                       \
        asnMsg.t   = T_Msgs##TARGET##_msg_##ASNVAR;                      \
        publisher.Send( asnMsg );                                        \
    }                                                                    \
};


#define GENERATE_NOCTX_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR  )         \
class ASNMSG                                                             \
{                                                                        \
public:                                                                  \
    ASNMSG() {}                                                          \
    ASNMSG(Msg##ASNMSG& asn):asnTmp(asn) {}                        \
    template< typename P >                                               \
    void Send( P& publisher)                                             \
    {                                                                    \
        Msgs##TARGET asnMsg;                                       \
        asnMsg.t              = T_Msgs##TARGET##_msg_##ASNVAR;           \
        asnMsg().msg_##ASNVAR = &asnTmp;                                 \
        publisher.Send( asnMsg );                                        \
    }                                                                    \
                                                                         \
    Msg##ASNMSG& operator()() { return asnTmp; }                   \
private:                                                                 \
    Msg##ASNMSG asnTmp;                                            \
                                                                         \
};

#define GENERATE_NOCTX_NOPTR_ASN_MSG_SENDER( TARGET, ASNMSG, ASNVAR )    \
class ASNMSG                                                             \
{                                                                        \
public:                                                                  \
    ASNMSG() {}                                                          \
    ASNMSG(Msg##ASNMSG& asn):asnTmp(asn) {}                        \
    template< typename P >                                               \
    void Send( P& publisher)                                             \
    {                                                                    \
        Msgs##TARGET asnMsg;                                       \
        asnMsg.t              = T_Msgs##TARGET##_msg_##ASNVAR;           \
        asnMsg().msg_##ASNVAR = asnTmp;                                  \
        publisher.Send( asnMsg );                                        \
    }                                                                    \
                                                                         \
    Msg##ASNMSG& operator()() { return asnTmp; }                   \
private:                                                                 \
    Msg##ASNMSG asnTmp;                                            \
                                                                         \
};

#endif // __Message_Senders_h_
