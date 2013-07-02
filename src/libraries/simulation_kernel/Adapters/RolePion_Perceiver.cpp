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
#include "MIL_AgentServer.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/Units/Radars/PHY_RadarClass.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
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

BOOST_CLASS_EXPORT_IMPLEMENT( sword::RolePion_Perceiver )

namespace
{
    const unsigned int nNbrStepsBetweenPeriphericalVision = 12; //$$$ En dur ...
    unsigned int nNbr = 0; // $$$$ MCO 2012-08-14: size_t ?
    void Initialize( core::Model& entity )
    {
        entity[ "perceptions/peripherical-vision/next-tick" ] = ++nNbr % nNbrStepsBetweenPeriphericalVision;
    }
}
DECLARE_HOOK( IsUsingActiveRadar, bool, ( const SWORD_Model* entity ) )
DECLARE_HOOK( IsUsingSpecializedActiveRadar, bool, ( const SWORD_Model* entity, const char* radarType ) )
DECLARE_HOOK( ComputeKnowledgeObjectPerception, size_t, ( const SWORD_Model* model, const SWORD_Model* entity, const SWORD_Model* knowledgeObject ) )
DECLARE_HOOK( GetPerceptionId, int, () )
DECLARE_HOOK( GetPerception, double, ( const SWORD_Model* entity, const MT_Vector2D* point, const MT_Vector2D* target ) )

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver constructor
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
RolePion_Perceiver::RolePion_Perceiver()
    : sink_  ( 0 )
    , model_ ( 0 )
    , owner_ ( 0 )
    , entity_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver constructor
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
RolePion_Perceiver::RolePion_Perceiver( Sink& sink, const core::Model& model, MIL_Agent_ABC& pion, core::Model& entity )
    : sink_  ( &sink )
    , model_ ( &model )
    , owner_ ( &pion )
    , entity_( &entity )
{
    Initialize( entity );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver destructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
RolePion_Perceiver::~RolePion_Perceiver()
{
    // NOTHING
}

namespace
{
    void ToggleRecordMode( const core::Model& activation, DEC_KS_Perception& perception )
    {
        if( !activation )
            perception.MakePerceptionsAvailable();
    }
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::Finalize
// Created: BAX 2012-10-08
// -----------------------------------------------------------------------------
void RolePion_Perceiver::Finalize()
{
    recordModeListener_.reset( new ListenerHelper( *sink_, (*entity_)[ "perceptions/record-mode/activated" ], boost::bind( &ToggleRecordMode, _1, boost::ref( owner_->GetKnowledge().GetKsPerception() ) ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::load
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void RolePion_Perceiver::load( Archive& file, const unsigned int )
{
    file >> boost::serialization::base_object< PHY_RoleInterface_Perceiver >( *this );
    file >> sink_;
    file >> model_;
    file >> owner_;
    file >> entity_;
    Initialize( *entity_ );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::save
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void RolePion_Perceiver::save( Archive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< PHY_RoleInterface_Perceiver >( *this );
    file << sink_;
    file << model_;
    file << owner_;
    file << entity_;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableSensors
// Created: JVT 2005-04-28
// -----------------------------------------------------------------------------
void RolePion_Perceiver::EnableSensors()
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableSensors
// Created: JVT 2005-04-28
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableSensors()
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableCoupDeSonde
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
void RolePion_Perceiver::EnableCoupDeSonde()
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableCoupDeSonde
// Created: NLD 2004-10-14
// Modified: JVT 2004-10-21
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableCoupDeSonde()
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableRecoAlat
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
void RolePion_Perceiver::EnableRecoAlat( const TER_Localisation& /*localisation*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableRecoAlat
// Created: NLD 2004-10-14
// Modified: JVT 2004-10-21
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableRecoAlat()
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableSurveillanceLocalisation
// Created: NLD 2004-11-17
// -----------------------------------------------------------------------------
int RolePion_Perceiver::EnableSurveillanceLocalisation( const TER_Localisation& /*localisation*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableSurveillanceLocalisation
// Created: NLD 2004-11-17
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableSurveillanceLocalisation( int /*id*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableRecoPoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
int RolePion_Perceiver::EnableRecoPoint( const MT_Vector2D& /*center*/, double /*rSize*/, double /*rSpeed*/, DEC_Decision_ABC& /*callerAgent*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableRecoPoint
// Created: JVT 2004-10-21
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableRecoPoint( int /*id*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableRecoObjects
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
int RolePion_Perceiver::EnableRecoObjects( const TER_Localisation& /*localisation*/, const MT_Vector2D& /*vCenter*/, double /*rSpeed*/, DEC_Decision_ABC& /*callerAgent*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableRecoObjects
// Created: JVT 2005-01-19
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableRecoObjects( int /*id*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableRecoLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
int RolePion_Perceiver::EnableRecoLocalisation( const TER_Localisation& /*localisation*/, DEC_Decision_ABC& /*callerAgent*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableRecoLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
int RolePion_Perceiver::EnableRecoLocalisation( const TER_Localisation& localisation, float rGrowthSpeed, DEC_Decision_ABC& /*callerAgent*/ )  // $$$$ _RC_ SLI 2012-08-28: Remove this, only for PHY_ActionControlZone
{
    const int perceptionId = GET_HOOK( GetPerceptionId )();
    sink_->PostCommand( "toggle reco", core::MakeModel( "identifier", owner_->GetID() )
                                                     ( "activated", true )
                                                     ( "perception-id", perceptionId )
                                                     ( "has-growth-speed", true )
                                                     ( "growth-speed", rGrowthSpeed )
                                                     ( "localization/data", core::MakeUserData( boost::make_shared< TER_Localisation >(  localisation ) ) ) );
    return perceptionId;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableRecoLocalisation
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
int RolePion_Perceiver::EnableRecoUrbanBlock( MIL_UrbanObject_ABC* /*pUrbanBlock*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableControlLocalisation
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
int RolePion_Perceiver::EnableControlLocalisation( const TER_Localisation& localisation, DEC_Decision_ABC& /*callerAgent*/ ) // $$$$ _RC_ SLI 2012-08-28: Remove this, only for PHY_ActionControlZone
{
    const int perceptionId = GET_HOOK( GetPerceptionId )();
    sink_->PostCommand( "toggle reco", core::MakeModel( "identifier", owner_->GetID() )
                                                     ( "activated", true )
                                                     ( "perception-id", perceptionId )
                                                     ( "has-growth-speed", false )
                                                     ( "localization/data", core::MakeUserData( boost::make_shared< TER_Localisation >(  localisation ) ) ) );
    return perceptionId;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableRecoLocalisation
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableRecoLocalisation( int id ) // $$$$ _RC_ SLI 2012-08-28: Remove this, only for PHY_ActionControlZone
{
    sink_->PostCommand( "toggle reco", core::MakeModel( "identifier", owner_->GetID() )
                                                     ( "activated", false )
                                                     ( "perception-id", id ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableRecoUrbanBlock
// Created: MGD 2010-02-11
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableRecoUrbanBlock( int /*id*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::IsUsingActiveRadar
// Created: NLD 2005-02-23
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::IsUsingActiveRadar() const
{
    return GET_HOOK( IsUsingActiveRadar )( core::Convert( entity_ ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::IsUsingActiveRadar
// Created: JSR 2010-03-18
// LTO
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::IsUsingActiveRadar( const PHY_RadarClass& radarClass ) const
{
    static const std::vector< std::string > translation = boost::assign::list_of( "radar" )( "tapping" )( "tapping-radar" );
    return GET_HOOK( IsUsingSpecializedActiveRadar )( core::Convert( entity_ ), translation.at( radarClass.GetID() ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableRadarOnLocalisation
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
int RolePion_Perceiver::EnableRadarOnLocalisation( const PHY_RadarClass& /*radarClass*/, const TER_Localisation& /*localisation*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableRadarOnLocalisation
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableRadarOnLocalisation( const PHY_RadarClass& /*radarClass*/, int /*id*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableRadar
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void RolePion_Perceiver::EnableRadar( const PHY_RadarClass& /*radarClass*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableRadar
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableRadar( const PHY_RadarClass& /*radarClass*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableFlyingShellDetection
// Created: JVT 2004-10-28
// -----------------------------------------------------------------------------
int RolePion_Perceiver::EnableFlyingShellDetection( const TER_Localisation& /*localisation*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableFlyingShellDetection
// Created: JVT 2004-10-22
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableFlyingShellDetection( int /*id*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::GetMaxAgentPerceptionDistance
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
double RolePion_Perceiver::GetMaxAgentPerceptionDistance() const
{
    return (*entity_)[ "perceptions/max-agent-perception-distance" ];
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::GetMaxTheoreticalAgentPerceptionDistance
// Created: MMC 2011-05-11
// -----------------------------------------------------------------------------
double RolePion_Perceiver::GetMaxTheoreticalAgentPerceptionDistance() const
{
    return (*entity_)[ "perceptions/max-theoretical-agent-perception-distance" ];
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableAllPerceptions
// Created: NLD 2005-04-01
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableAllPerceptions()
{
    sink_->PostCommand( "disable all perceptions", core::MakeModel( "identifier", owner_->GetID() ) );
}

namespace
{
    void NotifyAgentPerception( const core::Model& effect, DEC_KS_Perception& perceptions )
    {
        for( std::size_t i = 0; i < effect.GetSize(); ++i )
        {
            const core::Model& notification = effect.GetElement( i );
            MIL_AgentPion& target = notification[ "target/data" ].GetUserData< MIL_AgentPion >();
            const PHY_PerceptionLevel& level = PHY_PerceptionLevel::FindPerceptionLevel( notification[ "level" ] );
            const bool recorded = notification[ "recorded" ];
            perceptions.NotifyPerception( target, level, recorded );
        }
    }
    void NotifyObjectPerception( const core::Model& effect, DEC_KS_Perception& perceptions )
    {
        for( std::size_t i = 0; i < effect.GetSize(); ++i )
        {
            const core::Model& notification = effect.GetElement( i );
            MIL_Object_ABC& target = notification[ "target/data" ].GetUserData< MIL_Object_ABC >();
            const PHY_PerceptionLevel& level = PHY_PerceptionLevel::FindPerceptionLevel( notification[ "level" ] );
            const bool recorded = notification[ "recorded" ];
            perceptions.NotifyPerception( target, level, recorded );
        }
    }
    void NotifyFlowPerception( const core::Model& effect, DEC_KS_Perception& perceptions )
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
            perceptions.NotifyPerception( flow, level, shape, recorded );
        }
    }
    void NotifyConcentrationPerception( const core::Model& effect, DEC_KS_Perception& perceptions )
    {
        for( std::size_t i = 0; i < effect.GetSize(); ++i )
        {
            const core::Model& notification = effect.GetElement( i );
            MIL_PopulationConcentration& target = *dynamic_cast< MIL_PopulationConcentration* >( notification[ "target/data" ].GetUserData< MIL_PopulationElement_ABC* >() );
            const PHY_PerceptionLevel& level = PHY_PerceptionLevel::FindPerceptionLevel( notification[ "level" ] );
            const bool recorded = notification[ "recorded" ];
            perceptions.NotifyPerception( target, level, recorded );
        }
    }
    void NotifyUrbanObjectPerception( const core::Model& effect, DEC_KS_Perception& perceptions )
    {
        for( std::size_t i = 0; i < effect.GetSize(); ++i )
        {
            const core::Model& notification = effect.GetElement( i );
            const MIL_UrbanObject_ABC& object = notification[ "target/data" ].GetUserData< MIL_UrbanObject_ABC >();
            const PHY_PerceptionLevel& level = PHY_PerceptionLevel::FindPerceptionLevel( notification[ "level" ] );
            perceptions.NotifyPerception( object, level );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::ExecutePerceptions
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void RolePion_Perceiver::ExecutePerceptions()
{
    DEC_KS_Perception& perceptions = owner_->GetKnowledge().GetKsPerception();
    const core::Model& notifications = (*entity_)[ "perceptions/notifications" ];
    NotifyAgentPerception( notifications[ "agents" ], perceptions );
    NotifyAgentPerception( notifications[ "agents-in-zone" ], perceptions );
    NotifyObjectPerception( notifications[ "objects" ], perceptions );
    NotifyConcentrationPerception( notifications[ "population-concentrations" ], perceptions );
    NotifyFlowPerception( notifications[ "population-flows" ], perceptions );
    NotifyUrbanObjectPerception( notifications[ "urban-blocks" ], perceptions );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::ExecuteCollisions
// Created: LGY 2013-01-23
// -----------------------------------------------------------------------------
void RolePion_Perceiver::ExecuteCollisions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::ComputePerception
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& RolePion_Perceiver::ComputePerception( const DEC_Knowledge_Object& knowledge ) const // $$$$ _RC_ SLI 2012-10-02: only called by automat
{
    core::Model model;
    model[ "data" ].SetUserData( &knowledge );
    const unsigned int level = static_cast< unsigned int >( GET_HOOK( ComputeKnowledgeObjectPerception )( core::Convert( model_ ), core::Convert( entity_ ), core::Convert( &model ) ) );
    return PHY_PerceptionLevel::FindPerceptionLevel( level );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::ComputePerception
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& RolePion_Perceiver::ComputePerception( const MT_Vector2D& /*vPoint*/ ) const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::Update
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
void RolePion_Perceiver::Update( bool /*bIsDead*/ )
{
    // Debug - Cones de vision
    if( MIL_AgentServer::GetWorkspace().GetAgentServer().MustSendUnitVisionCones() || MIL_AgentServer::GetWorkspace().GetAgentServer().MustInitUnitVisionCones() )
        SendDebugState();
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::GetKnowledgeGroup
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_KnowledgeGroup > RolePion_Perceiver::GetKnowledgeGroup() const
{
    return owner_->GetKnowledgeGroup();
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::IsKnown
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::IsKnown( const MIL_Agent_ABC& agent ) const
{
    auto bbKg = owner_->GetKnowledgeGroup()->GetKnowledge();
    return bbKg ? bbKg->IsKnown( agent ) : false;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::IsIdentified
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::IsIdentified( const MIL_Agent_ABC& agent ) const
{
    return owner_->GetKnowledge().IsIdentified( agent );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::IsKnown
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::IsKnown( const MIL_Object_ABC& object ) const
{
    if( DEC_BlackBoard_CanContainKnowledgeObject* container = owner_->GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
        return container->HasKnowledgeObject( object );
    return false;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::IsIdentified
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::IsIdentified( const MIL_Object_ABC& object ) const
{
    return owner_->GetKnowledge().IsIdentified( object );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::IsIdentified
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::IsIdentified( const MIL_PopulationConcentration& concentration ) const
{
    return owner_->GetKnowledge().IsIdentified( concentration );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::IsIdentified
// Created: MGD 2009-11-25
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::IsIdentified( const MIL_UrbanObject_ABC& object ) const
{
    return owner_->GetKnowledge().IsIdentified( object );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyPerception
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
void RolePion_Perceiver::NotifyPerception( const MIL_Effect_IndirectFire& /*flyingShell*/ ) const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyPerception
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void RolePion_Perceiver::NotifyPerception( MIL_Agent_ABC& /*agent*/, const PHY_PerceptionLevel& /*level*/, bool /*bPerceptionRecorded*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyPerception
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::NotifyPerception( MIL_Agent_ABC& /*agent*/, const PHY_PerceptionLevel& /*level*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyPerception
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void RolePion_Perceiver::NotifyPerception( MIL_Object_ABC& /*object*/, const PHY_PerceptionLevel& /*level*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyPerception
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::NotifyPerception( MIL_PopulationConcentration& /*concentration*/, const PHY_PerceptionLevel& /*level*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyPerception
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::NotifyPerception( MIL_PopulationFlow& /*flow*/, const PHY_PerceptionLevel& /*level*/, const T_PointVector& /*shape*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyPerceptionUrban
// Created: MGD 2009-11-20
// -----------------------------------------------------------------------------
void RolePion_Perceiver::NotifyPerceptionUrban( const MIL_UrbanObject_ABC& /*object*/, const PHY_PerceptionLevel& /*level*/ ) const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifyPerception
// Created: LGY 2012-12-06
// -----------------------------------------------------------------------------
void RolePion_Perceiver::NotifyPerception( MIL_Object_ABC& /*object*/, const MT_Vector2D& /*vPosition*/, const MT_Vector2D& /*vDirection*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::NotifytExternalPerception
// Created: NLD 2005-03-23
// -----------------------------------------------------------------------------
void RolePion_Perceiver::NotifyExternalPerception( MIL_Agent_ABC& target, const PHY_PerceptionLevel& level )
{
    sink_->PostCommand( "external perception", core::MakeModel( "perceiver", owner_->GetID() )
                                                             ( "level", level.GetID() )
                                                             ( "target", target.GetID() ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableRecordMode
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
void RolePion_Perceiver::EnableRecordMode()
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableRecordMode
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableRecordMode()
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::HasDelayedPerceptions
// Created: JVT 2004-12-03
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::HasDelayedPerceptions() const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::SendDebugState
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
void RolePion_Perceiver::SendDebugState() const
{
    client::UnitVisionCones message;
    message().mutable_unit()->set_id( owner_->GetID() );
    std::auto_ptr< detection::PerceptionDistanceComputer_ABC > algorithm = owner_->GetAlgorithms().detectionComputerFactory_->CreateDistanceComputer();
    message().set_elongation( static_cast< float >( owner_->Execute( *algorithm ).GetElongationFactor() ) ); //@TODO MGD share
    message().mutable_cones();
    const core::Model& cones = (*entity_)[ "perceptions/cones" ];
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
    MT_Vector2D direction;
    GetMainPerceptionDirection( direction );
    NET_ASN_Tools::WriteDirection( direction, *msg().mutable_sensors_direction() );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void RolePion_Perceiver::SendChangedState( client::UnitAttributes& msg ) const
{
    SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::IsPeriphericalVisionEnabled
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::IsPeriphericalVisionEnabled() const
{
    return (*entity_)[ "perceptions/peripherical-vision/activated" ];
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::GetPion
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
MIL_Agent_ABC& RolePion_Perceiver::GetPion() const
{
    return *owner_;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::SetVisionModeNormal
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void RolePion_Perceiver::SetVisionModeNormal()
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::SetVisionModeDirection
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void RolePion_Perceiver::SetVisionModeDirection( const MT_Vector2D& /*vDirection*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::SetVisionModePoint
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void RolePion_Perceiver::SetVisionModePoint( const MT_Vector2D& /*vPoint*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::GetMainPerceptionDirection
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void RolePion_Perceiver::GetMainPerceptionDirection( MT_Vector2D& vDirection ) const
{
    vDirection.rX_ = (*entity_)[ "perceptions/main-perception-direction/x" ];
    vDirection.rY_ = (*entity_)[ "perceptions/main-perception-direction/y" ];
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::Clean
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void RolePion_Perceiver::Clean()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::GetSurfacesAgent
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
const RolePion_Perceiver::T_SurfaceAgentMap& RolePion_Perceiver::GetSurfacesAgent() const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::GetSurfacesObject
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
const RolePion_Perceiver::T_SurfaceObjectMap& RolePion_Perceiver::GetSurfacesObject() const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::GetDisasterDetectors
// Created: LGY 2012-12-06
// -----------------------------------------------------------------------------
const RolePion_Perceiver::T_DisasterDetectors& RolePion_Perceiver::GetDisasterDetectors() const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::GetRadars
// Created: NLD 2005-05-02
// -----------------------------------------------------------------------------
const RolePion_Perceiver::T_RadarMap& RolePion_Perceiver::GetRadars( const PHY_RadarClass& /*radarClass*/ )
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::Execute
// Created: MGD 2009-09-21
// -----------------------------------------------------------------------------
void RolePion_Perceiver::Execute( detection::DetectionComputer_ABC& algorithm ) const
{
    if( algorithm.GetTarget() != *owner_ && owner_->GetKnowledge().WasPerceived( algorithm.GetTarget() ) )
        algorithm.AlreadyPerceived();
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnableFireObserver
// Created: LGY 2012-06-28
// -----------------------------------------------------------------------------
void RolePion_Perceiver::EnableFireObserver()
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisableFireObserver
// Created: LGY 2012-06-28
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisableFireObserver()
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::IsFireObserver
// Created: LGY 2012-06-28
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::IsFireObserver() const
{
    return (*entity_)[ "perceptions/fire-observer/activated" ];
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::EnablePerceptionUponRequest
// Created: LGY 2012-12-12
// -----------------------------------------------------------------------------
void RolePion_Perceiver::EnablePerceptionUponRequest()
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::DisablePerceptionUponRequest
// Created: LGY 2012-12-12
// -----------------------------------------------------------------------------
void RolePion_Perceiver::DisablePerceptionUponRequest()
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::CanPerceive
// Created: MMC 2013-01-11
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::CanPerceive( const MIL_ObjectType_ABC& /*objectType*/ ) const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::GetPerception
// Created: LDC 2013-02-26
// -----------------------------------------------------------------------------
double RolePion_Perceiver::GetPerception( const MT_Vector2D & from, const MT_Vector2D& to ) const
{
    return GET_HOOK( GetPerception )( core::Convert( entity_ ), &from, &to );
}
