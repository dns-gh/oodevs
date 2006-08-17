// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "HumanFactors.h"
#include "Experience.h"
#include "Tiredness.h"
#include "Morale.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/DataDictionary.h"

// -----------------------------------------------------------------------------
// Name: HumanFactors constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
HumanFactors::HumanFactors( Controller& controller, DataDictionary& dictionary )
    : controller_( controller )
    , pExperience_( 0 )
    , pTiredness_( 0 )
    , pMorale_( 0 )
{
    dictionary.Register( "Facteurs Humains/Experience", pExperience_ );
    dictionary.Register( "Facteurs Humains/Fatigue", pTiredness_ );
    dictionary.Register( "Facteurs Humains/Moral", pMorale_ );
}

// -----------------------------------------------------------------------------
// Name: HumanFactors destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
HumanFactors::~HumanFactors()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HumanFactors::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void HumanFactors::DoUpdate( const ASN1T_MsgUnitAttributes& message )
{
    if( message.m.moralPresent )
        pMorale_ = Morale::Find( message.moral );

    if( message.m.experiencePresent )
        pExperience_ = Experience::Find( message.experience );

    if( message.m.fatiguePresent )
        pTiredness_ = Tiredness::Find( message.fatigue );

    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: HumanFactors::Display
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void HumanFactors::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( "Facteurs humains" )
                .Display( "Experience:", pExperience_ )
                .Display( "Moral:", pMorale_ )
                .Display( "Fatigue:", pTiredness_ );
}
   
// -----------------------------------------------------------------------------
// Name: HumanFactors::GetExperience
// Created: SBO 2006-06-27
// -----------------------------------------------------------------------------
const Experience& HumanFactors::GetExperience() const
{
    return *pExperience_;
}

// -----------------------------------------------------------------------------
// Name: HumanFactors::GetTiredness
// Created: SBO 2006-06-27
// -----------------------------------------------------------------------------
const Tiredness& HumanFactors::GetTiredness() const
{
    return *pTiredness_;
}

// -----------------------------------------------------------------------------
// Name: HumanFactors::GetMorale
// Created: SBO 2006-06-27
// -----------------------------------------------------------------------------
const Morale& HumanFactors::GetMorale() const
{
    return *pMorale_;
}
