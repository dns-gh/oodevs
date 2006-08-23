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

#include "ASN_Types.h"
#include "clients_kernel/Resolver.h"

namespace kernel
{
    class Team_ABC;
    class KnowledgeGroup_ABC;
}

namespace DIN {
    class DIN_Input;
};

class TeamFactory_ABC;

// =============================================================================
/** @class  TeamsModel
    @brief  TeamsModel
*/
// Created: AGE 2006-02-10
// =============================================================================
class TeamsModel : public kernel::Resolver< kernel::Team_ABC >
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

    void CreateTeam( DIN::DIN_Input& );
    kernel::Team_ABC& GetTeam( unsigned long id );
    kernel::Team_ABC* FindTeam( const QString& team );

    kernel::KnowledgeGroup_ABC* FindKnowledgeGroup( const unsigned long& identifier ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TeamsModel( const TeamsModel& );            //!< Copy constructor
    TeamsModel& operator=( const TeamsModel& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    TeamFactory_ABC& factory_;
    //@}
};

#endif // __TeamsModel_h_
