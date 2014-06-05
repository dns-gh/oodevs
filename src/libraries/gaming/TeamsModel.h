// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TeamsModel_h_
#define __TeamsModel_h_

#include "protocol/Simulation.h"
#include <tools/Resolver.h>

namespace kernel
{
    class Team_ABC;
    class KnowledgeGroup_ABC;
    class Formation_ABC;
    class HierarchyLevel_ABC;
}

class TeamFactory_ABC;

class TeamModelResolver : public tools::Resolver< kernel::Team_ABC >
{
    public:
        TeamModelResolver();
        virtual ~TeamModelResolver();
        virtual kernel::Team_ABC* Find( const unsigned long& identifier ) const;
        virtual kernel::Team_ABC& Get ( const unsigned long& identifier ) const;
        virtual kernel::Team_ABC& GetNoSideTeam() const = 0;
};

// =============================================================================
/** @class  TeamsModel
    @brief  TeamsModel
*/
// Created: AGE 2006-02-10
// =============================================================================
class TeamsModel : public TeamModelResolver
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

    void CreateTeam( const sword::PartyCreation& message );
    void CreateFormation( const sword::FormationCreation& message );
    void DestroyFormation( const sword::FormationDestruction& message );

    kernel::Team_ABC& GetTeam( unsigned long id );
    kernel::Team_ABC* FindTeam( const QString& team );
    virtual kernel::Team_ABC& GetNoSideTeam() const;
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
    std::unique_ptr< kernel::Team_ABC > noSideTeam_;
    //@}
};

#endif // __TeamsModel_h_
