// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PerceptionHooks.h"
#include "Hook.h"
#include "Helpers.h"
#include "RolePion_Perceiver.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Meteo/RawVisionData/PHY_RawVisionDataIterator.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Objects/StructuralCapacity.h"
#include "Entities/Orders/MIL_AutomateOrderManager.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeAgent_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Population/PHY_RoleInterface_Population.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Roles/Transported/PHY_RoleInterface_Transported.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Effects/MIL_Effect_IndirectFire.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "DetectionComputer_ABC.h"
#include "DetectionComputerFactory_ABC.h"
#include "PerceptionDistanceComputer_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "AlgorithmsFactories.h"
#include "MIL_Random.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Urban.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeUrban.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeAgentPerception.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgePopulationPerception.h"
#include "Tools/MIL_Geometry.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Urban/UrbanPhysicalCapacity.h"
#include "MIL_AgentServer.h"
#include "OnComponentFunctor_ABC.h"
#include "OnComponentFunctorComputer_ABC.h"
#include "OnComponentFunctorComputerFactory_ABC.h"
#include "Urban/MIL_UrbanCache.h"
#include "Urban/PHY_MaterialCompositionType.h"
#include "simulation_terrain/TER_AgentManager.h"
#include "simulation_terrain/TER_PopulationManager.h"
#include "simulation_terrain/TER_PopulationConcentrationManager.h"
#include "simulation_terrain/TER_PopulationFlowManager.h"
#include "meteo/PHY_Lighting.h"
#include "meteo/PHY_Precipitation.h"
#include "MT_Tools/MT_Vector2D.h"
#include "Tools/MIL_Tools.h"
#include <core/Facade.h>
#include <core/UserData.h>
#include <core/Convert.h>
#include <module_api/Hook.h>
#include <module_api/Model.h>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <vector>

using namespace sword;
using namespace dotation;
using namespace transport;

namespace
{
    const double epsilon = 1e-8;

    double ComputeEnvironmentFactor( ElevationGrid::envBits nEnv, double(*FindEnvironmentFactor)( unsigned int environment, const void* userData ), const void* userData )
    {
        double res = nEnv & PHY_RawVisionData::eVisionEmpty ? FindEnvironmentFactor( 0, userData ) : 1.;
        for( unsigned int mask = 1, idx = 1; idx < PHY_RawVisionData::eNbrVisionObjects; mask <<= 1, ++idx )
            if( mask & nEnv )
                res *= FindEnvironmentFactor( mask, userData );
        return res;
    }
    double ComputeExtinction( const PHY_RawVisionDataIterator& env, double rDistanceModificator, double rVisionNRJ, bool bIsAroundBU,
                              double /*maxDetectionDistance*/, const double* lightingFactors, const double* precipitationFactors,
                              double(*FindEnvironmentFactor)( unsigned int environment, const void* userData ), const void* userData )
    {
        rDistanceModificator *= lightingFactors[ env.GetLighting().GetID() ];
        rDistanceModificator *= precipitationFactors[ env.GetPrecipitation().GetID() ];
        if( !bIsAroundBU )
            rDistanceModificator *= ComputeEnvironmentFactor( env.GetCurrentEnv(), FindEnvironmentFactor, userData );
        return rDistanceModificator <= epsilon ? -1. : rVisionNRJ - env.Length() / rDistanceModificator ;
    }
    bool ComputeUrbanExtinction( const MT_Vector2D& vSource, const MT_Vector2D& vTarget, double& rVisionNRJ, const double* urbanBlockFactors )
    {
        bool bIsAroundBU = false;

        std::vector< const MIL_UrbanObject_ABC* > list;
        MIL_AgentServer::GetWorkspace().GetUrbanCache().GetUrbanBlocksWithinSegment( vSource, vTarget, list );

        if( !list.empty() )
        {
            for( std::vector< const MIL_UrbanObject_ABC* >::const_iterator it = list.begin(); it != list.end() && rVisionNRJ > 0; ++it )
            {
                const MIL_UrbanObject_ABC& object = **it;
                const UrbanPhysicalCapacity* pPhysical = object.Retrieve< UrbanPhysicalCapacity >();
                if( pPhysical == 0 )
                    continue;
                const PHY_MaterialCompositionType* materialCompositionType = PHY_MaterialCompositionType::Find( pPhysical->GetMaterial() );
                if( !materialCompositionType )
                    continue;

                const TER_Localisation& footPrint = object.GetLocalisation();

                TER_DistanceLess cmp ( vSource );
                T_PointSet intersectPoints( cmp );
                if( footPrint.IsInside( vSource ) || footPrint.IsInside( vTarget ) || footPrint.Intersect2D( MT_Line( vSource, vTarget ), intersectPoints ) )
                {
                    bIsAroundBU = true;
                    double intersectionDistance = 0;
                    if( intersectPoints.size() == 1 )
                    {
                        if( footPrint.IsInside( vSource ) )
                            intersectionDistance = vSource.Distance( *intersectPoints.begin() );
                        else if( footPrint.IsInside( vTarget ) )
                            intersectionDistance = vTarget.Distance( *intersectPoints.begin() );
                    }
                    else if( intersectPoints.empty() )
                        intersectionDistance = vSource.Distance( vTarget );
                    else
                        intersectionDistance = ( *intersectPoints.begin() ).Distance( *intersectPoints.rbegin() );

                    double rDistanceModificator = urbanBlockFactors[ materialCompositionType->GetId() ];
                    double occupationFactor = std::sqrt( pPhysical->GetOccupation() );
                    if( occupationFactor == 1. && rDistanceModificator <= epsilon )
                        rVisionNRJ = -1 ;
                    else
                    {
                        double referenceDistance = 200; // $$$$ LDC Hard coded 200m. reference distance
                        double distanceFactor = std::min( ( intersectionDistance / referenceDistance ) * occupationFactor * ( 1 - rDistanceModificator ), 1. );
                        rVisionNRJ -= rVisionNRJ * distanceFactor + intersectionDistance;
                    }
                }
            }
        }
        return bIsAroundBU;
    }

