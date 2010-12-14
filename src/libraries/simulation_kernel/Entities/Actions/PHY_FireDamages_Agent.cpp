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
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Agent constructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_FireDamages_Agent::PHY_FireDamages_Agent()
    : humanResults_( PHY_HumanRank::GetHumanRanks().size() )
{
    const unsigned int nNbrWounds = PHY_HumanWound::GetHumanWounds().size();
    for( IT_HumansPerRankVector it = humanResults_.begin(); it != humanResults_.end(); ++it )
        it->resize( nNbrWounds, 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Agent destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_FireDamages_Agent::~PHY_FireDamages_Agent()
{
    // NOTHING
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
void PHY_FireDamages_Agent::NotifyHumanWoundChanged( const Human_ABC& human, const PHY_HumanWound& oldWound )
{
    T_HumansPerWoundVector& humanWounds = humanResults_[ human.GetRank().GetID() ];
    -- humanWounds[       oldWound  .GetID() ];
    ++ humanWounds[ human.GetWound().GetID() ];
}

//-----------------------------------------------------------------------------
// Name: PHY_FireDamages_Agent::Serialize
// Created: JVT 04-03-29
//-----------------------------------------------------------------------------
void PHY_FireDamages_Agent::Serialize( const MIL_Agent_ABC& target, sword::UnitFireDamages& asn ) const
{
    asn.mutable_target()->set_id( target.GetID() );

    asn.mutable_equipments();
    // Composantes
    for( CIT_ComposanteResults itResult = composanteResults_.begin(); itResult != composanteResults_.end(); ++itResult )
    {
        const PHY_ComposanteType_ABC& type   = *itResult->first;
        const T_ComposanteStates&      states =  itResult->second;

        sword::UnitEquipmentFireDamage& asnEquipement = *asn.mutable_equipments()->add_elem();
        asnEquipement.mutable_equipement_type()->set_id( type.GetMosID().id() );
        asnEquipement.set_available_nbr( states[ PHY_ComposanteState::undamaged_.GetID() ] );
        asnEquipement.set_repairable_nbr( states[ PHY_ComposanteState::repairableWithEvacuation_.GetID() ] + states[ PHY_ComposanteState::repairableWithoutEvacuation_.GetID() ] );
        asnEquipement.set_unavailable_nbr( states[ PHY_ComposanteState::dead_.GetID() ] );
    }

    asn.mutable_humans();
    // Humans
    for( PHY_HumanRank::CIT_HumanRankMap it = PHY_HumanRank::GetHumanRanks().begin(); it != PHY_HumanRank::GetHumanRanks().end(); ++it )
    {
        const PHY_HumanRank& rank = *it->second;
        sword::UnitHumanFireDamage& personnel = *asn.mutable_humans()->add_elem();
        const T_HumansPerWoundVector& wounds = humanResults_[ rank.GetID() ];

        personnel.set_rank           ( rank.GetAsnID() );
        personnel.set_alive_nbr      ( wounds[ PHY_HumanWound::notWounded_.GetID() ] );
        personnel.set_wounded_u1_nbr ( wounds[ PHY_HumanWound::woundedU1_ .GetID() ] );
        personnel.set_wounded_u2_nbr ( wounds[ PHY_HumanWound::woundedU2_ .GetID() ] );
        personnel.set_wounded_u3_nbr ( wounds[ PHY_HumanWound::woundedU3_ .GetID() ] );
        personnel.set_wounded_ue_nbr ( wounds[ PHY_HumanWound::woundedUE_ .GetID() ] );
        personnel.set_dead_nbr       ( wounds[ PHY_HumanWound::killed_    .GetID() ] );
    }
}
