// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "RolePion_Perceiver.h"
#include "PerceptionView.h"
#include "PerceptionCoupDeSonde.h"
#include "PerceptionAlat.h"
#include "PerceptionRecoSurveillance.h"
#include "PerceptionLevel.h"
#include "SurfacesAgent_ABC.h"
#include "SurfacesAgentVisitor_ABC.h"
#include "SurfacesObject_ABC.h"
#include "SurfacesObjectVisitor_ABC.h"
#include "SensorTypeAgent.h"
#include "SensorType.h"
#include "SensorTypeObject.h"
#include "RadarType.h"
#include "RadarClass.h"
#include "PerceptionObserver.h"
#include "PerceptionSurfaceAgent.h"
#include "PerceptionSurfaceObject.h"
#include "PerceptionRecoObjects.h"
#include "PerceptionRecoPoint.h"
#include "PerceptionRecoLocalisation.h"
#include "PerceptionRecoUrbanBlock.h"
#include "PerceptionRadar.h"
#include "ListInCircleVisitor.h"
#include "wrapper/View.h"
#include "wrapper/Hook.h"
#include "wrapper/Event.h"
#include "wrapper/Effect.h"
#include <module_api/Log.h>
#include <geometry/Types.h>
#include <map>
#include <set>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

using namespace sword;
using namespace sword::perception;

class MIL_UrbanObject_ABC;

DECLARE_HOOK( CanComponentPerceive, bool, ( const SWORD_Model* entity, const SWORD_Model* component ) )
DECLARE_HOOK( ComputePerceptionDistanceFactor, double, ( const SWORD_Model* entity ) );
DECLARE_HOOK( GetAgentListWithinCircle, void, ( const SWORD_Model* root, const MT_Vector2D& vCenter, double rRadius, void (*callback)( const SWORD_Model* agent, void* userData ), void* userData ) )
DECLARE_HOOK( GetObjectListWithinCircle, void, ( const MT_Vector2D& vCenter, double rRadius, void (*callback)( MIL_Object_ABC* object, void* userData ), void* userData ) )
DECLARE_HOOK( GetConcentrationListWithinCircle, void, ( const MT_Vector2D& vCenter, double rRadius, void (*callback)( const MIL_PopulationConcentration* concentration, void* userData ), void* userData ) )
DECLARE_HOOK( GetFlowListWithinCircle, void, ( const MT_Vector2D& vCenter, double rRadius, void (*callback)( const MIL_PopulationFlow* flow, void* userData ), void* userData ) )
DECLARE_HOOK( GetUrbanObjectListWithinCircle, void, ( const MT_Vector2D& center, float radius, void (*callback)( const MIL_UrbanObject_ABC* urbanObjectWrapper, void* userData ), void* userData ) )
DECLARE_HOOK( AppendAddedKnowledge, void, ( const SWORD_Model* root, const SWORD_Model* entity,
                                            void (*agentCallback)( const SWORD_Model* agent, void* userData ),
                                            void (*objectCallback)( MIL_Object_ABC* object, void* userData ),
                                            void (*concentrationCallback)( const MIL_PopulationConcentration* concentration, void* userData ),
                                            void (*flowCallback)( const MIL_PopulationFlow* flow, void* userData ),
                                            void* userData ) )
DECLARE_HOOK( GetUrbanObjectOccupation, double, ( const MIL_UrbanObject_ABC* urbanObject ) )
DECLARE_HOOK( GetTransporter, const SWORD_Model*, ( const SWORD_Model* model, const SWORD_Model* agent ) )

