// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef MODULE_FIXTURE_H
#define MODULE_FIXTURE_H

#include "module_tester/ApiFixture.h"
#include "module_tester/MakeModel.h"
#include <core/MakeModel.h>

// module dependencies
#include "MT_Tools/MT_Vector2D.h"

class PHY_Volume;

#define REGISTERED_HOOKS( APPLY ) \
    APPLY( InitializePerceptionTypes, 1, void, ( const char* xml ) ) \
    APPLY( IsPointVisible, 3, bool, ( const SWORD_Model* model, const SWORD_Model* entity, const MT_Vector2D* point ) ) \
    APPLY( AgentHasRadar, 2, bool, ( const SWORD_Model* entity, size_t radarType ) ) \
    APPLY( GetPerception, 3, double, ( const SWORD_Model* entity, const MT_Vector2D* point, const MT_Vector2D* target ) ) \
    APPLY( ComputeKnowledgeObjectPerception, 3, size_t, ( const SWORD_Model* model, const SWORD_Model* entity, const SWORD_Model* knowledgeObject ) )

#define USED_HOOKS( APPLY ) \
    APPLY( GetAgentListWithinCircle, 5, void, ( const SWORD_Model* root, const MT_Vector2D& vCenter, double rRadius, void (*callback)( const SWORD_Model* agent, void* userData ), void* userData ) ) \
    APPLY( GetAgentListWithinLocalisation, 4, void, ( const SWORD_Model* root, const SWORD_Model* localization, void (*callback)( const SWORD_Model* agent, void* userData ), void* userData ) ) \
    APPLY( GetObjectListWithinCircle, 5, void, ( const SWORD_Model* root, const MT_Vector2D& vCenter, double rRadius, void (*callback)( const SWORD_Model* object, void* userData ), void* userData ) ) \
    APPLY( GetConcentrationListWithinCircle, 5, void, ( const SWORD_Model* root, const MT_Vector2D& vCenter, double rRadius, void (*callback)( const SWORD_Model* concentration, void* userData ), void* userData ) ) \
    APPLY( GetFlowListWithinCircle, 5, void, ( const SWORD_Model* root, const MT_Vector2D& vCenter, double rRadius, void (*callback)( const SWORD_Model* flow, void* userData ), void* userData ) ) \
    APPLY( GetUrbanObjectListWithinCircle, 5, void, ( const SWORD_Model* root, const MT_Vector2D& center, float radius, void (*callback)( const SWORD_Model* urbanObjectWrapper, void* userData ), void* userData ) ) \
    APPLY( GetUrbanBlocksListWithinSegment, 5, void, ( const SWORD_Model* root, MT_Vector2D first, MT_Vector2D second, void (*callback)( const SWORD_Model* urbanObjectWrapper, void* userData ), void* userData ) ) \
    APPLY( AppendAddedKnowledge, 7, void, ( const SWORD_Model* root, const SWORD_Model* entity, \
                                            void (*agentCallback)( const SWORD_Model* agent, void* userData ), \
                                            void (*objectCallback)( const SWORD_Model* object, void* userData ), \
                                            void (*concentrationCallback)( const SWORD_Model* concentration, void* userData ), \
                                            void (*flowCallback)( const SWORD_Model* flow, void* userData ), \
                                            void* userData ) ) \
    APPLY( IsInCity, 1 , bool, ( const SWORD_Model* entity ) ) \
    APPLY( BelongsToKnowledgeGroup, 2, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) ) \
    APPLY( IsAgentPerceptionDistanceHacked, 2, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) ) \
    APPLY( IsObjectPerceptionDistanceHacked, 2, bool, ( const SWORD_Model* perceiver, const SWORD_Model* object ) ) \
    APPLY( IsPopulationElementPerceptionDistanceHacked, 2, bool, ( const SWORD_Model* perceiver, const SWORD_Model* element ) ) \
    APPLY( GetHackedPerceptionLevel, 2, int, ( const SWORD_Model* perceiver, const SWORD_Model* target ) ) \
    APPLY( GetObjectPerceptionLevel, 2, int, ( const SWORD_Model* perceiver, const SWORD_Model* object ) ) \
    APPLY( GetPopulationElementPerceptionLevel, 2, int, ( const SWORD_Model* perceiver, const SWORD_Model* element ) ) \
    APPLY( CanBeSeen, 2, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) ) \
    APPLY( CanObjectBePerceived, 1, bool, ( const SWORD_Model* object ) ) \
    APPLY( CanPopulationElementBePerceived, 1, bool, ( const SWORD_Model* element ) ) \
    APPLY( IsCivilian, 1, bool, ( const SWORD_Model* agent ) ) \
    APPLY( IsAgentNewlyPerceived, 3, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target, int level ) ) \
    APPLY( IsPopulationElementNewlyPerceived, 3, bool, ( const SWORD_Model* perceiver, const SWORD_Model* element, int level ) ) \
    APPLY( ConvertSecondsToSim, 1, double, ( double seconds ) ) \
    APPLY( GetConsumptionTypeSize, 0, size_t, () ) \
    APPLY( FindConsumptionType, 3, void, ( const char* consumptionType, void(*callback)( unsigned int identifier, void* userData ), void* userData ) ) \
    APPLY( GetConsumptionMode, 1, unsigned int, ( const SWORD_Model* entity ) ) \
    APPLY( ComputeRayTrace, 11, double, ( const MT_Vector2D& source, double sourceAltitude, const MT_Vector2D& target, double targetAltitude, \
                                          double maxDectectionDistance, double distanceMaxModificator, \
                                          const double* urbanBlockFactors, const double* lightingFactors, const double* precipitationFactors, \
                                          double(*FindEnvironmentFactor)( unsigned int environment, const void* userData ), const void* userData ) ) \
    APPLY( GetPostureSize, 0, size_t, () ) \
    APPLY( CanEmit, 1, bool, ( const SWORD_Model* entity ) ) \
    APPLY( GetPostureIdentifier, 2, bool, ( const char* type, size_t* identifier ) ) \
    APPLY( PostureCanModifyDetection, 1, bool, ( const char* type ) ) \
    APPLY( GetLastPostureIdentifier, 1, size_t, ( const SWORD_Model* entity ) ) \
    APPLY( GetCurrentPostureIdentifier, 1, size_t, ( const SWORD_Model* entity ) ) \
    APPLY( GetPostureCompletionPercentage, 1, double, ( const SWORD_Model* entity ) ) \
    APPLY( ComputePerceptionDistanceFactor, 1, double, ( const SWORD_Model* entity ) ) \
    APPLY( GetCollidingPopulationDensity, 1, double, ( const SWORD_Model* entity ) ) \
    APPLY( ObjectIntersectWithCircle, 3, bool, ( const SWORD_Model* object, const MT_Vector2D& center, double radius ) ) \
    APPLY( KnowledgeObjectIntersectWithCircle, 3, bool, ( const SWORD_Model* knowledgeObject, const MT_Vector2D& center, double radius ) ) \
    APPLY( FindObjectType, 1, unsigned int, ( const char* type ) ) \
    APPLY( GetObjectType, 1, size_t, ( const SWORD_Model* object ) ) \
    APPLY( GetKnowledgeObjectType, 1, size_t, ( const SWORD_Model* object ) ) \
    APPLY( GetEnvironmentAssociation, 1, size_t, ( const char* environment ) ) \
    APPLY( GetAltitude, 2, double, ( double x, double y ) ) \
    APPLY( GetSignificantVolume, 2, const PHY_Volume*, ( const SWORD_Model* entity, const double* volumeFactors ) ) \
    APPLY( GetVolumeIdentifierFromInstance, 1, size_t, ( const PHY_Volume* volume ) ) \
    APPLY( GetVolumeSize, 0, size_t, () ) \
    APPLY( PopulationElementIntersectWithCircle, 5, bool, ( const SWORD_Model* element, MT_Vector2D circleCenter, double radius, void(*AddShapePoint)( MT_Vector2D point, void* userData ), void* userData ) ) \
    APPLY( GetUrbanBlockFactor, 2, double, ( const SWORD_Model* block, const double* urbanBlockFactors ) ) \
    APPLY( IsMaterialType, 1, bool, ( const char* material ) ) \
    APPLY( GetPrecipitationSize, 0, size_t, () ) \
    APPLY( GetLightingSize, 0, size_t, () ) \
    APPLY( GetMaterialTypeSize, 0, size_t, () ) \
    APPLY( GetVolumeIdentifier, 2, bool, ( const char* type, size_t* identifier ) ) \
    APPLY( GetPrecipitationIdentifier, 2, bool, ( const char* type, size_t* identifier ) ) \
    APPLY( GetLightingIdentifier, 2, bool, ( const char* type, size_t* identifier ) ) \
    APPLY( IsObjectUniversal, 1, bool, ( const SWORD_Model* object ) ) \
    APPLY( GetPerceptionRandom, 0, double, () ) \
    APPLY( IsKnown, 2, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) ) \
    APPLY( ComputeAgentRatioInsidePerceptionPolygon, 4, double, ( const SWORD_Model* perceiver, const SWORD_Model* target, double distance, double roll ) ) \
    APPLY( GetCurrentUrbanBlock, 2, const SWORD_Model*, ( const SWORD_Model* root, const SWORD_Model* entity ) ) \
    APPLY( GetUrbanObjectStructuralHeight, 1, double, ( const SWORD_Model* urbanObject ) ) \
    APPLY( GetUrbanObjectOccupation, 1, double, ( const SWORD_Model* urbanObject ) ) \
    APPLY( GetUrbanObjectStructuralState, 1, double, ( const SWORD_Model* urbanObject ) ) \
    APPLY( HasUrbanObjectArchitecture, 1, bool, ( const SWORD_Model* urbanObject ) ) \
    APPLY( CanUrbanBlockBeSeen, 2, bool, ( const SWORD_Model* perceiver, const SWORD_Model* urbanBlock ) ) \
    APPLY( IsPostureStationed, 1, bool, ( const SWORD_Model* entity ) ) \
    APPLY( CanComponentPerceive, 2, bool, ( const SWORD_Model* entity, const SWORD_Model* component ) ) \
    APPLY( GetTransporter, 2, const SWORD_Model*, ( const SWORD_Model* model, const SWORD_Model* agent ) ) \
    APPLY( GetVisionObjectsInSurface, 4, void, ( const SWORD_Model* localisation, unsigned int& emptySurface, unsigned int& forestSurface, unsigned int& urbanSurface ) ) \
    APPLY( GetVisionObject, 1, unsigned char, ( const MT_Vector2D* point ) ) \
    APPLY( IsPointInsideLocalisation, 2, bool, ( const SWORD_Model* localisation, const MT_Vector2D* point ) ) \
    APPLY( IsLocalizationInsideCircle, 3, bool, ( const SWORD_Model* localization, const MT_Vector2D* center, double radius ) ) \
    APPLY( IsKnowledgeObjectInsidePerception, 4, bool, ( const SWORD_Model* localization, const MT_Vector2D* center, double radius, const SWORD_Model* knowledgeObject ) ) \
    APPLY( IsObjectIntersectingLocalization, 2, bool, ( const SWORD_Model* localization, const SWORD_Model* object ) ) \
    APPLY( IsKnowledgeObjectIntersectingWithCircle, 3, bool, ( const MT_Vector2D* center, double radius, const SWORD_Model* knowledgeObject ) ) \
    APPLY( GetLocalizationRadius, 1, double, ( const SWORD_Model* localization ) ) \
    APPLY( CanFlyingShellBePerceived, 4, bool, ( const SWORD_Model* flyingShell, const SWORD_Model* zone, const MT_Vector2D* source, double radius ) )

#define REGISTERED_AND_USED_HOOKS( APPLY ) \
    APPLY( IsUsingActiveRadar, 1, bool, ( const SWORD_Model* entity ) ) \
    APPLY( IsUsingSpecializedActiveRadar, 2, bool, ( const SWORD_Model* entity, const char* radarType ) ) \
    APPLY( GetPerceptionId, 0, int, () ) \

HOOK_FIXTURE( HOOKS )

namespace sword
{
namespace perception
{
    struct ModuleFixture : core::HookFixture
    {
        ModuleFixture()
            : HookFixture( "perception_module", boost::assign::list_of( "perception" )
                                                                      ( "toggle perception" )
                                                                      ( "toggle radar" )
                                                                      ( "toggle localized radar" )
                                                                      ( "toggle localized perception" )
                                                                      ( "toggle reco" )
                                                                      ( "toggle object detection" )
                                                                      ( "toggle recognition point" )
                                                                      ( "toggle alat monitoring" )
                                                                      ( "vision" )
                                                                      ( "identify all agents in zone" )
                                                                      ( "external perception" )
                                                                      ( "disable all perceptions" ) )
        {}
    };
}
}

#endif // MODULE_FIXTURE_H
