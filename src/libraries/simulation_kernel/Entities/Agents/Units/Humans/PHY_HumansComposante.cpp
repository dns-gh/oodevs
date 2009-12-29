// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Humans/PHY_HumansComposante.cpp $
// $Author: Jvt $
// $Modtime: 1/04/05 11:26 $
// $Revision: 12 $
// $Workfile: PHY_HumansComposante.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_HumansComposante.h"
#include "PHY_Human.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Actions/PHY_FireDamages_Agent.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Entities/Agents/Units/Humans/MIL_Injury_ABC.h"

#include "simulation_kernel/HumansActionsNotificationHandler_ABC.h"

BOOST_CLASS_EXPORT_GUID( PHY_HumansComposante, "PHY_HumansComposante" )

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante constructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_HumansComposante::PHY_HumansComposante( const MIL_Time_ABC& time, PHY_ComposantePion& composante, uint nNbrMdr )
    : pComposante_     ( &composante )
    , humans_          ()
    , nNbrUsableHumans_( 0 )
{
    while ( nNbrMdr-- )
        humans_.push_back( new PHY_Human( time, *this ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante constructor
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
PHY_HumansComposante::PHY_HumansComposante()
    : pComposante_     ( 0 )
    , humans_          ()
    , nNbrUsableHumans_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_HumansComposante::~PHY_HumansComposante()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::IsViable
// Created: NLD 2006-02-09
// -----------------------------------------------------------------------------
bool PHY_HumansComposante::IsViable() const
{
    return nNbrUsableHumans_ > 0 || pComposante_->GetRole().GetPion().IsAutonomous();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::ChangeHumanRank
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
bool PHY_HumansComposante::ChangeHumanRank( const PHY_HumanRank& oldRank, const PHY_HumanRank& newRank, const PHY_HumanWound& wound )
{
    if( oldRank == newRank )
        return false;

    for( PHY_Human::CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
    {
        PHY_Human& human = **it;
        if( human.GetRank() == oldRank && human.GetWound() == wound )
        {
            human.SetRank( newRank );
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::HealAllHumans
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
void PHY_HumansComposante::HealAllHumans()
{
    for( PHY_Human::CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
        (**it).Heal();
}
    
// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::HealHumans
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
uint PHY_HumansComposante::HealHumans( const PHY_HumanRank& rank, uint nNbrToChange )
{
    std::random_shuffle( humans_.begin(), humans_.end() );

    uint nNbrChanged = 0;

    for( PHY_Human::CIT_HumanVector it = humans_.begin(); it != humans_.end() && nNbrToChange; ++it )
    {
        PHY_Human& human = **it;

        if( human.GetRank() == rank && ( human.NeedMedical() || human.IsDead() ) )  //$$$ POURRI
        {
            human.Heal();
            -- nNbrToChange;
            ++ nNbrChanged;
        }
    }
    return nNbrChanged;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::WoundHumans
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
uint PHY_HumansComposante::WoundHumans( const PHY_HumanRank& rank, uint nNbrToChange, const PHY_HumanWound& newWound )
{
    if( newWound == PHY_HumanWound::notWounded_ )
        return 0;

    uint nNbrChanged = 0;
    for( PHY_Human::CIT_HumanVector it = humans_.begin(); it != humans_.end() && nNbrToChange ; ++it )
    {
        PHY_Human& human = **it;
        if( human.GetRank() != rank )
            continue;

        if( human.NeedMedical() || human.IsDead() ) //$$$ BUG si humain en santé
            continue;

        human.SetWound( newWound );
        -- nNbrToChange;
        ++ nNbrChanged; 
    }
    return nNbrChanged;
}

namespace {

    MT_Float round( MT_Float rValue )
    {
        MT_Float rIntegralPart;
        MT_Float rFractionalPart = modf( rValue, &rIntegralPart );

        if( rFractionalPart >= 0.5 )
            return rIntegralPart + 1.;
        else
            return rIntegralPart;
    }

}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::ApplyWounds
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_HumansComposante::ApplyWounds( const PHY_ComposanteState& newComposanteState, PHY_FireDamages_Agent& fireDamages )
{
    std::random_shuffle( humans_.begin(), humans_.end() );

    assert( pComposante_ );

    const PHY_Protection& protection = pComposante_->GetType().GetProtection();

    uint nNbrDead    = (uint)round( humans_.size() * protection.GetHumanDeadRatio   ( newComposanteState ) );
    uint nNbrWounded = (uint)round( humans_.size() * protection.GetHumanWoundedRatio( newComposanteState ) );

    PHY_Human::CIT_HumanVector itCur = humans_.begin();
    while( itCur != humans_.end() && nNbrDead != 0 )
    {
        PHY_Human& human = **itCur;
        const PHY_HumanWound& oldWound = human.GetWound();
        if( human.ApplyWound( PHY_HumanWound::killed_ ) ) //return true only if newWound (here = killed_) > oldWound
            fireDamages.NotifyHumanWoundChanged( human, oldWound );
        ++ itCur;
    }

    while( itCur != humans_.end() && nNbrWounded != 0 )
    {
        PHY_Human& human = **itCur;
        const PHY_HumanWound& oldWound = human.GetWound();
        if( human.ApplyWound( PHY_HumanWound::ChooseRandomWound() ) )
            fireDamages.NotifyHumanWoundChanged( human, oldWound );
        human.ApplyMentalDisease();
        ++ itCur;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::ApplyContamination
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
void PHY_HumansComposante::ApplyContamination( const MIL_ToxicEffectManipulator& contamination )
{
    for( PHY_Human::CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
        (**it).ApplyContamination( contamination );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::ApplyPoisonous
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
void PHY_HumansComposante::ApplyPoisonous( const MIL_ToxicEffectManipulator& contamination )
{
    for( PHY_Human::CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
        (**it).ApplyPoisonous( contamination );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::ApplyInjury
// Created: RFT
// -----------------------------------------------------------------------------
void PHY_HumansComposante::ApplyInjury( MIL_Injury_ABC& injury )
{
    assert( pComposante_ );
    
    for( PHY_Human::CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
    {
        if( injury.IsInjured( GetComposante() ) )
        {
            //on doit supprimer aussi le human du human vector et il va devenir un InjuredHuman
            //qui doit avoir une existence propre (mise a jour, que les autres sachent qu ils existent, position, peut etre une faculte de deplacement, ...) 
        }
    }
}

// =============================================================================
// COMPOSANTE NOTIFICATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyComposanteHandledByMaintenance
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
void PHY_HumansComposante::NotifyComposanteHandledByMaintenance()
{
    for( PHY_Human::CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
        (**it).NotifyComposanteHandledByMaintenance();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyComposanteBackFromMaintenance
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
void PHY_HumansComposante::NotifyComposanteBackFromMaintenance()
{
    for( PHY_Human::CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
        (**it).NotifyComposanteBackFromMaintenance();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyComposanteTransfered
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
void PHY_HumansComposante::NotifyComposanteTransfered( PHY_RoleInterface_Composantes& src, PHY_RoleInterface_Composantes& dest )
{
    for ( PHY_Human::CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
    {
        (**it).CancelLogisticRequest();
        const_cast< MIL_AgentPion& >( src.GetPion() ).Apply( &human::HumansActionsNotificationHandler_ABC::NotifyHumanRemoved, **it );
        const_cast< MIL_AgentPion& >( dest.GetPion() ).Apply( &human::HumansActionsNotificationHandler_ABC::NotifyHumanAdded, **it );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyHumanAdded
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_HumansComposante::NotifyHumanAdded( PHY_Human& human )
{
    if( human.IsUsable() )
    {
        assert( pComposante_->GetState() != PHY_ComposanteState::dead_ );
        ++ nNbrUsableHumans_;        
    }
    assert( pComposante_ );
    const_cast< MIL_AgentPion& >( pComposante_->GetRole().GetPion() ).Apply( &human::HumansActionsNotificationHandler_ABC::NotifyHumanAdded, human );
}
    
// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyHumanRemoved
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_HumansComposante::NotifyHumanRemoved( PHY_Human& human )
{
    if( human.IsUsable() )
    {
        assert( nNbrUsableHumans_ > 0 );
        -- nNbrUsableHumans_;
    }

    assert( pComposante_ );
    const_cast< MIL_AgentPion& >( pComposante_->GetRole().GetPion() ).Apply( &human::HumansActionsNotificationHandler_ABC::NotifyHumanRemoved, human );
    if( !IsViable() )
        pComposante_->ReinitializeState( PHY_ComposanteState::dead_ );
}
    
// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyHumanChanged
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_HumansComposante::NotifyHumanChanged( PHY_Human& human, const PHY_Human& copyOfOldHumanState )
{
    if( copyOfOldHumanState.IsUsable() )
    {
        assert( nNbrUsableHumans_ > 0 );
        -- nNbrUsableHumans_;
    }
    if( human.IsUsable() )
    {
        assert( pComposante_->GetState() != PHY_ComposanteState::dead_ );
        ++ nNbrUsableHumans_;
    }
  
    assert( pComposante_ );
    const_cast< MIL_AgentPion& >( pComposante_->GetRole().GetPion() ).Apply( &human::HumansActionsNotificationHandler_ABC::NotifyHumanChanged, human, copyOfOldHumanState );
    if( !IsViable() )
        pComposante_->ReinitializeState( PHY_ComposanteState::dead_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::EvacuateWoundedHumans
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
void PHY_HumansComposante::EvacuateWoundedHumans( MIL_AutomateLOG& destinationTC2 ) const
{
    for( PHY_Human::CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
        (**it).Evacuate( destinationTC2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::HasWoundedHumansToEvacuate
// Created: NLD 2005-08-08
// -----------------------------------------------------------------------------
bool PHY_HumansComposante::HasWoundedHumansToEvacuate() const
{
    for( PHY_Human::CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
    {
        if( (**it).NeedEvacuation() )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::GetOperationalState
// Created: NLD 2006-02-09
// -----------------------------------------------------------------------------
MT_Float PHY_HumansComposante::GetOperationalState() const
{
    if( humans_.empty() )
        return 0.;
    return (float)nNbrUsableHumans_ / (float)humans_.size();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::GetNbrUsableHumans
// Created: NLD 2006-02-09
// -----------------------------------------------------------------------------
uint PHY_HumansComposante::GetNbrUsableHumans() const
{
    return nNbrUsableHumans_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::GetComposante
// Created: NLD 2006-02-09
// -----------------------------------------------------------------------------
const PHY_ComposantePion& PHY_HumansComposante::GetComposante() const
{
    assert( pComposante_ );
    return *pComposante_;
}
