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
#include "Entities/Agents/Units/Humans/Human_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Agent constructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_FireDamages_Agent::PHY_FireDamages_Agent()
    : humanResults_(
        PHY_HumanRank::GetHumanRanks().size(),
        T_HumansPerWoundVector( PHY_HumanWound::GetHumanWounds().size() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Agent destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_FireDamages_Agent::~PHY_FireDamages_Agent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Agent::NotifyComposanteStateChanged
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_FireDamages_Agent::NotifyComposanteStateChanged( const PHY_Composante_ABC& composante, const PHY_ComposanteState& oldState, const PHY_ComposanteState& newState )
{
    T_ComposanteStates& composanteStates = composanteResults_[ &composante.GetType() ];
    composanteStates.resize( PHY_ComposanteState::GetNbrStates(), 0 );
    --composanteStates[ oldState.GetID() ];
    ++composanteStates[ newState.GetID() ];
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
    DoSerializeDamages( asn );
}

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Agent::SerializeDamages
// Created: JSR 2011-05-13
// -----------------------------------------------------------------------------
void PHY_FireDamages_Agent::SerializeDamages( sword::UnitDamagedByUnitFire& msg ) const
{
    DoSerializeDamages( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Agent::SerializeDamages
// Created: JSR 2011-05-13
// -----------------------------------------------------------------------------
void PHY_FireDamages_Agent::SerializeDamages( sword::UnitDamagedByCrowdFire& msg ) const
{
    DoSerializeDamages( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_FireDamages_Agent::DoSerializeDamages
// Created: JSR 2011-05-13
// -----------------------------------------------------------------------------
template< typename T >
void PHY_FireDamages_Agent::DoSerializeDamages( T& msg ) const
{
    msg.mutable_equipments();
    // Composantes
    for( auto it = composanteResults_.begin(); it != composanteResults_.end(); ++it )
    {
        const PHY_ComposanteType_ABC& type = *it->first;
        const T_ComposanteStates& states = it->second;
        sword::UnitEquipmentFireDamage& asnEquipement = *msg.mutable_equipments()->add_elem();
        asnEquipement.mutable_equipement()->set_id( type.GetMosID().id() );
        asnEquipement.set_available( states[ PHY_ComposanteState::undamaged_.GetID() ] );
        asnEquipement.set_repairable( states[ PHY_ComposanteState::repairableWithEvacuation_.GetID() ] + states[ PHY_ComposanteState::repairableWithoutEvacuation_.GetID() ] );
        asnEquipement.set_unavailable( states[ PHY_ComposanteState::dead_.GetID() ] );
    }
    msg.mutable_humans();
    // Humans
    for( auto it = PHY_HumanRank::GetHumanRanks().begin(); it != PHY_HumanRank::GetHumanRanks().end(); ++it )
    {
        const PHY_HumanRank& rank = *it->second;
        sword::UnitHumanFireDamage& personnel = *msg.mutable_humans()->add_elem();
        const T_HumansPerWoundVector& wounds = humanResults_[ rank.GetID() ];
        personnel.set_rank( rank.GetAsnID() );
        personnel.set_alive( wounds[ PHY_HumanWound::notWounded_.GetID() ] );
        personnel.set_wounded_u1( wounds[ PHY_HumanWound::woundedU1_.GetID() ] );
        personnel.set_wounded_u2( wounds[ PHY_HumanWound::woundedU2_.GetID() ] );
        personnel.set_wounded_u3( wounds[ PHY_HumanWound::woundedU3_.GetID() ] );
        personnel.set_wounded_ue( wounds[ PHY_HumanWound::woundedUE_.GetID() ] );
        personnel.set_dead( wounds[ PHY_HumanWound::killed_.GetID() ] );
    }
}

void PHY_FireDamages_Agent::VisitComponents( const std::function< void( const std::string&, unsigned int ,unsigned int, unsigned int ) >& visitor ) const
{
    for( auto it = composanteResults_.begin(); it != composanteResults_.end(); ++it )
        visitor(
            it->first->GetName(),
            it->second[ PHY_ComposanteState::repairableWithEvacuation_.GetID() ],
            it->second[ PHY_ComposanteState::repairableWithoutEvacuation_.GetID() ],
            it->second[ PHY_ComposanteState::dead_.GetID() ] );
}

unsigned int PHY_FireDamages_Agent::GetWounded() const
{
    unsigned int result = 0;
    for( auto it = PHY_HumanRank::GetHumanRanks().begin(); it != PHY_HumanRank::GetHumanRanks().end(); ++it )
    {
        const auto& wounds = humanResults_[ it->second->GetID() ];
        result += wounds[ PHY_HumanWound::woundedU1_.GetID() ]
            + wounds[ PHY_HumanWound::woundedU2_.GetID() ]
            + wounds[ PHY_HumanWound::woundedU3_.GetID() ]
            + wounds[ PHY_HumanWound::woundedUE_.GetID() ];
    }
    return result;
}

unsigned int PHY_FireDamages_Agent::GetDead() const
{
    unsigned int result = 0;
    for( auto it = PHY_HumanRank::GetHumanRanks().begin(); it != PHY_HumanRank::GetHumanRanks().end(); ++it )
        result += humanResults_[ it->second->GetID() ][ PHY_HumanWound::killed_.GetID() ];
    return result;
}
