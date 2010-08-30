// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Sendable_h_
#define __Sendable_h_

#include "SimpleEntity.h"

namespace kernel
{
    class ModelVisitor_ABC;
}

namespace dispatcher
{
    class ClientPublisher_ABC;

// =============================================================================
/** @class  Sendable
    @brief  Sendable
*/
// Created: MGD 2009-12-18
// =============================================================================
template< typename B = kernel::Entity_ABC >
class Sendable : public SimpleEntity< B >
{
public :
    //! @name Constructors/Destructor
    //@{
    explicit Sendable( unsigned long id, const QString& name = "" )
        : SimpleEntity( id, name )
    {
        // NOTHING
    }
    virtual ~Sendable()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual void SendCreation( ClientPublisher_ABC& publisher ) const = 0;
    virtual void SendFullUpdate( ClientPublisher_ABC& publisher ) const = 0;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const = 0;
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const = 0;
    //@}
};

}

#endif // __Sendable_h_