namespace
{
    DEFINE_HOOK( IsPointVisible, bool, ( const SWORD_Model* model, const SWORD_Model* entity, const MT_Vector2D* point ) )
    {
        bool result = false;
        try
        {
            RolePion_Perceiver perceiver;
            const PerceptionLevel& level = perceiver.ComputePerception( model, entity, *point );
            result = level != PerceptionLevel::notSeen_;
        }
        catch( std::exception& e )
        {
            ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, e.what() );
        }
        catch( ... )
        {
            ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Unknown exception in IsPointVisible hook" );
        }
        if( GET_PREVIOUS_HOOK( IsPointVisible ) )
            result = result || GET_PREVIOUS_HOOK( IsPointVisible )( model, entity, point );
        return result;
    }
    DEFINE_HOOK( ComputeKnowledgeObjectPerception, size_t, ( const SWORD_Model* model, const SWORD_Model* entity, const DEC_Knowledge_Object* object ) )
    {
        size_t level = PerceptionLevel::notSeen_.GetID();
        try
        {
            RolePion_Perceiver perceiver;
            level = perceiver.ComputePerception( model, entity, *object ).GetID();
        }
        catch( std::exception& e )
        {
            ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, e.what() );
        }
        catch( ... )
        {
            ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Unknown exception in ComputeKnowledgeObjectPerception hook" );
        }
        if( GET_PREVIOUS_HOOK( ComputeKnowledgeObjectPerception ) )
            level = std::max( level, GET_PREVIOUS_HOOK( ComputeKnowledgeObjectPerception )( model, entity, object ) );
        return level;
    }
    DEFINE_HOOK( AgentHasRadar, bool, ( const SWORD_Model* entity, size_t radarType ) )
    {
        bool result = false;
        try
        {
            RolePion_Perceiver perceiver;
            result = perceiver.HasRadar( entity, radarType );
        }
        catch( std::exception& e )
        {
            ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, e.what() );
        }
        catch( ... )
        {
            ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Unknown exception in AgentHasRadar hook" );
        }
        if( GET_PREVIOUS_HOOK( AgentHasRadar ) )
            result = result || GET_PREVIOUS_HOOK( AgentHasRadar )( entity, radarType );
        return result;
    }
    DEFINE_HOOK( GetPerception, double, ( const SWORD_Model* entity, const MT_Vector2D* point, const MT_Vector2D* target ) )
    {
        if( !target || !point )
            return 0.;
        double energy = 0.;
        try
        {
            const sword::wrapper::View view( entity );
            for( std::size_t i = 0; i < view[ "components" ].GetSize(); ++i )
            {
                const wrapper::View& component = view[ "components" ].GetElement( i );
                if( !GET_HOOK( CanComponentPerceive )( entity, component ) )
                    continue;
                for( std::size_t j = 0; j < component[ "sensors" ].GetSize(); ++j )
                {
                    const wrapper::View& sensor = component[ "sensors" ].GetElement( j );
                    const SensorType* type = SensorType::FindSensorType( static_cast< std::string >( sensor[ "type" ] ) );
                    if( !type )
                        throw std::runtime_error( "unknown sensor type " + static_cast< std::string >( sensor[ "type" ] ) );
                    const SensorTypeAgent* pSensorTypeAgent = type->GetTypeAgent();
                    const double height = sensor[ "height" ];
                    if( pSensorTypeAgent )
                        energy = std::max( energy, pSensorTypeAgent->RayTrace( *point, *target, height ) );
                }
            }
        }
        catch( std::exception& e )
        {
            ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, e.what() );
        }
        catch( ... )
        {
            ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Unknown exception in GetPerception hook" );
        }
        if( GET_PREVIOUS_HOOK( GetPerception ) )
            energy = std::max( energy, GET_PREVIOUS_HOOK( GetPerception )( entity, point, target ) );
        return energy;
    }
}

// =============================================================================
// UPDATE
// =============================================================================
namespace
{
    const unsigned int nNbrStepsBetweenPeriphericalVision = 12; //$$$ En dur ...

    typedef std::pair< const SensorTypeAgent* , double /*height*/ >   T_SurfaceAgentKeyPair;
    typedef std::map< T_SurfaceAgentKeyPair, PerceptionSurfaceAgent > T_SurfaceAgentMap;
    typedef T_SurfaceAgentMap::const_iterator                         CIT_SurfaceAgentMap;

