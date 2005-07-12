// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Hla/HLA_DirectFire.cpp $
// $Author: Nld $
// $Modtime: 21/02/05 12:09 $
// $Revision: 7 $
// $Workfile: HLA_DirectFire.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "HLA_DirectFire.h"
#include "hla/Interaction.h"
#include "hla/Attribute.h"
#include "HLA_Serialization.h"

#include "Entities/Agents/Units/Composantes/PHY_Composante_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteTypePion.h"
#include "Entities/Agents/Actions/Firing/PHY_FireResults_ABC.h"
#include "Entities/Effects/MIL_Effect_DirectFire.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "MIL_AgentServer.h"

// -----------------------------------------------------------------------------
// Name: HLA_DirectFire constructor
// Created: AGE 2004-11-24
// -----------------------------------------------------------------------------
HLA_DirectFire::HLA_DirectFire()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_DirectFire constructor
// Created: AGE 2004-11-24
// -----------------------------------------------------------------------------
HLA_DirectFire::HLA_DirectFire( const HLA_RoleInterface& role, const PHY_Composante_ABC& composante, const PHY_DotationCategory& ammunition )
    : HLA_TargetedInteraction( role  )
    , HLA_FireInteraction( ammunition )
    , strTargetComposante_( composante.GetType().GetName() )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_DirectFire destructor
// Created: AGE 2004-11-24
// -----------------------------------------------------------------------------
HLA_DirectFire::~HLA_DirectFire()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_DirectFire::CreateInteractionClass
// Created: AGE 2004-11-24
// -----------------------------------------------------------------------------
Interaction< HLA_DirectFire >& HLA_DirectFire::CreateInteractionClass( InteractionNotification_ABC< HLA_DirectFire >& callback )
{
    Interaction< HLA_DirectFire >& interaction = *new Interaction< HLA_DirectFire >( callback );
    HLA_TargetedInteraction::RegisterAttributes( interaction );
    HLA_FireInteraction::RegisterAttributes( interaction );
    interaction.Register( ParameterIdentifier( "composante" ), CreateAttribute( &HLA_DirectFire::strTargetComposante_ ) );
    return interaction;
}

// -----------------------------------------------------------------------------
// Name: HLA_DirectFire::Execute
// Created: AGE 2004-11-26
// -----------------------------------------------------------------------------
void HLA_DirectFire::Execute() const
{
    MIL_Agent_ABC* pTarget = GetTarget();
    if( ! pTarget )
        return; // Normal behavior with more than 2 federates : the target may not be ours
    
    const PHY_DotationCategory* pCategory = GetAmmunition();
    if( ! pCategory )
        return;

    T_ComposanteVector totalTargets;
    pTarget->GetRole< PHY_RoleInterface_Composantes >().GetComposantesAbleToBeFired( totalTargets );

    PHY_FireResults_ABC* pFireResult = new PHY_FireResults_ABC();
    pFireResult->IncRef();
    ApplyFire( *pCategory, *pTarget, strTargetComposante_, totalTargets, *pFireResult );
    pFireResult->DecRef();
}

namespace 
{
    struct ComposanteTypeIsNot
    {
        ComposanteTypeIsNot( const std::string& strTargetComposante )
            : strTargetComposante_( strTargetComposante ) {}
        bool operator()( const PHY_Composante_ABC* pComposante )
        {
            return pComposante->GetType().GetName() != strTargetComposante_;
        }
        private:
            std::string strTargetComposante_;
    };
}

// -----------------------------------------------------------------------------
// Name: HLA_DirectFire::ApplyFire
// Created: AGE 2004-11-26
// -----------------------------------------------------------------------------
void HLA_DirectFire::ApplyFire( const PHY_DotationCategory& ammunition, MIL_Agent_ABC& target, const std::string& strTargetComposante, const T_ComposanteVector& allComposantes, PHY_FireResults_ABC& fireResult ) const
{
    T_ComposanteVector typeTargets;
    std::remove_copy_if( allComposantes.begin(), allComposantes.end(), std::back_inserter( typeTargets ), ComposanteTypeIsNot( strTargetComposante ) );
    std::random_shuffle( typeTargets.begin(), typeTargets.end() );
    if( ! typeTargets.empty() )
    {
        MIL_Effect_DirectFire* pFire = new MIL_Effect_DirectFire( ammunition, target, *typeTargets.front(), fireResult );
        MIL_AgentServer::GetWorkspace().GetEntityManager().GetEffectManager().Register( *pFire );
    }
}
