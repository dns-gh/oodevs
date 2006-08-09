// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Agent_ABC_h_
#define __Agent_ABC_h_

#include "Entity_ABC.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Resolver.h"
#include "Drawable_ABC.h"
#include "Aggregatable_ABC.h"

class KnowledgeGroup_ABC;
class Team_ABC;
class AgentType;
class AutomatType;

// =============================================================================
/** @class  Agent_ABC
    @brief  Agent_ABC base class.
*/
// Created: AGE 2006-02-13
// =============================================================================
class Agent_ABC : public Entity_ABC
                , public Resolver< Agent_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             Agent_ABC();
    virtual ~Agent_ABC();
    //@}

    //! @name Accessors
    //@{
    virtual bool IsInTeam( const Team_ABC& team ) const = 0; // $$$$ AGE 2006-08-03: descendre dans Entity_ABC
    virtual const Agent_ABC* GetSuperior() const = 0;
    virtual const Team_ABC& GetTeam() const = 0;
    virtual KnowledgeGroup_ABC& GetKnowledgeGroup() const = 0;

    virtual std::string GetName() const = 0;
    virtual unsigned long GetId() const = 0;

    virtual const AutomatType* GetAutomatType() const = 0;
    virtual const AgentType& GetType() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Select( ActionController& controller ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    virtual void Activate( ActionController& controller ) const;
    //@}
};

#endif // __Agent_ABC_h_