    typedef std::pair< const SensorTypeObject* , double /*height*/ >    T_SurfaceObjectKeyPair;
    typedef std::map< T_SurfaceObjectKeyPair, PerceptionSurfaceObject > T_SurfaceObjectMap;
    typedef T_SurfaceObjectMap::const_iterator                          CIT_SurfaceObjectMap;

    // -----------------------------------------------------------------------------
    // Name: RolePion_Perceiver::CanPerceive
    // Created: NLD 2004-08-20
    // -----------------------------------------------------------------------------
    bool CanPerceive( const wrapper::View& entity ) 
    {
        return !entity[ "is-dead" ] &&
               !entity[ "is-underground" ] &&
               !( entity[ "is-transported" ] || entity[ "is-prisoner" ] );
    }

    void UpdatePerceptionDataSensors( const wrapper::View& sensors, T_SurfaceAgentMap& surfacesAgent, T_SurfaceObjectMap& surfacesObject, const MT_Vector2D& vOrigin, const MT_Vector2D& vDirection, double& rMaxAgentPerceptionDistance )
    {
        for( std::size_t i = 0; i < sensors.GetSize(); ++i )
        {
            const wrapper::View& sensor = sensors.GetElement( i );
            const SensorType* type = SensorType::FindSensorType( static_cast< std::string >( sensor[ "type" ] ) );
            if( !type )
                throw std::runtime_error( "unknown sensor type " + static_cast< std::string >( sensor[ "type" ] ) );
            const SensorTypeAgent* pSensorTypeAgent = type->GetTypeAgent();
            const double height = sensor[ "height" ];
            if( pSensorTypeAgent )
            {
                rMaxAgentPerceptionDistance = std::max( rMaxAgentPerceptionDistance, pSensorTypeAgent->GetMaxDistance() );
                PerceptionSurfaceAgent& surface = surfacesAgent[ std::make_pair( pSensorTypeAgent, height ) ];
                if( !surface.IsInitialized() )
                    surface = PerceptionSurfaceAgent( *pSensorTypeAgent, vOrigin, height );
                surface.AddDirection( vDirection );
            }
            const SensorTypeObject* pSensorTypeObject = type->GetTypeObject();
            if( pSensorTypeObject )
            {
                PerceptionSurfaceObject& surface = surfacesObject[ std::make_pair( pSensorTypeObject, height ) ];
                if( !surface.IsInitialized() )
                    surface = PerceptionSurfaceObject( *pSensorTypeObject, vOrigin, height );
            }
        }
    }
    void UpdatePerceptionDataComposantes( const wrapper::View& entity, T_SurfaceAgentMap& surfacesAgent, T_SurfaceObjectMap& surfacesObject,
                                          const MT_Vector2D& vMainPerceptionDirection, double rDirectionRotation,
                                          double& rMaxAgentPerceptionDistance )
        {
            const MT_Vector2D position( entity[ "movement/position/x" ], entity[ "movement/position/y" ] );
            const MT_Vector2D direction( entity[ "movement/direction/x" ], entity[ "movement/direction/y" ] );
            int nRotationIdx = vMainPerceptionDirection != direction ? -1 : 0; // détection lockée
            for( std::size_t i = 0; i < entity[ "components" ].GetSize(); ++i )
            {
                const wrapper::View& component = entity[ "components" ].GetElement( i );
                if( !GET_HOOK( CanComponentPerceive )( entity, component ) )
                    continue;
                MT_Vector2D vComposantePerceptionDirection( vMainPerceptionDirection );
                if( nRotationIdx == 0 )
                    vComposantePerceptionDirection = direction;
                else if( nRotationIdx > 0 )
                    vComposantePerceptionDirection.Rotate( ( ( 2 * ( nRotationIdx & 0x1 ) - 1 ) * ( ( nRotationIdx + 1 ) >> 1 ) ) * rDirectionRotation );
                ++nRotationIdx;
                UpdatePerceptionDataSensors( component[ "sensors" ], surfacesAgent, surfacesObject, position, vComposantePerceptionDirection, rMaxAgentPerceptionDistance );
            }
    }
    double ComputePerceptionRotation( const wrapper::View& entity )
    {
        double rAngle = std::numeric_limits< double >::max();
        for( std::size_t i = 0; i < entity[ "components" ].GetSize(); ++i )
        {
            const wrapper::View& component = entity[ "components" ].GetElement( i );
            rAngle = std::min< double >( rAngle, component[ "type/sensor-rotation-angle" ] );
        }
        return rAngle == std::numeric_limits< double >::max() ? 0. : rAngle;
    }
    MT_Vector2D ToVector( const wrapper::View& vector )
    {
        return MT_Vector2D( vector[ "x" ], vector[ "y" ] );
    }
    void NotifyCones( const wrapper::View& entity, const T_SurfaceAgentMap& surfacesAgent )
    {
        wrapper::Effect effect( entity[ "perceptions/cones" ] );
        BOOST_FOREACH( const T_SurfaceAgentMap::value_type& surface, surfacesAgent )
            surface.second.NotifyCone( effect );
        effect.Post();
    }

