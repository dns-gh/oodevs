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
                           , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationKnowledgeCreation >
                           , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationKnowledgeUpdate >
                           , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationKnowledgeDestruction >
                           , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationConcentrationKnowledgeCreation >
                           , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationConcentrationKnowledgeUpdate >
                           , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationConcentrationKnowledgeDestruction >
                           , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationFlowKnowledgeCreation >
                           , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationFlowKnowledgeUpdate >
                           , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationFlowKnowledgeDestruction >
                           , public tools::Resolver< kernel::PopulationKnowledge_ABC >
                           , public kernel::Creatable< PopulationKnowledges >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationKnowledges( kernel::Controller& controller, const kernel::KnowledgeGroup_ABC& owner, PopulationKnowledgeFactory_ABC& factory );
    virtual ~PopulationKnowledges();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationKnowledges( const PopulationKnowledges& );            //!< Copy constructor
    PopulationKnowledges& operator=( const PopulationKnowledges& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationKnowledgeCreation&                 message );

    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationKnowledgeUpdate&                   message );
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationKnowledgeDestruction&              message );
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationConcentrationKnowledgeCreation&    message );
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationConcentrationKnowledgeUpdate&      message );
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationConcentrationKnowledgeDestruction& message );
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationFlowKnowledgeCreation&             message );
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationFlowKnowledgeUpdate&               message );
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationFlowKnowledgeDestruction&          message );

    template< typename T >
    void UpdatePopulation( const T& message );
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
