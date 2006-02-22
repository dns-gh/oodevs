// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "HumanFactors.h"
#include "Experience.h"
#include "Tiredness.h"
#include "Morale.h"
#include "Controller.h"
#include "Displayer_ABC.h"

// -----------------------------------------------------------------------------
// Name: HumanFactors constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
HumanFactors::HumanFactors( Controller& controller )
     : controller_( controller )
{

}

// -----------------------------------------------------------------------------
// Name: HumanFactors destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
HumanFactors::~HumanFactors()
{

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
                .Display( "Experience:", pExperience_->GetName().c_str() )
                .Display( "Moral:", pMorale_->GetName().c_str() )
                .Display( "Fatigue:", pTiredness_->GetName().c_str() );
}
   
