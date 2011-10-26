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

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RoleAction_Moving* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* const pion = &role->pion_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RoleAction_Moving* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* pion;
    archive >> pion;
    ::new( role )PHY_RoleAction_Moving( *pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving constructor
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
PHY_RoleAction_Moving::PHY_RoleAction_Moving( MIL_AgentPion& pion )
    : pion_                  ( pion )
    , pRoleLocation_         ( &pion_.GetRole< PHY_RoleInterface_Location >() )
    , rSpeed_                ( 0.)
    , rSpeedModificator_     ( 1. )
    , rMaxSpeedModificator_  ( 1. )
    , bCurrentPathHasChanged_( true )
    , bEnvironmentHasChanged_( true )
    , bHasMove_              ( false )
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
    file & boost::serialization::base_object< tools::Role_ABC >( *this );
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
    algorithm.AddModifier( rMaxSpeedModificator_, true );
    algorithm.AddModifier( rSpeedModificator_, false );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetMaxSpeed( const MIL_Object_ABC& object ) const
{
    SpeedComputerStrategy strategy( true, false, object );
    std::auto_ptr< SpeedComputer_ABC > computer = pion_.GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy );
    pion_.Execute( *computer );
    return computer->GetSpeed();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetMaxSpeed( const TerrainData& environment ) const
{
    SpeedComputerStrategy strategy( true, false, &environment );
    std::auto_ptr< SpeedComputer_ABC > computer = pion_.GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy );
    pion_.Execute( *computer );
    return computer->GetSpeed();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeed
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetMaxSpeed() const
{
    SpeedComputerStrategy strategy( true, false, 0 );
    std::auto_ptr< SpeedComputer_ABC > computer = pion_.GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy );
    pion_.Execute( *computer );
    return computer->GetSpeed();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSlope
// Created: AGE 2005-04-13
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetMaxSlope() const
{
    std::auto_ptr< moving::MaxSlopeComputer_ABC > computer =
            pion_.GetAlgorithms().moveComputerFactory_->CreateMaxSlopeComputer();
    pion_.Execute< OnComponentComputer_ABC >( *computer );

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
        computer = pion_.GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy );
    pion_.Execute( *computer );
    return computer->GetSpeed();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetTheoricMaxSpeed
// Created: LMT 2010-05-04
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetTheoricMaxSpeed( bool loaded ) const
{
    SpeedComputerStrategy strategy( true, false, 0 );
    std::auto_ptr< SpeedComputer_ABC > computer;
    computer = pion_.GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy, loaded );
    pion_.Execute( *computer );
    return computer->GetSpeed();
}


// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetSpeedWithReinforcement
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetSpeedWithReinforcement( const TerrainData& environment ) const
{
    SpeedComputerStrategy strategy( false, true, &environment );
    std::auto_ptr< SpeedComputer_ABC > computer = pion_.GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy );
    pion_.Execute( *computer );
    double rSpeed = computer->GetSpeed();
    rSpeed = std::min( rSpeed, GetMaxSpeedWithReinforcement() );
    return rSpeed;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetSpeedWithReinforcement
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetSpeedWithReinforcement( const TerrainData& environment, const MIL_Object_ABC& object ) const
{
    if( !object().HasMobilityInfluence() )
        return std::numeric_limits< double >::max();

    if( !object().IsTrafficable( pion_ ) )
        return 0.;

    SpeedComputerStrategy strategy( false, true, object );
    std::auto_ptr< SpeedComputer_ABC > computer = pion_.GetAlgorithms().moveComputerFactory_->CreateSpeedComputer( strategy );
    pion_.Execute( *computer );
    double rObjectSpeed = computer->GetSpeed();

    const double rCurrentMaxSpeed = GetMaxSpeed();
    const double rCurrentEnvSpeed = GetSpeedWithReinforcement( environment );

    rObjectSpeed = std::min( rObjectSpeed, rCurrentMaxSpeed );
    rObjectSpeed = ApplySpeedModificators( rObjectSpeed );
    return object().ApplySpeedPolicy( rObjectSpeed, rCurrentEnvSpeed, rCurrentMaxSpeed, pion_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ExtrapolatePosition
// Created: NLD 2005-10-03
// -----------------------------------------------------------------------------
MT_Vector2D PHY_RoleAction_Moving::ExtrapolatePosition( const double rTime, const bool bBoundOnPath ) const
{
    return PHY_MovingEntity_ABC::ExtrapolatePosition( pRoleLocation_->GetPosition(), pRoleLocation_->GetCurrentSpeed(), rTime, bBoundOnPath );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SendEnvironmentType
// Created: SBO 2005-06-15
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SendEnvironmentType( unsigned int context ) const
{
    client::UnitEnvironmentType asn;
    asn().mutable_unit()->set_id( pion_.GetID() );
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
    asnMsg().mutable_unit()->set_id( pion_.GetID() );
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
    assert( pRoleLocation_ );
    return pRoleLocation_->GetPosition();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetDirection
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_RoleAction_Moving::GetDirection() const
{
    assert( pRoleLocation_ );
    return pRoleLocation_->GetDirection();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ApplyMove
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::ApplyMove( const MT_Vector2D& position, const MT_Vector2D& direction, double rSpeed, double /*rWalkedDistance*/ )
{
    rSpeed_ = rSpeed;
    bHasMove_ = ( rSpeed != 0. );
    assert( pRoleLocation_ );
    return pRoleLocation_->Move( position, direction, rSpeed );
}


// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::NotifyMovingOnPathPoint
// Created: NLD 2005-10-04
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::NotifyMovingOnPathPoint( const DEC_PathPoint& /*point*/ )
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
    point->SendToDIA( pion_.GetRole< DEC_Representations >(), point );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::NotifyMovingInsideObject
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::NotifyMovingInsideObject( MIL_Object_ABC& object )
{
    pion_.Apply(&terrain::ObjectCollisionNotificationHandler_ABC::NotifyMovingInsideObject, object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::NotifyMovingOutsideObject
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::NotifyMovingOutsideObject( MIL_Object_ABC& object )
{
    pion_.Apply(&terrain::ObjectCollisionNotificationHandler_ABC::NotifyMovingOutsideObject, object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::CanMove
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Moving::CanMove() const
{
    std::auto_ptr< moving::MoveComputer_ABC > moveComputer = pion_.GetAlgorithms().moveComputerFactory_->CreateMoveComputer();
    pion_.Execute( *moveComputer );

    return moveComputer->CanMoveOverride() || moveComputer->CanMove();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::CanObjectInteractWith
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Moving::CanObjectInteractWith( const MIL_Object_ABC& object ) const
{
    return object.CanInteractWith( pion_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::HasResources
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Moving::HasResources()
{
    std::auto_ptr< moving::MoveComputer_ABC > moveComputer = pion_.GetAlgorithms().moveComputerFactory_->CreateMoveComputer();
    pion_.Execute( *moveComputer );

    if( moveComputer->CanMoveOverride() )
        return true;

    std::auto_ptr< dotation::ConsumptionModeChangeRequest_ABC > request =
            pion_.GetAlgorithms().consumptionComputerFactory_->CreateConsumptionModeChangeRequest(PHY_ConsumptionType::moving_);
    pion_.Apply( &dotation::ConsumptionChangeRequestHandler_ABC::ChangeConsumptionMode, *request ); // automatic rollback

    return request->AllChanged();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::SendRC
// Created: NLD 2005-10-05
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::SendRC( int nReportID ) const
{
    MIL_Report::PostEvent( pion_, (MIL_Report::E_EngineReport)nReportID );
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
        throw std::runtime_error( "Cannot set max speed to 0 or less: Would prevent pathfind from working. Please inhibit move actions in decisional instead." );
    rMaxSpeedModificator_ = rFactor;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeedModificator
// Created: LMT 2011-07-06
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetMaxSpeedModificator()
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
