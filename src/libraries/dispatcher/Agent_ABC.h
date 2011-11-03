// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_Agent_ABC_h_
#define __dispatcher_Agent_ABC_h_

#include "Observable.h"
#include "Sendable.h"
#include "clients_kernel/Agent_ABC.h"
#include "protocol/Protocol.h"
#include "tools/Resolver.h"

namespace sword
{
    class UnitAttributes;
    class UnitEnvironmentType;
    class UnitPathFind;
}

namespace dispatcher
{
    class Automat_ABC;
    class Equipment;
    class Humans;
    class Order_ABC;

// =============================================================================
/** @class  Agent_ABC
    @brief  Agent_ABC
*/
// Created: SBO 2010-06-07
// =============================================================================
class Agent_ABC : public Sendable< kernel::Agent_ABC >
                , public Observable< sword::UnitAttributes >
                , public Observable< sword::UnitEnvironmentType >
                , public Observable< sword::UnitPathFind >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Agent_ABC( unsigned long id, const QString& name = "" )
        : Sendable< kernel::Agent_ABC >( id, name )
    {
        // NOTHING
    }
    virtual ~Agent_ABC()
    {
        // NOTHING
    }
    //@}

    //! @name Accessors
    //@{
    virtual const geometry::Point2d& GetPosition() const = 0;
    virtual const dispatcher::Automat_ABC& GetSuperior() const = 0;
    virtual sword::EnumOperationalStatus GetOperationalState() const = 0;
    virtual unsigned short GetOperationalStateValue() const = 0;
    virtual const tools::Resolver< dispatcher::Equipment >& Equipments() const = 0;
    virtual const std::vector< Humans >& Troops() const = 0;
    virtual unsigned short GetAltitude() const = 0;
    virtual unsigned short GetSpeed() const = 0;
    virtual unsigned short GetDirection() const = 0;
    virtual sword::ForceRatio_Value GetForceRatio() const = 0;
    virtual const Order_ABC* GetOrder() const = 0;
    virtual bool IsMounted() const = 0;
    virtual bool IsCommandPost() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void SetSuperior( dispatcher::Automat_ABC& superior ) = 0;
    //@}
};

}

#endif // __dispatcher_Agent_ABC_h_
