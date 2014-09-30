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
#include "DefaultSpeedComputer.h"
#include "DefaultMaxSlopeComputer.h"
#include "DefaultMoveComputer.h"
#include "DefaultConsumptionModeChangeRequest.h"
#include "ConsumptionChangeRequestHandler_ABC.h"
#include "ObjectCollisionNotificationHandler_ABC.h"
#include "SlopeDecelerationComputer.h"
#include "SpeedComputerStrategy.h"
#include "Entities/Agents/Actions/Underground/PHY_RoleAction_MovingUnderground.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "Decision/DEC_Representations.h"
#include "Decision/DEC_PathPoint.h"
#include "Decision/DEC_Agent_PathClass.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( moving::PHY_RoleAction_Moving )

using namespace moving;

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving constructor
// Created: LDC 2013-01-09
// -----------------------------------------------------------------------------
PHY_RoleAction_Moving::PHY_RoleAction_Moving()
    : owner_                 ( 0 )
    , rSpeed_                ( 0.)
    , rSpeedModificator_     ( 1. )
    , rMaxSpeedModificator_  ( 1. )
    , rTrafficModificator_   ( 1. )
    , bCurrentPathHasChanged_( true )
    , bEnvironmentHasChanged_( true )
    , bHasMove_              ( false )
    , bIntentToMove_         ( false )
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
    , rTrafficModificator_   ( 1. )
    , bCurrentPathHasChanged_( true )
    , bEnvironmentHasChanged_( true )
    , bHasMove_              ( false )
    , bIntentToMove_         ( false )
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
    return rSpeed * rMaxSpeedModificator_ * rTrafficModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ApplySpeedModificators