    DEFINE_HOOK( ComputeRayTrace, 11, double, ( const MT_Vector2D& source, double sourceAltitude, const MT_Vector2D& target, double targetAltitude,
                                                double maxDectectionDistance, double distanceMaxModificator,
                                                const double* urbanBlockFactors, const double* lightingFactors, const double* precipitationFactors,
                                                double(*FindEnvironmentFactor)( unsigned int environment, const void* userData ), const void* userData ) )
    {
        const MT_Vector3D vSource3D( source.rX_, source.rY_, sourceAltitude );
        const MT_Vector3D vTarget3D( target.rX_, target.rY_, targetAltitude );
        double rVisionNRJ = maxDectectionDistance;
        bool bIsAroundBU = ComputeUrbanExtinction( source, target, rVisionNRJ, urbanBlockFactors );
        PHY_RawVisionDataIterator it( vSource3D, vTarget3D );
        if( rVisionNRJ > 0 )
            rVisionNRJ = it.End() ? std::numeric_limits< double >::max() : ComputeExtinction( it, distanceMaxModificator, rVisionNRJ, bIsAroundBU, maxDectectionDistance, lightingFactors, precipitationFactors, FindEnvironmentFactor, userData );
        while ( rVisionNRJ > 0 && !(++it).End() )
            rVisionNRJ = ComputeExtinction( it, distanceMaxModificator, rVisionNRJ, bIsAroundBU, maxDectectionDistance, lightingFactors, precipitationFactors, FindEnvironmentFactor, userData );
        return rVisionNRJ;
    }
    DEFINE_HOOK( GetAltitude, 2, double, ( double x, double y ) )
    {
        return MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetAltitude( x, y );
    }
    DEFINE_HOOK( GetModifiedAltitude, 2, double, ( double x, double y ) )
    {
        return MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetAltitude( x, y, true );
    }
    DEFINE_HOOK( GetSignificantVolume, 2, const PHY_Volume*, ( const SWORD_Model* entity, const double* volumeFactors ) )
    {
        struct SensorTypeAgentAdapter : public PHY_SensorTypeAgent_ABC
        {
        public:
            explicit SensorTypeAgentAdapter( const double* volumeFactors )
                : volumeFactors_( volumeFactors )
            {}
            virtual double GetFactor( const PHY_Volume& volume ) const
            {
                return volumeFactors_[ volume.GetID() ];
            }
        private:
            const double* volumeFactors_;
        } adapter( volumeFactors );
        return GET_ROLE( entity, PHY_RoleInterface_Composantes ).GetSignificantVolume( adapter );
    }
    DEFINE_HOOK( GetVolumeIdentifierFromInstance, 1, size_t, ( const PHY_Volume* volume ) )
    {
        return volume->GetID();
    }
    DEFINE_HOOK( GetVolumeSize, 0, size_t, () )
    {
        return PHY_Volume::GetVolumes().size();
    }
    DEFINE_HOOK( PopulationElementIntersectWithCircle, 5, bool, ( const SWORD_Model* element, MT_Vector2D circleCenter, double radius, void(*AddShapePoint)( MT_Vector2D point, void* userData ), void* userData ) )
    {
        T_PointVector points;
        bool result = GET_DATA( element, MIL_PopulationElement_ABC ).Intersect2DWithCircle( circleCenter, radius, points );
        if( AddShapePoint )
            BOOST_FOREACH( const MT_Vector2D& point, points )
                AddShapePoint( point, userData );
        return result;
    }
    DEFINE_HOOK( GetUrbanBlockFactor, 2, double, ( const SWORD_Model* block, const double* urbanBlockFactors ) )
    {
        if( const UrbanPhysicalCapacity* pPhysical = GET_DATA( block, MIL_UrbanObject_ABC ).Retrieve< UrbanPhysicalCapacity >() )
            if( const PHY_MaterialCompositionType* materialCompositionType = PHY_MaterialCompositionType::Find( pPhysical->GetMaterial() ) )
                return urbanBlockFactors[ materialCompositionType->GetId() ];
        return 1.f;
    }
    DEFINE_HOOK( IsMaterialType, 1, bool, ( const char* material ) )
    {
        return PHY_MaterialCompositionType::Find( material ) != 0;
    }
    DEFINE_HOOK( GetPrecipitationSize, 0, size_t, () )
    {
        return weather::PHY_Precipitation::GetPrecipitations().size();
    }
    DEFINE_HOOK( GetLightingSize, 0, size_t, () )
    {
        return weather::PHY_Lighting::GetLightings().size();
    }
    DEFINE_HOOK( GetMaterialTypeSize, 0, size_t, () )
    {
        return PHY_MaterialCompositionType::Count();
    }
    DEFINE_HOOK( GetVolumeIdentifier, 2, bool, ( const char* type, size_t* identifier ) )
    {
        PHY_Volume::CIT_VolumeMap it = PHY_Volume::GetVolumes().find( type );
        if( it == PHY_Volume::GetVolumes().end() )
            return false;
        *identifier = it->second->GetID();
        return true;
    }
    DEFINE_HOOK( GetPrecipitationIdentifier, 2, bool, ( const char* type, size_t* identifier ) )
    {
        weather::PHY_Precipitation::CIT_PrecipitationMap it = weather::PHY_Precipitation::GetPrecipitations().find( type );
        if( it == weather::PHY_Precipitation::GetPrecipitations().end() )
            return false;
        *identifier = it->second->GetID();
        return true;
    }
    DEFINE_HOOK( GetLightingIdentifier, 2, bool, ( const char* type, size_t* identifier ) )
    {
        weather::PHY_Lighting::CIT_LightingMap it = weather::PHY_Lighting::GetLightings().find( type );
        if( it == weather::PHY_Lighting::GetLightings().end() )
            return false;
        *identifier = it->second->GetID();
        return true;
    }
    DEFINE_HOOK( GetKnowledgeObjectType, 1, size_t, ( const SWORD_Model* object ) )
    {
        return GET_DATA( object, DEC_Knowledge_Object ).GetType().GetID();
    }
    DEFINE_HOOK( GetPostureSize, 0, size_t, () )
    {
        return PHY_Posture::GetPostureCount();
    }
    DEFINE_HOOK( GetPostureIdentifier, 2, bool, ( const char* type, size_t* identifier ) )
    {
        const PHY_Posture* posture = PHY_Posture::FindPosture( type );
        if( ! posture )
            return false;
        *identifier = posture->GetID();
        return true;
    }
    DEFINE_HOOK( PostureCanModifyDetection, 1, bool, ( const char* type ) )
    {
        const PHY_Posture* posture = PHY_Posture::FindPosture( type );
        if( posture )
            return posture->CanModifyDetection();
        return false;
    }
    DEFINE_HOOK( GetLastPostureIdentifier, 1, size_t, ( const SWORD_Model* entity ) )
    {
        return GET_ROLE( entity, PHY_RoleInterface_Posture ).GetLastPosture().GetID();
    }
    DEFINE_HOOK( GetCurrentPostureIdentifier,1 , size_t, ( const SWORD_Model* entity ) )
    {
        return GET_ROLE( entity, PHY_RoleInterface_Posture ).GetCurrentPosture().GetID();
    }
    DEFINE_HOOK( GetPostureCompletionPercentage, 1, double, ( const SWORD_Model* entity ) )
    {
        return GET_ROLE( entity, PHY_RoleInterface_Posture ).GetPostureCompletionPercentage();
    }
    DEFINE_HOOK( ComputePerceptionDistanceFactor, 1, double, ( const SWORD_Model* entity ) )
    {
        MIL_Agent_ABC& tempSource = GET_PION( entity ); //@TODO MGD FIND A BETTER WAY
        std::auto_ptr< detection::PerceptionDistanceComputer_ABC > computer( tempSource.GetAlgorithms().detectionComputerFactory_->CreateDistanceComputer() );
        return tempSource.Execute( *computer ).GetFactor();
    }
    DEFINE_HOOK( GetCollidingPopulationDensity, 1, double, ( const SWORD_Model* entity ) )
    {
        return GET_ROLE( entity, PHY_RoleInterface_Population ).GetCollidingPopulationDensity();
    }
    DEFINE_HOOK( ObjectIntersectWithCircle, 3, bool, ( const SWORD_Model* object, const MT_Vector2D& center, double radius ) )
    {
        return GET_DATA( object, MIL_Object_ABC ).Intersect2DWithCircle( center, radius );
    }
    DEFINE_HOOK( KnowledgeObjectIntersectWithCircle, 3, bool, ( const SWORD_Model* knowledgeObject, const MT_Vector2D& center, double radius ) )
    {
        return GET_DATA( knowledgeObject, DEC_Knowledge_Object ).GetLocalisation().Intersect2DWithCircle( center, radius );
    }
    DEFINE_HOOK( GetEnvironmentAssociation, 1, size_t, ( const char* environment ) )
    {
        typedef std::map< std::string, PHY_RawVisionData::E_VisionObject > T_Association;
        static const T_Association environmentAssociation = boost::assign::map_list_of( "Sol"   , PHY_RawVisionData::eVisionGround )
                                                                                      ( "Vide"  , PHY_RawVisionData::eVisionEmpty )
                                                                                      ( "Foret" , PHY_RawVisionData::eVisionForest )
                                                                                      ( "Urbain", PHY_RawVisionData::eVisionUrban );
        auto it = environmentAssociation.find( environment );
        if( it == environmentAssociation.end() )
            return PHY_RawVisionData::eNbrVisionObjects;
        return it->second;
    }
    DEFINE_HOOK( GetPerceptionRandom, 0, double, () )
    {
        return MIL_Random::rand_ii( MIL_Random::ePerception );
    }
    DEFINE_HOOK( IsKnown, 2, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) )
    {
        auto bbKg = GET_PION( perceiver ).GetKnowledgeGroup()->GetKnowledge();
        return bbKg ? bbKg->IsKnown( GET_PION( target ) ) : false;
    }
    DEFINE_HOOK( ComputeAgentRatioInsidePerceptionPolygon, 4, double, ( const SWORD_Model* perceiver, const SWORD_Model* target, double distance, double roll ) )
    {
        const PHY_Posture& currentPerceiverPosture = GET_ROLE( perceiver, PHY_RoleInterface_Posture ).GetCurrentPosture();
        const MIL_UrbanObject_ABC* perceiverUrbanBlock = GET_ROLE( perceiver, PHY_RoleInterface_UrbanLocation ).GetCurrentUrbanBlock();
        TER_Polygon polygon;
        if( perceiverUrbanBlock && ( &currentPerceiverPosture == &PHY_Posture::poste_ || &currentPerceiverPosture == &PHY_Posture::posteAmenage_ ) )
            MIL_Geometry::Scale( polygon, perceiverUrbanBlock->GetLocalisation().GetPoints(), distance );
        else
        {
            const core::Model& position = (*core::Convert( perceiver ))[ "movement/position" ];
            const MT_Vector2D targetPosition( position[ "x" ], position[ "y" ] );
            T_PointVector vector;
            vector.push_back( MT_Vector2D( targetPosition.rX_ - distance, targetPosition.rY_ - distance ) ); // bottom left
            vector.push_back( MT_Vector2D( targetPosition.rX_ - distance, targetPosition.rY_ + distance ) ); // top left
            vector.push_back( MT_Vector2D( targetPosition.rX_ + distance, targetPosition.rY_ + distance ) ); // top right
            vector.push_back( MT_Vector2D( targetPosition.rX_ + distance, targetPosition.rY_ - distance ) ); // bottom right
            polygon.Reset( vector );
        }
        return GET_ROLE( target, PHY_RoleInterface_UrbanLocation ).ComputeRatioPionInside( polygon, roll );
    }
    DEFINE_HOOK( GetCurrentUrbanBlock, 2, const SWORD_Model*, ( const SWORD_Model* root, const SWORD_Model* entity ) )
    {
        const MIL_UrbanObject_ABC* block = GET_ROLE( entity, PHY_RoleInterface_UrbanLocation ).GetCurrentUrbanBlock();
        if( !block )
            return 0;
        return core::Convert( &(*core::Convert( root ))[ "urban-objects" ][ block->GetID() ] );
    }
    DEFINE_HOOK( GetUrbanObjectStructuralHeight, 1, double, ( const SWORD_Model* urbanObject ) )
    {
        const MIL_UrbanObject_ABC& object = GET_DATA( urbanObject, MIL_UrbanObject_ABC );
        if( const UrbanPhysicalCapacity* physical = object.Retrieve< UrbanPhysicalCapacity >() )
            if( const StructuralCapacity* structuralCapacity = object.Retrieve< StructuralCapacity >() )
                return structuralCapacity->GetStructuralState() * physical->GetHeight();
        return 0;
    }
    DEFINE_HOOK( GetUrbanObjectOccupation, 1, double, ( const SWORD_Model* urbanObject ) )
    {
        if( const UrbanPhysicalCapacity* physical = GET_DATA( urbanObject, MIL_UrbanObject_ABC ).Retrieve< UrbanPhysicalCapacity >() )
            return physical->GetOccupation();
        return 0;
    }
    DEFINE_HOOK( GetUrbanObjectLength, 1, double, ( const SWORD_Model* urbanObject ) )
    {
        return GET_DATA( urbanObject, MIL_UrbanObject_ABC ).GetLocalisation().GetLength();
    }
    DEFINE_HOOK( GetUrbanObjectStructuralState, 1, double, ( const SWORD_Model* urbanObject ) )
    {
        const StructuralCapacity* structuralCapacity = GET_DATA( urbanObject, MIL_UrbanObject_ABC ).Retrieve< StructuralCapacity >();
        return structuralCapacity ? structuralCapacity->GetStructuralState() : 1;
    }
    DEFINE_HOOK( HasUrbanObjectArchitecture, 1, bool, ( const SWORD_Model* urbanObject ) )
    {
        return GET_DATA( urbanObject, MIL_UrbanObject_ABC ).Retrieve< UrbanPhysicalCapacity >() != 0;
    }
    DEFINE_HOOK( CanUrbanBlockBeSeen, 2, bool, ( const SWORD_Model* perceiver, const SWORD_Model* urbanBlock ) )
    {
        boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge = GET_PION( perceiver ).GetArmy().GetKnowledge().GetKnowledgeUrbanContainer().GetKnowledgeUrban( GET_DATA( urbanBlock, MIL_UrbanObject_ABC ) );
        if( pKnowledge )
            return pKnowledge->GetCurrentRecceProgress() >= ( 1. - MIL_Random::rand_ii( MIL_Random::ePerception ) );
        return false;
    }
    DEFINE_HOOK( IsPostureStationed, 1, bool, ( const SWORD_Model* entity ) )
    {
        const PHY_Posture& currentPerceiverPosture = GET_ROLE( entity, PHY_RoleInterface_Posture ).GetCurrentPosture();
        return &currentPerceiverPosture == &PHY_Posture::poste_ || &currentPerceiverPosture == &PHY_Posture::posteAmenage_;
    }
    DEFINE_HOOK( AppendAddedKnowledge, 7, void, ( const SWORD_Model* root, const SWORD_Model* entity,
                                                  void (*agentCallback)( const SWORD_Model* agent, void* userData ),
                                                  void (*objectCallback)( const SWORD_Model* object, void* userData ),
                                                  void (*concentrationCallback)( const SWORD_Model* concentration, void* userData ),
                                                  void (*flowCallback)( const SWORD_Model* flow, void* userData ),
                                                  void* userData ) )
    {
        const core::Model& rootNode = *core::Convert( root );
        TER_Agent_ABC::T_AgentPtrVector perceivableAgents;
        TER_Object_ABC::T_ObjectVector perceivableObjects;
        TER_PopulationConcentration_ABC::T_ConstPopulationConcentrationVector perceivableConcentrations;
        TER_PopulationFlow_ABC::T_ConstPopulationFlowVector perceivableFlows;
        GET_PION( entity ).GetKnowledgeGroup()->AppendAddedKnowledge( perceivableAgents, perceivableObjects, perceivableConcentrations, perceivableFlows );
        BOOST_FOREACH( TER_Agent_ABC* agent, perceivableAgents )
            agentCallback( core::Convert( &rootNode[ "entities" ][ static_cast< const PHY_RoleInterface_Location* >( agent )->GetAgent().GetID() ] ), userData );
        BOOST_FOREACH( TER_Object_ABC* object, perceivableObjects )
            objectCallback( core::Convert( &rootNode[ "objects" ][ static_cast< MIL_Object_ABC* >( object )->GetID() ] ), userData );
        BOOST_FOREACH( const TER_PopulationConcentration_ABC* terConcentration, perceivableConcentrations )
        {
            const MIL_PopulationConcentration* concentration = static_cast< const MIL_PopulationConcentration* >( terConcentration );
            concentrationCallback( core::Convert( &rootNode[ "populations" ][ concentration->GetPopulation().GetID() ][ "elements" ][ concentration->GetID() ] ), userData );
        }
        BOOST_FOREACH( const TER_PopulationFlow_ABC* terFlow, perceivableFlows )
        {
            const MIL_PopulationFlow* flow = static_cast< const MIL_PopulationFlow* >( terFlow );
            flowCallback( core::Convert( &rootNode[ "populations" ][ flow->GetPopulation().GetID() ][ "elements" ][ flow->GetID() ] ), userData );
        }
    }
    DEFINE_HOOK( IsInCity, 1, bool, ( const SWORD_Model* entity ) )
    {
        return GET_ROLE( entity, PHY_RoleInterface_UrbanLocation ).IsInCity();
    }
    DEFINE_HOOK( BelongsToKnowledgeGroup, 2, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) )
    {
        return GET_PION( target ).BelongsTo( *( GET_PION( perceiver ).GetKnowledgeGroup() ) );
    }
    DEFINE_HOOK( IsAgentPerceptionDistanceHacked, 2, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) )
    {
        return GET_PION( perceiver ).GetKnowledgeGroup()->IsPerceptionDistanceHacked( GET_PION( target ) );
    }
    DEFINE_HOOK( IsObjectPerceptionDistanceHacked, 2, bool, ( const SWORD_Model* perceiver, const SWORD_Model* object ) )
    {
        return GET_PION( perceiver ).GetKnowledgeGroup()->IsPerceptionDistanceHacked( GET_DATA( object, MIL_Object_ABC ) );
    }
    DEFINE_HOOK( IsPopulationElementPerceptionDistanceHacked, 2, bool, ( const SWORD_Model* perceiver, const SWORD_Model* element ) )
    {
        return GET_PION( perceiver ).GetKnowledgeGroup()->IsPerceptionDistanceHacked( GET_DATA( element, MIL_PopulationElement_ABC ).GetPopulation() );
    }
    DEFINE_HOOK( GetHackedPerceptionLevel, 2, int, ( const SWORD_Model* perceiver, const SWORD_Model* target ) )
    {
        return GET_PION( perceiver ).GetKnowledgeGroup()->GetPerceptionLevel( GET_PION( target ) ).GetID();
    }
    DEFINE_HOOK( GetObjectPerceptionLevel, 2, int, ( const SWORD_Model* perceiver, const SWORD_Model* object ) )
    {
        return GET_PION( perceiver ).GetKnowledgeGroup()->GetPerceptionLevel( GET_DATA( object, MIL_Object_ABC ) ).GetID();
    }
    DEFINE_HOOK( GetPopulationElementPerceptionLevel, 2, int, ( const SWORD_Model* perceiver, const SWORD_Model* element ) )
    {
        return GET_PION( perceiver ).GetKnowledgeGroup()->GetPerceptionLevel( GET_DATA( element, MIL_PopulationElement_ABC ).GetPopulation() ).GetID();
    }
    DEFINE_HOOK( CanBeSeen, 2, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) )
    {
        std::auto_ptr< detection::DetectionComputer_ABC > detectionComputer( GET_PION( perceiver ).GetAlgorithms().detectionComputerFactory_->Create( GET_PION( target ) ) );
        GET_PION( perceiver ).Execute( *detectionComputer );
        GET_PION( target ).Execute( *detectionComputer );
        return detectionComputer->CanBeSeen();
    }
    DEFINE_HOOK( IsAgentNewlyPerceived, 3, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target, int level ) )
    {
        if( PHY_PerceptionLevel::FindPerceptionLevel( level ) == PHY_PerceptionLevel::notSeen_ )
            return false;
        return GET_PION( perceiver ).GetKnowledge().GetKnowledgeAgentPerceptionContainer().GetKnowledgeAgentPerception( GET_PION( target ) ) == 0;
    }
    DEFINE_HOOK( IsPopulationElementNewlyPerceived, 3, bool, ( const SWORD_Model* perceiver, const SWORD_Model* element, int level ) )
    {
        if( PHY_PerceptionLevel::FindPerceptionLevel( level ) == PHY_PerceptionLevel::notSeen_ )
            return false;
        return GET_PION( perceiver ).GetKnowledge().GetKnowledgePopulationPerceptionContainer().GetKnowledgePopulationPerception( GET_DATA( element, MIL_PopulationElement_ABC ).GetPopulation() ) == 0;
    }
    DEFINE_HOOK( ConvertSecondsToSim, 1, double, ( double seconds ) )
    {
        return MIL_Tools::ConvertSecondsToSim( seconds );
    }
    DEFINE_HOOK( GetConsumptionTypeSize, 0, size_t, () )
    {
        return PHY_ConsumptionType::GetConsumptionTypes().size();
    }
    DEFINE_HOOK( FindConsumptionType, 3, void, ( const char* consumptionType, void(*callback)( unsigned int identifier, void* userData ), void* userData ) )
    {
        const PHY_ConsumptionType::T_ConsumptionTypeMap& consumptionTypes = PHY_ConsumptionType::GetConsumptionTypes();
        PHY_ConsumptionType::CIT_ConsumptionTypeMap it = consumptionTypes.find( consumptionType );
        if( it != consumptionTypes.end() )
        {
            const PHY_ConsumptionType& conso = *it->second;
            callback( conso.GetID(), userData );
        }
    }
    DEFINE_HOOK( GetConsumptionMode, 1, unsigned int, ( const SWORD_Model* entity ) )
    {
        return GET_ROLE( entity, PHY_RoleInterface_Dotations ).GetConsumptionMode().GetID();
    }
    DEFINE_HOOK( GetTransporter, 2, const SWORD_Model*, ( const SWORD_Model* model, const SWORD_Model* agent ) )
    {
        const MIL_Agent_ABC* transporter = GET_ROLE( agent, PHY_RoleInterface_Transported ).GetTransporter();
        if( !transporter )
            return 0;
        const core::Model& rootNode = *core::Convert( model );
        return core::Convert( &rootNode[ "entities" ][ transporter->GetID() ] );
    }
    DEFINE_HOOK( GetVisionObjectsInSurface, 4, void, ( const SWORD_Model* localisation, unsigned int& emptySurface, unsigned int& forestSurface, unsigned int& urbanSurface ) )
    {
        MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetVisionObjectsInSurface( *GET_DATA( localisation, boost::shared_ptr< TER_Localisation > ), emptySurface, forestSurface, urbanSurface );
    }
    DEFINE_HOOK( GetVisionObject, 1, unsigned char, ( const MT_Vector2D* point ) )
    {
        return MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetVisionObject( *point );
    }
    DEFINE_HOOK( IsPointInsideLocalisation, 2, bool, ( const SWORD_Model* localisation, const MT_Vector2D* point ) )
    {
        return GET_DATA( localisation, boost::shared_ptr< TER_Localisation > )->IsInside( *point );
    }
    DEFINE_HOOK( IsLocalizationInsideCircle, 3, bool, ( const SWORD_Model* localization, const MT_Vector2D* center, double radius ) )
    {
        const TER_Localisation circle( *center, radius );
        const T_PointVector& pointLocalisationFinale = GET_DATA( localization, boost::shared_ptr< TER_Localisation > )->GetPoints();
        bool result = true;
        for( auto it = pointLocalisationFinale.begin(); result && it != pointLocalisationFinale.end(); ++it )
            result = circle.IsInside( *it );
        return result;
    }
    DEFINE_HOOK( IsKnowledgeObjectInsidePerception, 4, bool, ( const SWORD_Model* localization, const MT_Vector2D* center, double radius, const SWORD_Model* knowledgeObject ) )
    {
        const TER_Localisation& knowledgeLocalization = GET_DATA( knowledgeObject, DEC_Knowledge_Object ).GetLocalisation();
        const TER_Localisation circle( *center, radius );
        return GET_DATA( localization, boost::shared_ptr< TER_Localisation > )->IsIntersecting( knowledgeLocalization ) && circle.IsIntersecting( knowledgeLocalization );
    }
    DEFINE_HOOK( IsObjectIntersectingLocalization, 2, bool, ( const SWORD_Model* localization, const SWORD_Model* object ) )
    {
        return GET_DATA( localization, boost::shared_ptr< TER_Localisation > )->IsIntersecting( GET_DATA( object, MIL_Object_ABC ).GetLocalisation() );
    }
    DEFINE_HOOK( IsKnowledgeObjectIntersectingWithCircle, 3, bool, ( const MT_Vector2D* center, double radius, const SWORD_Model* knowledgeObject ) )
    {
        return GET_DATA( knowledgeObject, DEC_Knowledge_Object ).GetLocalisation().Intersect2DWithCircle( *center, radius );
    }
    DEFINE_HOOK( GetLocalizationRadius, 1, double, ( const SWORD_Model* localization ) )
    {
        const MT_Rect& boundingBox = GET_DATA( localization, boost::shared_ptr< TER_Localisation > )->GetBoundingBox();
        return boundingBox.GetCenter().Distance( boundingBox.GetPointUpLeft() );
    }
    DEFINE_HOOK( CanFlyingShellBePerceived, 4, bool, ( const SWORD_Model* flyingShell, const SWORD_Model* zone, const MT_Vector2D* source, double radius ) )
    {
        const TER_Localisation& localisation = *GET_DATA( zone, boost::shared_ptr< TER_Localisation > );
        return localisation.Intersect2DWithCircle( *source, radius ) && GET_DATA( flyingShell, MIL_Effect_IndirectFire ).IsFlyingThroughLocalisation( localisation );
    }
    DEFINE_HOOK( ComputePerceptionPosition, 3, void, ( const SWORD_Model* source, const MT_Vector2D* targetPosition, MT_Vector2D* result ) )
    {
        *result = GET_ROLE( source, PHY_RoleInterface_UrbanLocation ).GetFirerPosition( *targetPosition );
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionHooks::Initialize
// Created: SLI 2012-10-19
// -----------------------------------------------------------------------------
void PerceptionHooks::Initialize()
{
    // NOTHING
}
