 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogConsignSupply_h_
#define __LogConsignSupply_h_


#include "SimpleEntity.h"
#include "tools/Resolver.h"

namespace MsgsSimToClient
{
    enum EnumLogSupplyHandlingStatus;
    class MsgLogSupplyHandlingCreation;
    class MsgLogSupplyHandlingUpdate;
}

namespace kernel
{
    class Automat_ABC;
    class Agent_ABC;
    class ModelVisitor_ABC;
}

namespace dispatcher
{
    class Agent;
    class Automat;
    class Model;
    class LogSupplyDotation;
    class ClientPublisher_ABC;

// =============================================================================
/** @class  LogConsignSupply
    @brief  LogConsignSupply
*/
// Created: NLD 2006-09-19
// =============================================================================
class LogConsignSupply : public SimpleEntity< >
{
public:
    //! @name Constructors/Destructor
    //@{F
             LogConsignSupply( const Model& model, const MsgsSimToClient::MsgLogSupplyHandlingCreation& msg );
    virtual ~LogConsignSupply();
    //@}

    //! @name Operations
    //@{
    using kernel::Entity_ABC::Update;
    void Update( const MsgsSimToClient::MsgLogSupplyHandlingUpdate& msg );
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;
    void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogConsignSupply( const LogConsignSupply& );            //!< Copy constructor
    LogConsignSupply& operator=( const LogConsignSupply& ); //!< Assignment operator
    //@}

private:
    const Model&               model_;
    const kernel::Automat_ABC& automat_;
    const unsigned long        nTickCreation_;

    const kernel::Automat_ABC*        pTreatingAutomat_;
    const kernel::Automat_ABC*        pConvoyingAutomat_;
    const kernel::Agent_ABC*          pConvoy_;
    MsgsSimToClient::EnumLogSupplyHandlingStatus nState_;
    tools::Resolver< LogSupplyDotation > dotations_;
};

}

#endif // __LogConsignSupply_h_
