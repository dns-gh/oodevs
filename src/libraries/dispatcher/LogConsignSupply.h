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

#include "game_asn/Simulation.h"
#include "SimpleEntity.h"
#include "clients_kernel/Resolver.h"

namespace kernel
{
    class Automat_ABC;
    class Agent_ABC;
}

namespace dispatcher
{
    class Agent;
    class Automat;
    class Model;
    class ModelVisitor_ABC;
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
             LogConsignSupply( const Model& model, const ASN1T_MsgLogSupplyHandlingCreation& msg );
    virtual ~LogConsignSupply();
    //@}

    //! @name Operations
    //@{
    using kernel::Entity_ABC::Update;
    void Update( const ASN1T_MsgLogSupplyHandlingUpdate& msg );
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;
    void Accept( ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogConsignSupply( const LogConsignSupply& );            //!< Copy constructor
    LogConsignSupply& operator=( const LogConsignSupply& ); //!< Assignement operator
    //@}

private:
    const Model&               model_;
    const kernel::Automat_ABC& automat_;
    const unsigned long        nTickCreation_;
          
    const kernel::Automat_ABC*        pTreatingAutomat_;
    const kernel::Automat_ABC*        pConvoyingAutomat_;
    const kernel::Agent_ABC*          pConvoy_;
    ASN1T_EnumLogSupplyHandlingStatus nState_;
    kernel::Resolver< LogSupplyDotation > dotations_;
};

}

#endif // __LogConsignSupply_h_
