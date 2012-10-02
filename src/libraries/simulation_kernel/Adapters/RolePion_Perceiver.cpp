// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "RolePion_Perceiver.h"
#include "ListenerHelper.h"
#include "Hook.h"
#include "Sink.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Units/Radars/PHY_RadarClass.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorType.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeObject.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/Orders/MIL_Report.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_KS_Perception.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Network/NET_AgentServer.h"
#include "Network/NET_Publisher_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/ClientSenders.h"
#include "simulation_kernel/AlgorithmsFactories.h"
#include "simulation_kernel/DetectionComputer_ABC.h"
#include "simulation_kernel/DetectionComputerFactory_ABC.h"
#include "simulation_kernel/PerceptionDistanceComputer_ABC.h"
#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include "simulation_kernel/VisionConeNotificationHandler_ABC.h"
#include <core/Convert.h>
#include <core/Model.h>
#include <core/MakeModel.h>
#include <boost/serialization/map.hpp>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>
#include <boost/assign.hpp>

using namespace detection;
using namespace sword;

const unsigned int RolePion_Perceiver::nNbrStepsBetweenPeriphericalVision_ = 12; //$$$ En dur ...

BOOST_CLASS_EXPORT_IMPLEMENT( sword::RolePion_Perceiver )

namespace sword
{
    template< typename Archive >
    void save_construct_data( Archive& archive, const RolePion_Perceiver* role, const unsigned int /*version*/ )
    {
        const core::Model* model = &role->model_;
        const Sink* sink = &role->sink_;
        MIL_Agent_ABC* const pion = &role->owner_;
        const core::Model* const entity = &role->entity_;
        archive << model << sink << pion << entity;
    }

    template< typename Archive >
    void load_construct_data( Archive& archive, RolePion_Perceiver* role, const unsigned int /*version*/ )
    {
        core::Model* model;
        Sink* sink;
        MIL_Agent_ABC* pion;
        core::Model* entity;
        archive >> model >> sink >> pion >> entity;
        ::new( role )RolePion_Perceiver( *sink, *model, *pion, *entity );
    }
}

