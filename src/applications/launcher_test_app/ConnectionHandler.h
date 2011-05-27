// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************


#ifndef __LAUNCHER_TEST_APP_CONNECTIONHANDLER_H__
#define __LAUNCHER_TEST_APP_CONNECTIONHANDLER_H__

#include <string>
#include "frontend/ConnectionHandler_ABC.h"

namespace boost
{
    class mutex;
    class condition_variable;
}
namespace launcher_test_app
{
// =============================================================================
/** @class  ConnectionHandler
    @brief  ConnectionHandler
*/
// Created: AHC 2011-05-27
// =============================================================================
class ConnectionHandler : public frontend::ConnectionHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    ConnectionHandler( boost::mutex& mutex, boost::condition_variable& cond );
    virtual ~ConnectionHandler();
    //@}

    //! @name Accessors
    const std::string& Error() const { return error_; }
    bool Connected() const { return connected_; }
    //@{

    //! @name Operations
    //@{
    void OnConnectionSucceeded();
    void OnConnectionFailed( const std::string& reason );
    void OnConnectionLost( const std::string& reason );
    void OnError( const std::string& message );
    //@}

private:
    bool connected_;
    std::string error_;
    boost::mutex& mutex_;
    boost::condition_variable& cond_;
};

}

#endif // __LAUNCHER_TEST_APP_CONNECTIONHANDLER_H__
