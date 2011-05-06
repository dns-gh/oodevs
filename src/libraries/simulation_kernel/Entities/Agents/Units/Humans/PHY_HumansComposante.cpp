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
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Units/Humans/HumanStateHelper.h"
#include "Entities/Agents/Units/Humans/PHY_Human.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/Units/Humans/PHY_InjuredHuman.h"
#include "Entities/Actions/PHY_FireDamages_Agent.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Entities/Agents/Units/Humans/MIL_Injury_ABC.h"
#include "Entities/Objects/MIL_NbcAgentType.h"
#include "Entities/Objects/MIL_ToxicEffectManipulator.h"
#include "HumansActionsNotificationHandler_ABC.h"
#include <boost/serialization/vector.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_HumansComposante )

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante constructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_HumansComposante::PHY_HumansComposante( const MIL_Time_ABC& time, PHY_ComposantePion& composante, unsigned int nNbrMdr )
    : pComposante_     ( &composante )
    , nNbrUsableHumans_( 0 )
    , injury_          ( 0 )
{
    while( nNbrMdr-- )
        humans_.push_back( new PHY_Human( time, *this ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante constructor
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
PHY_HumansComposante::PHY_HumansComposante()
    : pComposante_     ( 0 )
    , nNbrUsableHumans_( 0 )
    , injury_          ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_HumansComposante::~PHY_HumansComposante()
{
    if( injury_ )
        delete injury_;
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
    for( std::vector< Human_ABC* >::const_iterator it = humans_.begin(); it != humans_.end(); ++it )
    {
        Human_ABC& human = **it;
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
    for( std::vector< Human_ABC* >::const_iterator it = humans_.begin(); it != humans_.end(); ++it )
        ( **it ).Heal();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::HealHumans
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
unsigned int PHY_HumansComposante::HealHumans( const PHY_HumanRank& rank, unsigned int nNbrToChange )
{
    MIL_Random::random_shuffle( humans_, MIL_Random::eWounds );
    unsigned int nNbrChanged = 0;
    for( std::vector< Human_ABC* >::const_iterator it = humans_.begin(); it != humans_.end() && nNbrToChange; ++it )
    {
        Human_ABC& human = **it;
        if( human.GetRank() == rank && ( human.NeedMedical() || human.IsDead() ) )  //$$$ POURRI
        {
            human.Heal();
            --nNbrToChange;
            ++nNbrChanged;
        }
    }
    return nNbrChanged;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::OverloadHumans
// Created: ABR 2011-03-07
// -----------------------------------------------------------------------------
unsigned int PHY_HumansComposante::OverloadHumans( const PHY_HumanRank& rank, unsigned int nNbrToChange, const PHY_HumanWound& newWound, bool psyop /*= false*/, bool contaminated /*= false*/ )
{
    if( newWound == PHY_HumanWound::notWounded_ )
        return 0;
    unsigned int nNbrChanged = 0;
    for( std::vector< Human_ABC* >::const_iterator it = humans_.begin(); it != humans_.end() && nNbrToChange ; ++it )
    {
        Human_ABC& human = **it;
        if( human.GetRank() != rank || human.GetWound() != PHY_HumanWound::notWounded_ || human.IsMentalDiseased() || human.IsContaminated() )
            continue;
        if( psyop )
            human.ForceMentalDisease();
        if( newWound != PHY_HumanWound::notWounded_ )
            human.SetWound( newWound );
        if( contaminated )
        {
            std::vector< const MIL_NbcAgentType* > nbcTypes;
            MIL_ToxicEffectManipulator nbcAgent( nbcTypes, 1 );
            human.ApplyContamination( nbcAgent );
        }
        --nNbrToChange;
        ++nNbrChanged;
    }
    return nNbrChanged;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::WoundHumans
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
unsigned int PHY_HumansComposante::WoundHumans( const PHY_HumanRank& rank, unsigned int nNbrToChange, const PHY_HumanWound& newWound )
{
    if( newWound == PHY_HumanWound::notWounded_ )
        return 0;
    unsigned int nNbrChanged = 0;
    for( std::vector< Human_ABC* >::const_iterator it = humans_.begin(); it != humans_.end() && nNbrToChange ; ++it )
    {
        Human_ABC& human = **it;
        if( human.GetRank() != rank )
            continue;
        if( human.NeedMedical() || human.IsDead() ) //$$$ BUG si humain en santé
            continue;
        human.SetWound( newWound );
        --nNbrToChange;
        ++nNbrChanged;
    }
    return nNbrChanged;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::FillHumanStateHelper
// Created: ABR 2011-03-08
// -----------------------------------------------------------------------------
void PHY_HumansComposante::FillHumanStateHelper( HumanStateHelper& helper ) const
{
    for( std::vector< Human_ABC* >::const_iterator it = humans_.begin(); it != humans_.end(); ++it )
    {
        const Human_ABC& human = **it;
        if( human.GetWound() != PHY_HumanWound::notWounded_ || human.IsContaminated() || human.IsMentalDiseased() )
            helper.AddHumanMergeIfNeeded( human );
    }
}

// -----------------------------------------------------------------------------
namespace
{
    double round( double rValue )
    {
        double rIntegralPart;
        double rFractionalPart = modf( rValue, &rIntegralPart );
        if( rFractionalPart >= 0.5 )
            return rIntegralPart + 1.;
        return rIntegralPart;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::ApplyWounds
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_HumansComposante::ApplyWounds( const PHY_ComposanteState& newComposanteState, PHY_FireDamages_Agent& fireDamages )
{
    MIL_Random::random_shuffle( humans_, MIL_Random::eWounds );
    assert( pComposante_ );
    const PHY_Protection& protection = pComposante_->GetType().GetProtection();
    unsigned int nNbrDead = static_cast< unsigned int >( round( humans_.size() * protection.GetHumanDeadRatio( newComposanteState ) ) );
    unsigned int nNbrWounded = static_cast< unsigned int >( round( humans_.size() * protection.GetHumanWoundedRatio( newComposanteState ) ) );
    std::vector< Human_ABC* >::const_iterator itCur = humans_.begin();
    while( itCur != humans_.end() && nNbrDead != 0 )
    {
        Human_ABC& human = **itCur;
        const PHY_HumanWound& oldWound = human.GetWound();
        if( human.ApplyWound( PHY_HumanWound::killed_ ) ) //return true only if newWound (here = killed_) > oldWound
            fireDamages.NotifyHumanWoundChanged( human, oldWound );
        --nNbrDead;
        ++itCur;
    }
    while( itCur != humans_.end() && nNbrWounded != 0 )
    {
        Human_ABC& human = **itCur;
        const PHY_HumanWound& oldWound = human.GetWound();
        if( human.ApplyWound( PHY_HumanWound::ChooseRandomWound() ) )
            fireDamages.NotifyHumanWoundChanged( human, oldWound );
        human.ApplyMentalDisease();
        --nNbrWounded;
        ++itCur;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::ApplyContamination
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
void PHY_HumansComposante::ApplyContamination( const MIL_ToxicEffectManipulator& contamination )
{
    for( std::vector< Human_ABC* >::const_iterator it = humans_.begin(); it != humans_.end(); ++it )
        ( **it ).ApplyContamination( contamination );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::ApplyPoisonous
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
void PHY_HumansComposante::ApplyPoisonous( const MIL_ToxicEffectManipulator& contamination )
{
    for( std::vector< Human_ABC* >::const_iterator it = humans_.begin(); it != humans_.end(); ++it )
        ( **it ).ApplyPoisonous( contamination );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::ApplyBurn
// Created: BCI 2010-12-14
// -----------------------------------------------------------------------------
void PHY_HumansComposante::ApplyBurn( const MIL_BurnEffectManipulator& burn )
{
    for( std::vector< Human_ABC* >::const_iterator it = humans_.begin(); it != humans_.end(); ++it )
        ( **it ).ApplyBurn( burn );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::ApplyFlood
// Created: JSR 2011-01-11
// -----------------------------------------------------------------------------
void PHY_HumansComposante::ApplyFlood( const MIL_FloodEffectManipulator& flood )
{
    for( std::vector< Human_ABC* >::const_iterator it = humans_.begin(); it != humans_.end(); ++it )
        ( **it ).ApplyFlood( flood);
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::ApplyInjury
// Created: RFT
// -----------------------------------------------------------------------------
void PHY_HumansComposante::ApplyInjury( MIL_Injury_ABC& injury )
{
    for( std::vector< Human_ABC* >::const_iterator it = humans_.begin(); it != humans_.end(); ++it )
    {
        if( injury.IsInjured( GetComposante() ) )
        {
            //on doit supprimer aussi le human du human vector et il va devenir un InjuredHuman
            //qui doit avoir une existence propre (mise a jour, que les autres sachent qu ils existent, position, peut etre une faculte de deplacement, ...)
        }
    }
    delete injury_;
    injury_ = new PHY_InjuredHuman( injury );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::GetInjury
// Created: LDC 2010-07-02
// -----------------------------------------------------------------------------
PHY_InjuredHuman* PHY_HumansComposante::GetInjury()
{
    return injury_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyComposanteHandledByMaintenance
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
void PHY_HumansComposante::NotifyComposanteHandledByMaintenance()
{
    for( std::vector< Human_ABC* >::const_iterator it = humans_.begin(); it != humans_.end(); ++it )
        ( **it ).NotifyComposanteHandledByMaintenance();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyComposanteBackFromMaintenance
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
void PHY_HumansComposante::NotifyComposanteBackFromMaintenance()
{
    for( std::vector< Human_ABC* >::const_iterator it = humans_.begin(); it != humans_.end(); ++it )
        ( **it ).NotifyComposanteBackFromMaintenance();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyComposanteTransfered
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
void PHY_HumansComposante::NotifyComposanteTransfered( PHY_RoleInterface_Composantes& src, PHY_RoleInterface_Composantes& dest )
{
    for ( std::vector< Human_ABC* >::const_iterator it = humans_.begin(); it != humans_.end(); ++it )
    {
        ( **it ).CancelLogisticRequest();
        const_cast< MIL_Agent_ABC& >( src.GetPion() ).Apply( &human::HumansActionsNotificationHandler_ABC::NotifyHumanRemoved, **it );
        const_cast< MIL_Agent_ABC& >( dest.GetPion() ).Apply( &human::HumansActionsNotificationHandler_ABC::NotifyHumanAdded, **it );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyHumanAdded
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_HumansComposante::NotifyHumanAdded( Human_ABC& human )
{
    if( human.IsUsable() )
    {
        assert( pComposante_->GetState() != PHY_ComposanteState::dead_ );
        ++ nNbrUsableHumans_;
    }
    assert( pComposante_ );
    const_cast< MIL_Agent_ABC& >( pComposante_->GetRole().GetPion() ).Apply( &human::HumansActionsNotificationHandler_ABC::NotifyHumanAdded, human );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyHumanRemoved
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_HumansComposante::NotifyHumanRemoved( Human_ABC& human )
{
    if( human.IsUsable() )
    {
        assert( nNbrUsableHumans_ > 0 );
        --nNbrUsableHumans_;
    }
    assert( pComposante_ );
    const_cast< MIL_Agent_ABC& >( pComposante_->GetRole().GetPion() ).Apply( &human::HumansActionsNotificationHandler_ABC::NotifyHumanRemoved, human );
    if( !IsViable() )
        pComposante_->ReinitializeState( PHY_ComposanteState::dead_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyHumanChanged
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_HumansComposante::NotifyHumanChanged( Human_ABC& human, const Human_ABC& copyOfOldHumanState )
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
    const_cast< MIL_Agent_ABC& >( pComposante_->GetRole().GetPion() ).Apply( &human::HumansActionsNotificationHandler_ABC::NotifyHumanChanged, human, copyOfOldHumanState );
    if( !IsViable() )
        pComposante_->ReinitializeState( PHY_ComposanteState::dead_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::EvacuateWoundedHumans
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
void PHY_HumansComposante::EvacuateWoundedHumans( MIL_AutomateLOG& destinationTC2 ) const
{
    for( std::vector< Human_ABC* >::const_iterator it = humans_.begin(); it != humans_.end(); ++it )
        ( **it ).Evacuate( destinationTC2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::HasWoundedHumansToEvacuate
// Created: NLD 2005-08-08
// -----------------------------------------------------------------------------
bool PHY_HumansComposante::HasWoundedHumansToEvacuate() const
{
    for( std::vector< Human_ABC* >::const_iterator it = humans_.begin(); it != humans_.end(); ++it )
        if( ( **it ).NeedEvacuation() )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::GetOperationalState
// Created: NLD 2006-02-09
// -----------------------------------------------------------------------------
double PHY_HumansComposante::GetOperationalState() const
{
    if( humans_.empty() )
        return 1.;
    return static_cast< float >( nNbrUsableHumans_ ) / static_cast< float >( humans_.size() );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::GetNbrUsableHumans
// Created: NLD 2006-02-09
// -----------------------------------------------------------------------------
unsigned int PHY_HumansComposante::GetNbrUsableHumans() const
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

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::serialize
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_HumansComposante::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< HumansComposante_ABC >( *this );
    file & pComposante_
         & humans_
         & nNbrUsableHumans_;
    assert( pComposante_ );
}
