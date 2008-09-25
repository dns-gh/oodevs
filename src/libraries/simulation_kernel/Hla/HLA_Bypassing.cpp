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
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_Bypassing.cpp $
// $Author: Age $
// $Modtime: 6/12/04 17:32 $
// $Revision: 1 $
// $Workfile: HLA_Bypassing.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "HLA_Bypassing.h"
#include <hla/Interaction.h>
#include <hla/Attribute.h>
#include "HLA_Serialization.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"

using namespace hla;

// -----------------------------------------------------------------------------
// Name: HLA_Bypassing constructor
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
HLA_Bypassing::HLA_Bypassing()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_Bypassing constructor
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
HLA_Bypassing::HLA_Bypassing( const HLA_Object_ABC& targetObject, double rDelta )
    : HLA_ObjectInteraction( targetObject )
    , rDelta_( rDelta )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_Bypassing destructor
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
HLA_Bypassing::~HLA_Bypassing()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_Bypassing::Execute
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
void HLA_Bypassing::Execute() const
{
    MIL_RealObject_ABC* pObject = GetObject();
    if( pObject )
        pObject->Bypass( rDelta_ ); // $$$$ AGE 2004-12-06: Effect ?
}

// -----------------------------------------------------------------------------
// Name: Interaction< HLA_Bypassing >& HLA_Bypassing::CreateInteractionClass
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
Interaction< HLA_Bypassing >& HLA_Bypassing::CreateInteractionClass( InteractionNotification_ABC< HLA_Bypassing >& callback )
{
    Interaction< HLA_Bypassing >& interaction = *new Interaction< HLA_Bypassing >( callback );
    HLA_ObjectInteraction::RegisterAttributes( interaction );
    interaction.Register( ParameterIdentifier( "pourcentage" ), CreateAttribute( &HLA_Bypassing::rDelta_ ) );
    return interaction;
}
