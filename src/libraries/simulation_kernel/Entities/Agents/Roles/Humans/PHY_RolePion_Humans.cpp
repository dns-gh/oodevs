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
#include "Entities/Agents/Roles/Logistic/PHY_MedicalHumanState.h"
#include "Entities/Agents/Units/Humans/PHY_Human.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "Network/NET_AsnException.h"
#include "protocol/ClientSenders.h"
#include "simulation_kernel/AlgorithmsFactories.h"
#include "simulation_kernel/CheckPoints/MIL_CheckPointInArchive.h"
#include "simulation_kernel/CheckPoints/MIL_CheckPointOutArchive.h"
#include "simulation_kernel/HealComputer_ABC.h"
#include "simulation_kernel/HealComputerFactory_ABC.h"
#include "simulation_kernel/HumansChangedNotificationHandler_ABC.h"
#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include "MT_Tools/MT_ScipioException.h"
#include <boost/serialization/set.hpp>
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( human::PHY_RolePion_Humans )

namespace human
{

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans constructor
// Created: LDC 2013-01-09
// -----------------------------------------------------------------------------
PHY_RolePion_Humans::PHY_RolePion_Humans()
    : pion_                   ( 0 )
    , humansStates_           ()
    , nNbrUsableHumans_       ( 0 )
    , humansToUpdate_         ()
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
    : pion_                   ( &pion )
    , humansStates_           ()
    , nNbrUsableHumans_       ( 0 )
    , humansToUpdate_         ()
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
    file & pion_;
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
        const PHY_HumanState& state = **it;
        if( ( state.contaminated_ || state.psyop_ || state.state_ != &PHY_HumanWound::notWounded_ ) )
        {
            if( state.number_ != 0 )
            {
                if( ! found )
                {
                    found = true;
                    xos.start( "humans" );
                }
                state.Write( xos );
            }
        }
        else
            unwounded.push_back( &state );
    }
    if( found )
    {
        for( std::list< const PHY_HumanState* >::const_iterator it = unwounded.begin(); it != unwounded.end(); ++it )
            (*it)->Write( xos );
        xos.end(); // humans
    }
}


