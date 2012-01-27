// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
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
#include "simulation_terrain/TER_AgentManager.h"
#include "simulation_terrain/TER_World.h"

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
            const std::size_t size = set.size();
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
            std::size_t nNbr;
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

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_NBC::PHY_RolePion_NBC( MIL_AgentPion& pion )
    : owner_                 ( pion )
    , bNbcProtectionSuitWorn_( false )
    , rContaminationState_   ( 0. )
    , rContaminationQuantity_( 0. )
    , bHasChanged_           ( true )
    , poisoned_              ( false )
    , lastStatePoisoning_    ( false )
    , immune_                ( false )
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
         & rContaminationState_
         & immune_;
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
        MIL_Report::PostEvent( owner_, MIL_Report::eReport_Poisoned );
    poisoned_ = true;
    owner_.Apply( &nbc::ToxicEffectHandler_ABC::ApplyPoisonous, contamination );
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
        owner_.Apply( &nbc::ToxicEffectHandler_ABC::ApplyContamination, contamination );
    nbcAgentTypesContaminating_.insert( &contamination.GetType() );
    if( rContaminationQuantity_ == 0 )
        MIL_Report::PostEvent( owner_, MIL_Report::eReport_Contaminated );
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
void PHY_RolePion_NBC::Decontaminate( double rRatioAgentsWorking )
{
    if( rContaminationState_ == 0. )
    {
        assert( nbcAgentTypesContaminating_.empty() );
        return;
    }
    double rNewContaminationState = rContaminationState_ - ( owner_.GetType().GetUnitType().GetCoefDecontaminationPerTimeStep() * rRatioAgentsWorking );
    rNewContaminationState = std::max( rNewContaminationState, 0. );
    if( static_cast< unsigned int >( rNewContaminationState * 100. ) != ( rContaminationState_ * 100. ) )
        bHasChanged_ = true;
    rContaminationQuantity_ = std::max( 0., ( rContaminationQuantity_ / rContaminationState_ ) * rNewContaminationState );
    rContaminationState_ = rNewContaminationState;
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
        {
            sword::NBCAgentType& data = *msg().mutable_contamination_agents()->add_elem();
            data.set_id( (**itNbcAgent).GetID() );
        }
    msg().set_protective_suits( bNbcProtectionSuitWorn_ );
    msg().mutable_contamination_state()->set_percentage( static_cast< unsigned int >( rContaminationState_ * 100. ) );
    msg().mutable_contamination_state()->set_quantity( static_cast< float >( rContaminationQuantity_ ) );
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
// Name: PHY_RolePion_NBC::IsIntoxicated
// Created: LGY 2012-01-13
// -----------------------------------------------------------------------------
bool PHY_RolePion_NBC::IsIntoxicated() const
{
    return lastStatePoisoning_;
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
// Name: PHY_RolePion_NBC::IsImmune
// Created: JSR 2011-06-14
// -----------------------------------------------------------------------------
bool PHY_RolePion_NBC::IsImmune() const
{
    return immune_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::ImmunizeAgent
// Created: JSR 2011-06-14
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::ImmunizeAgent()
{
    immune_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::StopImmunizeAgent
// Created: JSR 2011-06-14
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::StopImmunizeAgent()
{
    immune_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Update( bool /*bIsDead*/ )
{
    if( HasChanged() )
        owner_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
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
    TER_World::GetWorld().GetAgentManager().GetListWithinCircle( owner_.Get< PHY_RoleInterface_Location >().GetPosition() ,
         MIL_NbcAgentType::GetContaminationDistance() , perceivableAgents );
    double minQuantity = MIL_NbcAgentType::GetMinContaminationQuantity();
    for( TER_Agent_ABC::CIT_AgentPtrVector it  = perceivableAgents.begin(); it != perceivableAgents.end(); ++it )
    {
        MIL_Agent_ABC& target = static_cast< PHY_RoleInterface_Location& >( **it ).GetAgent();
        if( target.GetID() != owner_.GetID() && !target.Get< PHY_RoleInterface_NBC >().IsImmune() &&
            ( rContaminationQuantity_ - target.Get< PHY_RoleInterface_NBC >().GetContaminationQuantity()  >  minQuantity )  )
        {
            MIL_ToxicEffectManipulator* manipulator = new MIL_ToxicEffectManipulator( typeNbcContaminating, minQuantity );
            rContaminationQuantity_ -= minQuantity;
            bHasChanged_ = true;
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
double PHY_RolePion_NBC::GetContaminationQuantity() const
{
    return rContaminationQuantity_;
}
}