    // -----------------------------------------------------------------------------
    // Name: RolePion_Perceiver::ComputeMainPerceptionDirection
    // Created: NLD 2004-09-07
    // -----------------------------------------------------------------------------
    void ComputeMainPerceptionDirection( const wrapper::View& entity, MT_Vector2D& vMainPerceptionDirection )
    {
        const std::string mode = entity[ "perceptions/vision/mode" ];
        if( mode == "normal" )
            vMainPerceptionDirection = ToVector( entity[ "movement/direction" ] );
        else if( mode == "direction" )
            vMainPerceptionDirection = ToVector( entity[ "perceptions/vision/location" ] );
        else if( mode == "location" )
        {
            const MT_Vector2D vSensorInfo = ToVector( entity[ "perceptions/vision/location" ] );
            const MT_Vector2D vDirection = ToVector( entity[ "movement/direction" ] );
            const MT_Vector2D vPosition = ToVector( entity[ "movement/position" ] );
            if( vSensorInfo != vPosition )
                vMainPerceptionDirection = ( vSensorInfo - vPosition ).Normalize();
            else
                vMainPerceptionDirection = vDirection;
        }
    }

    // -----------------------------------------------------------------------------
    // Name: RolePion_Perceiver::PreparePerceptionData
    // Created: NLD 2004-08-20
    // -----------------------------------------------------------------------------
    void PreparePerceptionData( const wrapper::View& entity, T_SurfaceAgentMap& surfacesAgent, T_SurfaceObjectMap& surfacesObject, double& rMaxAgentPerceptionDistance, MT_Vector2D& vMainPerceptionDirection )
    {
        ComputeMainPerceptionDirection( entity, vMainPerceptionDirection );
        const double rotation = ComputePerceptionRotation( entity );
        UpdatePerceptionDataComposantes( entity, surfacesAgent, surfacesObject, vMainPerceptionDirection, rotation, rMaxAgentPerceptionDistance );
    }

    const float maxBlockPerceptionDistance = 100.f; // Distance under which we consider urban blocks for perception.

