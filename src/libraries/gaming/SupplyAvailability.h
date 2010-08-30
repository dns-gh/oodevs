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

#include "protocol/Protocol.h"
#include "tools/Resolver_ABC.h"

using namespace Common;

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
                , total_    ( message.nbr_total() )
                , available_( message.nbr_disponibles() ) 
                , atWork_   ( message.nbr_au_travail() )
                , atRest_   ( message.has_nbr_au_repos() ? message.nbr_au_repos() : 0 )
             {};
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
