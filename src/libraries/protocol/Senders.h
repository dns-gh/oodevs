// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __protocol_Senders_h_
#define __protocol_Senders_h_

//=============================================================================
// ENCODER WRAPPER MACROS
//=============================================================================
#define GENERATE_ASN_MSG_SENDER( TARGET, NAMESPACE, MSG, VAR ) \
class MSG                                                      \
{                                                              \
public:                                                        \
    MSG() { operator()(); }                                    \
    template< typename T >                                     \
    void Send( T& publisher, int context = 0 )                 \
    {                                                          \
        message.set_context( context );                        \
        publisher.Send( message );                             \
    }                                                          \
    NAMESPACE##::MSG& operator()()                             \
    { return *message.mutable_message()->mutable_##VAR(); }    \
private:                                                       \
    TARGET message;                               \
};

#define GENERATE_NOCTX_ASN_MSG_SENDER( TARGET, NAMESPACE, MSG, VAR  )                      \
class MSG                                                                                  \
{                                                                                          \
public:                                                                                    \
    MSG() { operator()(); }                                                                \
    MSG( NAMESPACE##::MSG& msg ){ *message.mutable_message()->mutable_##VAR() = msg; }     \
    template< typename P >                                                                 \
    void Send( P& publisher)                                                               \
    {                                                                                      \
        publisher.Send( message );                                                         \
    }                                                                                      \
                                                                                           \
    NAMESPACE##::MSG& operator()() { return *message.mutable_message()->mutable_##VAR(); } \
private:                                                                                   \
    TARGET message;                                                           \
};

#endif // __protocol_Senders_h_
