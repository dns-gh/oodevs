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

#include "ASN_Types.h"
#include "Resolver.h"

class Team;
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
class TeamsModel : public Resolver< Team >
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