namespace
{
    class ToggleListener : public ListenerHelper
    {
    public:
        ToggleListener( core::Model& node, boost::function< void() > enable, boost::function< void() > disable )
            : ListenerHelper( node, boost::bind( &ToggleListener::Toggled, this, _1 ) )
            , enable_ ( enable )
            , disable_( disable )
        {}
    private:
        void Toggled( const core::Model& activation )
        {
            activation ? enable_() : disable_();
        }
    private:
        boost::function< void() > enable_;
        boost::function< void() > disable_;
    };
    typedef boost::function< void( DEC_KS_Perception& ) > T_Notification;
    void NotifyAgentPerception( const core::Model& effect, std::vector< T_Notification >& notifications )
    {
        for( std::size_t i = 0; i < effect.GetSize(); ++i )
        {
            const core::Model& notification = effect.GetElement( i );
            MIL_AgentPion& target = notification[ "target" ].GetUserData< MIL_AgentPion >();
            const PHY_PerceptionLevel& level = PHY_PerceptionLevel::FindPerceptionLevel( notification[ "level" ] );
            const bool recorded = notification[ "recorded" ];
            T_Notification notifier = boost::bind( static_cast< bool(DEC_KS_Perception::*)( MIL_Agent_ABC&, const PHY_PerceptionLevel&, bool ) >(&DEC_KS_Perception::NotifyPerception), _1, boost::ref( target ), boost::ref( level ), recorded );
            notifications.push_back( notifier );
        }
    }
    void NotifyObjectPerception( const core::Model& effect, std::vector< T_Notification >& notifications )
    {
        for( std::size_t i = 0; i < effect.GetSize(); ++i )
        {
            const core::Model& notification = effect.GetElement( i );
            MIL_Object_ABC& target = notification[ "target/data" ].GetUserData< MIL_Object_ABC >();
            const PHY_PerceptionLevel& level = PHY_PerceptionLevel::FindPerceptionLevel( notification[ "level" ] );
            const bool recorded = notification[ "recorded" ];
            T_Notification notifier = boost::bind( static_cast< void (DEC_KS_Perception::*)( MIL_Object_ABC&, const PHY_PerceptionLevel&, bool ) >(&DEC_KS_Perception::NotifyPerception), _1, boost::ref( target ), boost::ref( level ), recorded );
            notifications.push_back( notifier );
        }
    }
    void NotifyFlowPerception( const core::Model& effect, std::vector< T_Notification >& notifications )
    {
        for( std::size_t i = 0; i < effect.GetSize(); ++i )
        {
            const core::Model& notification = effect.GetElement( i );
            MIL_PopulationFlow& flow = *dynamic_cast< MIL_PopulationFlow* >( notification[ "target/data" ].GetUserData< MIL_PopulationElement_ABC* >() );
            const PHY_PerceptionLevel& level = PHY_PerceptionLevel::FindPerceptionLevel( notification[ "level" ] );
            const bool recorded = notification[ "recorded" ];
            const core::Model& points = notification[ "shape" ];
            T_PointVector shape;
            for( std::size_t i = 0; i < points.GetSize(); ++i )
                shape.push_back( MT_Vector2D( points.GetElement( i )[ "x" ], points.GetElement( i )[ "y" ] ) );
            T_Notification notifier = boost::bind( static_cast< bool(DEC_KS_Perception::*)( MIL_PopulationFlow&, const PHY_PerceptionLevel&, const std::vector< MT_Vector2D >&, bool ) >(&DEC_KS_Perception::NotifyPerception), _1, boost::ref( flow ), boost::ref( level ), shape, recorded );
            notifications.push_back( notifier );
        }
    }
    void NotifyConcentrationPerception( const core::Model& effect, std::vector< T_Notification >& notifications )
    {
        for( std::size_t i = 0; i < effect.GetSize(); ++i )
        {
            const core::Model& notification = effect.GetElement( i );
            MIL_PopulationConcentration& target = *dynamic_cast< MIL_PopulationConcentration* >( notification[ "target/data" ].GetUserData< MIL_PopulationElement_ABC* >() );
            const PHY_PerceptionLevel& level = PHY_PerceptionLevel::FindPerceptionLevel( notification[ "level" ] );
            const bool recorded = notification[ "recorded" ];
            T_Notification notifier = boost::bind( static_cast< bool (DEC_KS_Perception::*)( MIL_PopulationConcentration&, const PHY_PerceptionLevel&, bool ) >(&DEC_KS_Perception::NotifyPerception), _1, boost::ref( target ), boost::ref( level ), recorded );
            notifications.push_back( notifier );
        }
    }
    void NotifyUrbanObjectPerception( const core::Model& effect, std::vector< T_Notification >& notifications )
    {
        for( std::size_t i = 0; i < effect.GetSize(); ++i )
        {
            const core::Model& notification = effect.GetElement( i );
            const MIL_UrbanObject_ABC& object = notification[ "target/data" ].GetUserData< MIL_UrbanObject_ABC >();
            const PHY_PerceptionLevel& level = PHY_PerceptionLevel::FindPerceptionLevel( notification[ "level" ] );
            T_Notification notifier = boost::bind( static_cast< void(DEC_KS_Perception::*)( const MIL_UrbanObject_ABC&, const PHY_PerceptionLevel& ) >(&DEC_KS_Perception::NotifyPerception), _1, boost::ref( object ), boost::ref( level ) );
            notifications.push_back( notifier );
        }
    }
}