    struct UrbanObjectVisitor : private boost::noncopyable
    {
        UrbanObjectVisitor( PerceptionObserver_ABC& perceiver, double maxPerceptionDistance )
            : perceiver_            ( perceiver )
            , maxPerceptionDistance_( maxPerceptionDistance )
            , occupation_           ( 0 )
            , count_                ( 0 )
        {}
        static void NotifyUrbanObject( const MIL_UrbanObject_ABC* urbanObjectWrapper, void* userData )
        {
            static_cast< UrbanObjectVisitor* >( userData )->NotifyPerception( urbanObjectWrapper );
        }
        void NotifyPerception( const MIL_UrbanObject_ABC* urbanObjectWrapper )
        {
            count_++;
            perceiver_.NotifyPerceptionUrban( urbanObjectWrapper, sword::perception::PerceptionLevel::identified_ );
            occupation_ += GET_HOOK( GetUrbanObjectOccupation )( urbanObjectWrapper );
        }
        double ComputeMaxPerceptionDistance() const
        {
            if( !count_ )
                return maxPerceptionDistance_;
            const double occupation = occupation_ / count_;
            return maxPerceptionDistance_ * ( 1 - 9 * occupation / 10 );
        }
        PerceptionObserver_ABC& perceiver_;
        double maxPerceptionDistance_;
        double occupation_;
        unsigned int count_;
    };
    struct AddedKnowledgesVisitor : private boost::noncopyable
    {
        AddedKnowledgesVisitor( Perception_ABC::T_AgentPtrVector& perceivableAgents, Perception_ABC::T_ObjectVector& perceivableObjects,
                                Perception_ABC::T_ConstPopulationConcentrationVector& perceivableConcentrations, Perception_ABC::T_ConstPopulationFlowVector& perceivableFlows )
            : perceivableAgents_        ( perceivableAgents )
            , perceivableObjects_       ( perceivableObjects )
            , perceivableConcentrations_( perceivableConcentrations )
            , perceivableFlows_         ( perceivableFlows )
        {}
        static void AddAgent( const SWORD_Model* agent, void* userData )
        {
            static_cast< AddedKnowledgesVisitor* >( userData )->perceivableAgents_.push_back( agent );
        }
        static void AddObject( MIL_Object_ABC* object, void* userData )
        {
            static_cast< AddedKnowledgesVisitor* >( userData )->perceivableObjects_.push_back( object );
        }
        static void AddPopulationConcentration( const MIL_PopulationConcentration* concentation, void* userData )
        {
            static_cast< AddedKnowledgesVisitor* >( userData )->perceivableConcentrations_.push_back( concentation );
        }
        static void AddPopulationFlow( const MIL_PopulationFlow* flow, void* userData )
        {
            static_cast< AddedKnowledgesVisitor* >( userData )->perceivableFlows_.push_back( flow );
        }
        Perception_ABC::T_AgentPtrVector& perceivableAgents_;
        Perception_ABC::T_ObjectVector& perceivableObjects_;
        Perception_ABC::T_ConstPopulationConcentrationVector& perceivableConcentrations_;
        Perception_ABC::T_ConstPopulationFlowVector& perceivableFlows_;
    };
    template< typename Visitable, typename Visitor, typename T >
    struct Surfaces : public Visitable
    {
        explicit Surfaces( const T& surfaces ) : surfaces_( surfaces ){}
        virtual void Apply( Visitor& visitor ) const
        {
            BOOST_FOREACH( const T::value_type& surface, surfaces_ )
                if( visitor.Notify( surface.second ) )
                    return;
        }
        const T& surfaces_;
    };
    typedef Surfaces< SurfacesAgent_ABC, SurfacesAgentVisitor_ABC, T_SurfaceAgentMap > T_SurfacesAgents;
    typedef Surfaces< SurfacesObject_ABC, SurfacesObjectVisitor_ABC, T_SurfaceObjectMap > T_SurfacesObjects;

    // -----------------------------------------------------------------------------
    // Name: RolePion_Perceiver::UpdatePeriphericalVisionState
    // Created: NLD 2004-09-16
    // -----------------------------------------------------------------------------
    void UpdatePeriphericalVisionState( const wrapper::View& model, const wrapper::View& entity )
    {
        const unsigned int nCurrentTime = model[ "tick" ];
        const wrapper::View& periphericalVision = entity[ "perceptions/peripherical-vision" ];
        unsigned int nNextPeriphericalVisionStep = periphericalVision[ "next-tick" ];
        const bool activation = nNextPeriphericalVisionStep <= nCurrentTime;
        wrapper::Effect effect( periphericalVision );
        effect[ "activated" ] = activation;
        if( !activation )
        {
            effect.Post();
            return;
        }
        while( nNextPeriphericalVisionStep <= nCurrentTime )
            nNextPeriphericalVisionStep += nNbrStepsBetweenPeriphericalVision;
        effect[ "next-tick" ] = nNextPeriphericalVisionStep;
        effect.Post();
    }

