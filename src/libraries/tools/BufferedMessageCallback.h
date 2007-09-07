// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __BufferedMessageCallback_h_
#define __BufferedMessageCallback_h_

#include "MessageCallback_ABC.h"
#include "Message.h"
#include <boost/thread/mutex.hpp>

namespace tools
{

// =============================================================================
/** @class  BufferedMessageCallback
    @brief  BufferedMessageCallback
*/
// Created: AGE 2007-09-06
// =============================================================================
class BufferedMessageCallback : public MessageCallback_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             BufferedMessageCallback();
    virtual ~BufferedMessageCallback();
    //@}

    //! @name Operations
    //@{
    virtual void OnError  ( const std::string& endpoint, const std::string& error );
    virtual void OnMessage( const std::string& endpoint, Message& message );

    void Commit( MessageCallback_ABC& callback );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BufferedMessageCallback( const BufferedMessageCallback& );            //!< Copy constructor
    BufferedMessageCallback& operator=( const BufferedMessageCallback& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    struct Event
    {
        Event( const std::string& endpoint, const std::string& error );
        Event( const std::string& endpoint, const Message& message );
        std::string endpoint_, error_;
        Message message_;
    };
    typedef std::vector< Event >   T_Events;
    typedef T_Events::iterator    IT_Events;
    //@}

private:
    //! @name Member data
    //@{
    boost::mutex mutex_;
    T_Events     events_;
    //@}
};

}

#endif // __BufferedMessageCallback_h_
