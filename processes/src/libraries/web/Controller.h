// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Observer_ABC.h"

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

namespace boost
{
    class shared_mutex;
}

namespace process
{
    class Process_ABC;
    class Runtime_ABC;
}

namespace web
{
    class Request_ABC;

// =============================================================================
/** @class  Controller
    @brief  Process controller
*/
// Created: BAX 2012-03-07
// =============================================================================
class Controller : public Observer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Controller( const process::Runtime_ABC& runtime );
    virtual ~Controller();
    //@}

    //! @name Operations
    //@{
    virtual std::string Notify( const Request_ABC& request );
    //@}

private:
    //! @name Private typedef helpers
    //@{
    typedef std::map< int, boost::shared_ptr< process::Process_ABC > > T_Processes;
    //@}

    //! @name Private operations
    //@{
    std::string List( int offset, int limit );
    std::string List( const Request_ABC& request );
    std::string Start( const std::string& app, const std::vector< std::string >& args, const std::string& run );
    std::string Start( const Request_ABC& request );
    std::string Stop( int pid );
    std::string Stop( const Request_ABC& request );
    boost::shared_ptr< process::Process_ABC > Extract( int pid );
    void AddProcess( boost::shared_ptr< process::Process_ABC > ptr );
    //@}

private:
    //! @name Member data
    //@{
    const process::Runtime_ABC& runtime_;
    std::auto_ptr< boost::shared_mutex > access_;
    T_Processes processes_;
    //@}
};

}

#endif // CONTROLLER_H
