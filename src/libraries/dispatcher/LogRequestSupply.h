 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __LogRequestSupply_h_
#define __LogRequestSupply_h_

#include "SimpleEntity.h"
#include "protocol/ClientSenders.h"
#include <tools/Resolver.h>

namespace kernel
{
    class ModelVisitor_ABC;
}

namespace dispatcher
{
    class ClientPublisher_ABC;
    class Model;

// =============================================================================
/** @class  LogRequestSupply
    @brief  LogRequestSupply
*/
// Created: LGY 2014-04-30
// =============================================================================
class LogRequestSupply : public SimpleEntity< >
{
public:
    //! @name Constructors/Destructor
    //@{F
             LogRequestSupply( const Model& model, const sword::LogSupplyRequestCreation& msg );
    virtual ~LogRequestSupply();
    //@}

    //! @name Operations
    //@{
    using kernel::Entity_ABC::Update;
    void Update( const sword::LogSupplyRequestUpdate& msg );
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;
    void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

private:
    const unsigned int nDotationType_;
    const unsigned int requester_;
    sword::LogSupplyRequestUpdate::EnumLogSupplyRequestStatus nState_;
    unsigned int recipient_;
    unsigned int supplier_;
    unsigned int requested_;
    unsigned int granted_;
    unsigned int convoyed_;
    unsigned int delivered_;
};

}

#endif // __LogRequestSupply_h_
