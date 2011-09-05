// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef shield_Server_ABC_h
#define shield_Server_ABC_h

#include <boost/noncopyable.hpp>

namespace sword
{
    class ClientToAar;
    class ClientToMessenger;
    class ClientToAuthentication;
    class ClientToReplay;
    class ClientToSim;
    class AdminToLauncher;
}

namespace shield
{
// =============================================================================
/** @class  Server_ABC
    @brief  Server declaration
*/
// Created: MCO 2010-12-01
// =============================================================================
class Server_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Server_ABC() {}
    virtual ~Server_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Send( sword::ClientToSim& message ) = 0;
    virtual void Send( sword::ClientToAuthentication& message ) = 0;
    virtual void Send( sword::ClientToReplay& message ) = 0;
    virtual void Send( sword::ClientToAar& message ) = 0;
    virtual void Send( sword::ClientToMessenger& message ) = 0;
    virtual void Send( sword::AdminToLauncher& message ) = 0;
    //@}
};

}

#endif // shield_Server_ABC_h
