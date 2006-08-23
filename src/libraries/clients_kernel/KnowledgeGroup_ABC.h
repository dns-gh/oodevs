// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeGroup_ABC_h_
#define __KnowledgeGroup_ABC_h_

#include "Entity_ABC.h"
#include "Resolver.h"

namespace kernel
{
    class Agent_ABC;
    class Team_ABC;

// =============================================================================
/** @class  KnowledgeGroup_ABC
    @brief  Knowledge group base class
*/
// Created: SBO 2006-08-09
// =============================================================================
class KnowledgeGroup_ABC : public Entity_ABC
                         , public Resolver< Agent_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroup_ABC();
    virtual ~KnowledgeGroup_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Select( ActionController& controller ) const = 0;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const = 0;
    virtual void Activate( ActionController& controller ) const = 0;
    virtual void AddAutomat( unsigned long id, Agent_ABC& automat ) = 0;
    virtual void RemoveAutomat( unsigned long id ) = 0;
    //@}

    //! @name Accessors
    //@{
    virtual bool IsInTeam( const Team_ABC& team ) const = 0;
    virtual const Team_ABC& GetTeam() const = 0;
    virtual unsigned long GetId() const = 0;
    virtual QString GetName() const = 0;
    //@}
};

}

#endif // __KnowledgeGroup_ABC_h_
