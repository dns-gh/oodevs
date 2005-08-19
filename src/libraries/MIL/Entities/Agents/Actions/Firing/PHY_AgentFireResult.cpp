// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/PHY_AgentFireResult.cpp $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 6 $
// $Workfile: PHY_AgentFireResult.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_AgentFireResult.h"
#include "Entities/Agents/Units/Composantes/PHY_Composante_ABC.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteType_ABC.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteState.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/Units/Humans/PHY_Human.h"
#include "Entities/Agents/MIL_Agent_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_AgentFireResult constructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_AgentFireResult::PHY_AgentFireResult()
    : humanResults_( PHY_HumanRank::GetHumanRanks().size() )
{
    const uint nNbrWounds = PHY_HumanWound::GetHumanWounds().size();
    for( IT_HumansPerRankVector it = humanResults_.begin(); it != humanResults_.end(); ++it )
        it->resize( nNbrWounds, 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_AgentFireResult destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_AgentFireResult::~PHY_AgentFireResult()
{

}

// =============================================================================
// NOTIFICATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_AgentFireResult::NotifyComposanteStateChanged
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_AgentFireResult::NotifyComposanteStateChanged( const PHY_Composante_ABC& composante, const PHY_ComposanteState& oldState, const PHY_ComposanteState& newState )
{
    T_ComposanteStates& composanteStates = composanteResults_[ &composante.GetType() ];
    composanteStates.resize( PHY_ComposanteState::GetNbrStates(), 0 );

    -- composanteStates[ oldState.GetID() ];
    ++ composanteStates[ newState.GetID() ];
}

// -----------------------------------------------------------------------------
// Name: PHY_AgentFireResult::NotifyHumanWoundChanged
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_AgentFireResult::NotifyHumanWoundChanged( const PHY_Human& human, const PHY_HumanWound& oldWound )
{
    T_HumansPerWoundVector& humanWounds = humanResults_[ human.GetRank().GetID() ];
    -- humanWounds[       oldWound  .GetID() ];
    ++ humanWounds[ human.GetWound().GetID() ];
}

// =============================================================================
// NETWORK
// =============================================================================

//-----------------------------------------------------------------------------
// Name: PHY_AgentFireResult::Serialize
// Created: JVT 04-03-29
//-----------------------------------------------------------------------------
void PHY_AgentFireResult::Serialize( const MIL_Agent_ABC& target, ASN1T_FireResult& asn ) const
{
    asn.oid_cible = target.GetID();

    // Composantes
    asn.resultats_equipements.n = composanteResults_.size();
    if( asn.resultats_equipements.n )
    {
        asn.resultats_equipements.elem = new ASN1T_FireResultEquipement[ composanteResults_.size() ]; 
        uint i = 0;
        for( CIT_ComposanteResults itResult = composanteResults_.begin(); itResult != composanteResults_.end(); ++itResult, ++i )
        {
            const PHY_ComposanteType_ABC& type   = *itResult->first;
            const T_ComposanteStates&      states =  itResult->second;

            ASN1T_FireResultEquipement& asnEquipement = asn.resultats_equipements.elem[i];
            asnEquipement.type_equipement = type.GetMosID();
            asnEquipement.nb_disponibles   = states[ PHY_ComposanteState::undamaged_.GetID() ];
            asnEquipement.nb_reparables    = states[ PHY_ComposanteState::repairableWithEvacuation_.GetID() ] + states[ PHY_ComposanteState::repairableWithoutEvacuation_.GetID() ];
            asnEquipement.nb_indisponibles = states[ PHY_ComposanteState::dead_.GetID() ];
        }
    }

    // Humans
    asn.resultats_humains.n    = PHY_HumanRank::GetHumanRanks().size();
    asn.resultats_humains.elem = new ASN1T_FireResultHuman[ asn.resultats_humains.n ]; 

    uint i = 0;
    for( PHY_HumanRank::CIT_HumanRankMap it = PHY_HumanRank::GetHumanRanks().begin(); it != PHY_HumanRank::GetHumanRanks().end(); ++it, ++i )
    {
        const PHY_HumanRank& rank = *it->second;

        ASN1T_FireResultHuman& personnel = asn.resultats_humains.elem[ i ];

        const T_HumansPerWoundVector& wounds = humanResults_[ rank.GetID() ];

        personnel.rang                       = rank.GetAsnID();
        personnel.nb_non_blesses             = wounds[ PHY_HumanWound::notWounded_.GetID() ];
        personnel.nb_blesses_urgence_1       = wounds[ PHY_HumanWound::woundedU1_ .GetID() ];
        personnel.nb_blesses_urgence_2       = wounds[ PHY_HumanWound::woundedU2_ .GetID() ];
        personnel.nb_blesses_urgence_3       = wounds[ PHY_HumanWound::woundedU3_ .GetID() ];
        personnel.nb_blesses_urgence_extreme = wounds[ PHY_HumanWound::woundedUE_ .GetID() ];
        personnel.nb_morts                   = wounds[ PHY_HumanWound::killed_    .GetID() ];
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_AgentFireResult::CleanAfterSerialization
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_AgentFireResult::CleanAfterSerialization( ASN1T_FireResult& asn )
{
    if( asn.resultats_equipements.n > 0 )    
        delete [] asn.resultats_equipements.elem;

    if( asn.resultats_humains.n > 0 )
        delete [] asn.resultats_humains.elem;
}
