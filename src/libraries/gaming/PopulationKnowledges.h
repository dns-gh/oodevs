// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationKnowledges_h_
#define __PopulationKnowledges_h_

#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Creatable.h"
#include "protocol/Protocol.h"
#include "tools/Resolver.h"

namespace kernel
{
    class Controller;
    class KnowledgeGroup_ABC;
    class PopulationKnowledge_ABC;
}

class PopulationKnowledgeFactory_ABC;

// =============================================================================
/** @class  PopulationKnowledges
    @brief  PopulationKnowledges
*/
// Created: AGE 2006-02-15
// =============================================================================
class PopulationKnowledges : public kernel::Extension_ABC
                           , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdKnowledgeCreation >
                           , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdKnowledgeUpdate >
                           , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdKnowledgeDestruction >
                           , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdConcentrationKnowledgeCreation >
                           , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdConcentrationKnowledgeUpdate >
                           , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdConcentrationKnowledgeDestruction >
                           , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdFlowKnowledgeCreation >
                           , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdFlowKnowledgeUpdate >
                           , public kernel::Updatable_ABC< MsgsSimToClient::MsgCrowdFlowKnowledgeDestruction >
                           , public tools::Resolver< kernel::PopulationKnowledge_ABC >
                           , public kernel::Creatable< PopulationKnowledges >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationKnowledges( kernel::Controller& controller, const kernel::KnowledgeGroup_ABC& owner, PopulationKnowledgeFactory_ABC& factory );
    virtual ~PopulationKnowledges();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationKnowledges( const PopulationKnowledges& );            //!< Copy constructor
    PopulationKnowledges& operator=( const PopulationKnowledges& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdKnowledgeCreation&                 message );

    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdKnowledgeUpdate&                   message );
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdKnowledgeDestruction&              message );
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdConcentrationKnowledgeCreation&    message );
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdConcentrationKnowledgeUpdate&      message );
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdConcentrationKnowledgeDestruction& message );
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdFlowKnowledgeCreation&             message );
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdFlowKnowledgeUpdate&               message );
    virtual void DoUpdate( const MsgsSimToClient::MsgCrowdFlowKnowledgeDestruction&          message );

    template< typename T >
    void UpdatePopulation( const T& message );
    template< typename T >
    void UpdatePopulationPart( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::KnowledgeGroup_ABC& owner_;
    PopulationKnowledgeFactory_ABC& factory_;
    kernel::Controller& controller_;
    //@}
};

#endif // __PopulationKnowledges_h_
