// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Location/PHY_RolePion_Location.cpp $
// $Author: Jvt $
// $Modtime: 12/04/05 10:52 $
// $Revision: 22 $
// $Workfile: PHY_RolePion_Location.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RolePion_Location.h"
#include "CheckPoints/SerializationTools.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KS_ObjectInteraction.h"
#include "Knowledge/DEC_KS_PopulationInteraction.h"
#include "Network/NET_ASN_Tools.h"
#include "AlgorithmsFactories.h"
#include "DefaultLocationComputer.h"
#include "MoveComputer_ABC.h"
#include "MoveComputerFactory_ABC.h"
#include "MovementHandler_ABC.h"
#include "NetworkNotificationHandler_ABC.h"
#include "VisionConeNotificationHandler_ABC.h"
#include "UrbanLocationComputer_ABC.h"
#include "simulation_terrain/TER_World.h"
#include "simulation_terrain/TER_ObjectManager.h"
#include "protocol/ClientSenders.h"
#include "Tools/MIL_Tools.h"

using namespace location;

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_RolePion_Location )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location constructor
// Created: LDC 2013-01-09
// -----------------------------------------------------------------------------
PHY_RolePion_Location::PHY_RolePion_Location()
    : owner_                  ( 0 )
    , vDirection_             ( 0., 0. )
    , pvPosition_             ( new MT_Vector2D ( -1., -1. ) )   //$$$ Devrait être 'NULL'
    , rHeight_                ( -1. )
    , rCurrentSpeed_          ( -1. )
    , bHasDoneMagicMove_      ( false )
    , bHasMove_               ( false )
    , bPositionHasChanged_    ( true )
    , bDirectionHasChanged_   ( true )
    , bCurrentSpeedHasChanged_( true )
    , bHeightHasChanged_      ( true )
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Location::PHY_RolePion_Location( MIL_AgentPion& pion )
    : owner_                  ( &pion )
    , vDirection_             ( 0., 0. )
    , pvPosition_             ( new MT_Vector2D ( -1., -1. ) )   //$$$ Devrait être 'NULL'
    , rHeight_                ( -1. )
    , rCurrentSpeed_          ( -1. )
    , bHasDoneMagicMove_      ( false )
    , bHasMove_               ( false )
    , bPositionHasChanged_    ( true )
    , bDirectionHasChanged_   ( true )
    , bCurrentSpeedHasChanged_( true )
    , bHeightHasChanged_      ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Location::~PHY_RolePion_Location()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::load
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    MT_Vector2D vPosition;
    file >> boost::serialization::base_object< PHY_RoleInterface_Location >( *this );
    file >> owner_;
    file >> vDirection_;
    file >> pvPosition_;
    file >> bHasDoneMagicMove_;
    file >> bHasMove_;
    UpdatePatch();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::save
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< PHY_RoleInterface_Location >( *this );
    file << owner_;
    file << vDirection_;
    file << pvPosition_;
    file << bHasDoneMagicMove_;
    file << bHasMove_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::GetAgent
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MIL_Agent_ABC& PHY_RolePion_Location::GetAgent() const
{
    return *owner_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::GetAltitude
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
double PHY_RolePion_Location::GetAltitude() const
{
    return MIL_Tools::GetAltitude( *pvPosition_ ) + rHeight_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::SetHeight
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::SetHeight( double rHeight )
{
    if( rHeight == rHeight_ )
        return;

    bHeightHasChanged_ = true;
    rHeight_           = rHeight;
}

//-----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::SetPosition
// Created: DFT 02-02-27
// Last modified: JVT 04-06-01
//-----------------------------------------------------------------------------
void PHY_RolePion_Location::SetPosition( const MT_Vector2D& vPosition )
{
    if( vPosition == *pvPosition_ )
        return;

    bPositionHasChanged_ = true;
    *pvPosition_           = vPosition;
    TER_World::GetWorld().ClipPointInsideWorld( *pvPosition_ );

    UpdatePatch();
}

//-----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::SetDirection
// Created: JVT 02-07-18
//-----------------------------------------------------------------------------
void PHY_RolePion_Location::SetDirection( const MT_Vector2D& vDirection )
{
    if( vDirection == vDirection_ )
        return;

    bDirectionHasChanged_ = true;
    vDirection_           = vDirection;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::SetCurrentSpeed
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::SetCurrentSpeed( double rSpeed )
{
    if( rCurrentSpeed_ == rSpeed )
        return;
    bCurrentSpeedHasChanged_ = true;
    rCurrentSpeed_           = rSpeed;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::Move
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::Move( const MT_Vector2D& vNewPosition, const MT_Vector2D& vNewDirection, double rNewSpeed )
{
    SetCurrentSpeed( rNewSpeed     );
    SetPosition    ( vNewPosition  );
    SetDirection   ( vNewDirection );

    //@TODO MGD see if bHasMove_ always usefull
    if( rCurrentSpeed_ != 0. )
        bHasMove_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::Follow
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::Follow( const MIL_Agent_ABC& agent )
{
    const PHY_RoleInterface_Location& roleLocation = agent.GetRole< PHY_RoleInterface_Location >();
    Move     ( roleLocation.GetPosition(), roleLocation.GetDirection(), roleLocation.GetCurrentSpeed() );
    SetHeight( roleLocation.GetHeight() );

    if( bPositionHasChanged_ || bDirectionHasChanged_|| bHeightHasChanged_ || bCurrentSpeedHasChanged_ )
        owner_->Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::Hide
// Created: NLD 2004-11-24
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::Hide()
{
    TER_Object_ABC::T_ObjectVector objectsColliding;
    TER_World::GetWorld().GetObjectManager().GetListAt( *pvPosition_, objectsColliding );
    for( TER_Object_ABC::CIT_ObjectVector itObject = objectsColliding.begin(); itObject != objectsColliding.end(); ++itObject )
        NotifyPutOutsideObject( static_cast< MIL_Object_ABC& >( **itObject ) );

    RemoveFromPatch();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::Show
// Created: NLD 2004-11-24
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::Show( const MT_Vector2D& vPosition )
{
    if( owner_->IsMarkedForDestruction() )
        return;
    Move( vPosition, vDirection_, 0. );

    TER_Object_ABC::T_ObjectVector objectsColliding;
    TER_World::GetWorld().GetObjectManager().GetListAt( *pvPosition_, objectsColliding );
    for( TER_Object_ABC::CIT_ObjectVector itObject = objectsColliding.begin(); itObject != objectsColliding.end(); ++itObject )
        NotifyPutInsideObject( static_cast< MIL_Object_ABC& >( **itObject ) );
    bHasDoneMagicMove_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::MagicMove
// Created: NLD 2004-11-24
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::MagicMove( const MT_Vector2D& vPosition )
{
    std::auto_ptr< moving::MoveComputer_ABC > moveComputer = owner_->GetAlgorithms().moveComputerFactory_->CreateMagicMoveComputer();
    owner_->Execute( *moveComputer );

    if( !moveComputer->CanMove() && !moveComputer->CanMoveOverride() )
        return;

    Hide();
    Show( vPosition );
}

// =============================================================================
// NOTIFICATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::NotifyPopulationCollision
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::NotifyFlowCollision( MIL_PopulationFlow& population )
{
    owner_->GetKnowledge().GetKsPopulationInteraction().NotifyPopulationCollision( population );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::NotifyPopulationCollision
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::NotifyConcentrationCollision( MIL_PopulationConcentration& population )
{
    owner_->GetKnowledge().GetKsPopulationInteraction().NotifyPopulationCollision( population );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::NotifyObjectCollision
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::NotifyTerrainObjectCollision( MIL_Object_ABC& object )
{
    owner_->GetKnowledge().GetKsObjectInteraction().NotifyObjectCollision( object, *pvPosition_, vDirection_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::NotifyMovingInsideObject
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::NotifyMovingInsideObject( MIL_Object_ABC& object )
{
    object.NotifyAgentMovingInside( *owner_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::NotifyMovingOutsideObject
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::NotifyMovingOutsideObject( MIL_Object_ABC& object )
{
    object.NotifyAgentMovingOutside( *owner_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::NotifyPutInsideObject
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::NotifyPutInsideObject( MIL_Object_ABC& object )
{
    object.NotifyAgentPutInside( *owner_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::NotifyPutOutsideObject
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::NotifyPutOutsideObject( MIL_Object_ABC& object )
{
    object.NotifyAgentPutOutside( *owner_ );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::SendChangedState( client::UnitAttributes& msg ) const
{
    if( bPositionHasChanged_ )
        NET_ASN_Tools::WritePoint( *pvPosition_, *msg().mutable_position() );
    if( bDirectionHasChanged_ )
        NET_ASN_Tools::WriteDirection( vDirection_, *msg().mutable_direction() );
    if( bHeightHasChanged_ )
        msg().set_height( (unsigned int)rHeight_ );
    if( bHeightHasChanged_ || bPositionHasChanged_ )
        msg().set_altitude( (unsigned int)( rHeight_ + MIL_Tools::GetAltitude( *pvPosition_ ) ) );
    if( bCurrentSpeedHasChanged_ )
        msg().set_speed( MIL_Tools::ConvertSpeedSimToMos( rCurrentSpeed_ ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::SendFullState( client::UnitAttributes& msg ) const
{
    NET_ASN_Tools::WritePoint( *pvPosition_, *msg().mutable_position() );
    NET_ASN_Tools::WriteDirection( vDirection_, *msg().mutable_direction() );
    msg().set_height( (unsigned int)rHeight_ );
    msg().set_altitude( (unsigned int)( rHeight_ + MIL_Tools::GetAltitude( *pvPosition_ ) ) );
    msg().set_speed( MIL_Tools::ConvertSpeedSimToMos( rCurrentSpeed_ ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::GetHeight
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
double PHY_RolePion_Location::GetHeight() const
{
    return rHeight_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::GetPosition
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_RolePion_Location::GetPosition() const
{
    return *pvPosition_;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr<MT_Vector2D> PHY_RolePion_Location::GetSharedPosition
// Created: LDC 2009-11-17
// -----------------------------------------------------------------------------
boost::shared_ptr<MT_Vector2D> PHY_RolePion_Location::GetSharedPosition() const
{
    return pvPosition_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::GetDirection
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_RolePion_Location::GetDirection() const
{
    return vDirection_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::GetCurrentSpeed
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
double PHY_RolePion_Location::GetCurrentSpeed() const
{
    return rCurrentSpeed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::HasDoneMagicMove
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool PHY_RolePion_Location::HasDoneMagicMove() const
{
    return bHasDoneMagicMove_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::Update( bool bIsDead )
{
    if( bIsDead || !bHasMove_ )
        Move( *pvPosition_, vDirection_, 0. );

    location::DefaultLocationComputer computer;
    SetHeight( owner_->Execute< LocationComputer_ABC >( computer ).GetHeight() );

    bool locationChanged = HasLocationChanged();
    if( locationChanged || HasSpeedChanged() )
    {
        owner_->Apply( &network::VisionConeNotificationHandler_ABC::NotifyVisionConeDataHasChanged );
        owner_->Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
    }
    if( locationChanged )
        owner_->Apply( &location::MovementHandler_ABC::NotifyHasMoved, GetPosition() );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::Clean
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::Clean()
{
    bHasMove_                = false;
    bHasDoneMagicMove_       = false;
    bPositionHasChanged_     = false;
    bDirectionHasChanged_    = false;
    bCurrentSpeedHasChanged_ = false;
    bHeightHasChanged_       = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::HasSpeedChanged
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
bool PHY_RolePion_Location::HasSpeedChanged() const
{
    return bCurrentSpeedHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::HasLocationChanged
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
bool PHY_RolePion_Location::HasLocationChanged() const
{
    return bPositionHasChanged_ || bDirectionHasChanged_ || bHeightHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::NotifyTerrainPutInsideObject
// Created: SLG 2010-03-31
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::NotifyTerrainPutInsideObject( MIL_Object_ABC& object )
{
    owner_->Apply( &terrain::ObjectCollisionNotificationHandler_ABC::NotifyPutInsideObject, object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::NotifyTerrainPutOutsideObject
// Created: SLG 2010-03-31
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::NotifyTerrainPutOutsideObject( MIL_Object_ABC& object )
{
    owner_->Apply( &terrain::ObjectCollisionNotificationHandler_ABC::NotifyPutOutsideObject, object );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Location::Execute
// Created: SLG 2010-04-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Location::Execute( urbanLocation::UrbanLocationComputer_ABC& algorithm ) const
{
    algorithm.SetPosition( *pvPosition_ );
}
