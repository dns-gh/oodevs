// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "HLA_Mining.h"
#include "HLA_Serialization.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include <hla/Interaction.h>
#include <hla/Attribute.h>

using namespace hla;

// -----------------------------------------------------------------------------
// Name: HLA_Mining constructor
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
HLA_Mining::HLA_Mining()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_Mining constructor
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
HLA_Mining::HLA_Mining( const HLA_Object_ABC& object, double rDelta )
    : HLA_ObjectInteraction( object )
    , rDelta_( rDelta )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_Mining destructor
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
HLA_Mining::~HLA_Mining()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_Mining::Execute
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
void HLA_Mining::Execute() const
{
    MIL_Object_ABC* pObject = GetObject();
    if( pObject )
    {
        if( rDelta_ > 0 )
            (*pObject)().Mine( rDelta_ ); // $$$$ AGE 2004-12-06: Effect ?
        else
            (*pObject)().Demine( -rDelta_ ); // $$$$ AGE 2004-12-06: Effect ?
    }
}

// -----------------------------------------------------------------------------
// Name: Interaction< HLA_Mining >& HLA_Mining::CreateInteractionClass
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
Interaction< HLA_Mining >& HLA_Mining::CreateInteractionClass( InteractionNotification_ABC< HLA_Mining >& callback )
{
    Interaction< HLA_Mining >& interaction = *new Interaction< HLA_Mining >( callback );
    HLA_ObjectInteraction::RegisterAttributes( interaction );
    interaction.Register( ParameterIdentifier( "pourcentage" ), CreateAttribute( &HLA_Mining::rDelta_ ) );
    return interaction;
}
