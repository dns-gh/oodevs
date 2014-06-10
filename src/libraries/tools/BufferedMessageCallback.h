// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BufferedMessageCallback_h_
#define __BufferedMessageCallback_h_

#include "ConnectionCallback_ABC.h"
#include "MessageCallback_ABC.h"
#include "Message.h"
#include <boost/thread/mutex.hpp>
#include <boost/noncopyable.hpp>

namespace tools
{

// BufferedMessageCallback is a thread-safe connection and message events
// gateway between the networking and application layer. The networking layer
// pushes connection success/failures and message data/warning/errors by
// calling the OnSomething() methods and the application layer evaluate them
// all at once when calling Commit().
class BufferedMessageCallback : public MessageCallback_ABC,
                                public ConnectionCallback_ABC,
                                private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             BufferedMessageCallback();
    virtual ~BufferedMessageCallback();
    //@}

    //! @name Operations
    //@{
    virtual void OnError( const std::string& endpoint, const std::string& error );
    virtual void OnWarning( const std::string& endpoint, const std::string& error );
    virtual void OnMessage( const std::string& endpoint, Message& message );

    virtual void ConnectionSucceeded( const std::string& local, const std::string& remote );
    virtual void ConnectionFailed   ( const std::string& address, const std::string& error );

    void Commit( ConnectionCallback_ABC& connection, MessageCallback_ABC& message );
    //@}

private:
    //! @name Helpers
    //@{
    void Commit( MessageCallback_ABC& callback, const std::string& endpoint, Message& message ) const;
    //@}

private:
    // A combined representation of connection and message events.
    // Connection events typically set connection_ to true and fill local_,
    // remote_ and possibly error_ fields, while messages set connection_ to
    // false and fill only remote_, message_ and possibly error_ or warning_.
    struct Event
    {
        Event( const std::string& local, const std::string& remote,
            const std::string& error, const std::string& warning,
            bool connection );
        Event( const std::string& remote, const Message& message );
        std::string local_;
        std::string remote_;
        std::string error_;
        std::string warning_;
        Message message_;
        bool connection_;
    };
    typedef std::vector< Event >   T_Events;
    typedef T_Events::iterator    IT_Events;

private:
    //! @name Member data
    //@{
    boost::mutex mutex_;
    T_Events     events_;
    //@}
};

}

#endif // __BufferedMessageCallback_h_
