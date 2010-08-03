// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_NBC.h"
#include "ToxicEffectHandler_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Objects/MIL_NbcAgentType.h"
#include "Entities/Objects/MIL_ToxicEffectManipulator.h"
#include "Entities/Orders/MIL_Report.h"
#include "protocol/ClientSenders.h"
#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include "simulation_kernel/WeaponReloadingComputer_ABC.h"
#include "simulation_kernel/SpeedComputer_ABC.h"

BOOST_CLASS_EXPORT_IMPLEMENT( nbc::PHY_RolePion_NBC )

namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, nbc::PHY_RolePion_NBC::T_NbcAgentTypeSet& set, const unsigned int nVersion )
        {
            split_free( file, set, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const nbc::PHY_RolePion_NBC::T_NbcAgentTypeSet& set, const unsigned int )
        {
            const unsigned size = set.size();
            file << size;
            for ( nbc::PHY_RolePion_NBC::CIT_NbcAgentTypeSet it = set.begin(); it != set.end(); ++it )
            {
                const unsigned id = (*it)->GetID();
                file << id;
            }
        }

        template< typename Archive >
        void load( Archive& file, nbc::PHY_RolePion_NBC::T_NbcAgentTypeSet& set, const unsigned int )
        {
            unsigned int nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                unsigned int nID;
                file >> nID;
                set.insert( MIL_NbcAgentType::Find( nID ) );
            }
        }
    }
}

