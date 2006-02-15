// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Team.h $
// $Author: Ape $
// $Modtime: 16/02/05 15:52 $
// $Revision: 8 $
// $Workfile: Team.h $
//
// *****************************************************************************

#ifndef __Team_h_
#define __Team_h_

#include "DIN_Types.h"
#include "Entity_ABC.h"
#include "Resolver.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"

class KnowledgeGroup;
class KnowledgeGroupFactory_ABC;
class Controller;

// =============================================================================
/** @class  Team
    @brief  Represents a team.
    @par    Holds the team's knowledge on dynamic objects and a concatenation
            of it's gtia's agent knowledges.     
*/
// Created: AGN 2003-12-22
// =============================================================================
class Team : public Entity_ABC, 
             public Resolver< KnowledgeGroup >
{

public:
    //! @name Constructors/Destructor
    //@{
             Team( uint nID, DIN::DIN_Input& input, Controller& controller, KnowledgeGroupFactory_ABC& factory );
    virtual ~Team();
    //@}

    //! @name Operations
    //@{
    void CreateKnowledgeGroup( unsigned int id );
    //@}

    //! @name Accessors
    //@{
    unsigned long GetId  () const;
    std::string GetName() const;
    //@}

private:
    //! @name Copy / Assignement
    //@{
    Team( const Team& );
    Team& operator=( const Team& );
    //@}

    //! @name Helpers
    //@{
    
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    KnowledgeGroupFactory_ABC& factory_;
    std::string         strName_;
    uint32              nID_;
    //@}
};

#endif // __Team_h_
