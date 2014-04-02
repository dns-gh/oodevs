// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Humans/PHY_RolePion_Humans.cpp $
// $Author: Jvt $
// $Modtime: 28/04/05 16:51 $
// $Revision: 6 $
// $Workfile: PHY_RolePion_Humans.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_HumanState.h"
#include "PHY_RolePion_Humans.h"
#include "AlgorithmsFactories.h"
#include "HealComputer_ABC.h"
#include "HealComputerFactory_ABC.h"
#include "HumansChangedNotificationHandler_ABC.h"
#include "NetworkNotificationHandler_ABC.h"
#include "CheckPoints/MIL_CheckPointInArchive.h"
#include "CheckPoints/MIL_CheckPointOutArchive.h"
#include "Entities/Agents/Roles/Logistic/PHY_MedicalHumanState.h"
#include "Entities/Agents/Units/Humans/Human_ABC.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "MIL_Time_ABC.h"
#include "tools/NET_AsnException.h"
#include "protocol/ClientSenders.h"
#include "protocol/CompatibilityHelper.h"
#include <boost/ptr_container/serialize_ptr_vector.hpp>
#include <boost/range/algorithm_ext/erase.hpp>
#include <boost/range/algorithm.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( human::PHY_RolePion_Humans )

using namespace human;

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::HumanState Constructor
// Created: ABR 2011-07-18
// -----------------------------------------------------------------------------
PHY_RolePion_Humans::HumanState::HumanState()
    : number_               ( 0 )
    , rank_                 ( &PHY_HumanRank::officier_ )
    , state_                ( &PHY_HumanWound::notWounded_ )
    , location_             ( eHumanLocation_Battlefield )
    , contaminated_         ( false )
    , psyop_                ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::HumanState constructor
// Created: ABR 2011-07-18
// -----------------------------------------------------------------------------
PHY_RolePion_Humans::HumanState::HumanState( unsigned int number, const PHY_HumanRank& rank, const PHY_HumanWound& state, E_HumanLocation location /*= eHumanLocation_Battlefield*/, bool contaminated /*= false*/, bool psyop /*= false*/ )
    : number_               ( number )
    , rank_                 ( &rank )
    , state_                ( &state )
    , location_             ( location )
    , contaminated_         ( contaminated )
    , psyop_                ( psyop )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::HumanState destructor
