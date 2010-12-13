// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "HumanFactors.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: HumanFactors constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
HumanFactors::HumanFactors( Controller& controller, PropertiesDictionary& dictionary )
    : controller_( controller )
    , experience_( (E_UnitExperience)0 )
    , tiredness_( (E_UnitTiredness)0 )
    , morale_( (E_UnitMorale)0 )
{
    CreateDictionary( dictionary );
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
// Name: HumanFactors::CreateDictionary
// Created: SBO 2006-10-19
// -----------------------------------------------------------------------------
void HumanFactors::CreateDictionary( kernel::PropertiesDictionary& dictionary ) const
{
    dictionary.Register( *this, tools::translate( "Human factors", "Human factors/Experience" ), experience_ );
    dictionary.Register( *this, tools::translate( "Human factors", "Human factors/Tiredness" ), tiredness_ );
    dictionary.Register( *this, tools::translate( "Human factors", "Human factors/Moral" ), morale_ );
}

// -----------------------------------------------------------------------------
// Name: HumanFactors::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void HumanFactors::DoUpdate( const sword::UnitAttributes& message )
{
    if( message.has_moral()  )
        morale_ = (E_UnitMorale)message.moral();

    if( message.has_experience()  )
        experience_ = (E_UnitExperience)message.experience();

    if( message.has_fatigue()  )
        tiredness_ = (E_UnitTiredness)message.fatigue();

    controller_.Update( *(HumanFactors_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: HumanFactors::Display
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void HumanFactors::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Human factors", "Human factors" ) )
                .Display( tools::translate( "Human factors", "Experience:" ), experience_ )
                .Display( tools::translate( "Human factors", "Moral:" ), morale_ )
                .Display( tools::translate( "Human factors", "Tiredness:" ), tiredness_ );
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
E_UnitTiredness HumanFactors::GetTiredness() const
{
    return tiredness_;
}

// -----------------------------------------------------------------------------
// Name: HumanFactors::GetMorale
// Created: SBO 2006-06-27
// -----------------------------------------------------------------------------
E_UnitMorale HumanFactors::GetMorale() const
{
    return morale_;
}
