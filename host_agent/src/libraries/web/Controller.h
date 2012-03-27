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
#include <string>

namespace host
{
    class Agent_ABC;
};

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
             Controller( host::Agent_ABC& host );
    virtual ~Controller();
    //@}

    //! @name Operations
    //@{
    virtual std::string Notify( const Request_ABC& request );
    //@}

private:
    //! @name Session Methods
    //@{
    std::string ListSessions ( const Request_ABC& request );
    std::string CountSessions( const Request_ABC& request );
    std::string GetSession   ( const Request_ABC& request );
    std::string CreateSession( const Request_ABC& request );
    std::string DeleteSession( const Request_ABC& request );
    std::string ListExercises( const Request_ABC& request );
    //@}

private:
    //! @name Member data
    //@{
    host::Agent_ABC& agent_;
    //@}
};

}

#endif // CONTROLLER_H
