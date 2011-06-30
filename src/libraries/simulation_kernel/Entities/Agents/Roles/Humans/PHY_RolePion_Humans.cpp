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
#include "PHY_RolePion_Humans.h"
#include "Entities/Agents/Roles/Logistic/PHY_MedicalHumanState.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Agents/Units/Humans/PHY_Human.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "protocol/ClientSenders.h"
#include "simulation_kernel/AlgorithmsFactories.h"
#include "simulation_kernel/HealComputer_ABC.h"
#include "simulation_kernel/HealComputerFactory_ABC.h"
#include "simulation_kernel/HumansChangedNotificationHandler_ABC.h"
#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include "MT_Tools/MT_ScipioException.h"
#include <boost/serialization/set.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( human::PHY_RolePion_Humans )

namespace human
{

template< typename Archive >
inline void save_construct_data( Archive& archive, const PHY_RolePion_Humans* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* const pion = &role->pion_;
    archive << pion;
}

template< typename Archive >
inline void load_construct_data( Archive& archive, PHY_RolePion_Humans* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* pion;
    archive >> pion;
    ::new( role )PHY_RolePion_Humans( *pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::T_HumanData::T_HumanData
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
PHY_RolePion_Humans::T_HumanData::T_HumanData()
    : nNbrTotal_                ( 0 )
    , nNbrOperational_          ( 0 )
    , nNbrDead_                 ( 0 )
    , nNbrWounded_              ( 0 )
    , nNbrMentalDiseased_       ( 0 )
    , nNbrNBC_                  ( 0 )
    , nNbrInLogisticMedical_    ( 0 )
    , nNbrInLogisticMaintenance_( 0 )
    , bHasChanged_              ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Humans::T_HumanData::serialize( Archive& file, const unsigned int )
{
    file & nNbrTotal_
         & nNbrOperational_
         & nNbrDead_
         & nNbrWounded_
         & nNbrMentalDiseased_
         & nNbrNBC_
         & nNbrInLogisticMedical_
         & nNbrInLogisticMaintenance_
         & bHasChanged_;
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
PHY_RolePion_Humans::PHY_RolePion_Humans( MIL_AgentPion& pion )
    : pion_                   ( pion )
    , humansData_             ( PHY_HumanRank::GetHumanRanks().size(), T_HumanData() )
    , nNbrHumansDataChanged_  ( humansData_.size() )
    , nNbrUsableHumans_       ( 0 )
    , nNbrHumans_             ( 0 )
    , humansToUpdate_         ()
    , medicalHumanStates_     ()
    , nTickRcMedicalQuerySent_( 0 )
    , nEvacuationMode_        ( eEvacuationMode_Auto )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
PHY_RolePion_Humans::~PHY_RolePion_Humans()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_Humans::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Humans >( *this )
         & humansData_
         & nNbrUsableHumans_
         & nNbrHumans_
         & humansToUpdate_
         & nNbrHumansDataChanged_
         & medicalHumanStates_
         & nTickRcMedicalQuerySent_;
}

namespace
{
    struct sRankData
    {
        sRankData() : nNbrOfficiers_( 0 ), nNbrSousOfficiers_( 0 ), nNbrMdrs_( 0 ) {}
        unsigned int nNbrOfficiers_;
        unsigned int nNbrSousOfficiers_;
        unsigned int nNbrMdrs_;
    };
    typedef std::map< const PHY_HumanWound*, sRankData > T_HumanWoundData;
    typedef T_HumanWoundData::const_iterator             CIT_HumanWoundData;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
//void PHY_RolePion_Humans::WriteODB( xml::xostream& /*xos*/ ) const
//{
//}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::ChangeHumansAvailability
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::ChangeHumansAvailability( const PHY_HumanRank& rank, unsigned int nNewNbrFullyAliveHumans )
{

    const T_HumanData& humanData = humansData_[ rank.GetID() ];
    nNewNbrFullyAliveHumans = std::min( nNewNbrFullyAliveHumans, humanData.nNbrTotal_ );

    std::auto_ptr< HealComputer_ABC > healComputer( pion_.GetAlgorithms().healComputerFactory_->Create() );
    pion_.Execute< OnComponentComputer_ABC >( *healComputer );

    if( nNewNbrFullyAliveHumans > humanData.nNbrOperational_ )
        healComputer->Heal( rank, nNewNbrFullyAliveHumans - humanData.nNbrOperational_ );
    else if( nNewNbrFullyAliveHumans < humanData.nNbrOperational_ )
        healComputer->Wound( rank, humanData.nNbrOperational_ - nNewNbrFullyAliveHumans );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::CureAllHumans
// Created: NLD 2004-09-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::HealAllHumans()
{
    std::auto_ptr< HealComputer_ABC > healComputer( pion_.GetAlgorithms().healComputerFactory_->Create() );
    pion_.Execute< OnComponentComputer_ABC >( *healComputer );
    healComputer->HealAll();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::Update( bool /*bIsDead*/ )
{
    for( CIT_HumanSet it = humansToUpdate_.begin(); it != humansToUpdate_.end(); )
    {
        Human_ABC& human = **it;
        ++it;
        human.Update(); // !!! Can erase the human from humansToUpdate_
    }

    if( HasChanged() )
    {
        pion_.Apply( &human::HumansChangedNotificationHandler_ABC::NotifyHumanHasChanged );
        pion_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::Clean
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::Clean()
{
    if( nNbrHumansDataChanged_ > 0 )
    {
        for( IT_HumanDataVector it = humansData_.begin(); it != humansData_.end(); ++it )
            it->bHasChanged_ = false;
        nNbrHumansDataChanged_ = 0;
    }

    for( CIT_MedicalHumanStateSet it = medicalHumanStates_.begin(); it != medicalHumanStates_.end(); ++it )
        (**it).Clean();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::GetNbrHumans
// Created: SBO 2005-12-21
// -----------------------------------------------------------------------------
unsigned int PHY_RolePion_Humans::GetNbrHumans( const PHY_HumanRank& rank ) const
{
    const T_HumanData& humanData = humansData_[ rank.GetID() ];
    return humanData.nNbrTotal_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::GetNbrAliveHumans
// Created: SBO 2005-12-21
// -----------------------------------------------------------------------------
unsigned int PHY_RolePion_Humans::GetNbrAliveHumans( const PHY_HumanRank& rank ) const
{
    const T_HumanData& humanData = humansData_[ rank.GetID() ];
    return humanData.nNbrOperational_;
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
    assert( nNbrHumans_ > 0 );
    -- nNbrHumans_;

    T_HumanData& humanData = humansData_[ human.GetRank().GetID() ];
    if( !humanData.bHasChanged_ )
    {
        ++nNbrHumansDataChanged_;
        humanData.bHasChanged_ = true;
    }

    assert( humanData.nNbrTotal_ > 0 );
    -- humanData.nNbrTotal_;

    if( human.IsUsable() )
    {
        assert( nNbrUsableHumans_ > 0 );
        -- nNbrUsableHumans_;
    }
    if( human.IsDead() )
    {
        assert( humanData.nNbrDead_ > 0 );
        -- humanData.nNbrDead_;
    }
    if( human.IsWounded() )
    {
        assert( humanData.nNbrWounded_ > 0 );
        -- humanData.nNbrWounded_;
    }
    if( human.IsContaminated() )
    {
        assert( humanData.nNbrNBC_ > 0 );
        -- humanData.nNbrNBC_;
    }
    if( human.IsMentalDiseased() )
    {
        assert( humanData.nNbrMentalDiseased_ > 0 );
        -- humanData.nNbrMentalDiseased_;
    }
    if( human.IsUsable() && !human.NeedMedical() )
    {
        assert( humanData.nNbrOperational_ > 0 );
        -- humanData.nNbrOperational_;
    }

    switch( human.GetLocation() )
    {
        case Human_ABC::eBattleField: break;
        case Human_ABC::eMaintenance: assert( humanData.nNbrInLogisticMaintenance_ > 0 ); -- humanData.nNbrInLogisticMaintenance_; break;
        case Human_ABC::eMedical    : assert( humanData.nNbrInLogisticMedical_     > 0 ); -- humanData.nNbrInLogisticMedical_; break;
        default:
            assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::UpdateDataWhenHumanAdded
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::UpdateDataWhenHumanAdded( const Human_ABC& human )
{
    ++ nNbrHumans_;

    T_HumanData& humanData = humansData_[ human.GetRank().GetID() ];
    if( !humanData.bHasChanged_ )
    {
        ++nNbrHumansDataChanged_;
        humanData.bHasChanged_ = true;
    }
    ++ humanData.nNbrTotal_;

    if( human.IsUsable() )
        ++ nNbrUsableHumans_;
    if( human.IsDead() )
        ++ humanData.nNbrDead_;
    if( human.IsWounded() )
        ++ humanData.nNbrWounded_;
    if( human.IsContaminated() )
        ++ humanData.nNbrNBC_;
    if( human.IsMentalDiseased() )
        ++ humanData.nNbrMentalDiseased_;
    if( human.IsUsable() && !human.NeedMedical() )
        ++ humanData.nNbrOperational_;

    switch( human.GetLocation() )
    {
        case Human_ABC::eBattleField: break;
        case Human_ABC::eMaintenance: ++ humanData.nNbrInLogisticMaintenance_; break;
        case Human_ABC::eMedical    : ++ humanData.nNbrInLogisticMedical_    ; break;
        default:
            assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::NotifyHumanAdded
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::NotifyHumanAdded( Human_ABC& human )
{
    UpdateDataWhenHumanAdded( human );
    if( human.NeedMedical() )
        humansToUpdate_.insert( &human );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::NotifyHumanRemoved
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::NotifyHumanRemoved( Human_ABC& human )
{
    UpdateDataWhenHumanRemoved( human );
    humansToUpdate_.erase( &human );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::NotifyHumanChanged
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::NotifyHumanChanged( Human_ABC& human, const Human_ABC& copyOfOldHumanState )
{
    UpdateDataWhenHumanRemoved( copyOfOldHumanState );
    UpdateDataWhenHumanAdded  ( human );

    const bool bOldHumanNeedUpdate = copyOfOldHumanState.NeedMedical();
    const bool bNewHumanNeedUpdate = human              .NeedMedical();

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
    std::auto_ptr< HealComputer_ABC > healComputer( pion_.GetAlgorithms().healComputerFactory_->Create() );
    pion_.Execute< OnComponentComputer_ABC >( *healComputer );
    healComputer->EvacuateWoundedHumans( destinationTC2 );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::HasWoundedHumansToEvacuate
// Created: NLD 2005-08-08
// -----------------------------------------------------------------------------
bool PHY_RolePion_Humans::HasWoundedHumansToEvacuate() const
{
    std::auto_ptr< HealComputer_ABC > healComputer( pion_.GetAlgorithms().healComputerFactory_->Create() );
    pion_.Execute< OnComponentComputer_ABC >( *healComputer );
    return healComputer->HasWoundedHumansToEvacuate();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::NotifyHumanEvacuatedByThirdParty
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::NotifyHumanEvacuatedByThirdParty( Human_ABC& human, MIL_AutomateLOG& destinationTC2 )
{
    PHY_MedicalHumanState* pMedicalHumanState = destinationTC2.MedicalHandleHumanEvacuatedByThirdParty( pion_, human );
    if( !pMedicalHumanState )
    {
        human.SetMedicalState( 0 );
        return;
    }
    if( ! medicalHumanStates_.insert( pMedicalHumanState ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
    human.SetMedicalState( pMedicalHumanState );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::NotifyHumanWaitingForMedical
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::NotifyHumanWaitingForMedical( Human_ABC& human )
{
    MIL_AutomateLOG* pTC2 = pion_.GetLogisticHierarchy().GetPrimarySuperior();
    if( !pTC2 || nEvacuationMode_ == eEvacuationMode_Manual )
    {
        human.SetMedicalState( 0 );
        return;
    }
    // Pas de RC si log non branchée ou si RC envoyé au tick précédent
    const unsigned int nCurrentTick = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    if( nCurrentTick > ( nTickRcMedicalQuerySent_ + 1 ) || nTickRcMedicalQuerySent_ == 0 )
        MIL_Report::PostEvent( pion_, MIL_Report::eReport_MedicalEvacuationRequest );
    nTickRcMedicalQuerySent_ = nCurrentTick;

    PHY_MedicalHumanState* pMedicalHumanState = pTC2->MedicalHandleHumanForEvacuation( pion_, human );
    if( !pMedicalHumanState )
    {
        human.SetMedicalState( 0 );
        return;
    }

    if( ! medicalHumanStates_.insert( pMedicalHumanState ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
    human.SetMedicalState( pMedicalHumanState );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::NotifyHumanBackFromMedical
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::NotifyHumanBackFromMedical( PHY_MedicalHumanState& humanState )
{
    if( medicalHumanStates_.erase( &humanState ) != 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Erase failed" );
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
    if( nNbrHumansDataChanged_ == 0 )
        return;
    assert( nNbrHumansDataChanged_ <= humansData_.size() );
    const PHY_HumanRank::T_HumanRankMap& ranks = PHY_HumanRank::GetHumanRanks();
    for( PHY_HumanRank::CIT_HumanRankMap itRank = ranks.begin(); itRank != ranks.end(); ++itRank )
    {
        const PHY_HumanRank& rank = *itRank->second;
        const T_HumanData& humanData = humansData_[ rank.GetID() ];
        sword::HumanDotations::HumanDotation& personnel = *message().mutable_human_dotations()->add_elem();
        personnel.set_rank               ( rank.GetAsnID() );
        personnel.set_total              ( humanData.nNbrTotal_ );
        personnel.set_operational        ( humanData.nNbrOperational_ );
        personnel.set_dead               ( humanData.nNbrDead_ );
        personnel.set_wounded            ( humanData.nNbrWounded_ );
        personnel.set_mentally_wounded   ( humanData.nNbrMentalDiseased_ );
        personnel.set_contaminated       ( humanData.nNbrNBC_ );
        personnel.set_healing            ( humanData.nNbrInLogisticMedical_ );
        personnel.set_maintenance        ( humanData.nNbrInLogisticMaintenance_ );
        personnel.set_unevacuated_wounded( 0 ); //$$$ RPD TO IMPLEMENT
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::SendFullState
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::SendFullState( client::UnitAttributes& message ) const
{
    const PHY_HumanRank::T_HumanRankMap& ranks = PHY_HumanRank::GetHumanRanks();
    for( PHY_HumanRank::CIT_HumanRankMap itRank = ranks.begin(); itRank != ranks.end(); ++itRank )
    {
        const PHY_HumanRank& rank = *itRank->second;
        const T_HumanData& humanData = humansData_[ rank.GetID() ];
        sword::HumanDotations::HumanDotation& personnel = *message().mutable_human_dotations()->add_elem();
        personnel.set_rank               ( rank.GetAsnID() );
        personnel.set_total              ( humanData.nNbrTotal_ );
        personnel.set_operational        ( humanData.nNbrOperational_ );
        personnel.set_dead               ( humanData.nNbrDead_ );
        personnel.set_wounded            ( humanData.nNbrWounded_ );
        personnel.set_mentally_wounded   ( humanData.nNbrMentalDiseased_ );
        personnel.set_contaminated       ( humanData.nNbrNBC_ );
        personnel.set_healing            ( humanData.nNbrInLogisticMedical_ );
        personnel.set_maintenance        ( humanData.nNbrInLogisticMaintenance_ );
        personnel.set_unevacuated_wounded( 0 ); //$$$ RPD TO IMPLEMENT
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::SendChangedState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::SendChangedState() const
{
    for( CIT_MedicalHumanStateSet it = medicalHumanStates_.begin(); it != medicalHumanStates_.end(); ++it )
        (**it).SendChangedState();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::SendFullState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::SendFullState( unsigned int context ) const
{
    for( CIT_MedicalHumanStateSet it = medicalHumanStates_.begin(); it != medicalHumanStates_.end(); ++it )
        (**it).SendFullState( context );
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
// Name: PHY_RolePion_Humans::HasChanged
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool PHY_RolePion_Humans::HasChanged() const
{
    return nNbrHumansDataChanged_ > 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Humans::ChangeEvacuationMode
// Created: NLD 2005-08-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Humans::ChangeEvacuationMode( E_EvacuationMode nMode )
{
    nEvacuationMode_ = nMode;
}

} // namespace human