    // -----------------------------------------------------------------------------
    // Name: RolePion_Perceiver::Update
    // Created: NLD 2004-08-30
    // -----------------------------------------------------------------------------
    void Update( const wrapper::View& model, const wrapper::View& entity, T_SurfaceAgentMap& surfacesAgent, T_SurfaceObjectMap& surfacesObject )
    {
        double rMaxAgentPerceptionDistance = 0;
        MT_Vector2D vMainPerceptionDirection;
        UpdatePeriphericalVisionState( model, entity );
        PreparePerceptionData        ( entity, surfacesAgent, surfacesObject, rMaxAgentPerceptionDistance, vMainPerceptionDirection );
        {
            wrapper::Effect effect( entity[ "perceptions/max-theoretical-agent-perception-distance" ] );
            effect = rMaxAgentPerceptionDistance;
            effect.Post();
        }
        {
            wrapper::Effect effect( entity[ "perceptions/max-agent-perception-distance" ] );
            effect = rMaxAgentPerceptionDistance * GET_HOOK( ComputePerceptionDistanceFactor )( entity );
            effect.Post();
        }
        {
            wrapper::Effect effect( entity[ "perceptions/main-perception-direction" ] );
            effect[ "x" ] = vMainPerceptionDirection.rX_;
            effect[ "y" ] = vMainPerceptionDirection.rY_;
            effect.Post();
        }
    }

    typedef boost::shared_ptr< Perception_ABC > T_Perception;
    typedef std::vector< T_Perception > T_PerceptionVector;

