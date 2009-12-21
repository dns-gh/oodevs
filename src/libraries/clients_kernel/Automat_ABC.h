// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Automat_ABC_h_
#define __Automat_ABC_h_

#include "Entity_ABC.h"
#include "tools/Resolver.h"

namespace kernel
{
    class Agent_ABC;
    class AutomatType;
    class KnowledgeGroup_ABC;

// =============================================================================
/** @class  Automat_ABC
    @brief  Automat_ABC
*/
// Created: AGE 2006-10-06
// =============================================================================
class Automat_ABC : public Entity_ABC
{
public:
    //! @name Static
    //@{
    static const QString typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Automat_ABC();
    virtual ~Automat_ABC();
    //@}

    //! @name Operations
    //@{
    virtual const AutomatType& GetType() const = 0;

    virtual QString GetTypeName() const;
    virtual void Select( ActionController& controller ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    virtual void Activate( ActionController& controller ) const;
    virtual void Register( Automat_ABC& automat ) = 0;
    virtual void Remove( Automat_ABC& automat ) = 0;
    virtual const tools::Resolver< Automat_ABC >& GetAutomats() const = 0;
    virtual void Register( Agent_ABC& automat ) = 0;
    virtual void Remove( Agent_ABC& automat ) = 0;
    virtual const tools::Resolver< Agent_ABC >& GetAgents() const = 0;
    //@}
};

}

#endif // __Automat_ABC_h_