// Created: ABR 2011-07-18
// -----------------------------------------------------------------------------
PHY_RolePion_Humans::HumanState::~HumanState()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::HumanState::load
// Created: ABR 2011-07-18
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::HumanState::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    unsigned int rankId;
    file >> number_
         >> rankId
         >> state_
         >> location_
         >> contaminated_
         >> psyop_;
    rank_ = PHY_HumanRank::Find( rankId );
    assert( rank_ && state_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::HumanState::save
// Created: ABR 2011-07-18
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::HumanState::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    const unsigned int rankId = rank_->GetID();
    file << number_
         << rankId
         << state_
         << location_
         << contaminated_
         << psyop_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans constructor
// Created: LDC 2013-01-09
// -----------------------------------------------------------------------------
PHY_RolePion_Humans::PHY_RolePion_Humans()
    : owner_                  ( 0 )
    , nNbrUsableHumans_       ( 0 )
    , nTickRcMedicalQuerySent_( 0 )
    , nEvacuationMode_        ( eEvacuationMode_Auto )
    , hasChanged_             ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PHY_RolePion_Humans::PHY_RolePion_Humans( MIL_AgentPion& pion )
    : owner_                  ( &pion )
    , nNbrUsableHumans_       ( 0 )
    , nTickRcMedicalQuerySent_( 0 )
    , nEvacuationMode_        ( eEvacuationMode_Auto )
    , hasChanged_             ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_RolePion_Humans::~PHY_RolePion_Humans()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Humans::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Humans >( *this );
    file & owner_;
    file & humansStates_;
    file & nNbrUsableHumans_;
    file & humansToUpdate_;
    file & nTickRcMedicalQuerySent_;
    file & nEvacuationMode_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::WriteODB( xml::xostream& xos ) const
{
    bool found = false;
    std::list< const PHY_HumanState* > unwounded;
    for( auto it = humansStates_.begin(); it != humansStates_.end(); ++it )
    {
        const PHY_HumanState& state = *it;
        if( ( state.contaminated_ || state.psyop_ || state.state_ != &PHY_HumanWound::notWounded_ ) )
        {
            if( state.number_ != 0 )
            {
                if( ! found )
                {
                    found = true;
                    xos << xml::start( "humans" );
                }
                state.Write( xos );
            }
        }
        else
            unwounded.push_back( &state );
    }
    if( found )
    {
        for( auto it = unwounded.begin(); it != unwounded.end(); ++it )
            (*it)->Write( xos );
        xos << xml::end; // humans
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::UpdateDataWhenHumanRemoved
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::UpdateDataWhenHumanRemoved( const Human_ABC& human )
{
    bool found = false;
    for( auto it = humansStates_.begin(); it != humansStates_.end(); ++it )
    {
        PHY_HumanState& state = *it;
        if( state.rank_->GetID() == human.GetRank().GetID() &&
            state.state_->GetID() == human.GetWound().GetID() &&
            state.location_ == human.GetLocation() &&
            state.contaminated_ == human.IsContaminated() &&
            state.psyop_ == human.IsMentalDiseased() )
        {
            found = true;
            if( state.number_ > 0 )
                --state.number_;
            break;
        }
    }
    assert( found );
    if( human.IsUsable() )
    {
        assert( nNbrUsableHumans_ > 0 );
        --nNbrUsableHumans_;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::UpdateDataWhenHumanAdded
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::UpdateDataWhenHumanAdded( const Human_ABC& human )
{
    bool merged = false;
    for( auto it = humansStates_.begin(); it != humansStates_.end(); ++it )
    {
        PHY_HumanState& state = *it;
        if( state.rank_->GetID() == human.GetRank().GetID() &&
            state.state_->GetID() == human.GetWound().GetID() &&
            state.location_ == human.GetLocation() &&
            state.contaminated_ == human.IsContaminated() &&
            state.psyop_ == human.IsMentalDiseased() )
        {
            ++state.number_;
            merged = true;
            break;
        }
    }
    if( !merged )
        humansStates_.push_back( new PHY_HumanState( 1, human.GetRank(), human.GetWound(), human.GetLocation(), human.IsContaminated(), human.IsMentalDiseased() ) );
    if( human.IsUsable() )
        ++nNbrUsableHumans_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::NotifyHumanAdded
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::NotifyHumanAdded( Human_ABC& human )
{
    hasChanged_ = true;
    UpdateDataWhenHumanAdded( human );
    if( human.NeedUpdate() )
        humansToUpdate_.push_back( &human );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::NotifyHumanRemoved
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::NotifyHumanRemoved( Human_ABC& human )
{
    hasChanged_ = true;
    UpdateDataWhenHumanRemoved( human );
    boost::remove_erase( humansToUpdate_, &human );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::NotifyHumanChanged
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::NotifyHumanChanged( Human_ABC& human, const Human_ABC& copyOfOldHumanState )
{
    hasChanged_ = true;
    UpdateDataWhenHumanRemoved( copyOfOldHumanState );
    UpdateDataWhenHumanAdded  ( human );

    const bool bOldHumanNeedUpdate = copyOfOldHumanState.NeedUpdate();
    const bool bNewHumanNeedUpdate = human              .NeedUpdate();

    if( bOldHumanNeedUpdate && !bNewHumanNeedUpdate )
        boost::remove_erase( humansToUpdate_, &human );
    else if( !bOldHumanNeedUpdate && bNewHumanNeedUpdate )
        humansToUpdate_.push_back( &human );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::EvacuateWoundedHumans
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::EvacuateWoundedHumans( MIL_AutomateLOG& destinationTC2 ) const
{
    std::auto_ptr< HealComputer_ABC > healComputer( owner_->GetAlgorithms().healComputerFactory_->Create() );
    owner_->Execute< OnComponentComputer_ABC >( *healComputer );
    healComputer->EvacuateWoundedHumans( destinationTC2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::HasWoundedHumansToEvacuate
// Created: NLD 2005-08-08
// -----------------------------------------------------------------------------
bool PHY_RolePion_Humans::HasWoundedHumansToEvacuate() const
{
    std::auto_ptr< HealComputer_ABC > healComputer( owner_->GetAlgorithms().healComputerFactory_->Create() );
    owner_->Execute< OnComponentComputer_ABC >( *healComputer );
    return healComputer->HasWoundedHumansToEvacuate();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::NotifyHumanEvacuatedByThirdParty
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::NotifyHumanEvacuatedByThirdParty( Human_ABC& human, MIL_AutomateLOG& destinationTC2 )
{
    human.SetMedicalState( destinationTC2.MedicalHandleHumanEvacuatedByThirdParty( *owner_, human ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::NotifyHumanWaitingForMedical
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::NotifyHumanWaitingForMedical( Human_ABC& human )
{
    MIL_AutomateLOG* pTC2 = owner_->GetLogisticHierarchy().GetPrimarySuperior();
    if( !pTC2 || nEvacuationMode_ == eEvacuationMode_Manual )
    {
        human.SetMedicalState( boost::shared_ptr< PHY_MedicalHumanState >() );
        return;
    }
    // Pas de RC si log non branchée ou si RC envoyé au tick précédent
    const unsigned int nCurrentTick = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    if( nCurrentTick > ( nTickRcMedicalQuerySent_ + 1 ) || nTickRcMedicalQuerySent_ == 0 )
        MIL_Report::PostEvent( *owner_, report::eRC_DemandeEvacuationSanitaire );
    nTickRcMedicalQuerySent_ = nCurrentTick;
    human.SetMedicalState( pTC2->MedicalHandleHumanForEvacuation( *owner_, human ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::SendChangedState
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::SendChangedState( client::UnitAttributes& message ) const
{
    if( hasChanged_ )
        SendFullState( message );
}

namespace
{
    sword::EnumInjuriesSeriousness Convert( const PHY_HumanWound& wound )
    {
        const unsigned int stateId = wound.GetID();
        if( stateId == PHY_HumanWound::woundedU1_.GetID() )
            return sword::wounded_u1;
        if( stateId == PHY_HumanWound::woundedU2_.GetID() )
            return sword::wounded_u2;
        if( stateId == PHY_HumanWound::woundedU3_.GetID() )
            return sword::wounded_u3;
        return sword::wounded_ue;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::SendFullState
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::SendFullState( client::UnitAttributes& message ) const
{
    message().mutable_human_dotations();
    for( auto it = humansStates_.begin(); it != humansStates_.end(); ++it )
    {
        const PHY_HumanState& state = *it;
        sword::HumanDotations::HumanDotation& personnel = *message().mutable_human_dotations()->add_elem();
        // Quantity
        personnel.set_quantity( state.number_ );
        // Rank
        personnel.set_rank( state.rank_->GetAsnID() );
        // State
        unsigned int stateId = state.state_->GetID();
        if( stateId == PHY_HumanWound::notWounded_.GetID() )
            personnel.set_state( sword::healthy );
        else if( stateId == PHY_HumanWound::killed_.GetID() )
            personnel.set_state( sword::deadly );
        else
        {
            personnel.set_state( sword::injured );
            sword::Injury* injury = personnel.add_injuries();
            injury->set_id( 0 );  // Never used but "required", default to 0.
            injury->set_seriousness( Convert( *state.state_ ) );
        }
        // Location
        switch( state.location_ )
        {
        case eHumanLocation_Battlefield:
            personnel.set_location( sword::battlefield );
            break;
        case eHumanLocation_Maintenance:
            personnel.set_location( sword::maintenance );
            break;
        case eHumanLocation_Medical:
            personnel.set_location( sword::medical );
            break;
        case eHumanLocation_Funeral:
            personnel.set_location( sword::funeral );
            break;
        }
        personnel.set_location_deprecated(
                protocol::RemapHumanLocation( personnel.location() ));
        // Psyop && contaminated
        personnel.set_mentally_wounded( state.psyop_ );
        personnel.set_contaminated( state.contaminated_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::SendChangedState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::SendChangedState() const
{
    boost::for_each( humansToUpdate_, std::mem_fn( &Human_ABC::SendChangedState ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::SendFullState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::SendFullState( unsigned int context ) const
{
    boost::for_each( humansToUpdate_, boost::bind( &Human_ABC::SendFullState, _1, context ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::ChangeHumansAvailability
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::ChangeHumansAvailability( const PHY_HumanRank& rank, unsigned int nNewNbrFullyAliveHumans )
{
    unsigned int nbrOperational = GetNbrOperational( rank );
    nNewNbrFullyAliveHumans = std::min( nNewNbrFullyAliveHumans, GetNbrTotal( rank ) );

    std::auto_ptr< HealComputer_ABC > healComputer( owner_->GetAlgorithms().healComputerFactory_->Create() );
    owner_->Execute< OnComponentComputer_ABC >( *healComputer );

    if( nNewNbrFullyAliveHumans > nbrOperational )
        healComputer->Heal( rank, nNewNbrFullyAliveHumans - nbrOperational );
    else if( nNewNbrFullyAliveHumans < nbrOperational )
        healComputer->Wound( rank, nbrOperational - nNewNbrFullyAliveHumans );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::ReduceHumansAvailability
// Created: MMC 2012-10-04
// -----------------------------------------------------------------------------
unsigned int PHY_RolePion_Humans::ReduceHumansAvailability( const PHY_HumanRank& rank, unsigned int reduce )
{
    if( reduce == 0 )
        return 0;
    unsigned int count = GetNbrOperational( rank );
    if( count == 0 )
        return 0;
    reduce = std::min( reduce, count );
    ChangeHumansAvailability( rank, count - reduce );
    unsigned int newCount = GetNbrOperational( rank );
    return newCount < count ? count - newCount : 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::HealAllHumans
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::HealAllHumans( bool withLog )
{
    std::auto_ptr< HealComputer_ABC > healComputer( owner_->GetAlgorithms().healComputerFactory_->Create() );
    owner_->Execute< OnComponentComputer_ABC >( *healComputer );
    healComputer->HealAll( withLog );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::Update( bool /*bIsDead*/ )
{
    const std::vector< Human_ABC* > humans = humansToUpdate_;
    boost::for_each( humans, std::mem_fn( &Human_ABC::Update ) ); // !!! Can erase the human from humansToUpdate_ = bullshit ...
    if( hasChanged_ )
    {
        owner_->Apply( &human::HumansChangedNotificationHandler_ABC::NotifyHumanHasChanged );
        owner_->Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
    }
    boost::remove_erase_if(
        humansStates_,
        []( const PHY_HumanState& s ) { return s.number_ <= 0; } );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::Clean
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::Clean()
{
    hasChanged_ = false;
    boost::for_each( humansToUpdate_, std::mem_fn( &Human_ABC::Clean ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::ChangeEvacuationMode
// Created: NLD 2005-08-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::ChangeEvacuationMode( E_EvacuationMode nMode )
{
    nEvacuationMode_ = nMode;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::GetNbrTotal
// Created: ABR 2011-07-18
// -----------------------------------------------------------------------------
unsigned int PHY_RolePion_Humans::GetNbrTotal( const PHY_HumanRank& rank ) const
{
    unsigned int result = 0;
    for( auto it = humansStates_.begin(); it != humansStates_.end(); ++it )
    {
        const PHY_HumanState& state = *it;
        if( state.rank_->GetID() == rank.GetID() )
            result += state.number_;
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::GetNbrOperational
// Created: ABR 2011-07-18
// -----------------------------------------------------------------------------
unsigned int PHY_RolePion_Humans::GetNbrOperational( const PHY_HumanRank& rank ) const
{
    unsigned int result = 0;
    for( auto it = humansStates_.begin(); it != humansStates_.end(); ++it )
    {
        const PHY_HumanState& state = *it;
        if( state.rank_->GetID() == rank.GetID() && state.state_->GetID() == PHY_HumanWound::notWounded_.GetID() && state.location_ != eHumanLocation_Medical && !state.psyop_ && !state.contaminated_ )
            result += state.number_;
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::HasNoMoreOperationalHumans
// Created: JSR 2012-06-25
// -----------------------------------------------------------------------------
bool PHY_RolePion_Humans::HasNoMoreOperationalHumans() const
{
    if( humansStates_.empty() )
        return false;
    for( auto it = humansStates_.begin(); it != humansStates_.end(); ++it )
    {
        const PHY_HumanState& state = *it;
        if( state.state_->GetID() == PHY_HumanWound::notWounded_.GetID() && state.location_ != eHumanLocation_Medical && !state.psyop_ )
            return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::GetNbrUsableHumans
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
unsigned int PHY_RolePion_Humans::GetNbrUsableHumans() const
{
    return nNbrUsableHumans_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::GetNumber
// Created: LDC 2012-06-21
// -----------------------------------------------------------------------------
unsigned int PHY_RolePion_Humans::GetNumber() const
{
    unsigned int result = 0;
    for( auto it = humansStates_.begin(); it != humansStates_.end(); ++it )
        result += it->number_;
    return result;
}