// Created: NLD 2004-11-04
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::ApplySpeedModificators( double rSpeed ) const
{
    return rSpeed * rSpeedModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::Execute
// Created: DDA 2010-06-14
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::Execute( moving::SpeedComputer_ABC& algorithm ) const
{
    if( !algorithm.IsTheoric() )
        algorithm.AddModifier( rMaxSpeedModificator_ * rTrafficModificator_, true );
    algorithm.AddModifier( rSpeedModificator_, false );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::ComputeSpeed
// Created: MCO 2013-06-13
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::ComputeSpeed( const SpeedStrategy_ABC& strategy ) const
{
    DefaultSpeedComputer computer( strategy );
    owner_->Execute< moving::SpeedComputer_ABC >( computer );
    return computer.GetSpeed();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeed
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetMaxSpeed() const
{
    return ComputeSpeed( SpeedComputerStrategy( true ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSpeed
// Created: MCO 2013-06-13
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetMaxSpeed( const TerrainData& environment ) const
{
    return ComputeSpeed( SpeedComputerStrategy( true, environment ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetSpeed
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetSpeed( const TerrainData& environment ) const
{
    const SpeedComputerStrategy strategy( false, environment );
    return std::min( ComputeSpeed( strategy ), GetMaxSpeed() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetTheoricSpeed
// Created: MCO 2013-06-13
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetTheoricSpeed( const TerrainData& environment ) const
{
    const SpeedComputerStrategy strategy( false, environment, true );
    return std::min( ComputeSpeed( strategy ), GetMaxSpeed() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetTheoricMaxSpeed
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetTheoricMaxSpeed() const
{
    return ComputeSpeed( SpeedComputerStrategy( true, true ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetSpeed
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetSpeed( const TerrainData& environment, const MIL_Object_ABC& object ) const
{
    if( !object().HasMobilityInfluence() )
        return std::numeric_limits< double >::max();
    if( !object().IsTrafficable( *owner_ ) )
        return 0;
    double rObjectSpeed = ComputeSpeed( SpeedComputerStrategy( false, object ) );
    const double rCurrentMaxSpeed = GetMaxSpeed();
    const double rCurrentEnvSpeed = GetSpeed( environment );
    rObjectSpeed = std::min( rObjectSpeed, rCurrentMaxSpeed );
    rObjectSpeed = ApplySpeedModificators( rObjectSpeed );
    return object().ApplySpeedPolicy( rObjectSpeed, rCurrentEnvSpeed, rCurrentMaxSpeed, *owner_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetMaxSlope
// Created: AGE 2005-04-13
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetMaxSlope() const
{
    DefaultMaxSlopeComputer computer;
    owner_->Execute< OnComponentComputer_ABC >( computer );
    return computer.GetMaxSlope();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetSlopeDeceleration
// Created: JSR 2013-11-08
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetSlopeDeceleration() const
{
    moving::SlopeDecelerationComputer computer;
    owner_->Execute< OnComponentComputer_ABC >( computer );
    return computer.GetSlopeDeceleration();
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
    sword::Path& path = *asnMsg().mutable_path();
    path.mutable_location()->set_type( sword::Location::line );
    SerializeCurrentPath( path );
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
// Name: PHY_RoleAction_Moving::Move
// Created: SLI 2013-01-30
// -----------------------------------------------------------------------------
int PHY_RoleAction_Moving::Move( boost::shared_ptr< DEC_PathResult > pPath )
{
    bIntentToMove_ = true;
    return PHY_MovingEntity_ABC::Move( pPath );
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
void PHY_RoleAction_Moving::NotifyMovingInsideObject( MIL_Object_ABC& object, const MT_Vector2D& startPos, const MT_Vector2D& endPos  )
{
    owner_->Apply( &terrain::ObjectCollisionNotificationHandler_ABC::NotifyMovingInsideObject, object, startPos, endPos );
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
    moving::DefaultMoveComputer moveComputer;
    owner_->Execute< moving::MoveComputer_ABC >( moveComputer );
    return moveComputer.CanMoveOverride() || moveComputer.CanMove();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::IsReady
// Created: SLI 2013-04-22
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Moving::IsReady() const
{
    return owner_->GetRole< PHY_RoleInterface_Posture >().IsMovingPosture();
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
// Name: PHY_RoleAction_Moving::GetKnowledgeObject
// Created: LGY 2013-01-21
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Object > PHY_RoleAction_Moving::GetKnowledgeObject( const MIL_Object_ABC& object ) const
{
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = owner_->GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        return container->GetKnowledgeObject( object );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::GetObjectCost
// Created: LGY 2013-01-21
// -----------------------------------------------------------------------------
double PHY_RoleAction_Moving::GetObjectCost( const MIL_ObjectType_ABC& objectType, const DEC_PathType& pathType ) const
{
    return DEC_Agent_PathClass::GetPathClass( pathType, *owner_ ).GetObjectCost( objectType );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::HasResources
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Moving::HasResources()
{
    moving::DefaultMoveComputer moveComputer;
    owner_->Execute< moving::MoveComputer_ABC >( moveComputer );

    if( moveComputer.CanMoveOverride() )
        return true;

    dotation::DefaultConsumptionModeChangeRequest request( PHY_ConsumptionType::moving_ );
    owner_->Apply( &dotation::ConsumptionChangeRequestHandler_ABC::ChangeConsumptionMode, request ); // automatic rollback

    return request.AllChanged();
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

void PHY_RoleAction_Moving::SendRC( const MIL_DecisionalReport& reportId, const boost::shared_ptr< DEC_Knowledge_Object >& object ) const
{
    MIL_Report::PostEvent( *owner_, reportId, object );
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
    bIntentToMove_          = false;
    rTrafficModificator_    = 1.;
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
// Name: PHY_RoleAction_Moving::ApplyTrafficModifier
// Created: JSR 2012-01-12
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::ApplyTrafficModifier()
{
    rTrafficModificator_ *= owner_->GetType().GetUnitType().GetSpeedModifier();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Moving::Execute
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void PHY_RoleAction_Moving::Execute( posture::PostureComputer_ABC& algorithm ) const
{
    if( bIntentToMove_ )
        algorithm.SetPostureMovement();
    else
        algorithm.UnsetPostureMovement();
}

bool PHY_RoleAction_Moving::IsUnderground() const
{
    const PHY_RoleAction_MovingUnderground* roleUnderground = owner_->RetrieveRole< PHY_RoleAction_MovingUnderground >();
    return roleUnderground && roleUnderground->IsUnderground();
}
