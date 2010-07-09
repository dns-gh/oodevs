// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TeamFactory_ABC_h_
#define __TeamFactory_ABC_h_

namespace Common
{
    class MsgFormationCreation;
}

namespace MsgsSimToClient
{
    class MsgTeamCreation;
}

namespace kernel
{
    class Team_ABC;
    class Formation_ABC;
}


// =============================================================================
/** @class  TeamFactory_ABC
    @brief  TeamFactory_ABC
*/
// Created: AGE 2006-02-15
// =============================================================================
class TeamFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TeamFactory_ABC() {};
    virtual ~TeamFactory_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual kernel::Team_ABC*      CreateTeam     ( const MsgsSimToClient::MsgTeamCreation& message ) = 0;
    virtual kernel::Formation_ABC* CreateFormation( const Common::MsgFormationCreation& message ) = 0;
    //@}
};

#endif // __TeamFactory_ABC_h_
