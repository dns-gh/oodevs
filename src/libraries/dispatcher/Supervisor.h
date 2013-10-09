// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Supervisor_h_
#define __Supervisor_h_

#include "Profile.h"

namespace sword
{
    class Profile;
    class ProfileDescription;
    class ClientToSim;
}

namespace dispatcher
{
    class Model;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  Supervisor
    @brief  Supervisor
*/
// Created: LGY 2013-10-07
// =============================================================================
class Supervisor : public Profile
{
public:
    //! @name Constructors/Destructor
    //@{
             Supervisor( const Model& model, ClientPublisher_ABC& clients );
    virtual ~Supervisor();
    //@}

    //! @name Main
    //@{
    virtual bool CheckRights( const sword::ClientToSim& msg ) const;
    virtual bool CheckRights( const sword::ClientToAuthentication& msg ) const;
    virtual bool CheckRights( const sword::ClientToReplay& msg ) const;
    virtual bool CheckRights( const sword::ChatTarget& source, const sword::ChatTarget& target ) const;

    virtual void Send( sword::ConnectedProfileList& msg ) const;
    virtual void Send( sword::AuthenticationResponse& msg ) const;
    virtual void Send( sword::ProfileDescription& msg ) const;
    virtual void SendCreation( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction() const;

    virtual void SerializeProfile( xml::xostream& xos ) const;
    //@}
};

}

#endif // __Supervisor_h_
