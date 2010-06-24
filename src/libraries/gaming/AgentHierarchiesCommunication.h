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

namespace MsgsSimToClient
{
    class MsgUnitCreation;
    class MsgUnitAttributes;
}
namespace Common
{
    class MsgUnitChangeSuperior;
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
                                    , public kernel::Updatable_ABC< MsgsSimToClient::MsgUnitAttributes >
{
public:
    //! @name Constructors/Destructor
    //@{
             AgentHierarchiesCommunication( kernel::Controller& controller, kernel::Agent_ABC& holder,
                               const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver,
                               const tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& groupResolver );
    virtual ~AgentHierarchiesCommunication();
    //@}

    virtual bool CanCommunicate() const;
    void DoUpdate( const MsgsSimToClient::MsgUnitCreation& message );
    virtual void DoUpdate( const MsgsSimToClient::MsgUnitAttributes& message );
    void DoUpdate( const Common::MsgUnitChangeSuperior& message );

private:
    bool        jammed_;
    bool        radioReceiverDisabled_;
    bool        radioEmitterDisabled_;
    kernel::Automat_ABC* superior_;
    const tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& groupResolver_;
};



#endif // __AgentHierarchiesCommunication_h_
