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
#include "ObjectCollisionNotificationHandler_ABC.h"
#include "AlgorithmsFactories.h"
#include "Entities/Agents/Actions/Moving/SpeedComputerStrategy.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "Decision/DEC_Representations.h"
#include "Decision/DEC_PathPoint.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( sword::RoleAction_Moving )

using namespace sword;

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving constructor
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
RoleAction_Moving::RoleAction_Moving( MIL_AgentPion& pion )
    : owner_                 ( pion )
    , rSpeed_                ( 0.)
    , rSpeedModificator_     ( 1. )
    , rMaxSpeedModificator_  ( 1. )
    , bCurrentPathHasChanged_( true )
    , bEnvironmentHasChanged_( true )
    , bHasMove_              ( false )
    , bTheoricMaxSpeed_      ( false )
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
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::Execute
// Created: DDA 2010-06-14
// -----------------------------------------------------------------------------
void RoleAction_Moving::Execute( moving::SpeedComputer_ABC& algorithm ) const
{
    if( !bTheoricMaxSpeed_ )
        algorithm.AddModifier( rMaxSpeedModificator_, true );
    algorithm.AddModifier( rSpeedModificator_, false );
}

namespace
{
    double ComputeSpeed( MIL_AgentPion& owner, const moving::SpeedComputerStrategy& strategy )
    {
        std::auto_ptr< moving::SpeedComputer_ABC > computer = owner.GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy );
        owner.Execute( *computer );
        return computer->GetSpeed();
    }
    double ComputeSpeed( MIL_AgentPion& owner, const moving::SpeedComputerStrategy& strategy, bool loaded )
    {
        std::auto_ptr< moving::SpeedComputer_ABC > computer = owner.GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy, loaded );
        owner.Execute( *computer );
        return computer->GetSpeed();
    }
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
double RoleAction_Moving::GetMaxSpeed( const MIL_Object_ABC& object ) const
{
    moving::SpeedComputerStrategy strategy( true, false, object );
    return ComputeSpeed( owner_, strategy );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
double RoleAction_Moving::GetMaxSpeed( const TerrainData& environment ) const
{
    moving::SpeedComputerStrategy strategy( true, false, &environment );
    return ComputeSpeed( owner_, strategy );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
double RoleAction_Moving::GetMaxSpeed() const
{
    moving::SpeedComputerStrategy strategy( true, false, 0 );
    return ComputeSpeed( owner_, strategy );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::GetMaxSlope
// Created: AGE 2005-04-13
// -----------------------------------------------------------------------------
double RoleAction_Moving::GetMaxSlope() const
{
    std::auto_ptr< moving::MaxSlopeComputer_ABC > computer =
            owner_.GetAlgorithms().moveComputerFactory_->CreateMaxSlopeComputer();
    owner_.Execute< OnComponentComputer_ABC >( *computer );

    return computer->GetMaxSlope();
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::GetMaxSpeedWithReinforcement
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
double RoleAction_Moving::GetMaxSpeedWithReinforcement() const
{
    moving::SpeedComputerStrategy strategy( true, true, 0 );
    return ComputeSpeed( owner_, strategy );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::GetTheoricMaxSpeedWithReinforcement
// Created: LDC 2012-08-27
// -----------------------------------------------------------------------------
double RoleAction_Moving::GetTheoricMaxSpeedWithReinforcement()
{
    SetTheoricSpeed( true );
    double result = GetMaxSpeedWithReinforcement();
    SetTheoricSpeed( false );
    return result;
}
    
// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::SetTheoricSpeed
// Created: LDC 2012-08-27
// -----------------------------------------------------------------------------
void RoleAction_Moving::SetTheoricSpeed( bool value ) const
{
    bTheoricMaxSpeed_ = value;
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::GetTheoricMaxSpeed
// Created: LMT 2010-05-04
// -----------------------------------------------------------------------------
double RoleAction_Moving::GetTheoricMaxSpeed( bool loaded ) const
{
    SetTheoricSpeed( true );
    moving::SpeedComputerStrategy strategy( true, false, 0 );
    double result = ComputeSpeed( owner_, strategy, loaded );
    SetTheoricSpeed( false );
    return result;
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::GetSpeedWithReinforcement
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
double RoleAction_Moving::GetSpeedWithReinforcement( const TerrainData& environment ) const
{
    moving::SpeedComputerStrategy strategy( false, true, &environment );
    double rSpeed = ComputeSpeed( owner_, strategy );
    rSpeed = std::min( rSpeed, GetMaxSpeedWithReinforcement() );
    return rSpeed;
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::GetSpeedWithReinforcement
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
double RoleAction_Moving::GetSpeedWithReinforcement( const TerrainData& environment, const MIL_Object_ABC& object ) const
{
    if( !object().HasMobilityInfluence() )
        return std::numeric_limits< double >::max();

    if( !object().IsTrafficable( owner_ ) )
        return 0.;

    moving::SpeedComputerStrategy strategy( false, true, object );
    double rObjectSpeed = ComputeSpeed( owner_, strategy );

    const double rCurrentMaxSpeed = GetMaxSpeed();
    const double rCurrentEnvSpeed = GetSpeedWithReinforcement( environment );

    rObjectSpeed = std::min( rObjectSpeed, rCurrentMaxSpeed );
    rObjectSpeed *= rSpeedModificator_;
    return object().ApplySpeedPolicy( rObjectSpeed, rCurrentEnvSpeed, rCurrentMaxSpeed, owner_ );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::ExtrapolatePosition
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
MT_Vector2D RoleAction_Moving::ExtrapolatePosition( const double rTime, const bool bBoundOnPath ) const
{
    return PHY_MovingEntity_ABC::ExtrapolatePosition(
        owner_.GetRole< PHY_RoleInterface_Location >().GetPosition(),
        owner_.GetRole< PHY_RoleInterface_Location >().GetCurrentSpeed(),
        rTime, bBoundOnPath );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::SendEnvironmentType
// Created: SBO 2005-06-15
// -----------------------------------------------------------------------------
void RoleAction_Moving::SendEnvironmentType( unsigned int context ) const
{
    client::UnitEnvironmentType asn;
    asn().mutable_unit()->set_id( owner_.GetID() );
    SerializeEnvironmentType( asn() );
    asn.Send( NET_Publisher_ABC::Publisher(), context );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::SendCurrentPath
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void RoleAction_Moving::SendCurrentPath( unsigned int context ) const
{
    client::UnitPathFind asnMsg;
    asnMsg().mutable_unit()->set_id( owner_.GetID() );
    if( !SerializeCurrentPath( *asnMsg().mutable_path() ) )
        return;
    asnMsg.Send( NET_Publisher_ABC::Publisher(), context );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::SendFullState
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void RoleAction_Moving::SendFullState( unsigned int context ) const
{
    SendCurrentPath( context );
    SendEnvironmentType( context );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::SendChangedState
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void RoleAction_Moving::SendChangedState() const
{
    if( bCurrentPathHasChanged_ )
        SendCurrentPath();
    if( bEnvironmentHasChanged_ )
        SendEnvironmentType();
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::GetPosition
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
const MT_Vector2D& RoleAction_Moving::GetPosition() const
{
    return owner_.GetRole< PHY_RoleInterface_Location >().GetPosition();
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::GetDirection
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
const MT_Vector2D& RoleAction_Moving::GetDirection() const
{
    return owner_.GetRole< PHY_RoleInterface_Location >().GetDirection();
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
    bHasMove_ = ( rSpeed != 0. );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::NotifyMovingOnPathPoint
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
void RoleAction_Moving::NotifyMovingOnPathPoint( const MT_Vector2D& /*point*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::NotifyMovingOnSpecialPoint
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void RoleAction_Moving::NotifyMovingOnSpecialPoint( boost::shared_ptr< DEC_PathPoint > point )
{
    //@TODO MGD Refactor Path_Point and DEC_Representation, DEC_Representations must be a manager to avoid auto dia register
    point->SendToDIA( owner_.GetRole< DEC_Representations >(), point );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::NotifyMovingInsideObject
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void RoleAction_Moving::NotifyMovingInsideObject( MIL_Object_ABC& object )
{
    owner_.Apply(&terrain::ObjectCollisionNotificationHandler_ABC::NotifyMovingInsideObject, object );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::NotifyMovingOutsideObject
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void RoleAction_Moving::NotifyMovingOutsideObject( MIL_Object_ABC& object )
{
    owner_.Apply(&terrain::ObjectCollisionNotificationHandler_ABC::NotifyMovingOutsideObject, object );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::CanMove
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
bool RoleAction_Moving::CanMove() const
{
    std::auto_ptr< moving::MoveComputer_ABC > moveComputer = owner_.GetAlgorithms().moveComputerFactory_->CreateMoveComputer();
    owner_.Execute( *moveComputer );

    return moveComputer->CanMoveOverride() || moveComputer->CanMove();
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::CanObjectInteractWith
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
bool RoleAction_Moving::CanObjectInteractWith( const MIL_Object_ABC& object ) const
{
    return object.CanInteractWith( owner_ );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::HasResources
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
bool RoleAction_Moving::HasResources()
{
    std::auto_ptr< moving::MoveComputer_ABC > moveComputer = owner_.GetAlgorithms().moveComputerFactory_->CreateMoveComputer();
    owner_.Execute( *moveComputer );

    if( moveComputer->CanMoveOverride() )
        return true;

    std::auto_ptr< dotation::ConsumptionModeChangeRequest_ABC > request =
            owner_.GetAlgorithms().consumptionComputerFactory_->CreateConsumptionModeChangeRequest(PHY_ConsumptionType::moving_);
    owner_.Apply( &dotation::ConsumptionChangeRequestHandler_ABC::ChangeConsumptionMode, *request ); // automatic rollback

    return request->AllChanged();
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::SendRC
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void RoleAction_Moving::SendRC( int nReportID ) const
{
    MIL_Report::PostEvent( owner_, static_cast< MIL_Report::E_EngineReport >( nReportID ) );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::SendRC
// Created: CMA 2012-02-02
// -----------------------------------------------------------------------------
void RoleAction_Moving::SendRC( int nReportID, const std::string& name ) const
{
    MIL_Report::PostEvent( owner_, static_cast< MIL_Report::E_EngineReport >( nReportID ), name );
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::Update
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void RoleAction_Moving::Update( bool /*bIsDead*/ )
{
    // NOTHING
    if (!bHasMove_)
        rSpeed_ = 0.;
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::Clean
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void RoleAction_Moving::Clean()
{
    PHY_MovingEntity_ABC::Clean();

    bCurrentPathHasChanged_ = false;
    bEnvironmentHasChanged_ = false;
    bHasMove_               = false;
    bTheoricMaxSpeed_       = false;
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::HasChanged
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
bool RoleAction_Moving::HasChanged() const
{
    return bCurrentPathHasChanged_ || bEnvironmentHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::NotifyEnvironmentChanged
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void RoleAction_Moving::NotifyEnvironmentChanged()
{
    bEnvironmentHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: RoleAction_Moving::NotifyCurrentPathChanged
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void RoleAction_Moving::NotifyCurrentPathChanged()
{
    bCurrentPathHasChanged_ = true;
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
        throw std::runtime_error( "Cannot set max speed to 0 or less: Would prevent pathfind from working. Please inhibit move actions in decisional instead." );
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
    if( rSpeed_ == 0. )
        algorithm.UnsetPostureMovement();
    else
        algorithm.SetPostureMovement();
}
