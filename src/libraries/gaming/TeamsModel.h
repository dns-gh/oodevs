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
class TeamsModel : public kernel::Resolver< kernel::Team_ABC >
                 , public kernel::Resolver< kernel::Formation_ABC >
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

    void CreateTeam( const ASN1T_MsgSideCreation& asnMsg );
    void CreateFormation( const ASN1T_MsgFormationCreation& asnMsg ); // $$$$ AGE 2006-10-19: 

    kernel::Team_ABC& GetTeam( unsigned long id );
    kernel::Team_ABC* FindTeam( const QString& team );
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
