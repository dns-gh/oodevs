// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BufferedConnectionCallback_h_
#define __BufferedConnectionCallback_h_

#include "ConnectionCallback_ABC.h"
#include <boost/thread/mutex.hpp>

namespace tools
{

// =============================================================================
/** @class  BufferedConnectionCallback
    @brief  BufferedConnectionCallback
*/
// Created: AGE 2007-09-06
// =============================================================================
class BufferedConnectionCallback : public ConnectionCallback_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             BufferedConnectionCallback();
    virtual ~BufferedConnectionCallback();
    //@}

    //! @name Operations
    //@{
    void Commit( ConnectionCallback_ABC& callback );
    //@}

private:
    //! @name Operations
    //@{
    virtual void ConnectionSucceeded( const std::string& source, const std::string& endpoint );
    virtual void ConnectionFailed   ( const std::string& address, const std::string& error );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BufferedConnectionCallback( const BufferedConnectionCallback& );            //!< Copy constructor
    BufferedConnectionCallback& operator=( const BufferedConnectionCallback& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    struct Event
    {
        Event( const std::string& source, const std::string& endpoint,
                const std::string& error );

        std::string source_;
        std::string endpoint_;
        std::string error_;
    };
    typedef std::vector< Event >       T_Events;
    typedef T_Events::const_iterator CIT_Events;
    //@}

private:
    //! @name Member data
    //@{
    boost::mutex mutex_;
    T_Events events_;
    //@}
};

}

#endif // __BufferedConnectionCallback_h_
