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

#define GENERATE_MESSAGE( TARGET, NAMESPACE, MSG, VAR )             \
class MSG                                                           \
{                                                                   \
public:                                                             \
    MSG()                                                           \
    {                                                               \
        operator()();                                               \
    }                                                               \
    template< typename P >                                          \
    void Send( P& publisher, int context = 0 )              \
    {                                                               \
        message.set_context( context );                             \
        publisher.Send( message );                          \
    }                                                               \
    NAMESPACE::MSG& operator()()                                  \
    {                                                               \
        return *message.mutable_message()->mutable_##VAR();         \
    }                                                               \
private:                                                            \
    TARGET message;                                                 \
};


#define GENERATE_MESSAGE_WITH_CLIENTID( TARGET, NAMESPACE, MSG, VAR )\
class MSG                                                            \
{                                                                    \
public:                                                              \
    MSG()                                                            \
    {                                                                \
        operator()();                                                \
    }                                                                \
    template< typename P >                                           \
    void Send( P& publisher, int context = 0, int clientId = 0 )     \
    {                                                                \
        message.set_context( context );                              \
        if( clientId != 0 )                                          \
            message.set_client_id( clientId );                       \
        publisher.Send( message );                                   \
    }                                                                \
    NAMESPACE::MSG& operator()()                                   \
    {                                                                \
        return *message.mutable_message()->mutable_##VAR();          \
    }                                                                \
private:                                                             \
    TARGET message;                                                  \
};

#define GENERATE_NO_CONTEXT_MESSAGE( TARGET, NAMESPACE, MSG, VAR ) \
class MSG                                                          \
{                                                                  \
public:                                                            \
    MSG()                                                          \
    {                                                              \
        operator()();                                              \
    }                                                              \
    MSG( NAMESPACE::MSG& msg )                                   \
    {                                                              \
        operator()() = msg;                                        \
    }                                                              \
    template< typename P >                                         \
    void Send( P& publisher)                                       \
    {                                                              \
        publisher.Send( message );                                 \
    }                                                              \
    NAMESPACE::MSG& operator()()                                 \
    {                                                              \
        return *message.mutable_message()->mutable_##VAR();        \
    }                                                              \
private:                                                           \
    TARGET message;                                                \
};

#endif // __protocol_Senders_h_
