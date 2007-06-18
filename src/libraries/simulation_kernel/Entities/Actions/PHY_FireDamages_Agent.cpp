// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/PHY_FireDamages_Agent.cpp $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 6 $
// $Workfile: PHY_FireDamages_Agent.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_FireDamages_Agent.h"
#include "Entities/Agents/Units/Composantes/PHY_Composante_ABC.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteType_ABC.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteState.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/Units/Humans/PHY_Human.h"
#include "Entities/Agents/MIL_Agent_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Agent constructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_FireDamages_Agent::PHY_FireDamages_Agent()
    : humanResults_( PHY_HumanRank::GetHumanRanks().size() )
{
    const uint nNbrWounds = PHY_HumanWound::GetHumanWounds().size();
    for( IT_HumansPerRankVector it = humanResults_.begin(); it != humanResults_.end(); ++it )
        it->resize( nNbrWounds, 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Agent destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_FireDamages_Agent::~PHY_FireDamages_Agent()
{

}

// =============================================================================
// NOTIFICATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Agent::NotifyComposanteStateChanged
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_FireDamages_Agent::NotifyComposanteStateChanged( const PHY_Composante_ABC& composante, const PHY_ComposanteState& oldState, const PHY_ComposanteState& newState )
{
    T_ComposanteStates& composanteStates = composanteResults_[ &composante.GetType() ];
    composanteStates.resize( PHY_ComposanteState::GetNbrStates(), 0 );

    -- composanteStates[ oldState.GetID() ];
    ++ composanteStates[ newState.GetID() ];
}

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Agent::NotifyHumanWoundChanged
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_FireDamages_Agent::NotifyHumanWoundChanged( const PHY_Human& human, const PHY_HumanWound& oldWound )
{
    T_HumansPerWoundVector& humanWounds = humanResults_[ human.GetRank().GetID() ];
    -- humanWounds[       oldWound  .GetID() ];
    ++ humanWounds[ human.GetWound().GetID() ];
}

// =============================================================================
// NETWORK
// =============================================================================

//-----------------------------------------------------------------------------
// Name: PHY_FireDamages_Agent::Serialize
// Created: JVT 04-03-29
//-----------------------------------------------------------------------------
void PHY_FireDamages_Agent::Serialize( const MIL_Agent_ABC& target, ASN1T_UnitFireDamages& asn ) const
{
    asn.cible = target.GetID();

    // Composantes
    asn.equipements.n = composanteResults_.size();
    if( asn.equipements.n )
    {
        asn.equipements.elem = new ASN1T_UnitEquipmentFireDamage[ composanteResults_.size() ]; 
        uint i = 0;
        for( CIT_ComposanteResults itResult = composanteResults_.begin(); itResult != composanteResults_.end(); ++itResult, ++i )
        {
            const PHY_ComposanteType_ABC& type   = *itResult->first;
            const T_ComposanteStates&      states =  itResult->second;

            ASN1T_UnitEquipmentFireDamage& asnEquipement = asn.equipements.elem[i];
            asnEquipement.type_equipement = type.GetMosID();
            asnEquipement.nb_disponibles   = states[ PHY_ComposanteState::undamaged_.GetID() ];
            asnEquipement.nb_reparables    = states[ PHY_ComposanteState::repairableWithEvacuation_.GetID() ] + states[ PHY_ComposanteState::repairableWithoutEvacuation_.GetID() ];
            asnEquipement.nb_indisponibles = states[ PHY_ComposanteState::dead_.GetID() ];
        }
    }

    // Humans
    asn.humains.n    = PHY_HumanRank::GetHumanRanks().size();
    asn.humains.elem = new ASN1T_UnitHumanFireDamage[ asn.humains.n ]; 

    uint i = 0;
    for( PHY_HumanRank::CIT_HumanRankMap it = PHY_HumanRank::GetHumanRanks().begin(); it != PHY_HumanRank::GetHumanRanks().end(); ++it, ++i )
    {
        const PHY_HumanRank& rank = *it->second;

        ASN1T_UnitHumanFireDamage& personnel = asn.humains.elem[ i ];

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
// Name: PHY_FireDamages_Agent::CleanAfterSerialization
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_FireDamages_Agent::CleanAfterSerialization( ASN1T_UnitFireDamages& asn )
{
    if( asn.equipements.n > 0 )    
        delete [] asn.equipements.elem;

    if( asn.humains.n > 0 )
        delete [] asn.humains.elem;
}
