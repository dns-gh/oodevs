// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Availability_h_
#define __Availability_h_

#include "ASN_Types.h"
#include "clients_kernel/Resolver_ABC.h"

class EquipmentType;
class Displayer_ABC;

// =============================================================================
/** @class  Availability
    @brief  Availability
*/
// Created: AGE 2006-02-28
// =============================================================================
class Availability
{

public:
    //! @name Constructors/Destructor
    //@{
             Availability();
             template< typename Message >
             Availability( const Resolver_ABC< EquipmentType >& resolver, const Message& message )
                : type_     ( & resolver.Get( message.type_equipement ) )
                , total_    ( message.nbr_total )
                , available_( message.nbr_disponibles ) 
                , atWork_   ( message.nbr_au_travail )
                , atRest_   ( message.m.nbr_au_reposPresent ? message.nbr_au_repos : 0 )
             {};
    virtual ~Availability();
    //@}

    //! @name Operations
    //@{
    void Display( Displayer_ABC& displayer ) const;
    //@}

public:
    //! @name Member data
    //@{
    const EquipmentType* type_;
    unsigned int total_;
    unsigned int available_;
    unsigned int atWork_;
    unsigned int atRest_;
    //@}
};

#endif // __Availability_h_
