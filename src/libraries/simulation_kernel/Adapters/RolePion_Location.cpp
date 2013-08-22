// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "RolePion_Location.h"
#include "ListenerHelper.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KS_ObjectInteraction.h"
#include "Knowledge/DEC_KS_PopulationInteraction.h"
#include "Network/NET_ASN_Tools.h"
#include "AlgorithmsFactories.h"
#include "MoveComputer_ABC.h"
#include "MoveComputerFactory_ABC.h"
#include "MovementHandler_ABC.h"
#include "NetworkNotificationHandler_ABC.h"
#include "Sink.h"
#include "VisionConeNotificationHandler_ABC.h"
#include "UrbanLocationComputer_ABC.h"
#include "simulation_terrain/TER_World.h"
#include "simulation_terrain/TER_ObjectManager.h"
#include "protocol/ClientSenders.h"
#include "Tools/MIL_Tools.h"
#include <core/Model.h>
#include <core/MakeModel.h>
#include <boost/make_shared.hpp>
#include <boost/lambda/lambda.hpp>

using namespace location;
using namespace sword;
namespace bl = boost::lambda;

BOOST_CLASS_EXPORT_IMPLEMENT( sword::RolePion_Location )

namespace sword
{
    class RoleListener_ABC : boost::noncopyable
    {
    public:
                 RoleListener_ABC() {}
        virtual ~RoleListener_ABC() {}

        virtual void Clean() = 0;

        virtual void SendChangedState( client::UnitAttributes& message ) = 0;
        virtual void SendFullState( client::UnitAttributes& message ) = 0;
    };
}

namespace
{
    class HeightListener : public RoleListener_ABC
    {
    public:
        HeightListener( Sink& sink, MIL_AgentPion& owner, core::Model& entity )
            : owner_   ( owner )
            , modified_( false )
            , height_  ( sink, entity[ "movement/height" ], bl::var( modified_ ) = true )
        {}
        virtual void Clean()
        {
            modified_ = false;
        }
        virtual void SendChangedState( client::UnitAttributes& message )
        {
            if( modified_ )
                SendFullState( message );
        }
        virtual void SendFullState( client::UnitAttributes& message )
        {
            message().set_height( *height_ );
        }
    private:
        MIL_AgentPion& owner_;
        bool modified_;
        ListenerHelper height_;
    };

    class AltitudeListener : public RoleListener_ABC
    {
    public:
        AltitudeListener( Sink& sink, core::Model& entity )
            : modified_( true )
            , height_  ( sink, entity[ "movement/height" ], bl::var( modified_ ) = true )
            , position_( sink, entity[ "movement/position" ], bl::var( modified_ ) = true )
        {}
        virtual void Clean()
        {
            modified_ = false;
        }
        virtual void SendChangedState( client::UnitAttributes& message )
        {
            if( modified_ )
                SendFullState( message );
        }
        virtual void SendFullState( client::UnitAttributes& message )
        {
            const MT_Vector2D position( position_[ "x" ], position_[ "y" ] );
            message().set_altitude( static_cast< unsigned int >( *height_ + MIL_Tools::GetAltitude( position ) ) );
        }
    private:
        bool modified_;
        ListenerHelper height_, position_;
    };

    class PositionListener : public RoleListener_ABC
    {
    public:
        PositionListener( Sink& sink, MIL_AgentPion& owner, core::Model& entity, TER_Agent_ABC& agent, boost::shared_ptr< MT_Vector2D >& pPosition )
            : owner_    ( owner )
            , agent_    ( agent )
            , modified_ ( false )
            , position_ ( sink, entity[ "movement/position" ], boost::bind( &PositionListener::Changed, this ) )
            , pPosition_( pPosition )
        {}
        virtual void Clean()
        {
            modified_ = false;
        }
        virtual void SendChangedState( client::UnitAttributes& message )
        {
            if( modified_ )
                SendFullState( message );
        }
        virtual void SendFullState( client::UnitAttributes& message )
        {
            NET_ASN_Tools::WritePoint( *pPosition_, *message().mutable_position() );
        }
    private:
        void Changed()
        {
            modified_ = true;
            pPosition_->rX_ = position_[ "x" ];
            pPosition_->rY_ = position_[ "y" ];
            agent_.UpdatePatch();
            owner_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
            owner_.Apply( &network::VisionConeNotificationHandler_ABC::NotifyVisionConeDataHasChanged );
            owner_.Apply( &location::MovementHandler_ABC::NotifyHasMove, *pPosition_ );
        }
    private:
        MIL_AgentPion& owner_;
        TER_Agent_ABC& agent_;
        bool modified_;
        ListenerHelper position_;
        boost::shared_ptr< MT_Vector2D >& pPosition_;
    };

