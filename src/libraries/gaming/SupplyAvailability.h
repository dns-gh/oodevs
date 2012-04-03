// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyAvailability_h_
#define __SupplyAvailability_h_

#include "tools/Resolver_ABC.h"

namespace kernel
{
    class EquipmentType;
    class Displayer_ABC;
}

// =============================================================================
/** @class  SupplyAvailability
    @brief  SupplyAvailability
*/
// Created: AGE 2006-02-28
// =============================================================================
class SupplyAvailability
{
public:
    //! @name Constructors/Destructor
    //@{
     SupplyAvailability();
     template< typename Message >
     SupplyAvailability( const tools::Resolver_ABC< kernel::EquipmentType >& resolver, const Message& message )
        : type_     ( & resolver.Get( message.equipment().id() ) )
        , total_    ( message.total() )
        , available_( message.available() )
        , atWork_   ( message.working() )
        , atRest_   ( message.has_resting() ? message.resting() : 0 )
     {}
    virtual ~SupplyAvailability();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer ) const;
    //@}

public:
    //! @name Member data
    //@{
    const kernel::EquipmentType* type_;
    unsigned int total_;
    unsigned int available_;
    unsigned int atWork_;
    unsigned int atRest_;
    //@}
};

#endif // __SupplyAvailability_h_
