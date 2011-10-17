 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogConsignFuneral_h_
#define __LogConsignFuneral_h_

#include "SimpleEntity.h"
#include "tools/Resolver.h"
#include "protocol/ClientSenders.h"
#include <boost/optional.hpp>

namespace kernel
{
    class Entity_ABC;
    class Agent_ABC;
    class ModelVisitor_ABC;
}

namespace dispatcher
{
    class Agent;
    class Model;
    class ClientPublisher_ABC;
    class LogSupplyRecipientResourcesRequest;

// =============================================================================
/** @class  LogConsignFuneral
    @brief  LogConsignFuneral
*/
// Created: NLD 2006-09-19
// =============================================================================
class LogConsignFuneral : public SimpleEntity< >
{
public:
    //! @name Constructors/Destructor
    //@{F
             LogConsignFuneral( const Model& model, const sword::LogFuneralHandlingCreation& msg );
    virtual ~LogConsignFuneral();
    //@}

    //! @name Operations
    //@{
    using kernel::Entity_ABC::Update;
    void Update( const sword::LogFuneralHandlingUpdate& msg );
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;
    void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogConsignFuneral( const LogConsignFuneral& );            //!< Copy constructor
    LogConsignFuneral& operator=( const LogConsignFuneral& ); //!< Assignment operator
    //@}
    //! @name Tools
    //@{
    kernel::Entity_ABC* FindLogEntity(const sword::ParentEntity& msg) const;
    void FillLogEntityID(sword::ParentEntity& msg, const kernel::Entity_ABC* entity) const;
    //@}

private:
    const Model&               model_;
    const unsigned long        nTickCreation_;

    const kernel::Agent_ABC*         pRequestingUnit_;
    const kernel::Entity_ABC*        pHandlingEntity_;
    const kernel::Agent_ABC*         pConvoy_;
    sword::LogFuneralHandlingUpdate::EnumLogFuneralHandlingStatus nState_;
    unsigned long currentStateEndTick_;
    boost::optional< unsigned > packaging_;
    const sword::EnumHumanRank rank_;
};

}

#endif // __LogConsignFuneral_h_
