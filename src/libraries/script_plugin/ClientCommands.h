// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ClientCommands_h_
#define __ClientCommands_h_

#include "dispatcher/Registrable_ABC.h"

namespace dispatcher
{
    class ClientPublisher_ABC;
    class LinkResolver_ABC;
}

namespace xml
{
    class xistream;
}

namespace plugins
{
namespace script
{
// =============================================================================
/** @class  ClientCommands
    @brief  Client commands
*/
// Created: AGE 2008-06-24
// =============================================================================
class ClientCommands : public dispatcher::Registrable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ClientCommands( dispatcher::ClientPublisher_ABC& clients, const dispatcher::LinkResolver_ABC& resolver );
    virtual ~ClientCommands();
    //@}

    //! @name Operations
    //@{
    virtual void RegisterIn( directia::brain::Brain& brain );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ClientCommands( const ClientCommands& );            //!< Copy constructor
    ClientCommands& operator=( const ClientCommands& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SendCommandToAll( const std::string& profile, const std::string& command );
    void SendCommand     ( const std::string& client, const std::string& profile, const std::string& command );
    void Send            ( const std::string& profile, const std::string& command, dispatcher::ClientPublisher_ABC& target );
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::ClientPublisher_ABC& clients_;
    const dispatcher::LinkResolver_ABC& resolver_;
    //@}
};

}
}

#endif // __ClientCommands_h_