// =============================================================================
// NOTIFICATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::UpdateDataWhenHumanRemoved
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::UpdateDataWhenHumanRemoved( const Human_ABC& human )
{
    bool founded = false;
    for( IT_HumanStateVector it = humansStates_.begin(); it != humansStates_.end(); ++it )
    {
        PHY_HumanState* state = *it;
        if( state->rank_->GetID() == human.GetRank().GetID() &&
            state->state_->GetID() == human.GetWound().GetID() &&
            state->location_ == human.GetLocation() &&
            state->contaminated_ == human.IsContaminated() &&
            state->psyop_ == human.IsMentalDiseased() )
        {
            founded = true;
            if( state->number_ > 1 )
                --state->number_;
            else
            {
                humansStates_.erase( it );
                delete state;
            }
            break;
        }
    }
    assert( founded );
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
    for( IT_HumanStateVector it = humansStates_.begin(); it != humansStates_.end(); ++it )
    {
        PHY_HumanState& state = **it;
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
        ++ nNbrUsableHumans_;
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
        humansToUpdate_.insert( &human );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::NotifyHumanRemoved
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::NotifyHumanRemoved( Human_ABC& human )
{
    hasChanged_ = true;
    UpdateDataWhenHumanRemoved( human );
    humansToUpdate_.erase( &human );
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
        humansToUpdate_.erase( &human );
    else if( !bOldHumanNeedUpdate && bNewHumanNeedUpdate )
        humansToUpdate_.insert( &human );
}

// =============================================================================
// LOGISTIC
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::EvacuateWoundedHumans
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::EvacuateWoundedHumans( MIL_AutomateLOG& destinationTC2 ) const
{
    std::auto_ptr< HealComputer_ABC > healComputer( pion_->GetAlgorithms().healComputerFactory_->Create() );
    pion_->Execute< OnComponentComputer_ABC >( *healComputer );
    healComputer->EvacuateWoundedHumans( destinationTC2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::HasWoundedHumansToEvacuate
// Created: NLD 2005-08-08
// -----------------------------------------------------------------------------
bool PHY_RolePion_Humans::HasWoundedHumansToEvacuate() const
{
    std::auto_ptr< HealComputer_ABC > healComputer( pion_->GetAlgorithms().healComputerFactory_->Create() );
    pion_->Execute< OnComponentComputer_ABC >( *healComputer );
    return healComputer->HasWoundedHumansToEvacuate();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::NotifyHumanEvacuatedByThirdParty
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::NotifyHumanEvacuatedByThirdParty( Human_ABC& human, MIL_AutomateLOG& destinationTC2 )
{
    PHY_MedicalHumanState* pMedicalHumanState = destinationTC2.MedicalHandleHumanEvacuatedByThirdParty( *pion_, human );
    human.SetMedicalState( pMedicalHumanState );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::NotifyHumanWaitingForMedical
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::NotifyHumanWaitingForMedical( Human_ABC& human )
{
    //$$$ Ne devrait contenir que la partie RC, et d�l�guer la gestion de la consign � l'Human
    MIL_AutomateLOG* pTC2 = pion_->GetLogisticHierarchy().GetPrimarySuperior();
    if( !pTC2 || nEvacuationMode_ == eEvacuationMode_Manual )
    {
        human.SetMedicalState( 0 );
        return;
    }
    // Pas de RC si log non branch�e ou si RC envoy� au tick pr�c�dent
    const unsigned int nCurrentTick = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    if( nCurrentTick > ( nTickRcMedicalQuerySent_ + 1 ) || nTickRcMedicalQuerySent_ == 0 )
        MIL_Report::PostEvent( *pion_, MIL_Report::eRC_DemandeEvacuationSanitaire );
    nTickRcMedicalQuerySent_ = nCurrentTick;

    PHY_MedicalHumanState* pMedicalHumanState = pTC2->MedicalHandleHumanForEvacuation( *pion_, human );
    human.SetMedicalState( pMedicalHumanState );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::NotifyHumanBackFromMedical
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::NotifyHumanBackFromMedical( PHY_MedicalHumanState& /*humanState*/ )
{
}


// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::SendChangedState
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::SendChangedState( client::UnitAttributes& message ) const
{
    if( hasChanged_ )
        SendFullState( message );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::SendFullState
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::SendFullState( client::UnitAttributes& message ) const
{
    for( auto it = humansStates_.begin(); it != humansStates_.end(); ++it )
    {
        const PHY_HumanState& state = **it;
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
            // Injuries
            sword::Injury* injury = personnel.mutable_injuries()->Add();
            injury->set_id( 0 ); // $$$$ ABR 2011-07-20: FIXME with story 660
            if( stateId == PHY_HumanWound::woundedU1_.GetID() )
                injury->set_seriousness( sword::wounded_u1 );
            else if( stateId == PHY_HumanWound::woundedU2_.GetID() )
                injury->set_seriousness( sword::wounded_u2 );
            else if( stateId == PHY_HumanWound::woundedU3_.GetID() )
                injury->set_seriousness( sword::wounded_u3 );
            else if( stateId == PHY_HumanWound::woundedUE_.GetID() )
                injury->set_seriousness( sword::wounded_ue );
        }
        // Location
        switch( state.location_ )
        {
        case Human_ABC::eBattleField:
            personnel.set_location( sword::battlefield );
            break;
        case Human_ABC::eMaintenance:
            personnel.set_location( sword::maintenance );
            break;
        case Human_ABC::eMedical:
            personnel.set_location( sword::medical );
            break;
        case Human_ABC::eFuneral: 
            personnel.set_location( sword::medical ); //$$$ NLD 2011-10-03 TEMPORAIRE EN ATTENDANT SCIPIO, remplacer par sword::funeral
            break;
        }
        // Psyop && contaminated
        if( state.psyop_ )
            personnel.set_mentally_wounded( true );
        if( state.contaminated_ )
            personnel.set_contaminated( true );

        // Final looks after story 660 ...
        //personnel.set_quantity( state.number_ );
        //personnel.set_rank( state.rank_.GetAsnID() );
        //personnel.set_state( state.state_->GetAsnID() );
        //personnel.set_location( state.location_->GetAsnID() );
        //for( auto it = state.injuries_.begin(); state.injuries_.end(); ++it )
        //{
        //    sword::Injury* injury = personnel.mutable_injuries()->Add();
        //    injury->set_id( it->first );
        //    injury->set_seriousness( it->second );
        //}
        //if( state.psyop_ )
        //    personnel.set_mentally_wounded( true );
        //if( state.contaminated_ )
        //    personnel.set_contaminated( true );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::SendChangedState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::SendChangedState() const
{
    std::for_each( humansToUpdate_.begin(), humansToUpdate_.end(), boost::bind( &Human_ABC::SendChangedState, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::SendFullState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::SendFullState( unsigned int context ) const
{
    std::for_each( humansToUpdate_.begin(), humansToUpdate_.end(), boost::bind( &Human_ABC::SendFullState, _1, context ) );
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::ChangeHumansAvailability
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::ChangeHumansAvailability( const PHY_HumanRank& rank, unsigned int nNewNbrFullyAliveHumans )
{
    unsigned int nbrOperational = GetNbrOperational( rank );
    nNewNbrFullyAliveHumans = std::min( nNewNbrFullyAliveHumans, GetNbrTotal( rank ) );

    std::auto_ptr< HealComputer_ABC > healComputer( pion_->GetAlgorithms().healComputerFactory_->Create() );
    pion_->Execute< OnComponentComputer_ABC >( *healComputer );

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
// Name: PHY_RolePion_Humans::CureAllHumans
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::HealAllHumans()
{
    std::auto_ptr< HealComputer_ABC > healComputer( pion_->GetAlgorithms().healComputerFactory_->Create() );
    pion_->Execute< OnComponentComputer_ABC >( *healComputer );
    healComputer->HealAll();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::Update( bool /*bIsDead*/ )
{
    for( auto it = humansToUpdate_.begin(); it != humansToUpdate_.end(); )
    {
        Human_ABC& human = **it;
        ++it;
        human.Update(); // !!! Can erase the human from humansToUpdate_ = bullshit ...
    }
    if( hasChanged_ )
    {
        pion_->Apply( &human::HumansChangedNotificationHandler_ABC::NotifyHumanHasChanged );
        pion_->Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::Clean
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::Clean()
{
    hasChanged_ = false;
    std::for_each( humansToUpdate_.begin(), humansToUpdate_.end(), boost::bind( &Human_ABC::Clean, _1 ) );
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
        const PHY_HumanState& state = **it;
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
        const PHY_HumanState& state = **it;
        if( state.rank_->GetID() == rank.GetID() && state.state_->GetID() == PHY_HumanWound::notWounded_.GetID() && state.location_ != Human_ABC::eMedical && !state.psyop_ && !state.contaminated_ )
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
        const PHY_HumanState& state = **it;
        if( state.state_->GetID() == PHY_HumanWound::notWounded_.GetID() && state.location_ != Human_ABC::eMedical && !state.psyop_ && !state.contaminated_ )
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
    {
        const PHY_HumanState& state = **it;
        result += state.number_;
    }
    return result;
}

} // namespace human
