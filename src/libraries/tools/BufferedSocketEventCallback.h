// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BufferedSocketEventCallback_h_
#define __BufferedSocketEventCallback_h_

#include "SocketEventCallback_ABC.h"
#include "Message.h"
#include <boost/thread/mutex.hpp>
#include <boost/function.hpp>

namespace tools
{

// =============================================================================
/** @class  BufferedSocketEventCallback
    @brief  BufferedSocketEventCallback
*/
// Created: AGE 2007-09-06
// =============================================================================
class BufferedSocketEventCallback : public SocketEventCallback_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             BufferedSocketEventCallback();
    virtual ~BufferedSocketEventCallback();
    //@}

    //! @name Operations
    //@{
    virtual void ConnectionSucceeded( const std::string& endpoint );
    virtual void ConnectionFailed   ( const std::string& address, const std::string& error );
    virtual void ConnectionError    ( const std::string& endpoint, const std::string& error );
    virtual void ConnectionWarning  ( const std::string& endpoint, const std::string& error );
    virtual void OnMessage( const std::string& endpoint, Message& message );

    void Commit( SocketEventCallback_ABC& callback );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BufferedSocketEventCallback( const BufferedSocketEventCallback& );            //!< Copy constructor
    BufferedSocketEventCallback& operator=( const BufferedSocketEventCallback& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void OnMessageWrapper( SocketEventCallback_ABC& callback, const std::string& endpoint, Message& message ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< boost::function< void( SocketEventCallback_ABC& ) > >   T_Events;
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

#endif // __BufferedSocketEventCallback_h_