namespace nbc
{

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePion_NBC* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* const pion = &role->pion_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RolePion_NBC* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* pion;
    archive >> pion;
    ::new( role )PHY_RolePion_NBC( *pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_NBC::PHY_RolePion_NBC( MIL_AgentPion& pion )
    : pion_                  ( pion )
    , bNbcProtectionSuitWorn_( false )
    , rContaminationState_   ( 0. )
    , rContaminationQuantity_( 0. )
    , bHasChanged_           ( true )
    , poisoned_              ( false )
    , lastStatePoisoning_    ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_NBC::~PHY_RolePion_NBC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePion_NBC::serialize( Archive& file, const unsigned int )
{
    file & ::boost::serialization::base_object< PHY_RoleInterface_NBC >( *this )
         & nbcAgentTypesContaminating_
         & bNbcProtectionSuitWorn_
         & rContaminationState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Poison
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Poison( const MIL_ToxicEffectManipulator& contamination )
{
    if( bNbcProtectionSuitWorn_ )
        return;
    if( ! lastStatePoisoning_ && ! poisoned_ )
        MIL_Report::PostEvent( pion_, MIL_Report::eReport_Poisoned );
    poisoned_ = true;
    pion_.Apply( &nbc::ToxicEffectHandler_ABC::ApplyPoisonous, contamination );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Contaminate
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Contaminate( const MIL_ToxicEffectManipulator& contamination )
{
    if( contamination.GetQuantity() < 1e-15 ) // TODO
        return;
    if( ! bNbcProtectionSuitWorn_ )
        pion_.Apply( &nbc::ToxicEffectHandler_ABC::ApplyContamination, contamination );
    nbcAgentTypesContaminating_.insert( &contamination.GetType() );
    if( rContaminationQuantity_ == 0 )
        MIL_Report::PostEvent( pion_, MIL_Report::eReport_Contaminated );
    rContaminationQuantity_ += contamination.GetQuantity();
    rContaminationState_ = 1.;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Decontaminate
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Decontaminate()
{
    if( rContaminationState_ == 0. )
    {
        assert( nbcAgentTypesContaminating_.empty() );
        return;
    }
    rContaminationState_ = 0.;
    rContaminationQuantity_ = 0.;
    bHasChanged_ = true;
    nbcAgentTypesContaminating_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Decontaminate
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Decontaminate( MT_Float rRatioAgentsWorking )
{
    if( rContaminationState_ == 0. )
    {
        assert( nbcAgentTypesContaminating_.empty() );
        return;
    }
    MT_Float rNewContaminationState = rContaminationState_ - ( pion_.GetType().GetUnitType().GetCoefDecontaminationPerTimeStep() * rRatioAgentsWorking );
    rNewContaminationState = std::max( rNewContaminationState, 0. );
    if( static_cast< unsigned int >( rNewContaminationState * 100. ) != ( rContaminationState_ * 100. ) )
        bHasChanged_ = true;
    rContaminationState_ = rNewContaminationState;
    rContaminationQuantity_ = std::max( 0., rContaminationQuantity_ * rContaminationState_ );
    if( rContaminationState_ == 0. )
    {
        rContaminationQuantity_ = 0.;
        nbcAgentTypesContaminating_.clear();
        bHasChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Execute
// Created: AHC 2009-10-02
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Execute( moving::SpeedComputer_ABC& algorithm ) const
{
    if( bNbcProtectionSuitWorn_ )
        algorithm.AddModifier( MIL_NbcAgentType::GetCoefMaxSpeedModificator(), true );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Execute
// Created: MGD 2009-10-05
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Execute( firing::WeaponReloadingComputer_ABC& algorithm ) const
{
    if( bNbcProtectionSuitWorn_ )
        algorithm.AddModifier( MIL_NbcAgentType::GetCoefReloadingTimeModificator() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::SendFullState( client::UnitAttributes& msg ) const
{
    if( !nbcAgentTypesContaminating_.empty() )
        for( CIT_NbcAgentTypeSet itNbcAgent = nbcAgentTypesContaminating_.begin(); itNbcAgent != nbcAgentTypesContaminating_.end(); ++itNbcAgent )
            msg().mutable_contamine_par_agents_nbc()->add_elem( (**itNbcAgent).GetID() );
    msg().set_en_tenue_de_protection_nbc( bNbcProtectionSuitWorn_ );
    msg().mutable_etat_contamination()->set_percentage( static_cast< unsigned int >( rContaminationState_ * 100. ) );
    msg().mutable_etat_contamination()->set_quantity( static_cast< float >( rContaminationQuantity_ ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::SendChangedState( client::UnitAttributes& msg ) const
{
    if( bHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitCanBeContaminated::IsContaminated
// Created: NLD 2004-05-05
// -----------------------------------------------------------------------------
bool PHY_RolePion_NBC::IsContaminated() const
{
    return !nbcAgentTypesContaminating_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::WearProtectionSuit
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::WearNbcProtectionSuit()
{
    if( bNbcProtectionSuitWorn_ )
        return;
    bNbcProtectionSuitWorn_ = true;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::RemoveProtectionSuit
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::RemoveNbcProtectionSuit()
{
    if( !bNbcProtectionSuitWorn_ )
        return;
    bNbcProtectionSuitWorn_ = false;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Update( bool /*bIsDead*/ )
{
    if( HasChanged() )
        pion_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
    if( IsContaminated() )
        ContaminateOtherUnits();
    lastStatePoisoning_ = poisoned_;
    poisoned_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Clean
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::HasChanged
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
bool PHY_RolePion_NBC::HasChanged() const
{
    return bHasChanged_;
}
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::ContaminateOtherUnits
// Created: HBD 2010-06-04
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::ContaminateOtherUnits()
{
    std::vector< const MIL_NbcAgentType* > typeNbcContaminating = GetContaminating();
    if( typeNbcContaminating.empty() || rContaminationState_ != 1. || rContaminationQuantity_ < 5 )
        return;
    TER_Agent_ABC::T_AgentPtrVector perceivableAgents;
    TER_World::GetWorld().GetAgentManager().GetListWithinCircle( pion_.Get< PHY_RoleInterface_Location >().GetPosition() ,
         MIL_NbcAgentType::GetContaminationDistance() , perceivableAgents );
    MT_Float minQuantity = MIL_NbcAgentType::GetMinContaminationQuantity();
    for( TER_Agent_ABC::CIT_AgentPtrVector it  = perceivableAgents.begin(); it != perceivableAgents.end(); ++it )
    {
        MIL_Agent_ABC& target = static_cast< PHY_RoleInterface_Location& >( **it ).GetAgent();
        if( target.GetID() != pion_.GetID() &&
            ( rContaminationQuantity_ - target.Get< PHY_RoleInterface_NBC >().GetContaminationQuantity()  >  minQuantity )  )
        {
            MIL_ToxicEffectManipulator* manipulator = new MIL_ToxicEffectManipulator( typeNbcContaminating, minQuantity );
            rContaminationQuantity_ -= minQuantity;
            target.Get< PHY_RoleInterface_NBC >().Contaminate( *manipulator );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: std::vector<const MIL_NbcAgentType*> PHY_RolePion_NBC::GetContaminating
// Created: HBD 2010-06-07
// -----------------------------------------------------------------------------
std::vector< const MIL_NbcAgentType* > PHY_RolePion_NBC::GetContaminating() const
{
    std::vector< const MIL_NbcAgentType* > result;
    if( !nbcAgentTypesContaminating_.empty() )
        for( CIT_NbcAgentTypeSet itNbcAgent = nbcAgentTypesContaminating_.begin(); itNbcAgent != nbcAgentTypesContaminating_.end(); ++itNbcAgent )
            if( (*itNbcAgent)->IsLiquidContaminating() || (*itNbcAgent)->IsGasContaminating() )
                result.push_back( *itNbcAgent );
    return result;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::GetContaminationQuantity
// Created: HBD 2010-06-08
// -----------------------------------------------------------------------------
MT_Float PHY_RolePion_NBC::GetContaminationQuantity() const
{
    return rContaminationQuantity_;
}
}
