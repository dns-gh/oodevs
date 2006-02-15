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
#include "Resolver.h"

class Team;
class KnowledgeGroup;
class TeamFactory_ABC;

namespace DIN {
    class DIN_Input;
};


// =============================================================================
/** @class  TeamsModel
    @brief  TeamsModel
    // $$$$ AGE 2006-02-15: TeamFactory
*/
// Created: AGE 2006-02-10
// =============================================================================
class TeamsModel : public Resolver< Team >
                 , public Resolver< KnowledgeGroup > 
{

public:
    //! @name Constructors/Destructor
    //@{
             TeamsModel( TeamFactory_ABC& factory );
    virtual ~TeamsModel();
    //@}

    //! @name Operations
    //@{
    void CreateTeam( DIN::DIN_Input& );
    Team& GetTeam( unsigned long id );
    Team* FindTeam( const std::string& team );

    void CreateKnowledgeGroup( DIN::DIN_Input& );
    KnowledgeGroup& GetKnowledgeGroup( unsigned long id );
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
