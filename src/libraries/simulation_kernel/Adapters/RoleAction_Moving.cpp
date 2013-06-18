// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "RoleAction_Moving.h"
#include "PostureComputer_ABC.h"
#include "SpeedComputer_ABC.h"
#include "MaxSlopeComputer_ABC.h"
#include "MoveComputer_ABC.h"
#include "MoveComputerFactory_ABC.h"
#include "ConsumptionComputerFactory_ABC.h"
#include "ConsumptionModeChangeRequest_ABC.h"
#include "ConsumptionChangeRequestHandler_ABC.h"
#include "AlgorithmsFactories.h"
#include "Entities/Agents/Actions/Moving/SpeedComputerStrategy.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include <core/Model.h>

BOOST_CLASS_EXPORT_IMPLEMENT( sword::RoleAction_Moving )

using namespace sword;

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving constructor
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
RoleAction_Moving::RoleAction_Moving()
    : owner_               ( 0 )
    , entity_              ( 0 )
    , rSpeed_              ( 0.)
    , rSpeedModificator_   ( 1. )
    , rMaxSpeedModificator_( 1. )
    , bHasMove_            ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving constructor
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
RoleAction_Moving::RoleAction_Moving( MIL_AgentPion& pion, const core::Model& entity )
    : owner_               ( &pion )
    , entity_              ( &entity )
    , rSpeed_              ( 0.)
    , rSpeedModificator_   ( 1. )
    , rMaxSpeedModificator_( 1. )
    , bHasMove_            ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving destructor
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
RoleAction_Moving::~RoleAction_Moving()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void RoleAction_Moving::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< moving::PHY_RoleAction_InterfaceMoving >( *this );
    file & owner_
         & entity_;
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::Execute
// Created: DDA 2010-06-14
// -----------------------------------------------------------------------------
void RoleAction_Moving::Execute( moving::SpeedComputer_ABC& algorithm ) const
{
    algorithm.AddModifier( rMaxSpeedModificator_ * (*entity_)[ "movement/traffic-modifier" ], true );
    algorithm.AddModifier( rSpeedModificator_, false );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::ComputeSpeed
// Created: MCO 2013-06-13
// -----------------------------------------------------------------------------
double RoleAction_Moving::ComputeSpeed( const moving::SpeedStrategy_ABC& strategy ) const
{
    std::auto_ptr< moving::SpeedComputer_ABC > computer = owner_->GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy );
    owner_->Execute( *computer );
    return computer->GetSpeed();
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
double RoleAction_Moving::GetMaxSpeed( const TerrainData& environment ) const
{
    return ComputeSpeed( moving::SpeedComputerStrategy( true, false, environment ) );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
double RoleAction_Moving::GetMaxSpeed() const
{
    return ComputeSpeed( moving::SpeedComputerStrategy( true, false ) );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::GetMaxSpeedWithReinforcement
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
double RoleAction_Moving::GetMaxSpeedWithReinforcement() const
{
    return ComputeSpeed( moving::SpeedComputerStrategy( true, true ) );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::GetTheoricMaxSpeedWithReinforcement
// Created: LDC 2012-08-27
// -----------------------------------------------------------------------------
double RoleAction_Moving::GetTheoricMaxSpeedWithReinforcement() const
{
    return ComputeSpeed( moving::SpeedComputerStrategy( true, true, true ) );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::GetTheoricSpeedWithReinforcement
// Created: MCO 2013-06-13
// -----------------------------------------------------------------------------
double RoleAction_Moving::GetTheoricSpeedWithReinforcement( const TerrainData& environment ) const
{
    const moving::SpeedComputerStrategy strategy( false, true, environment, true );
    return std::min( ComputeSpeed( strategy ), GetMaxSpeedWithReinforcement() );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::GetTheoricMaxSpeed
// Created: LMT 2010-05-04
// -----------------------------------------------------------------------------
double RoleAction_Moving::GetTheoricMaxSpeed( bool loaded ) const
{
    return ComputeSpeed( moving::SpeedComputerStrategy( true, false, true, loaded ) );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::GetSpeedWithReinforcement
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
double RoleAction_Moving::GetSpeedWithReinforcement( const TerrainData& environment ) const
{
    const moving::SpeedComputerStrategy strategy( false, true, environment );
    return std::min( ComputeSpeed( strategy ), GetMaxSpeedWithReinforcement() );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::GetSpeedWithReinforcement
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
double RoleAction_Moving::GetSpeedWithReinforcement( const TerrainData& environment, const MIL_Object_ABC& object ) const
{
    if( !object().HasMobilityInfluence() )
        return std::numeric_limits< double >::max();
    if( !object().IsTrafficable( *owner_ ) )
        return 0;
    const double rCurrentMaxSpeed = GetMaxSpeed();
    const double rCurrentEnvSpeed = GetSpeedWithReinforcement( environment );
    const moving::SpeedComputerStrategy strategy( false, true, object );
    double rObjectSpeed = std::min( ComputeSpeed( strategy ), rCurrentMaxSpeed );
    rObjectSpeed *= rSpeedModificator_;
    return object().ApplySpeedPolicy( rObjectSpeed, rCurrentEnvSpeed, rCurrentMaxSpeed, *owner_ );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::GetMaxSlope
// Created: AGE 2005-04-13
// -----------------------------------------------------------------------------
double RoleAction_Moving::GetMaxSlope() const
{
    std::auto_ptr< moving::MaxSlopeComputer_ABC > computer =
            owner_->GetAlgorithms().moveComputerFactory_->CreateMaxSlopeComputer();
    owner_->Execute< OnComponentComputer_ABC >( *computer );
    return computer->GetMaxSlope();
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::ApplyMove
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void RoleAction_Moving::ApplyMove( const MT_Vector2D& /*position*/, const MT_Vector2D& /*direction*/, double rSpeed, double /*rWalkedDistance*/ )
{
    if( ! CanMove() )
        return;
    rSpeed_ = rSpeed;
    bHasMove_ = ( rSpeed != 0 );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::CanMove
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
bool RoleAction_Moving::CanMove() const
{
    std::auto_ptr< moving::MoveComputer_ABC > moveComputer = owner_->GetAlgorithms().moveComputerFactory_->CreateMoveComputer();
    owner_->Execute( *moveComputer );
    return moveComputer->CanMoveOverride() || moveComputer->CanMove();
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::IsReady
// Created: SLI 2013-04-19
// -----------------------------------------------------------------------------
bool RoleAction_Moving::IsReady() const
{
    return owner_->GetRole< PHY_RoleInterface_Posture >().IsMovingPosture();
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::HasResources
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
bool RoleAction_Moving::HasResources()
{
    std::auto_ptr< moving::MoveComputer_ABC > moveComputer = owner_->GetAlgorithms().moveComputerFactory_->CreateMoveComputer();
    owner_->Execute( *moveComputer );
    if( moveComputer->CanMoveOverride() )
        return true;
    std::auto_ptr< dotation::ConsumptionModeChangeRequest_ABC > request =
            owner_->GetAlgorithms().consumptionComputerFactory_->CreateConsumptionModeChangeRequest(PHY_ConsumptionType::moving_);
    owner_->Apply( &dotation::ConsumptionChangeRequestHandler_ABC::ChangeConsumptionMode, *request ); // automatic rollback
    return request->AllChanged();
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::Update
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void RoleAction_Moving::Update( bool /*bIsDead*/ )
{
    if( ! bHasMove_ )
        rSpeed_ = 0;
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::Clean
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void RoleAction_Moving::Clean()
{
    bHasMove_ = false;
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::SetSpeedModificator
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void RoleAction_Moving::SetSpeedModificator( double rFactor )
{
    rSpeedModificator_ = rFactor;
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::SetMaxSpeedModificator
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void RoleAction_Moving::SetMaxSpeedModificator( double rFactor )
{
    if( rFactor <= 0. )
        throw MASA_EXCEPTION( "Cannot set max speed to 0 or less: Would prevent pathfind from working. Please inhibit move actions in decisional instead." );
    rMaxSpeedModificator_ = rFactor;
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::GetMaxSpeedModificator
// Created: LMT 2011-07-06
// -----------------------------------------------------------------------------
double RoleAction_Moving::GetMaxSpeedModificator() const
{
    return rMaxSpeedModificator_;
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::Execute
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void RoleAction_Moving::Execute( posture::PostureComputer_ABC& algorithm ) const
{
    if( (*entity_)[ "movement/intention" ] )
        algorithm.SetPostureMovement();
    else
        algorithm.UnsetPostureMovement();
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::HasKnowledgeObject
// Created: LGY 2013-01-21
// -----------------------------------------------------------------------------
bool RoleAction_Moving::HasKnowledgeObject( const MIL_Object_ABC& object ) const
{
    return owner_->GetKnowledgeGroup()->GetKnowledgeObjectContainer().HasKnowledgeObject( object );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::ApplyTrafficModifier
// Created: JSR 2012-01-12
// -----------------------------------------------------------------------------
void RoleAction_Moving::ApplyTrafficModifier()
{
    // NOTHING
}
