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
#include "MIL_Time_ABC.h"
#include "MIL_Random.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Units/Humans/PHY_Human.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Actions/PHY_FireDamages_Agent.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Entities/Objects/MIL_NbcAgentType.h"
#include "Entities/Agents/Units/Humans/WoundEffects_ABC.h"
#include "HumansActionsNotificationHandler_ABC.h"
#include <boost/serialization/shared_ptr.hpp>
#include <boost/make_shared.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_HumansComposante )

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante constructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_HumansComposante::PHY_HumansComposante( const MIL_Time_ABC& time, PHY_ComposantePion& composante, unsigned int nNbrMdr )
    : pComposante_     ( &composante )
    , nNbrUsableHumans_( 0 )
{
    while( nNbrMdr-- )
    {
        humans_.push_back( boost::make_shared< PHY_Human >( time, *this ) );
        NotifyHumanAdded( *humans_.back() );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante constructor
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
PHY_HumansComposante::PHY_HumansComposante()
    : pComposante_     ( 0 )
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

void PHY_HumansComposante::UpdateComponent() const
{
    if( pComposante_->GetState() != PHY_ComposanteState::dead_
        && nNbrUsableHumans_ == 0
        && !pComposante_->GetRole().GetPion().IsAutonomous() )
        pComposante_->ReinitializeState( PHY_ComposanteState::dead_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::IsEmpty
// Created: JSR 2011-09-15
// -----------------------------------------------------------------------------
bool PHY_HumansComposante::IsEmpty() const
{
    return humans_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::ChangeHumanRank
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
bool PHY_HumansComposante::ChangeHumanRank( const PHY_HumanRank& oldRank, const PHY_HumanRank& newRank, const PHY_HumanWound& wound )
{
    if( oldRank == newRank )
        return false;
    for( auto it = humans_.begin(); it != humans_.end(); ++it )
        if( (*it)->GetRank() == oldRank && (*it)->GetWound() == wound )
        {
            (*it)->SetRank( newRank );
            return true;
        }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::HealAllHumans
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
void PHY_HumansComposante::HealAllHumans( bool withLog )
{
    for( auto it = humans_.begin(); it != humans_.end(); ++it )
    {
        if( withLog && ( **it ).NeedMedical() )
            continue;
        ( **it ).Heal();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::KillAllHumans
// Created: ABR 2011-12-07
// -----------------------------------------------------------------------------
void PHY_HumansComposante::KillAllHumans()
{
    for( auto it = humans_.begin(); it != humans_.end(); ++it )
        (*it)->SetState( PHY_HumanWound::killed_, (*it)->IsMentalDiseased(), (*it)->IsContaminated() );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::HealHumans
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
unsigned int PHY_HumansComposante::HealHumans( const PHY_HumanRank& rank, unsigned int nNbrToChange )
{
    MIL_Random::random_shuffle( humans_, MIL_Random::eWounds );
    unsigned int nNbrChanged = 0;
    for( auto it = humans_.begin(); it != humans_.end() && nNbrToChange; ++it )
        if( (*it)->GetRank() == rank && ( (*it)->NeedMedical() || (*it)->IsDead() ) )  //$$$ POURRI
        {
            (*it)->Heal();
            --nNbrToChange;
            ++nNbrChanged;
        }
    return nNbrChanged;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::OverloadHumans
// Created: ABR 2011-03-07
// -----------------------------------------------------------------------------
unsigned int PHY_HumansComposante::OverloadHumans( const PHY_HumanRank& rank, unsigned int nNbrToChange, const PHY_HumanWound& newWound, bool psyop /*= false*/, bool contaminated /*= false*/ )
{
    if( newWound == PHY_HumanWound::notWounded_ && !psyop && !contaminated
        || pComposante_->GetState() == PHY_ComposanteState::dead_ && newWound != PHY_HumanWound::killed_ )
        return 0;
    unsigned int nNbrChanged = 0;
    for( auto it = humans_.begin(); it != humans_.end() && nNbrToChange ; ++it )
    {
        if( (*it)->GetRank() != rank || (*it)->GetWound() != PHY_HumanWound::notWounded_ || (*it)->IsMentalDiseased() || (*it)->IsContaminated() )
            continue;
        if( psyop )
            (*it)->ForceMentalDisease();
        (*it)->SetWound( newWound );
        if( contaminated )
            (*it)->ApplyContamination();
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
    for( auto it = humans_.begin(); it != humans_.end() && nNbrToChange ; ++it )
    {
        if( (*it)->GetRank() != rank )
            continue;
        if( (*it)->NeedMedical() || (*it)->IsDead() ) //$$$ BUG si humain en santé
            continue;
        (*it)->SetWound( newWound );
        --nNbrToChange;
        ++nNbrChanged;
    }
    return nNbrChanged;
}

namespace
{
    double round( double rValue )
    {
        double rIntegralPart;
        double rFractionalPart = std::modf( rValue, &rIntegralPart );
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
    auto it = humans_.begin();
    while( it != humans_.end() && nNbrDead != 0 )
    {
        const PHY_HumanWound& oldWound = (*it)->GetWound();
        if( (*it)->ApplyWound( PHY_HumanWound::killed_ ) ) //return true only if newWound (here = killed_) > oldWound
            fireDamages.NotifyHumanWoundChanged( **it, oldWound );
        --nNbrDead;
        ++it;
    }
    while( it != humans_.end() && nNbrWounded != 0 )
    {
        const PHY_HumanWound& oldWound = (*it)->GetWound();
        if( (*it)->ApplyWound( PHY_HumanWound::ChooseRandomWound() ) )
            fireDamages.NotifyHumanWoundChanged( **it, oldWound );
        (*it)->ApplyMentalDisease();
        --nNbrWounded;
        ++it;
    }
    if( !nNbrUsableHumans_ && !humans_.empty() )
        pComposante_->OnNoAvailableHuman();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::ApplyContamination
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
void PHY_HumansComposante::ApplyContamination()
{
    for( auto it = humans_.begin(); it != humans_.end(); ++it )
        (*it)->ApplyContamination();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::ApplyEffect
// Created: LGY 2012-11-29
// -----------------------------------------------------------------------------
void PHY_HumansComposante::ApplyEffect( const WoundEffects_ABC& effect )
{
    for( auto it = humans_.begin(); it != humans_.end(); ++it )
        effect.ApplyWound( **it );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyComposanteHandledByMaintenance
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
void PHY_HumansComposante::NotifyComposanteHandledByMaintenance()
{
    for( auto it = humans_.begin(); it != humans_.end(); ++it )
        (*it)->NotifyComposanteHandledByMaintenance();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyComposanteBackFromMaintenance
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
void PHY_HumansComposante::NotifyComposanteBackFromMaintenance()
{
    for( auto it = humans_.begin(); it != humans_.end(); ++it )
        (*it)->NotifyComposanteBackFromMaintenance();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::NotifyComposanteTransfered
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
void PHY_HumansComposante::NotifyComposanteTransfered( PHY_RoleInterface_Composantes& src, PHY_RoleInterface_Composantes& dest )
{
    for( auto it = humans_.begin(); it != humans_.end(); ++it )
    {
        (*it)->CancelLogisticRequests();
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
        ++nNbrUsableHumans_;
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
    UpdateComponent();
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
        --nNbrUsableHumans_;
    }
    if( !human.IsDead() )
        ++nNbrUsableHumans_;
    assert( pComposante_ );
    const_cast< MIL_Agent_ABC& >( pComposante_->GetRole().GetPion() ).Apply( &human::HumansActionsNotificationHandler_ABC::NotifyHumanChanged, human, copyOfOldHumanState );
    UpdateComponent();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::EvacuateWoundedHumans
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
void PHY_HumansComposante::EvacuateWoundedHumans( MIL_AutomateLOG& destinationTC2 )
{
    for( auto it = humans_.begin(); it != humans_.end(); ++it )
        (*it)->Evacuate( destinationTC2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::HasWoundedHumansToEvacuate
// Created: NLD 2005-08-08
// -----------------------------------------------------------------------------
bool PHY_HumansComposante::HasWoundedHumansToEvacuate() const
{
    for( auto it = humans_.begin(); it != humans_.end(); ++it )
        if( (*it)->NeedEvacuation() )
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
        return 1;
    double state = 0;
    for( auto it = humans_.begin(); it != humans_.end(); ++it )
        if( !(*it)->IsSeriouslyPhysicallyWounded() && !(*it)->IsDead() )
            ++state;
    return state / humans_.size();
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::GetNbrUsableHumans
// Created: NLD 2006-02-09
// -----------------------------------------------------------------------------
unsigned int PHY_HumansComposante::GetNbrUsableHumans() const
{
    return nNbrUsableHumans_;
}

unsigned int PHY_HumansComposante::GetNbrHumans() const
{
    return static_cast< unsigned int >( humans_.size() );
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

namespace
{
    const PHY_HumanWound& GetWound( const sword::MissionParameter_Value& elem )
    {
        sword::EnumHumanState state = static_cast< sword::EnumHumanState >( elem.list( 2 ).enumeration() );
        sword::EnumInjuriesSeriousness seriousness = sword::wounded_u1;
        if( state == sword::injured )
            seriousness = static_cast< sword::EnumInjuriesSeriousness >( elem.list( 3 ).list( 0 ).list( 1 ).enumeration() );
        switch( state )
        {
        case sword::healthy:
            return PHY_HumanWound::notWounded_;
        case sword::injured:
            if( seriousness == sword::wounded_u1 )
                return PHY_HumanWound::woundedU1_;
            if( seriousness == sword::wounded_u2 )
                return PHY_HumanWound::woundedU2_;
            if( seriousness == sword::wounded_u3 )
                return PHY_HumanWound::woundedU3_;
            return PHY_HumanWound::woundedUE_;
        case sword::deadly:
        default:
            return PHY_HumanWound::killed_;
        }
    }
    template< typename Checker >
    void ApplyOnHumans( sword::MissionParameters& msg, std::set< boost::shared_ptr< Human_ABC > >& done, const std::vector< boost::shared_ptr< Human_ABC > >& humans, const Checker& checker )
    {
        for( int i = 0; i < msg.elem( 0 ).value_size(); ++i )
        {
            sword::MissionParameter_Value& elem = *msg.mutable_elem( 0 )->mutable_value()->Mutable( i );
            const PHY_HumanRank* rank = PHY_HumanRank::Find( static_cast< unsigned int >( elem.list( 1 ).enumeration() ) );
            int32_t number = elem.list( 0 ).quantity();
            if( number == 0 )
                continue;
            const PHY_HumanWound& wound = GetWound( elem );
            bool psyop = elem.list( 4 ).booleanvalue();
            bool contaminated = elem.list( 5 ).booleanvalue();
            for( auto it = humans.begin(); it != humans.end() && number; ++it )
            {
                if( done.find( *it ) != done.end() )
                    continue;
                const boost::shared_ptr< Human_ABC >& human = *it;
                if( checker( *human, *rank, wound, psyop, contaminated ) )
                {
                    done.insert( *it );
                    --number;
                }
            }
            elem.mutable_list( 0 )->set_quantity( number );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::UpdateHumanState
// Created: LDC 2014-02-24
// -----------------------------------------------------------------------------
void PHY_HumansComposante::UpdateHumanState( sword::MissionParameters& msg, std::set< boost::shared_ptr< Human_ABC > >& done )
{
    ApplyOnHumans( msg, done, humans_, [&]( Human_ABC& human, const PHY_HumanRank& rank, const PHY_HumanWound& wound, bool psyop, bool contaminated )
    {
        return rank == human.GetRank() &&
               wound == human.GetWound() &&
               psyop == human.IsMentalDiseased() &&
               contaminated == human.IsContaminated();
    } );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::ChangeHumanState
// Created: ABR 2011-08-12
// -----------------------------------------------------------------------------
void PHY_HumansComposante::ChangeHumanState( sword::MissionParameters& msg, std::set< boost::shared_ptr< Human_ABC > >& done )
{
    ApplyOnHumans( msg, done, humans_, [&]( Human_ABC& human, const PHY_HumanRank& rank, const PHY_HumanWound& wound, bool psyop, bool contaminated ) -> bool
    {
        if( human.GetRank() != rank )
            return false;
        human.SetState( wound, psyop, contaminated );
        return true;
    } );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::GetNbrHealthyHumans
// Created: LDC 2012-10-26
// -----------------------------------------------------------------------------
unsigned int PHY_HumansComposante::GetNbrHealthyHumans( const PHY_HumanRank& rank ) const
{
    unsigned int result = 0;
    for( auto it = humans_.begin(); it != humans_.end(); ++it )
    {
        if( (*it)->GetRank() != rank || (*it)->IsDead() || (*it)->IsWounded() || (*it)->IsContaminated() || (*it)->IsMentalDiseased() )
            continue;
        ++result;
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::RemoveHealthyHumans
// Created: LDC 2012-10-26
// -----------------------------------------------------------------------------
void PHY_HumansComposante::RemoveHealthyHumans( const PHY_HumanRank& rank, unsigned int humansToRemove )
{
    for( auto it = humans_.begin(); it != humans_.end() && humansToRemove > 0; )
        if( (*it)->GetRank() != rank || (*it)->IsDead() || (*it)->IsWounded() || (*it)->IsContaminated() || (*it)->IsMentalDiseased() )
             ++it;
        else
        {
            --humansToRemove;
            NotifyHumanRemoved( **it );
            it = humans_.erase( it );
        }
}

// -----------------------------------------------------------------------------
// Name: PHY_HumansComposante::CancelLogisticRequests
// Created: JSR 2013-02-22
// -----------------------------------------------------------------------------
void PHY_HumansComposante::CancelLogisticRequests()
{
    for( auto it = humans_.begin(); it != humans_.end(); ++it )
        ( *it )->CancelLogisticRequests();
}