    class DirectionListener : public RoleListener_ABC
    {
    public:
        DirectionListener( Sink& sink, MIL_AgentPion& owner, core::Model& entity, MT_Vector2D& direction )
            : owner_     ( owner )
            , modified_  ( false )
            , direction_ ( sink, entity[ "movement/direction" ], boost::bind( &DirectionListener::Changed, this ) )
            , vDirection_( direction )
        {}
        virtual void Clean()
        {
            modified_ = false;
        }
        virtual void SendChangedState( client::UnitAttributes& message )
        {
            if( modified_ )
                SendFullState( message );
        }
        virtual void SendFullState( client::UnitAttributes& message )
        {
            NET_ASN_Tools::WriteDirection( vDirection_, *message().mutable_direction() );
        }
    private:
        void Changed()
        {
            modified_ = true;
            vDirection_.rX_ = direction_[ "x" ];
            vDirection_.rY_ = direction_[ "y" ];
            owner_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
            owner_.Apply( &network::VisionConeNotificationHandler_ABC::NotifyVisionConeDataHasChanged );
        }
    private:
        MIL_AgentPion& owner_;
        bool modified_;
        ListenerHelper direction_;
        MT_Vector2D& vDirection_;
    };

    class SpeedListener : public RoleListener_ABC
    {
    public:
        SpeedListener( Sink& sink, MIL_AgentPion& owner, core::Model& entity )
            : owner_   ( owner )
            , modified_( false )
            , speed_   ( sink, entity[ "movement/speed" ], boost::bind( &SpeedListener::Changed, this ) )
        {}
        virtual void Clean()
        {
            modified_ = false;
        }
        virtual void SendChangedState( client::UnitAttributes& message )
        {
            if( modified_ )
                SendFullState( message );
        }
        virtual void SendFullState( client::UnitAttributes& message )
        {
            message().set_speed( MIL_Tools::ConvertSpeedSimToMos( *speed_ ) );
        }
    private:
        void Changed()
        {
            modified_ = true;
            owner_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
            owner_.Apply( &network::VisionConeNotificationHandler_ABC::NotifyVisionConeDataHasChanged );
        }
    private:
        MIL_AgentPion& owner_;
        bool modified_;
        ListenerHelper speed_;
    };
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location constructor
// Created: LDC 2013-01-11
// -----------------------------------------------------------------------------
RolePion_Location::RolePion_Location()
    : sink_             ( 0 )
    , owner_            ( 0 )
    , entity_           ( 0 )
    , vDirection_       ( 0, 1 )
    , bHasDoneMagicMove_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
RolePion_Location::RolePion_Location( Sink& sink, MIL_AgentPion& pion, core::Model& entity, const MT_Vector2D& position )
    : sink_             ( &sink )
    , owner_            ( &pion )
    , entity_           ( &entity )
    , vDirection_       ( 0, 1 )
    , pvPosition_       ( new MT_Vector2D( position ) )
    , bHasDoneMagicMove_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
RolePion_Location::~RolePion_Location()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::Finalize
// Created: BAX 2012-10-18
// -----------------------------------------------------------------------------
void RolePion_Location::Finalize()
{
    listeners_.push_back( boost::make_shared< PositionListener >( boost::ref( *sink_ ), boost::ref( *owner_ ), boost::ref( *entity_ ), boost::ref( *this ), boost::ref( pvPosition_ ) ) );
    listeners_.push_back( boost::make_shared< DirectionListener >( boost::ref( *sink_ ), boost::ref( *owner_ ), boost::ref( *entity_ ), boost::ref( vDirection_ ) ) );
    listeners_.push_back( boost::make_shared< SpeedListener >( boost::ref( *sink_ ), boost::ref( *owner_ ), boost::ref( *entity_ ) ) );
    listeners_.push_back( boost::make_shared< HeightListener >( boost::ref( *sink_ ), boost::ref( *owner_ ), boost::ref( *entity_ ) ) );
    listeners_.push_back( boost::make_shared< AltitudeListener >( boost::ref( *sink_ ), boost::ref( *entity_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::load
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void RolePion_Location::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< PHY_RoleInterface_Location >( *this );
    file >> bHasDoneMagicMove_;
    file >> owner_;
    file >> sink_;
    file >> entity_;
    file >> pvPosition_;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::save
// Created: JVT 2005-04-05
// -----------------------------------------------------------------------------
void RolePion_Location::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< PHY_RoleInterface_Location >( *this );
    file << bHasDoneMagicMove_;
    file << owner_;
    file << sink_;
    file << entity_;
    file << pvPosition_;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::Move
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
void RolePion_Location::Move( const MT_Vector2D& /*vNewPosition*/, const MT_Vector2D& /*vNewDirection*/, double /*rNewSpeed*/ )
{
    throw MASA_EXCEPTION( "Deprecated call." );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void RolePion_Location::SendChangedState( client::UnitAttributes& message ) const
{
    std::for_each( listeners_.begin(), listeners_.end(), boost::bind( &RoleListener_ABC::SendChangedState, _1, boost::ref( message ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void RolePion_Location::SendFullState( client::UnitAttributes& message ) const
{
    std::for_each( listeners_.begin(), listeners_.end(), boost::bind( &RoleListener_ABC::SendFullState, _1, boost::ref( message ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void RolePion_Location::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::Clean
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void RolePion_Location::Clean()
{
    std::for_each( listeners_.begin(), listeners_.end(), boost::bind( &RoleListener_ABC::Clean, _1 ) );
    bHasDoneMagicMove_ = false;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::Follow
// Created: NLD 2004-11-19
// -----------------------------------------------------------------------------
void RolePion_Location::Follow( const MIL_Agent_ABC& agent )
{
    sink_->PostCommand( "follow", core::MakeModel( "identifier", owner_->GetID() )
                                                 ( "followed", agent.GetID() ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::Hide
// Created: NLD 2004-11-24
// -----------------------------------------------------------------------------
void RolePion_Location::Hide()
{
    TER_Object_ABC::T_ObjectVector objectsColliding;
    TER_World::GetWorld().GetObjectManager().GetListAt( *pvPosition_, objectsColliding );
    for( TER_Object_ABC::CIT_ObjectVector itObject = objectsColliding.begin(); itObject != objectsColliding.end(); ++itObject )
        NotifyPutOutsideObject( static_cast< MIL_Object_ABC& >( **itObject ) );
    RemoveFromPatch();
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::Show
// Created: NLD 2004-11-24
// -----------------------------------------------------------------------------
void RolePion_Location::Show( const MT_Vector2D& vPosition )
{
    sink_->PostCommand( "magic move", core::MakeModel( "identifier", owner_->GetID() )
                                                     ( "position/x", vPosition.rX_ )
                                                     ( "position/y", vPosition.rY_ ) );
    TER_Object_ABC::T_ObjectVector objectsColliding;
    TER_World::GetWorld().GetObjectManager().GetListAt( vPosition, objectsColliding );
    for( TER_Object_ABC::CIT_ObjectVector itObject = objectsColliding.begin(); itObject != objectsColliding.end(); ++itObject )
        NotifyPutInsideObject( static_cast< MIL_Object_ABC& >( **itObject ) );
    bHasDoneMagicMove_ = true;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::MagicMove
// Created: NLD 2004-11-24
// -----------------------------------------------------------------------------
void RolePion_Location::MagicMove( const MT_Vector2D& vPosition )
{
    std::auto_ptr< moving::MoveComputer_ABC > moveComputer = owner_->GetAlgorithms().moveComputerFactory_->CreateMagicMoveComputer();
    owner_->Execute( *moveComputer );
    if( !moveComputer->CanMove() && !moveComputer->CanMoveOverride() )
        return;
    Hide();
    Show( vPosition );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::GetAgent
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MIL_Agent_ABC& RolePion_Location::GetAgent() const
{
    return *owner_;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::GetAltitude
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
double RolePion_Location::GetAltitude() const
{
    return MIL_Tools::GetAltitude( *pvPosition_ ) + (*entity_)[ "movement/height" ];
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::GetHeight
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
double RolePion_Location::GetHeight() const
{
    return (*entity_)[ "movement/height" ];
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::GetPosition
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const MT_Vector2D& RolePion_Location::GetPosition() const
{
    return *pvPosition_;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::GetSharedPosition
// Created: LDC 2009-11-17
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > RolePion_Location::GetSharedPosition() const
{
    return pvPosition_;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::GetDirection
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const MT_Vector2D& RolePion_Location::GetDirection() const
{
    return vDirection_;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::GetCurrentSpeed
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
double RolePion_Location::GetCurrentSpeed() const
{
    return (*entity_)[ "movement/speed" ];
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::HasDoneMagicMove
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool RolePion_Location::HasDoneMagicMove() const
{
    return bHasDoneMagicMove_;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::NotifyPopulationCollision
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
void RolePion_Location::NotifyFlowCollision( MIL_PopulationFlow& population )
{
    owner_->GetKnowledge().GetKsPopulationInteraction().NotifyPopulationCollision( population );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::NotifyPopulationCollision
// Created: NLD 2005-10-21
// -----------------------------------------------------------------------------
void RolePion_Location::NotifyConcentrationCollision( MIL_PopulationConcentration& population )
{
    owner_->GetKnowledge().GetKsPopulationInteraction().NotifyPopulationCollision( population );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::NotifyObjectCollision
// Created: NLD 2004-09-15
// -----------------------------------------------------------------------------
void RolePion_Location::NotifyTerrainObjectCollision( MIL_Object_ABC& object )
{
    owner_->GetKnowledge().GetKsObjectInteraction().NotifyObjectCollision( object, *pvPosition_, vDirection_ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::NotifyMovingInsideObject
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void RolePion_Location::NotifyMovingInsideObject( MIL_Object_ABC& object )
{
    object.NotifyAgentMovingInside( *owner_ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::NotifyMovingOutsideObject
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void RolePion_Location::NotifyMovingOutsideObject( MIL_Object_ABC& object )
{
    object.NotifyAgentMovingOutside( *owner_ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::NotifyPutInsideObject
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void RolePion_Location::NotifyPutInsideObject( MIL_Object_ABC& object )
{
    object.NotifyAgentPutInside( *owner_ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::NotifyPutOutsideObject
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void RolePion_Location::NotifyPutOutsideObject( MIL_Object_ABC& object )
{
    object.NotifyAgentPutOutside( *owner_ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::NotifyTerrainPutInsideObject
// Created: SLG 2010-03-31
// -----------------------------------------------------------------------------
void RolePion_Location::NotifyTerrainPutInsideObject( MIL_Object_ABC& object )
{
    owner_->Apply( &terrain::ObjectCollisionNotificationHandler_ABC::NotifyPutInsideObject, object );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::NotifyTerrainPutOutsideObject
// Created: SLG 2010-03-31
// -----------------------------------------------------------------------------
void RolePion_Location::NotifyTerrainPutOutsideObject( MIL_Object_ABC& object )
{
    owner_->Apply( &terrain::ObjectCollisionNotificationHandler_ABC::NotifyPutOutsideObject, object );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Location::Execute
// Created: SLG 2010-04-12
// -----------------------------------------------------------------------------
void RolePion_Location::Execute( urbanLocation::UrbanLocationComputer_ABC& algorithm ) const
{
    algorithm.SetPosition( *pvPosition_ );
}