DECLARE_HOOK( IsUsingActiveRadar, bool, ( const SWORD_Model* entity ) )
DECLARE_HOOK( IsUsingSpecializedActiveRadar, bool, ( const SWORD_Model* entity, const char* radarType ) )
DECLARE_HOOK( ComputeKnowledgeObjectPerception, size_t, ( const SWORD_Model* model, const SWORD_Model* entity, const SWORD_Model* knowledgeObject ) )
DECLARE_HOOK( GetPerceptionId, int, () )

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::Initialize
// Created: SLI 2012-07-05
// -----------------------------------------------------------------------------
void RolePion_Perceiver::Initialize( core::Facade& facade )
{
    USE_HOOK( IsUsingActiveRadar, facade );
    USE_HOOK( IsUsingSpecializedActiveRadar, facade );
    USE_HOOK( ComputeKnowledgeObjectPerception, facade );
    USE_HOOK( GetPerceptionId, facade );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver constructor
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
RolePion_Perceiver::RolePion_Perceiver( Sink& sink, const core::Model& model, MIL_Agent_ABC& pion, core::Model& entity )
    : sink_                          ( sink )
    , model_                         ( model )
    , owner_                         ( pion )
    , entity_                        ( entity )
    , bHasChanged_                   ( true )
    , bExternalMustChangePerception_ ( false )
    , bExternalMustChangeRadar_      ( false )
    , bExternalCanPerceive_          ( true )
    , bExternalMustUpdateVisionCones_( false )
    , bRadarStateHasChanged_         ( true )
{
    static unsigned int nNbr = 0; // $$$$ MCO 2012-08-14: size_t ?
    nNextPeriphericalVisionStep_ = ++nNbr % nNbrStepsBetweenPeriphericalVision_;
    entity[ "perceptions/peripherical-vision/next-tick" ] = nNextPeriphericalVisionStep_;
    entity[ "perceptions/max-agent-perception-distance" ] = 0;
    entity[ "perceptions/max-theoretical-agent-perception-distance" ] = 0;
    AddListener< ToggleListener >( "perceptions/record-mode/activated", boost::bind( &RolePion_Perceiver::EnableRecordMode, this ), boost::bind( &RolePion_Perceiver::DisableRecordMode, this ) );
    listeners_.push_back( boost::make_shared< ListenerHelper >( boost::cref( entity[ "perceptions/notifications/agents" ] ), boost::bind( &::NotifyAgentPerception, _1, boost::ref( notifications_ ) ) ) );
    listeners_.push_back( boost::make_shared< ListenerHelper >( boost::cref( entity[ "perceptions/notifications/agents-in-zone" ] ), boost::bind( &::NotifyAgentPerception, _1, boost::ref( notifications_ ) ) ) );
    listeners_.push_back( boost::make_shared< ListenerHelper >( boost::cref( entity[ "perceptions/notifications/objects" ] ), boost::bind( &::NotifyObjectPerception, _1, boost::ref( notifications_ ) ) ) );
    listeners_.push_back( boost::make_shared< ListenerHelper >( boost::cref( entity[ "perceptions/notifications/population-concentrations" ] ), boost::bind( &::NotifyConcentrationPerception, _1, boost::ref( notifications_ ) ) ) );
    listeners_.push_back( boost::make_shared< ListenerHelper >( boost::cref( entity[ "perceptions/notifications/population-flows" ] ), boost::bind( &::NotifyFlowPerception, _1, boost::ref( notifications_ ) ) ) );
    listeners_.push_back( boost::make_shared< ListenerHelper >( boost::cref( entity[ "perceptions/notifications/urban-blocks" ] ), boost::bind( &::NotifyUrbanObjectPerception, _1, boost::ref( notifications_ ) ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver destructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
RolePion_Perceiver::~RolePion_Perceiver()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::AddListener
// Created: SLI 2012-04-04
// -----------------------------------------------------------------------------
template< typename Listener, typename Enable, typename Disable >
void RolePion_Perceiver::AddListener( const std::string& perception, Enable enable, Disable disable )
{
    listeners_.push_back( boost::make_shared< Listener >( boost::ref( entity_[ perception ] ), enable, disable ) );
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
namespace boost
{
    namespace serialization
    {
        // =============================================================================
        // T_SurfaceAgentKeyPair
        // =============================================================================
        template< typename Archive >
        inline
        void serialize( Archive& file, RolePion_Perceiver::T_SurfaceAgentKeyPair& pair, const unsigned int nVersion )
        {
            split_free( file, pair, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const RolePion_Perceiver::T_SurfaceAgentKeyPair& pair, const unsigned int )
        {
            assert( pair.first );
            unsigned id = pair.first->GetType().GetID();
            file << id
                 << pair.second;
        }

        template< typename Archive >
        void load( Archive& file, RolePion_Perceiver::T_SurfaceAgentKeyPair& pair, const unsigned int )
        {
            unsigned int nID;
            file >> nID;
            assert( PHY_SensorType::FindSensorType( nID ) );
            pair.first = PHY_SensorType::FindSensorType( nID )->GetTypeAgent();
            file >> pair.second;
        }

        // =============================================================================
        // T_SurfaceObjectKeyPair
        // =============================================================================
        template< typename Archive >
        inline
        void serialize( Archive& file, RolePion_Perceiver::T_SurfaceObjectKeyPair& pair, const unsigned int nVersion )
        {
            split_free( file, pair, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const RolePion_Perceiver::T_SurfaceObjectKeyPair& pair, const unsigned int )
        {
            assert( pair.first );
            unsigned id = pair.first->GetType().GetID();
            file << id
                 << pair.second;
        }

        template< typename Archive >
        void load( Archive& file, RolePion_Perceiver::T_SurfaceObjectKeyPair& pair, const unsigned int )
        {
            unsigned int nID;

            file >> nID;
            assert( PHY_SensorType::FindSensorType( nID ) );
            pair.first = PHY_SensorType::FindSensorType( nID )->GetTypeObject();

            file >> pair.second;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void RolePion_Perceiver::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Perceiver >( *this )
         & nNextPeriphericalVisionStep_
         & surfacesAgent_
         & surfacesObject_;
}

// =============================================================================
// PERCEPTIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableSensors
// Created: JVT 2005-04-28
// -----------------------------------------------------------------------------
void RolePion_Perceiver::EnableSensors()
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableSensors
// Created: JVT 2005-04-28
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableSensors()
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableCoupDeSonde
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
void RolePion_Perceiver::EnableCoupDeSonde()
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableCoupDeSonde
// Created: NLD 2004-10-14
// Modified: JVT 2004-10-21
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableCoupDeSonde()
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableRecoAlat
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
void RolePion_Perceiver::EnableRecoAlat( const TER_Localisation& /*localisation*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableRecoAlat
// Created: NLD 2004-10-14
// Modified: JVT 2004-10-21
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableRecoAlat()
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableSurveillanceLocalisation
// Created: NLD 2004-11-17
// -----------------------------------------------------------------------------
int RolePion_Perceiver::EnableSurveillanceLocalisation( const TER_Localisation& /*localisation*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableSurveillanceLocalisation
// Created: NLD 2004-11-17
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableSurveillanceLocalisation( int /*id*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableRecoPoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
int RolePion_Perceiver::EnableRecoPoint( const MT_Vector2D& /*center*/, double /*rSize*/, double /*rSpeed*/, DEC_Decision_ABC& /*callerAgent*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableRecoPoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableRecoPoint( int /*id*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableRecoObjects
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
int RolePion_Perceiver::EnableRecoObjects( const TER_Localisation& /*localisation*/, const MT_Vector2D& /*vCenter*/, double /*rSpeed*/, DEC_Decision_ABC& /*callerAgent*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableRecoObjects
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableRecoObjects( int /*id*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableRecoLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
int RolePion_Perceiver::EnableRecoLocalisation( const TER_Localisation& /*localisation*/, DEC_Decision_ABC& /*callerAgent*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableRecoLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
int RolePion_Perceiver::EnableRecoLocalisation( const TER_Localisation& localisation, float rGrowthSpeed, DEC_Decision_ABC& /*callerAgent*/ )  // $$$$ _RC_ SLI 2012-08-28: Remove this, only for PHY_ActionControlZone
{
    const int perceptionId = GET_HOOK( GetPerceptionId )();
    sink_.PostCommand( "toggle reco", core::MakeModel( "identifier", owner_.GetID() )
                                                       ( "activated", true )
                                                       ( "perception-id", perceptionId )
                                                       ( "has-growth-speed", true )
                                                       ( "growth-speed", rGrowthSpeed )
                                                       ( "localization", core::MakeUserData( localisation ) ) );
    return perceptionId;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableRecoLocalisation
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
int RolePion_Perceiver::EnableRecoUrbanBlock( MIL_UrbanObject_ABC* /*pUrbanBlock*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableControlLocalisation
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
int RolePion_Perceiver::EnableControlLocalisation( const TER_Localisation& localisation, DEC_Decision_ABC& /*callerAgent*/ ) // $$$$ _RC_ SLI 2012-08-28: Remove this, only for PHY_ActionControlZone
{
    const int perceptionId = GET_HOOK( GetPerceptionId )();
    sink_.PostCommand( "toggle reco", core::MakeModel( "identifier", owner_.GetID() )
                                                       ( "activated", true )
                                                       ( "perception-id", perceptionId )
                                                       ( "has-growth-speed", false )
                                                       ( "localization", core::MakeUserData( localisation ) ) );
    return perceptionId;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableRecoLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableRecoLocalisation( int id ) // $$$$ _RC_ SLI 2012-08-28: Remove this, only for PHY_ActionControlZone
{
    sink_.PostCommand( "toggle reco", core::MakeModel( "identifier", owner_.GetID() )
                                                       ( "activated", false )
                                                       ( "perception-id", id ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableRecoUrbanBlock
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableRecoUrbanBlock( int /*id*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::IsUsingActiveRadar
// Created: NLD 2005-02-23
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::IsUsingActiveRadar() const
{
    return GET_HOOK( IsUsingActiveRadar )( core::Convert( &entity_ ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::IsUsingActiveRadar
// Created: JSR 2010-03-18
// LTO
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::IsUsingActiveRadar( const PHY_RadarClass& radarClass ) const
{
    static const std::vector< std::string > translation = boost::assign::list_of( "radar" )( "tapping" )( "tapping-radar" );
    return GET_HOOK( IsUsingSpecializedActiveRadar )( core::Convert( &entity_ ), translation.at( radarClass.GetID() ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableRadarOnLocalisation
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
int RolePion_Perceiver::EnableRadarOnLocalisation( const PHY_RadarClass& /*radarClass*/, const TER_Localisation& /*localisation*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableRadarOnLocalisation
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableRadarOnLocalisation( const PHY_RadarClass& /*radarClass*/, int /*id*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableRadar
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void RolePion_Perceiver::EnableRadar( const PHY_RadarClass& /*radarClass*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableRadar
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableRadar( const PHY_RadarClass& /*radarClass*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableFlyingShellDetection
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
int RolePion_Perceiver::EnableFlyingShellDetection( const TER_Localisation& /*localisation*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableFlyingShellDetection
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableFlyingShellDetection( int /*id*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::GetMaxAgentPerceptionDistance
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
double RolePion_Perceiver::GetMaxAgentPerceptionDistance() const
{
    return entity_[ "perceptions/max-agent-perception-distance" ];
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::GetMaxTheoreticalAgentPerceptionDistance
// Created: MMC 2011-05-11
// -----------------------------------------------------------------------------
double RolePion_Perceiver::GetMaxTheoreticalAgentPerceptionDistance() const
{
    return entity_[ "perceptions/max-theoretical-agent-perception-distance" ];
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableAllPerceptions
// Created: NLD 2005-04-01
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableAllPerceptions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::ExecutePerceptions
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void RolePion_Perceiver::ExecutePerceptions()
{
    BOOST_FOREACH( const T_Notification& notification, notifications_ )
        notification( owner_.GetKnowledge().GetKsPerception() );
    notifications_.clear();
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::ComputePerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& RolePion_Perceiver::ComputePerception( const DEC_Knowledge_Object& knowledge ) const // $$$$ _RC_ SLI 2012-10-02: only called by automat
{
    core::Model model;
    model[ "data" ].SetUserData( &knowledge );
    return PHY_PerceptionLevel::FindPerceptionLevel( GET_HOOK( ComputeKnowledgeObjectPerception )( core::Convert( &model_ ), core::Convert( &entity_ ), core::Convert( &model ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::ComputePerception
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& RolePion_Perceiver::ComputePerception( const MT_Vector2D& /*vPoint*/ ) const
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::Update
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
void RolePion_Perceiver::Update( bool /*bIsDead*/ )
{
    if( HasChanged() )
    {
        if( HasRadarStateChanged() )
            owner_.Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
        owner_.Apply( &network::VisionConeNotificationHandler_ABC::NotifyVisionConeDataHasChanged );
    }
    // Debug - Cones de vision
    if( MIL_AgentServer::GetWorkspace().GetAgentServer().MustSendUnitVisionCones() )
    {
        if( bExternalMustUpdateVisionCones_
            || MIL_AgentServer::GetWorkspace().GetAgentServer().MustInitUnitVisionCones() )
            SendDebugState();
    }
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::GetKnowledgeGroup
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_KnowledgeGroup > RolePion_Perceiver::GetKnowledgeGroup() const
{
    return owner_.GetKnowledgeGroup();
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::IsKnown
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::IsKnown( const MIL_Agent_ABC& agent ) const
{
    return owner_.GetKnowledgeGroup()->GetKnowledge().IsKnown( agent );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::IsIdentified
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::IsIdentified( const MIL_Agent_ABC& agent ) const
{
    return owner_.GetKnowledge().IsIdentified( agent );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::IsKnown
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::IsKnown( const MIL_Object_ABC& object ) const
{
    return owner_.GetArmy().GetKnowledge().IsKnown( object );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::IsIdentified
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::IsIdentified( const MIL_Object_ABC& object ) const
{
    return owner_.GetKnowledge().IsIdentified( object );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::IsIdentified
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::IsIdentified( const MIL_PopulationConcentration& concentration ) const
{
    return owner_.GetKnowledge().IsIdentified( concentration );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::IsIdentified
// Created: MGD 2009-11-25
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::IsIdentified( const MIL_UrbanObject_ABC& object ) const
{
    return owner_.GetKnowledge().IsIdentified( object );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyPerception
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
void RolePion_Perceiver::NotifyPerception( const MIL_Effect_IndirectFire& /*flyingShell*/ ) const
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyPerception
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void RolePion_Perceiver::NotifyPerception( MIL_Agent_ABC& /*agent*/, const PHY_PerceptionLevel& /*level*/, bool /*bPerceptionRecorded*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyPerception
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::NotifyPerception( MIL_Agent_ABC& /*agent*/, const PHY_PerceptionLevel& /*level*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyPerception
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void RolePion_Perceiver::NotifyPerception( MIL_Object_ABC& /*object*/, const PHY_PerceptionLevel& /*level*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyPerception
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::NotifyPerception( MIL_PopulationConcentration& /*concentration*/, const PHY_PerceptionLevel& /*level*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyPerception
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::NotifyPerception( MIL_PopulationFlow& /*flow*/, const PHY_PerceptionLevel& /*level*/, const T_PointVector& /*shape*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyPerceptionUrban
// Created: MGD 2009-11-20
// -----------------------------------------------------------------------------
void RolePion_Perceiver::NotifyPerceptionUrban( const MIL_UrbanObject_ABC& /*object*/, const PHY_PerceptionLevel& /*level*/ ) const
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifytExternalPerception
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
void RolePion_Perceiver::NotifyExternalPerception( MIL_Agent_ABC& agent, const PHY_PerceptionLevel& level )
{
    sink_.PostCommand( "external perception", core::MakeModel( "identifier", owner_.GetID() )
                                                               ( "level", level.GetID() )
                                                               ( "target", reinterpret_cast< size_t >( &agent ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableRecordMode
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
void RolePion_Perceiver::EnableRecordMode()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableRecordMode
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableRecordMode()
{
    owner_.GetKnowledge().GetKsPerception().MakePerceptionsAvailable();
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::HasDelayedPerceptions
// Created: JVT 2004-12-03
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::HasDelayedPerceptions() const
{
    throw std::runtime_error( __FUNCTION__ );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::SendDebugState
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
void RolePion_Perceiver::SendDebugState() const
{
    client::UnitVisionCones message;
    message().mutable_unit()->set_id( owner_.GetID() );
    std::auto_ptr< detection::PerceptionDistanceComputer_ABC > algorithm = owner_.GetAlgorithms().detectionComputerFactory_->CreateDistanceComputer();
    message().set_elongation( static_cast< float >( owner_.Execute( *algorithm ).GetElongationFactor() ) ); //@TODO MGD share
    message().mutable_cones();
    const core::Model& cones = entity_[ "perceptions/cones" ];
    for( std::size_t i = 0; i != cones.GetSize(); ++i )
    {
        const core::Model& cone = cones.GetElement( i );
        sword::VisionCone& msg = *message().mutable_cones()->add_elem();
        NET_ASN_Tools::WritePoint( MT_Vector2D( cone[ "origin/x" ], cone[ "origin/y" ] ), *msg.mutable_origin() );
        msg.set_height( cone[ "height" ] );
        msg.set_sensor( cone[ "sensor" ] );
        msg.mutable_directions();
        const core::Model& sectors = cone[ "sectors" ];
        for( std::size_t j = 0u; j < sectors.GetSize(); ++j )
        {
            const core::Model& sector = sectors.GetElement( j );
            NET_ASN_Tools::WriteDirection( MT_Vector2D( sector[ "direction/x" ], sector[ "direction/y" ] ), *msg.mutable_directions()->add_elem() );
        }
    }
    message.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void RolePion_Perceiver::SendFullState( client::UnitAttributes& msg ) const
{
    msg().set_radar_active( IsUsingActiveRadar() );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void RolePion_Perceiver::SendChangedState( client::UnitAttributes& msg ) const
{
    if( bRadarStateHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::IsPeriphericalVisionEnabled
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::IsPeriphericalVisionEnabled() const
{
    return entity_[ "perceptions/peripherical-vision/activated" ];
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::GetPion
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
MIL_Agent_ABC& RolePion_Perceiver::GetPion() const
{
    return owner_;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::SetVisionModeNormal
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void RolePion_Perceiver::SetVisionModeNormal()
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::SetVisionModeDirection
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void RolePion_Perceiver::SetVisionModeDirection( const MT_Vector2D& /*vDirection*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::SetVisionModePoint
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void RolePion_Perceiver::SetVisionModePoint( const MT_Vector2D& /*vPoint*/ )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::HasChanged
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::HasChanged() const
{
    return bHasChanged_ || bRadarStateHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::GetMainPerceptionDirection
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void RolePion_Perceiver::GetMainPerceptionDirection( MT_Vector2D& vDirection ) const
{
    vDirection.rX_ = entity_[ "perceptions/main-perception-direction/x" ];
    vDirection.rY_ = entity_[ "perceptions/main-perception-direction/y" ];
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::HasRadarStateChanged
// Created: NLD 2005-02-23
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::HasRadarStateChanged() const
{
    return bRadarStateHasChanged_;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::Clean
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void RolePion_Perceiver::Clean()
{
    bHasChanged_           = false;
    bExternalMustChangePerception_ = false;
    bExternalMustChangeRadar_ = false;
    bRadarStateHasChanged_ = false;
    bExternalMustUpdateVisionCones_ = false;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::GetSurfacesAgent
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
const RolePion_Perceiver::T_SurfaceAgentMap& RolePion_Perceiver::GetSurfacesAgent() const
{
    return surfacesAgent_;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::GetSurfacesObject
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
const RolePion_Perceiver::T_SurfaceObjectMap& RolePion_Perceiver::GetSurfacesObject() const
{
    return surfacesObject_;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::GetRadars
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
const RolePion_Perceiver::T_RadarSet& RolePion_Perceiver::GetRadars( const PHY_RadarClass& radarClass )
{
    return radars_[ &radarClass ];
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::Execute
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void RolePion_Perceiver::Execute( detection::DetectionComputer_ABC& algorithm ) const
{
    if( algorithm.GetTarget() != owner_ && owner_.GetKnowledge().WasPerceived( algorithm.GetTarget() ) )
        algorithm.AlreadyPerceived();
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyHasChanged
// Created: MGD 2009-09-29
// -----------------------------------------------------------------------------
void RolePion_Perceiver::NotifyComponentHasChanged()
{
    bExternalMustChangePerception_ = true;
    bExternalMustChangeRadar_ = true;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyCaptured
// Created: MGD 2009-10-14
// -----------------------------------------------------------------------------
void RolePion_Perceiver::NotifyCaptured()
{
    bExternalCanPerceive_ = false;
    bExternalMustChangePerception_ = true;
    bExternalMustChangeRadar_ = true;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifySurrendered
// Created: LDC 2012-08-06
// -----------------------------------------------------------------------------
void RolePion_Perceiver::NotifySurrendered()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifySurrenderCanceled
// Created: LDC 2012-08-06
// -----------------------------------------------------------------------------
void RolePion_Perceiver::NotifySurrenderCanceled()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyReleased
// Created: MGD 2009-10-1
// -----------------------------------------------------------------------------
void RolePion_Perceiver::NotifyReleased()
{
    bExternalCanPerceive_ = true;
    bExternalMustChangePerception_ = true;
    bExternalMustChangeRadar_ = true;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyVisionConeDataHasChanged
// Created: MGD 2009-10-15
// -----------------------------------------------------------------------------
void RolePion_Perceiver::NotifyVisionConeDataHasChanged()
{
    bExternalMustUpdateVisionCones_ = true;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyIsLoaded
// Created: MGD 2009-10-15
// -----------------------------------------------------------------------------
void RolePion_Perceiver::NotifyIsLoadedForTransport()
{
    bExternalCanPerceive_ = false;
    bExternalMustChangePerception_ = true;
    bExternalMustChangeRadar_ = true;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyIsUnLoaded
// Created: MGD 2009-10-15
// -----------------------------------------------------------------------------
void RolePion_Perceiver::NotifyIsUnLoadedForTransport()
{
    bExternalCanPerceive_ = true;
    bExternalMustChangePerception_ = true;
    bExternalMustChangeRadar_ = true;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyIsLoaded
// Created: MGD 2009-10-15
// -----------------------------------------------------------------------------
void RolePion_Perceiver::NotifyIsLoadedInVab()
{
    bExternalMustChangePerception_ = true;
    bExternalMustChangeRadar_ = true;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyIsUnLoaded
// Created: MGD 2009-10-15
// -----------------------------------------------------------------------------
void RolePion_Perceiver::NotifyIsUnLoadedInVab()
{
    bExternalMustChangePerception_ = true;
    bExternalMustChangeRadar_ = true;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableFireObserver
// Created: LGY 2012-06-28
// -----------------------------------------------------------------------------
void RolePion_Perceiver::EnableFireObserver()
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableFireObserver
// Created: LGY 2012-06-28
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableFireObserver()
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::IsFireObserver
// Created: LGY 2012-06-28
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::IsFireObserver() const
{
    return entity_[ "perceptions/fire-observer/activated" ];
}
