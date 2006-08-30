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

#include "clients_kernel/Resolver.h"

namespace kernel
{
    class Team_ABC;
    class KnowledgeGroup_ABC;
}

class TeamFactory_ABC;

// =============================================================================
/** @class  TeamsModel
    @brief  TeamsModel
*/
// Created: SBO 2006-08-30
// =============================================================================
class TeamsModel : public kernel::Resolver< kernel::Team_ABC, QString >
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
    void CreateTeam();
    void CreateKnowledgeGroup( kernel::Team_ABC& team );
    bool Rename( kernel::Team_ABC& team, const QString& name );

    kernel::Team_ABC* FindTeam( const QString& team ) const;
    kernel::KnowledgeGroup_ABC* FindKnowledgeGroup( const unsigned long& identifier ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TeamsModel( const TeamsModel& );            //!< Copy constructor
    TeamsModel& operator=( const TeamsModel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    QString GenerateTeamName() const;
    //@}

private:
    //! @name Member data
    //@{
    TeamFactory_ABC& factory_;
    //@}
};

#endif // __TeamsModel_h_
