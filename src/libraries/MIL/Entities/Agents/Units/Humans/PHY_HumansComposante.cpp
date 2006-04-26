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

#include "MIL_pch.h"

#include "PHY_HumansComposante.h"

#include "PHY_Human.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Actions/PHY_FireDamages_Agent.h"
#include "Entities/Agents/MIL_AgentPion.h"

BOOST_CLASS_EXPORT_GUID( PHY_HumansComposante, "PHY_HumansComposante" )

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante constructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_HumansComposante::PHY_HumansComposante( PHY_ComposantePion& composante, uint nNbrMdr )
    : pComposante_     ( &composante )
    , humans_          ()
    , nNbrUsableHumans_( 0 )
{
    while ( nNbrMdr-- )
        humans_.push_back( new PHY_Human( *this ) );
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
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_HumansComposante::~PHY_HumansComposante()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

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

    for( CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
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
// Name: PHY_HumansComposante::KillAllUsableHumans
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_HumansComposante::KillAllUsableHumans()
{
    for( CIT_HumanVector it = humans_.begin(); it != humans_.end() ; ++it )
        (**it).ApplyWound( PHY_HumanWound::killed_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::KillAllUsableHumans
// Created: NLD 2004-10-07
// -----------------------------------------------------------------------------
void PHY_HumansComposante::KillAllUsableHumans( PHY_FireDamages_Agent& fireDamages )
{
    for( CIT_HumanVector it = humans_.begin(); it != humans_.end() ; ++it )
    {
        PHY_Human& human = **it;
        const PHY_HumanWound& oldWound = human.GetWound();
        if( human.ApplyWound( PHY_HumanWound::killed_ ) )
            fireDamages.NotifyHumanWoundChanged( human, oldWound );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::HealAllHumans
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
void PHY_HumansComposante::HealAllHumans()
{
    for( CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
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

    for( CIT_HumanVector it = humans_.begin(); it != humans_.end() && nNbrToChange; ++it )
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
    for( CIT_HumanVector it = humans_.begin(); it != humans_.end() && nNbrToChange ; ++it )
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

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::ApplyWounds
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_HumansComposante::ApplyWounds( const PHY_ComposanteState& newComposanteState, PHY_FireDamages_Agent& fireDamages )
{
    std::random_shuffle( humans_.begin(), humans_.end() );

    assert( pComposante_ );

    uint nNbrToDo = (uint)( humans_.size() * pComposante_->GetType().GetProtection().GetHumanWoundFactor( newComposanteState ) );
    for( CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
    {
        PHY_Human& human = **it;
        if( !human.IsUsable() )
            continue;

        if( nNbrToDo )
        {
            -- nNbrToDo;
            const PHY_HumanWound& oldWound = human.GetWound();
            if( human.ApplyWound() )
                fireDamages.NotifyHumanWoundChanged( human, oldWound );
        }
        human.ApplyMentalDisease();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::ApplyWounds
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
void PHY_HumansComposante::ApplyWounds( const MIL_NbcAgentType& nbcAgentType )
{
    for( CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
        (**it).ApplyWound( nbcAgentType );
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
    for( CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
        (**it).NotifyComposanteHandledByMaintenance();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyComposanteBackFromMaintenance
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
void PHY_HumansComposante::NotifyComposanteBackFromMaintenance()
{
    for( CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
        (**it).NotifyComposanteBackFromMaintenance();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyComposanteTransfered
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
void PHY_HumansComposante::NotifyComposanteTransfered( PHY_RolePion_Composantes& src, PHY_RolePion_Composantes& dest )
{
    for ( CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
    {
        (**it).CancelLogisticRequest();
        src .NotifyHumanRemoved( **it );
        dest.NotifyHumanAdded  ( **it );
    }
}

// =============================================================================
// NOTIFICATIONS HUMANS
// =============================================================================

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
    pComposante_->NotifyHumanAdded( human );
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
    pComposante_->NotifyHumanRemoved( human );
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
    pComposante_->NotifyHumanChanged( human, copyOfOldHumanState );
    if( !IsViable() )
        pComposante_->ReinitializeState( PHY_ComposanteState::dead_ );
}


// =============================================================================
// MEDICAL
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyHumanBackFromMedical
// Created: NLD 2006-02-09
// -----------------------------------------------------------------------------
void PHY_HumansComposante::NotifyHumanBackFromMedical( PHY_MedicalHumanState& humanState ) const
{
    assert( pComposante_ );
    pComposante_->NotifyHumanBackFromMedical( humanState );
}

// ----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyHumanWaitingForMedical
// Created: NLD 2006-02-09
// -----------------------------------------------------------------------------
PHY_MedicalHumanState* PHY_HumansComposante::NotifyHumanWaitingForMedical( PHY_Human& human ) const
{
    assert( pComposante_ );
    return pComposante_->NotifyHumanWaitingForMedical( human );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyHumanEvacuatedByThirdParty
// Created: NLD 2006-02-09
// -----------------------------------------------------------------------------
PHY_MedicalHumanState* PHY_HumansComposante::NotifyHumanEvacuatedByThirdParty( PHY_Human& human, MIL_AutomateLOG& destinationTC2 ) const
{
    assert( pComposante_ );
    return pComposante_->NotifyHumanEvacuatedByThirdParty( human, destinationTC2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::EvacuateWoundedHumans
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
void PHY_HumansComposante::EvacuateWoundedHumans( MIL_AutomateLOG& destinationTC2 ) const
{
    for( CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
        (**it).Evacuate( destinationTC2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::HasWoundedHumansToEvacuate
// Created: NLD 2005-08-08
// -----------------------------------------------------------------------------
bool PHY_HumansComposante::HasWoundedHumansToEvacuate() const
{
    for( CIT_HumanVector it = humans_.begin(); it != humans_.end(); ++it )
    {
        if( (**it).NeedEvacuation() )
            return true;
    }
    return false;
}
