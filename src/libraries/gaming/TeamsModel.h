// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TeamsModel_h_
#define __TeamsModel_h_

#include "protocol/Protocol.h"
#include "tools/Resolver.h"

namespace MsgsSimToClient
{
    class MsgPartyCreation;
    class MsgFormationCreation;
}

namespace kernel
{
    class Team_ABC;
    class KnowledgeGroup_ABC;
    class Formation_ABC;
    class HierarchyLevel_ABC;
}

class TeamFactory_ABC;

// =============================================================================
/** @class  TeamsModel
    @brief  TeamsModel
*/
// Created: AGE 2006-02-10
// =============================================================================
class TeamsModel : public tools::Resolver< kernel::Team_ABC >
                 , public tools::Resolver< kernel::Formation_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit TeamsModel( TeamFactory_ABC& factory );
    virtual ~TeamsModel();
    //@}

    //! @name Operations
    //@{
    void Purge();

    void CreateTeam( const MsgsSimToClient::MsgPartyCreation& message );
    void CreateFormation( const MsgsSimToClient::MsgFormationCreation& message );

    kernel::Team_ABC& GetTeam( unsigned long id );
    kernel::Team_ABC* FindTeam( const QString& team );
    kernel::Formation_ABC& GetFormation( unsigned long id );
    kernel::Formation_ABC* FindFormation( unsigned long id );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TeamsModel( const TeamsModel& );            //!< Copy constructor
    TeamsModel& operator=( const TeamsModel& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    TeamFactory_ABC& factory_;
    //@}
};

#endif // __TeamsModel_h_
