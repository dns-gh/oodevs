// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentKnowledge_ABC_h_
#define __AgentKnowledge_ABC_h_

#include "clients_kernel/Knowledge_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"

namespace kernel
{
    class Displayer_ABC;
    class Team_ABC;
    class Entity_ABC;
}

// =============================================================================
/** @class  AgentKnowledge_ABC
    @brief  Agent knowledge definition
*/
// Created: AGE 2006-10-16
// =============================================================================
class AgentKnowledge_ABC : public kernel::Knowledge_ABC
{
public:
    //! @name Static
    //@{
    static const QString typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             AgentKnowledge_ABC();
    virtual ~AgentKnowledge_ABC();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::Agent_ABC* GetEntity() const = 0;
    virtual const kernel::KnowledgeGroup_ABC& GetOwner() const = 0;

    virtual void Display( kernel::Displayer_ABC& displayer ) const = 0;
    //@}

    //! @name Operations
    //@{
    virtual QString GetTypeName() const;

    virtual void Select( kernel::ActionController& controller ) const;
    virtual void ContextMenu( kernel::ActionController& controller, const QPoint& where ) const;
    virtual void Activate( kernel::ActionController& controller ) const;
    //@}
};

#endif // __AgentKnowledge_ABC_h_
