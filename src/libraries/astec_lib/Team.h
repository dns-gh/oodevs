// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
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
    bool operator==( const Team& ) const;

    virtual void Select( ActionController& controller ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    virtual void Activate( ActionController& controller ) const;
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetId  () const;
    virtual std::string GetName() const;
    //@}

private:
    //! @name Copy / Assignement
    //@{
    Team( const Team& );
    Team& operator=( const Team& );
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    KnowledgeGroupFactory_ABC& factory_;
    std::string         strName_;
    unsigned long              nID_;
    //@}
};

#endif // __Team_h_
