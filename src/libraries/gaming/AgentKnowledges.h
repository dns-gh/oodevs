// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AgentKnowledges_h_
#define __AgentKnowledges_h_

#include "game_asn/Simulation.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Creatable.h"
#include "clients_kernel/Resolver.h"

namespace kernel
{
    class Controller;
    class KnowledgeGroup_ABC;
    class AgentKnowledge_ABC;
}

class AgentKnowledgeFactory_ABC;

// =============================================================================
/** @class  AgentKnowledges
    @brief  AgentKnowledges
*/
// Created: AGE 2006-02-15
// =============================================================================
class AgentKnowledges : public kernel::Extension_ABC
                      , public kernel::Updatable_ABC< ASN1T_MsgUnitKnowledgeCreation >
                      , public kernel::Updatable_ABC< ASN1T_MsgUnitKnowledgeUpdate >
                      , public kernel::Updatable_ABC< ASN1T_MsgUnitKnowledgeDestruction >
                      , public kernel::Resolver< kernel::AgentKnowledge_ABC >
                      , public kernel::Creatable< AgentKnowledges >
{

public:
    //! @name Constructors/Destructor
    //@{
             AgentKnowledges( kernel::Controller& controller, const kernel::KnowledgeGroup_ABC& group, AgentKnowledgeFactory_ABC& factory );
    virtual ~AgentKnowledges();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AgentKnowledges( const AgentKnowledges& );            //!< Copy constructor
    AgentKnowledges& operator=( const AgentKnowledges& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitKnowledgeCreation&    message );
    virtual void DoUpdate( const ASN1T_MsgUnitKnowledgeUpdate&      message );
    virtual void DoUpdate( const ASN1T_MsgUnitKnowledgeDestruction& message );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::KnowledgeGroup_ABC& group_;
    AgentKnowledgeFactory_ABC& factory_;
    //@}
};

#endif // __AgentKnowledges_h_
