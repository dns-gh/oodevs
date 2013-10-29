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
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/WoundEffectsHandler_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.h"
#include "Entities/Objects/MIL_NbcAgentType.h"
#include "Entities/Objects/MIL_ToxicEffectManipulator.h"
#include "Entities/Objects/MIL_DisasterEffectManipulator.h"
#include "Entities/Objects/MIL_DisasterType.h"
#include "Entities/Orders/MIL_Report.h"
#include "protocol/ClientSenders.h"
#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include "simulation_kernel/WeaponReloadingComputer_ABC.h"
#include "simulation_kernel/SpeedComputer_ABC.h"
#include "simulation_terrain/TER_AgentManager.h"
#include "simulation_terrain/TER_World.h"

BOOST_CLASS_EXPORT_IMPLEMENT( nbc::PHY_RolePion_NBC )

namespace nbc
{

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC constructor
// Created: JSR 2013-01-09
// -----------------------------------------------------------------------------
PHY_RolePion_NBC::PHY_RolePion_NBC()
    : owner_                    ( 0 )
    , rDecontaminationState_    ( 1. )
    , rContaminationQuantity_   ( 0. )
    , dose_                     ( 0. )
    , bNbcProtectionSuitWorn_   ( false )
    , bHasChanged_              ( true )
    , poisoned_                 ( false )
    , intoxicated_              ( false )
    , immune_                   ( false )
    , contaminated_             ( false )
    , currentAttritionThreshold_( -1 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_NBC::PHY_RolePion_NBC( MIL_AgentPion& pion )
    : owner_                   ( &pion )
    , rDecontaminationState_    ( 1. )
    , rContaminationQuantity_   ( 0. )
    , dose_                     ( 0. )
    , bNbcProtectionSuitWorn_   ( false )
    , bHasChanged_              ( true )
    , poisoned_                 ( false )
    , intoxicated_              ( false )
    , immune_                   ( false )
    , contaminated_             ( false )
    , currentAttritionThreshold_( -1 )
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
    file & ::boost::serialization::base_object< PHY_RoleInterface_NBC >( *this );
    file & owner_;
    file & nbcAgentTypesContaminating_;
    file & rDecontaminationState_;
    file & rContaminationQuantity_;
    file & bNbcProtectionSuitWorn_;
    file & contaminated_;
    file & poisoned_;
    file & intoxicated_;
    file & immune_;
    file & dose_;
    file & currentAttritionThreshold_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Poison
// Created: NLD 2006-10-27
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Poison( const MIL_ToxicEffectManipulator& contamination )
{
    if( owner_->GetType().GetUnitType().IsAutonomous() )
        return;
    if( bNbcProtectionSuitWorn_ )
        return;
    if( ! intoxicated_ && ! poisoned_ )
        MIL_Report::PostEvent( *owner_, report::eRC_Empoisonne );
    poisoned_ = true;
    owner_->Apply( &WoundEffectsHandler_ABC::ApplyEffect, contamination );
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
    {
        owner_->GetRole< PHY_RoleInterface_Composantes >().ApplyContamination();
        owner_->GetRole< transport::PHY_RoleAction_Transport >().ApplyContamination();
    }
    nbcAgentTypesContaminating_.insert( &contamination.GetType() );
    if( rContaminationQuantity_ == 0 )
        MIL_Report::PostEvent( *owner_, report::eRC_Contamine );
    rContaminationQuantity_ += contamination.GetQuantity();
    rDecontaminationState_ = 0.;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Decontaminate
// Created: NLD 2004-11-02
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Decontaminate()
{
    if( rDecontaminationState_ == 1. )
    {
        assert( nbcAgentTypesContaminating_.empty() );
        return;
    }
    rDecontaminationState_ = 1.;
    rContaminationQuantity_ = 0.;
    bHasChanged_ = true;
    contaminated_ = false;
    nbcAgentTypesContaminating_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Decontaminate
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Decontaminate( double rRatioAgentsWorking )
{
    if( rDecontaminationState_ >= 1. )
    {
        assert( nbcAgentTypesContaminating_.empty() );
        return;
    }
    double rNewDecontaminationState = rDecontaminationState_ + ( owner_->GetType().GetUnitType().GetCoefDecontaminationPerTimeStep() * rRatioAgentsWorking );
    rNewDecontaminationState = std::min( rNewDecontaminationState, 1. );
    if( static_cast< unsigned int >( rNewDecontaminationState * 100. ) != ( rDecontaminationState_ * 100. ) )
        bHasChanged_ = true;
    rContaminationQuantity_ = std::max( 0., ( rContaminationQuantity_ / rNewDecontaminationState ) * rDecontaminationState_ );
    rDecontaminationState_ = rNewDecontaminationState;
    if( rDecontaminationState_ >= 1. )
    {
        rContaminationQuantity_ = 0.;
        nbcAgentTypesContaminating_.clear();
        contaminated_ = false;
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
    for( auto it = nbcAgentTypesContaminating_.begin(); it != nbcAgentTypesContaminating_.end(); ++it )
        msg().mutable_contamination_agents()->add_elem()->set_id( (*it)->GetID() );
    msg().set_protective_suits( bNbcProtectionSuitWorn_ );
    const unsigned int rDecontaminationState = static_cast< unsigned int >( rDecontaminationState_ * 100 );
    msg().mutable_contamination_state()->set_decontamination_process( rDecontaminationState );
    msg().mutable_contamination_state()->set_percentage( 100u - rDecontaminationState );
    msg().mutable_contamination_state()->set_quantity( static_cast< float >( rContaminationQuantity_ ) );
    msg().mutable_contamination_state()->set_contaminated( IsContaminated() );
    msg().mutable_contamination_state()->set_dose( dose_ );
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
    return !nbcAgentTypesContaminating_.empty() || contaminated_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::IsIntoxicated
// Created: LGY 2012-01-13
// -----------------------------------------------------------------------------
bool PHY_RolePion_NBC::IsIntoxicated() const
{
    return intoxicated_;
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
// Name: PHY_RolePion_NBC::IsForcedImmune
// Created: GGE 2012-05-14
// -----------------------------------------------------------------------------
bool PHY_RolePion_NBC::IsForcedImmune() const
{
    return immune_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::TemporaryImmunizeAgent
// Created: GGE 2011-06-14
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::TemporaryImmunizeAgent( bool bImmunize )
{
    immune_ = bImmunize;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Update( bool /*bIsDead*/ )
{
    if( HasChanged() )
        owner_->Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
    if( IsContaminated() )
        ContaminateOtherUnits();
    intoxicated_ = poisoned_;
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
    if( typeNbcContaminating.empty() || !contaminated_ || rContaminationQuantity_ < 5 )
        return;
    TER_Agent_ABC::T_AgentPtrVector perceivableAgents;
    TER_World::GetWorld().GetAgentManager().GetListWithinCircle( owner_->Get< PHY_RoleInterface_Location >().GetPosition() ,
         MIL_NbcAgentType::GetContaminationDistance() , perceivableAgents );
    double minQuantity = MIL_NbcAgentType::GetMinContaminationQuantity();
    for( auto it  = perceivableAgents.begin(); it != perceivableAgents.end(); ++it )
    {
        MIL_Agent_ABC& target = static_cast< PHY_RoleInterface_Location& >( **it ).GetAgent();
        if( target.GetID() != owner_->GetID() && !target.Get< PHY_RoleInterface_NBC >().IsForcedImmune() &&
            ( rContaminationQuantity_ - target.Get< PHY_RoleInterface_NBC >().GetContaminationQuantity() > minQuantity ) )
        {
            rContaminationQuantity_ -= minQuantity;
            bHasChanged_ = true;
            const MIL_ToxicEffectManipulator manipulator( typeNbcContaminating, minQuantity );
            target.Get< PHY_RoleInterface_NBC >().Contaminate( manipulator );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::GetContaminating
// Created: HBD 2010-06-07
// -----------------------------------------------------------------------------
std::vector< const MIL_NbcAgentType* > PHY_RolePion_NBC::GetContaminating() const
{
    std::vector< const MIL_NbcAgentType* > result;
    for( auto it = nbcAgentTypesContaminating_.begin(); it != nbcAgentTypesContaminating_.end(); ++it )
        if( (*it)->IsLiquidContaminating() || (*it)->IsGasContaminating() )
            result.push_back( *it );
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

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Afflict
// Created: LGY 2012-11-22
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::Afflict( float dose, const MIL_DisasterType& type )
{
    dose_ += dose;
    ApplyWound( type );
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::ApplyWound
// Created: LGY 2012-11-29
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::ApplyWound( const MIL_DisasterType& type )
{
    int currentAttritionThreshold = type.GetAttritionThreshold( dose_ );
    if( currentAttritionThreshold != currentAttritionThreshold_ )
    {
        MIL_DisasterEffectManipulator manipulator( currentAttritionThreshold, type );
        owner_->Apply( &WoundEffectsHandler_ABC::ApplyEffect, manipulator );
        currentAttritionThreshold_ = currentAttritionThreshold;
        if( type.IsContaminated( dose_ ) )
        {
            contaminated_ = true;
            owner_->GetRole< PHY_RoleInterface_Composantes >().ApplyContamination();
        }
        rDecontaminationState_ = 0.;
        bHasChanged_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::StartDecontamination
// Created: LGY 2013-01-04
// -----------------------------------------------------------------------------
void PHY_RolePion_NBC::StartDecontamination()
{
    rDecontaminationState_ = 0.;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::GetDecontaminationState
// Created: LGY 2013-01-04
// -----------------------------------------------------------------------------
double PHY_RolePion_NBC::GetDecontaminationState() const
{
    return rDecontaminationState_;
}

}
