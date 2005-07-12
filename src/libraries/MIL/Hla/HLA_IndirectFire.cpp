// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-29 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Hla/HLA_IndirectFire.cpp $
// $Author: Nld $
// $Modtime: 21/02/05 12:08 $
// $Revision: 3 $
// $Workfile: HLA_IndirectFire.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "HLA_IndirectFire.h"
#include "hla/Interaction.h"
#include "hla/Attribute.h"
#include "HLA_Serialization.h"

#include "Entities/Effects/MIL_Effect_HLAIndirectFire.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/MIL_EntityManager.h"
#include "MIL_AgentServer.h"

// -----------------------------------------------------------------------------
// Name: HLA_IndirectFire constructor
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
HLA_IndirectFire::HLA_IndirectFire()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_IndirectFire constructor
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
HLA_IndirectFire::HLA_IndirectFire( const HLA_RoleInterface& role, const PHY_DotationCategory& ammunition )
    : HLA_TargetedInteraction( role       )
    , HLA_FireInteraction    ( ammunition )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_IndirectFire destructor
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
HLA_IndirectFire::~HLA_IndirectFire()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: Interaction< HLA_IndirectFire >& HLA_IndirectFire::CreateInteractionClass
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
Interaction< HLA_IndirectFire >& HLA_IndirectFire::CreateInteractionClass( InteractionNotification_ABC< HLA_IndirectFire >& callback )
{
    Interaction< HLA_IndirectFire >& interaction = *new Interaction< HLA_IndirectFire >( callback );
    HLA_TargetedInteraction::RegisterAttributes( interaction );
    HLA_FireInteraction::RegisterAttributes( interaction );
    return interaction;
}

// -----------------------------------------------------------------------------
// Name: HLA_IndirectFire::Execute
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
void HLA_IndirectFire::Execute() const
{
    MIL_Agent_ABC* pTarget = GetTarget();
    if( ! pTarget )
        return;

    const PHY_DotationCategory* pCategory = GetAmmunition();
    if( ! pCategory )
        return;

    MIL_Effect_HLAIndirectFire* pFire = new MIL_Effect_HLAIndirectFire( *pCategory, *pTarget );
    MIL_AgentServer::GetWorkspace().GetEntityManager().GetEffectManager().Register( *pFire );
}
