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
#include "astec_kernel/Resolver.h"

class Team_ABC;
class TeamFactory_ABC;
class KnowledgeGroup;

namespace DIN {
    class DIN_Input;
};


// =============================================================================
/** @class  TeamsModel
    @brief  TeamsModel
*/
// Created: AGE 2006-02-10
// =============================================================================
class TeamsModel : public Resolver< Team_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             TeamsModel( TeamFactory_ABC& factory );
    virtual ~TeamsModel();
    //@}

    //! @name Operations
    //@{
    void Purge();

    void CreateTeam( DIN::DIN_Input& );
    Team_ABC& GetTeam( unsigned long id );
    Team_ABC* FindTeam( const std::string& team );

    KnowledgeGroup* FindKnowledgeGroup( const unsigned long& identifier ) const;
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
