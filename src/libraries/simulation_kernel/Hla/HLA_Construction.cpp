// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-12-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_Construction.cpp $
// $Author: Age $
// $Modtime: 6/12/04 17:32 $
// $Revision: 1 $
// $Workfile: HLA_Construction.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "HLA_Construction.h"
#include <hla/Interaction.h>
#include <hla/Attribute.h>
#include "HLA_Serialization.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"

using namespace hla;

// -----------------------------------------------------------------------------
// Name: HLA_Construction constructor
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
HLA_Construction::HLA_Construction()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_Construction constructor
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
HLA_Construction::HLA_Construction( const HLA_Object_ABC& targetObject, double rDelta )
    : HLA_ObjectInteraction( targetObject )
    , rDelta_( rDelta )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_Construction destructor
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
HLA_Construction::~HLA_Construction()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_Construction::Execute
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
void HLA_Construction::Execute() const
{
    MIL_RealObject_ABC* pObject = GetObject();
    if( pObject )
    {
        if( rDelta_ > 0 )
            pObject->Construct( rDelta_ ); // $$$$ AGE 2004-12-06: Effect ?
        else
            pObject->Destroy( -rDelta_ ); // $$$$ AGE 2004-12-06: Effect ?
    }
}

// -----------------------------------------------------------------------------
// Name: Interaction< HLA_Construction >& HLA_Construction::CreateInteractionClass
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
Interaction< HLA_Construction >& HLA_Construction::CreateInteractionClass( InteractionNotification_ABC< HLA_Construction >& callback )
{
    Interaction< HLA_Construction >& interaction = *new Interaction< HLA_Construction >( callback );
    HLA_ObjectInteraction::RegisterAttributes( interaction );
    interaction.Register( ParameterIdentifier( "pourcentage" ), CreateAttribute( &HLA_Construction::rDelta_ ) );
    return interaction;
}
