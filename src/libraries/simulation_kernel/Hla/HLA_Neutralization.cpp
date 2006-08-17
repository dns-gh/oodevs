// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-12-02 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_Neutralization.cpp $
// $Author: Age $
// $Modtime: 2/12/04 11:58 $
// $Revision: 1 $
// $Workfile: HLA_Neutralization.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "HLA_Neutralization.h"
#include "hla/Interaction.h"
#include "hla/Attribute.h"
#include "HLA_Serialization.h"

#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/MIL_Agent_ABC.h"

// -----------------------------------------------------------------------------
// Name: HLA_Neutralization constructor
// Created: AGE 2004-12-02
// -----------------------------------------------------------------------------
HLA_Neutralization::HLA_Neutralization()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_Neutralization constructor
// Created: AGE 2004-12-02
// -----------------------------------------------------------------------------
HLA_Neutralization::HLA_Neutralization( const HLA_RoleInterface& role )
    : HLA_TargetedInteraction( role )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_Neutralization destructor
// Created: AGE 2004-12-02
// -----------------------------------------------------------------------------
HLA_Neutralization::~HLA_Neutralization()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: Interaction< HLA_Neutralization >& HLA_Neutralization::CreateInteractionClass
// Created: AGE 2004-12-02
// -----------------------------------------------------------------------------
Interaction< HLA_Neutralization >& HLA_Neutralization::CreateInteractionClass( InteractionNotification_ABC< HLA_Neutralization >& callback )
{
    Interaction< HLA_Neutralization >& interaction = *new Interaction< HLA_Neutralization >( callback );
    HLA_TargetedInteraction::RegisterAttributes( interaction );
    return interaction;
}

// -----------------------------------------------------------------------------
// Name: HLA_Neutralization::Execute
// Created: AGE 2004-12-02
// -----------------------------------------------------------------------------
void HLA_Neutralization::Execute() const
{
    MIL_Agent_ABC* pTarget = GetTarget();
    if( ! pTarget )
        return;

    // $$$$ AGE 2004-12-02: Effect ?
    pTarget->GetRole< PHY_RoleInterface_Composantes >().Neutralize();
}
