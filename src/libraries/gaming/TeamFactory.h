// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TeamFactory_h_
#define __TeamFactory_h_

#include "TeamFactory_ABC.h"
#include "KnowledgeGroupFactory_ABC.h"

namespace kernel
{
    class Controllers;
}

class Model;

// =============================================================================
/** @class  TeamFactory
    @brief  TeamFactory
*/
// Created: AGE 2006-02-15
// =============================================================================
class TeamFactory : public TeamFactory_ABC
                  , public KnowledgeGroupFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TeamFactory( kernel::Controllers& controllers, Model& model );
    virtual ~TeamFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Team_ABC* CreateTeam( const ASN1T_MsgSideCreation& asnMsg );
    virtual kernel::Formation_ABC* CreateFormation( const ASN1T_MsgFormationCreation& asnMsg );
    virtual kernel::KnowledgeGroup_ABC* CreateKnowledgeGroup( unsigned long id, kernel::Team_ABC& team );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TeamFactory( const TeamFactory& );            //!< Copy constructor
    TeamFactory& operator=( const TeamFactory& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    //@}
};

#endif // __TeamFactory_h_
