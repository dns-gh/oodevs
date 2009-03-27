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
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_Activation.cpp $
// $Author: Age $
// $Modtime: 8/03/05 15:11 $
// $Revision: 2 $
// $Workfile: HLA_Activation.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "HLA_Activation.h"
#include <hla/Interaction.h>
#include <hla/Attribute.h>
#include "HLA_Serialization.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
using namespace hla;

// -----------------------------------------------------------------------------
// Name: HLA_Activation constructor
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
HLA_Activation::HLA_Activation()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_Activation constructor
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
HLA_Activation::HLA_Activation( const HLA_Object_ABC& targetObject, bool bActivate )
    : HLA_ObjectInteraction( targetObject )
    , bActivate_( bActivate )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_Activation destructor
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
HLA_Activation::~HLA_Activation()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_Activation::Execute
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
void HLA_Activation::Execute() const
{
    MIL_Object_ABC* pObject = GetObject();
    if( pObject )
    {
        if( bActivate_ )
            (*pObject)().Activate(); // $$$$ AGE 2004-12-06: Effect ?
//        else
//            pObject->Prepare( 0 ); // $$$$ AGE 2004-12-06: Effect ?. I also know the internal object mechanism
    }
    // $$$$ NLD 2007-05-23: A Refaire
}

// -----------------------------------------------------------------------------
// Name: Interaction< HLA_Activation >& HLA_Activation::CreateInteractionClass
// Created: AGE 2004-12-06
// -----------------------------------------------------------------------------
Interaction< HLA_Activation >& HLA_Activation::CreateInteractionClass( InteractionNotification_ABC< HLA_Activation >& callback )
{
    Interaction< HLA_Activation >& interaction = *new Interaction< HLA_Activation >( callback );
    HLA_ObjectInteraction::RegisterAttributes( interaction );
    interaction.Register( ParameterIdentifier( "activer" ), CreateAttribute( &HLA_Activation::bActivate_ ) );
    return interaction;
}
