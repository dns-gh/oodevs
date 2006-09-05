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
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Team_ABC;
    class KnowledgeGroup_ABC;
    class Controllers;
}

class TeamFactory_ABC;

// =============================================================================
/** @class  TeamsModel
    @brief  TeamsModel
*/
// Created: SBO 2006-08-30
// =============================================================================
class TeamsModel : public kernel::Resolver< kernel::Team_ABC >
                 , public kernel::Observer_ABC
                 , public kernel::ElementObserver_ABC< kernel::Team_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             TeamsModel( kernel::Controllers& controllers, TeamFactory_ABC& factory );
    virtual ~TeamsModel();
    //@}

    //! @name Operations
    //@{
    void Purge();
    void CreateTeam();
    void CreateKnowledgeGroup( const kernel::Team_ABC& team );
    void Rename( unsigned long id, const QString& name );

    kernel::Team_ABC* FindTeam( const QString& name ) const;
    kernel::KnowledgeGroup_ABC* FindKnowledgeGroup( const unsigned long& id ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TeamsModel( const TeamsModel& );            //!< Copy constructor
    TeamsModel& operator=( const TeamsModel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    TeamFactory_ABC& factory_;
    //@}
};

#endif // __TeamsModel_h_
