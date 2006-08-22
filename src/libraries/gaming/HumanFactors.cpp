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
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/DataDictionary.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: HumanFactors constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
HumanFactors::HumanFactors( Controller& controller, DataDictionary& dictionary )
    : controller_( controller )
    , experience_( (E_UnitExperience)0 )
    , tiredness_( (E_UnitFatigue)0 )
    , morale_( (E_UnitMoral)0 )
{
    dictionary.Register( "Facteurs Humains/Experience", experience_ );
    dictionary.Register( "Facteurs Humains/Fatigue", tiredness_ );
    dictionary.Register( "Facteurs Humains/Moral", morale_ );
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
        morale_ = (E_UnitMoral)message.moral;

    if( message.m.experiencePresent )
        experience_ = (E_UnitExperience)message.experience;

    if( message.m.fatiguePresent )
        tiredness_ = (E_UnitFatigue)message.fatigue;

    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: HumanFactors::Display
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void HumanFactors::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( "Facteurs humains" )
                .Display( "Experience:", experience_ )
                .Display( "Moral:", morale_ )
                .Display( "Fatigue:", tiredness_ );
}
   
// -----------------------------------------------------------------------------
// Name: HumanFactors::GetExperience
// Created: SBO 2006-06-27
// -----------------------------------------------------------------------------
E_UnitExperience HumanFactors::GetExperience() const
{
    return experience_;
}

// -----------------------------------------------------------------------------
// Name: HumanFactors::GetTiredness
// Created: SBO 2006-06-27
// -----------------------------------------------------------------------------
E_UnitFatigue HumanFactors::GetTiredness() const
{
    return tiredness_;
}

// -----------------------------------------------------------------------------
// Name: HumanFactors::GetMorale
// Created: SBO 2006-06-27
// -----------------------------------------------------------------------------
E_UnitMoral HumanFactors::GetMorale() const
{
    return morale_;
}
