// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.cpp $
// $Author: Age $
// $Modtime: 15/06/05 18:46 $
// $Revision: 28 $
// $Workfile: PHY_RoleAction_Moving.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleAction_Moving.h"
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
#include "SpeedComputerStrategy.h"
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

BOOST_CLASS_EXPORT_IMPLEMENT( moving::PHY_RoleAction_Moving )

namespace moving
{

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving constructor
// Created: LDC 2013-01-09
// -----------------------------------------------------------------------------
PHY_RoleAction_Moving::PHY_RoleAction_Moving()
    : owner_                 ( 0 )
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
// Name: PHY_RoleAction_Moving constructor
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
PHY_RoleAction_Moving::PHY_RoleAction_Moving( MIL_AgentPion& pion )
    : owner_                 ( &pion )
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
// Name: PHY_RoleAction_Moving destructor
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
PHY_RoleAction_Moving::~PHY_RoleAction_Moving()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RoleAction_Moving::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< moving::PHY_RoleAction_InterfaceMoving >( *this );
    file & owner_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ApplyMaxSpeedModificators
// Created: NLD 2004-11-04
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::ApplyMaxSpeedModificators( double rSpeed ) const
{
    rSpeed *= rMaxSpeedModificator_;
    return rSpeed;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ApplySpeedModificators
// Created: NLD 2004-11-04
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::ApplySpeedModificators( double rSpeed ) const
{
    rSpeed *= rSpeedModificator_;
    return rSpeed;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::Execute
// Created: DDA 2010-06-14
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::Execute( moving::SpeedComputer_ABC& algorithm ) const
{
    if( !bTheoricMaxSpeed_ )
        algorithm.AddModifier( rMaxSpeedModificator_, true );
    algorithm.AddModifier( rSpeedModificator_, false );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SetTheoricSpeed
// Created: LDC 2012-08-22
// Set to true when speedcomputers must NOT take into account decisional or
// traffic limitations, typically when computing a path. Must be set back to true
// when computing speed for actual movement.
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SetTheoricSpeed( bool value ) const
{
    bTheoricMaxSpeed_ = value;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetMaxSpeed( const MIL_Object_ABC& object ) const
{
    SpeedComputerStrategy strategy( true, false, object );
    std::auto_ptr< SpeedComputer_ABC > computer = owner_->GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy );
    owner_->Execute( *computer );
    return computer->GetSpeed();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetMaxSpeed( const TerrainData& environment ) const
{
    SpeedComputerStrategy strategy( true, false, &environment );
    std::auto_ptr< SpeedComputer_ABC > computer = owner_->GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy );
    owner_->Execute( *computer );
    return computer->GetSpeed();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetMaxSpeed() const
{
    SpeedComputerStrategy strategy( true, false, 0 );
    std::auto_ptr< SpeedComputer_ABC > computer = owner_->GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy );
    owner_->Execute( *computer );
    return computer->GetSpeed();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSlope
// Created: AGE 2005-04-13
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetMaxSlope() const
{
    std::auto_ptr< moving::MaxSlopeComputer_ABC > computer =
            owner_->GetAlgorithms().moveComputerFactory_->CreateMaxSlopeComputer();
    owner_->Execute< OnComponentComputer_ABC >( *computer );

    return computer->GetMaxSlope();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeedWithReinforcement
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetMaxSpeedWithReinforcement() const
{
    SpeedComputerStrategy strategy( true, true, 0 );
    std::auto_ptr< SpeedComputer_ABC > computer;
        computer = owner_->GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy );
    owner_->Execute( *computer );
    return computer->GetSpeed();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetTheoricMaxSpeed
// Created: LMT 2010-05-04
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetTheoricMaxSpeed( bool loaded ) const
{
    SetTheoricSpeed( true );
    SpeedComputerStrategy strategy( true, false, 0 );
    std::auto_ptr< SpeedComputer_ABC > computer;
    computer = owner_->GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy, loaded );
    owner_->Execute( *computer );
    double result = computer->GetSpeed();
    SetTheoricSpeed( false );
    return result;
}


// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetSpeedWithReinforcement
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetSpeedWithReinforcement( const TerrainData& environment ) const
{
    SpeedComputerStrategy strategy( false, true, &environment );
    std::auto_ptr< SpeedComputer_ABC > computer = owner_->GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy );
    owner_->Execute( *computer );
    double rSpeed = computer->GetSpeed();
    rSpeed = std::min( rSpeed, GetMaxSpeedWithReinforcement() );
    return rSpeed;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetTheoricMaxSpeedWithReinforcement
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetTheoricMaxSpeedWithReinforcement() const
{
    SetTheoricSpeed( true );
    double result = GetMaxSpeedWithReinforcement();
    SetTheoricSpeed( false );
    return result;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetSpeedWithReinforcement
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetSpeedWithReinforcement( const TerrainData& environment, const MIL_Object_ABC& object ) const
{
    if( !object().HasMobilityInfluence() )
        return std::numeric_limits< double >::max();

    if( !object().IsTrafficable( *owner_ ) )
        return 0.;

    SpeedComputerStrategy strategy( false, true, object );
    std::auto_ptr< SpeedComputer_ABC > computer = owner_->GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy );
    owner_->Execute( *computer );
    double rObjectSpeed = computer->GetSpeed();

    const double rCurrentMaxSpeed = GetMaxSpeed();
    const double rCurrentEnvSpeed = GetSpeedWithReinforcement( environment );

    rObjectSpeed = std::min( rObjectSpeed, rCurrentMaxSpeed );
    rObjectSpeed = ApplySpeedModificators( rObjectSpeed );
    return object().ApplySpeedPolicy( rObjectSpeed, rCurrentEnvSpeed, rCurrentMaxSpeed, *owner_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ExtrapolatePosition
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
MT_Vector2D PHY_RoleAction_Moving::ExtrapolatePosition( const double rTime, const bool bBoundOnPath ) const
{
    return PHY_MovingEntity_ABC::ExtrapolatePosition(
        owner_->GetRole< PHY_RoleInterface_Location >().GetPosition(),
        owner_->GetRole< PHY_RoleInterface_Location >().GetCurrentSpeed(),
        rTime, bBoundOnPath );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SendEnvironmentType
// Created: SBO 2005-06-15
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SendEnvironmentType( unsigned int context ) const
{
    client::UnitEnvironmentType asn;
    asn().mutable_unit()->set_id( owner_->GetID() );
    SerializeEnvironmentType( asn() );
    asn.Send( NET_Publisher_ABC::Publisher(), context );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SendCurrentPath
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SendCurrentPath( unsigned int context ) const
{
    client::UnitPathFind asnMsg;
    asnMsg().mutable_unit()->set_id( owner_->GetID() );
    if( !SerializeCurrentPath( *asnMsg().mutable_path() ) )
        return;
    asnMsg.Send( NET_Publisher_ABC::Publisher(), context );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SendFullState
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SendFullState( unsigned int context ) const
{
    SendCurrentPath( context );
    SendEnvironmentType( context );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SendChangedState
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SendChangedState() const
{
    if( bCurrentPathHasChanged_ )
        SendCurrentPath();
    if( bEnvironmentHasChanged_ )
        SendEnvironmentType();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetPosition
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_RoleAction_Moving::GetPosition() const
{
    return owner_->GetRole< PHY_RoleInterface_Location >().GetPosition();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetDirection
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_RoleAction_Moving::GetDirection() const
{
    return owner_->GetRole< PHY_RoleInterface_Location >().GetDirection();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ApplyMove
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::ApplyMove( const MT_Vector2D& position, const MT_Vector2D& direction, double rSpeed, double /*rWalkedDistance*/ )
{
    if( ! CanMove() )
        return;
    rSpeed_ = rSpeed;
    bHasMove_ = ( rSpeed != 0. );
    return owner_->GetRole< PHY_RoleInterface_Location >().Move( position, direction, rSpeed );
}


// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::NotifyMovingOnPathPoint
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::NotifyMovingOnPathPoint( const MT_Vector2D& /*point*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::NotifyMovingOnSpecialPoint
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::NotifyMovingOnSpecialPoint( boost::shared_ptr< DEC_PathPoint > point )
{
    //@TODO MGD Refactor Path_Point and DEC_Representation, DEC_Representations must be a manager to avoid auto dia register
    point->SendToDIA( owner_->GetRole< DEC_Representations >(), point );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::NotifyMovingInsideObject
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::NotifyMovingInsideObject( MIL_Object_ABC& object )
{
    owner_->Apply(&terrain::ObjectCollisionNotificationHandler_ABC::NotifyMovingInsideObject, object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::NotifyMovingOutsideObject
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::NotifyMovingOutsideObject( MIL_Object_ABC& object )
{
    owner_->Apply(&terrain::ObjectCollisionNotificationHandler_ABC::NotifyMovingOutsideObject, object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::CanMove
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Moving::CanMove() const
{
    std::auto_ptr< moving::MoveComputer_ABC > moveComputer = owner_->GetAlgorithms().moveComputerFactory_->CreateMoveComputer();
    owner_->Execute( *moveComputer );

    return moveComputer->CanMoveOverride() || moveComputer->CanMove();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::CanObjectInteractWith
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Moving::CanObjectInteractWith( const MIL_Object_ABC& object ) const
{
    return object.CanInteractWith( *owner_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::HasResources
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Moving::HasResources()
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
// Name: PHY_RoleAction_Moving::SendRC
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SendRC( const MIL_DecisionalReport& reportId ) const
{
    MIL_Report::PostEvent( *owner_, reportId );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SendRC
// Created: CMA 2012-02-02
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SendRC( const MIL_DecisionalReport& reportId, const std::string& name ) const
{
    MIL_Report::PostEvent( *owner_, reportId, name );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::Update
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::Update( bool /*bIsDead*/ )
{
    // NOTHING
    if (!bHasMove_)
        rSpeed_ = 0.;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::Clean
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::Clean()
{
    PHY_MovingEntity_ABC::Clean();

    bCurrentPathHasChanged_ = false;
    bEnvironmentHasChanged_ = false;
    bHasMove_               = false;
    bTheoricMaxSpeed_       = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::HasChanged
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Moving::HasChanged() const
{
    return bCurrentPathHasChanged_ || bEnvironmentHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::NotifyEnvironmentChanged
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::NotifyEnvironmentChanged()
{
    bEnvironmentHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::NotifyCurrentPathChanged
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::NotifyCurrentPathChanged()
{
    bCurrentPathHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SetSpeedModificator
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SetSpeedModificator( double rFactor )
{
    rSpeedModificator_ = rFactor;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SetMaxSpeedModificator
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SetMaxSpeedModificator( double rFactor )
{
    if( rFactor <= 0. )
        throw MASA_EXCEPTION( "Cannot set max speed to 0 or less: Would prevent pathfind from working. Please inhibit move actions in decisional instead." );
    rMaxSpeedModificator_ = rFactor;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeedModificator
// Created: LMT 2011-07-06
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetMaxSpeedModificator() const
{
    return rMaxSpeedModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::Execute
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::Execute( posture::PostureComputer_ABC& algorithm ) const
{
    if( rSpeed_ == 0. )
        algorithm.UnsetPostureMovement();
    else
        algorithm.SetPostureMovement();
}

} // namespace moving