    template< typename Type >
    void AddActivePerception( const std::string& sensor, T_PerceptionVector& activeperceptions, const wrapper::View& model, const wrapper::View& entity, PerceptionObserver_ABC& observer )
    {
        if( entity[ "perceptions" ][ sensor ][ "activated" ] )
            activeperceptions.push_back( boost::shared_ptr< Type >( new Type( model, entity, observer ) ) );
    }
    template< typename Type >
    void AddActiveListPerception( const std::string& sensor, T_PerceptionVector& activeperceptions, const wrapper::View& model, const wrapper::View& entity, PerceptionObserver_ABC& observer )
    {
        bool activated = false;
        entity[ "perceptions" ][ sensor ].VisitIntegerChildren( boost::lambda::var( activated ) = true );
        if( activated )
            activeperceptions.push_back( boost::shared_ptr< Type >( new Type( model, entity, observer ) ) );
    }
    T_PerceptionVector CreateActivePerceptions( const wrapper::View& model, const wrapper::View& entity, PerceptionObserver_ABC& observer )
    {
        T_PerceptionVector activePerceptions;
        AddActivePerception< PerceptionView >( "sensor", activePerceptions, model, entity, observer );
        AddActivePerception< PerceptionCoupDeSonde >( "scan", activePerceptions, model, entity, observer );
        AddActiveListPerception< PerceptionAlat >( "alat/reco", activePerceptions, model, entity, observer );
        AddActiveListPerception< PerceptionRecoSurveillance >( "alat/monitoring", activePerceptions, model, entity, observer );
        AddActiveListPerception< PerceptionRecoObjects >( "object-detection", activePerceptions, model, entity, observer );
        AddActiveListPerception< PerceptionRecoPoint >( "recognition-point", activePerceptions, model, entity, observer );
        AddActiveListPerception< PerceptionRecoLocalisation >( "reco", activePerceptions, model, entity, observer );
        AddActiveListPerception< PerceptionRecoUrbanBlock >( "urban", activePerceptions, model, entity, observer );
        activePerceptions.push_back( boost::shared_ptr< PerceptionRadar >( new PerceptionRadar( entity, observer ) ) );
        return activePerceptions;
    }
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::ExecutePerceptions
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void RolePion_Perceiver::ExecutePerceptions( const wrapper::View& model, const wrapper::View& entity ) const
{
    PerceptionObserver observer( entity );
    T_SurfaceAgentMap surfacesAgent;
    T_SurfaceObjectMap surfacesObject;

    Update( model, entity, surfacesAgent, surfacesObject );
    NotifyCones( entity, surfacesAgent );

    T_PerceptionVector activePerceptions = CreateActivePerceptions( model, entity, observer );

    if( CanPerceive( entity ) )
    {
        UrbanObjectVisitor urbanVisitor( observer, entity[ "perceptions/max-agent-perception-distance" ] );

        const MT_Vector2D position( entity[ "movement/position/x" ], entity[ "movement/position/y" ] );
        GET_HOOK( GetUrbanObjectListWithinCircle )( position, maxBlockPerceptionDistance, &UrbanObjectVisitor::NotifyUrbanObject, &urbanVisitor );
        const double maxPerceptionDistance = urbanVisitor.ComputeMaxPerceptionDistance();

        Perception_ABC::T_AgentPtrVector perceivableAgents;
        Perception_ABC::T_ObjectVector perceivableObjects;
        Perception_ABC::T_ConstPopulationConcentrationVector perceivableConcentrations;
        Perception_ABC::T_ConstPopulationFlowVector perceivableFlows;

        AddedKnowledgesVisitor knowledgesVisitor( perceivableAgents, perceivableObjects, perceivableConcentrations, perceivableFlows );
        GET_HOOK( AppendAddedKnowledge )( model, entity,
                                          &AddedKnowledgesVisitor::AddAgent, &AddedKnowledgesVisitor::AddObject,
                                          &AddedKnowledgesVisitor::AddPopulationConcentration, &AddedKnowledgesVisitor::AddPopulationFlow,
                                          &knowledgesVisitor );

        const T_SurfacesAgents surfacesAgent( surfacesAgent );
        ListInCircleVisitor< wrapper::View > agentVisitor( perceivableAgents );
        GET_HOOK( GetAgentListWithinCircle )( model, position, maxPerceptionDistance, &ListInCircleVisitor< const SWORD_Model* >::Add, &agentVisitor );
        BOOST_FOREACH( T_Perception activePerception, activePerceptions )
            activePerception->Execute( model, entity, surfacesAgent, perceivableAgents );

        const T_SurfacesObjects surfacesObject( surfacesObject );
        ListInCircleVisitor< MIL_Object_ABC* > objectVisitor( perceivableObjects );
        GET_HOOK( GetObjectListWithinCircle )( position, maxPerceptionDistance, &ListInCircleVisitor< MIL_Object_ABC* >::Add, &objectVisitor );
        BOOST_FOREACH( T_Perception activePerception, activePerceptions )
            activePerception->Execute( entity, surfacesObject, perceivableObjects );

        ListInCircleVisitor< const MIL_PopulationConcentration* > concentrationVisitor( perceivableConcentrations );
        GET_HOOK( GetConcentrationListWithinCircle )( position, maxPerceptionDistance, &ListInCircleVisitor< const MIL_PopulationConcentration* >::Add, &concentrationVisitor );
        BOOST_FOREACH( T_Perception activePerception, activePerceptions )
            activePerception->Execute( entity, surfacesAgent, perceivableConcentrations );

        ListInCircleVisitor< const MIL_PopulationFlow* > flowVisitor( perceivableFlows );
        GET_HOOK( GetFlowListWithinCircle )( position, maxPerceptionDistance, &ListInCircleVisitor< const MIL_PopulationFlow* >::Add, &flowVisitor );
        BOOST_FOREACH( T_Perception activePerception, activePerceptions )
            activePerception->Execute( entity, surfacesAgent, perceivableFlows );

        BOOST_FOREACH( T_Perception activePerception, activePerceptions )
            activePerception->FinalizePerception( entity, surfacesAgent );
    }
    observer.NotifyPerception( entity, PerceptionLevel::identified_, false );
    const SWORD_Model* transporter = GET_HOOK( GetTransporter )( model, entity );
    if( transporter )
        observer.NotifyPerception( transporter, PerceptionLevel::identified_, false );
}

namespace
{
    struct NullObserver : public PerceptionObserver_ABC
    {
        virtual void NotifyPerceptionUrban( const MIL_UrbanObject_ABC*, const PerceptionLevel& ) {}
        virtual void NotifyPerception( const wrapper::View&, const PerceptionLevel& ) {}
        virtual void NotifyPerception( const wrapper::View&, const PerceptionLevel&, bool ) {}
        virtual void NotifyPerception( const MIL_Object_ABC*, const PerceptionLevel& ) {}
        virtual void NotifyPerception( const MIL_PopulationFlow*, const PerceptionLevel&, const std::vector< MT_Vector2D >& ) {}
        virtual void NotifyPerception( const MIL_PopulationConcentration*, const PerceptionLevel& ) {}
    };
    template< typename Result, typename Map, typename Drop >
    boost::shared_ptr< Result > CreateSurfaces( const Map& surfaces, const Drop& )
    {
        return boost::shared_ptr< Result >( new Result( surfaces ) );
    }
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::ComputePerception
// Created: SLI 2012-07-10
// -----------------------------------------------------------------------------
template< typename Target, typename Constructor >
const PerceptionLevel& RolePion_Perceiver::ComputePerception( const wrapper::View& model, const wrapper::View& entity, const Target& target, Constructor surfaceConstructor ) const
{
    NullObserver observer;
    T_SurfaceAgentMap surfacesAgent;
    T_SurfaceObjectMap surfacesObject;
    double maxDistance;
    MT_Vector2D vMainPerceptionDirection;
    PreparePerceptionData( entity, surfacesAgent, surfacesObject, maxDistance, vMainPerceptionDirection );
    T_PerceptionVector activePerceptions = CreateActivePerceptions( model, entity, observer );
    if( !CanPerceive( entity ) )
        return PerceptionLevel::notSeen_;
    Constructor::result_type surfaces = surfaceConstructor( surfacesAgent, surfacesObject );
    const PerceptionLevel* pBestPerceptionLevel = &PerceptionLevel::notSeen_;
    BOOST_FOREACH( T_Perception activePerception, activePerceptions )
    {
        pBestPerceptionLevel = &activePerception->Compute( entity, *surfaces, target );
        if( pBestPerceptionLevel->IsBestLevel() )
            return *pBestPerceptionLevel;
    }
    return *pBestPerceptionLevel;
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::ComputePerception
// Created: SLI 2012-06-21
// -----------------------------------------------------------------------------
const PerceptionLevel& RolePion_Perceiver::ComputePerception( const wrapper::View& model, const wrapper::View& entity, const MT_Vector2D& vPoint ) const
{
    return ComputePerception( model, entity, vPoint, boost::bind( &CreateSurfaces< T_SurfacesAgents, T_SurfaceAgentMap, T_SurfaceObjectMap >, _1, _2 ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::ComputePerception
// Created: SLI 2012-07-10
// -----------------------------------------------------------------------------
const PerceptionLevel& RolePion_Perceiver::ComputePerception( const wrapper::View& model, const wrapper::View& entity, const DEC_Knowledge_Object& object ) const
{
    return ComputePerception( model, entity, object, boost::bind( &CreateSurfaces< T_SurfacesObjects, T_SurfaceObjectMap, T_SurfaceAgentMap >, _2, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Perceiver::HasRadar
// Created: SLI 2012-07-06
// -----------------------------------------------------------------------------
bool RolePion_Perceiver::HasRadar( const wrapper::View& entity, size_t radarType ) const
{
    NullObserver observer;
    PerceptionRadar perceptionRadar( entity, observer );
    return perceptionRadar.HasRadar( entity, radarType );
}
