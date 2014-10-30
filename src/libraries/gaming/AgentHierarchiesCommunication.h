// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentHierarchiesCommunication_h_
#define __AgentHierarchiesCommunication_h_

#include "AgentHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"

namespace sword
{
    class UnitCreation;
    class UnitAttributes;
    class UnitChangeSuperior;
}

namespace kernel
{
    class CommunicationHierarchies;
    class Controller;
    class Automat_ABC;
    class AgentType;
}

// =============================================================================
/** @class  AgentHierarchiesCommunication
    @brief  AgentHierarchiesCommunication
*/
// Created: FDS 2010-03-29
// =============================================================================
class AgentHierarchiesCommunication : public AgentHierarchies< kernel::CommunicationHierarchies >
                                    , public kernel::Updatable_ABC< sword::UnitAttributes >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentHierarchiesCommunication( kernel::Controllers& controllers, kernel::Agent_ABC& holder,
                               const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver,
                               const tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& groupResolver );
    virtual ~AgentHierarchiesCommunication();
    //@}

    virtual bool CanCommunicate() const;
    virtual bool CanReceive() const;
    virtual bool IsJammed() const;
    void DoUpdate( const sword::UnitCreation& message );
    virtual void DoUpdate( const sword::UnitAttributes& message );
    void DoUpdate( const sword::UnitChangeSuperior& message );

private:
    bool        jammed_;
    bool        radioReceiverDisabled_;
    bool        radioEmitterDisabled_;
    const tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& groupResolver_;
    kernel::Controllers& controllers_;
};

#endif // __AgentHierarchiesCommunication_h_
