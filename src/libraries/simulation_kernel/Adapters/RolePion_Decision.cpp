// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "RolePion_Decision.h"
#include "Hook.h"
#include "Sink.h"
#include "Action.h"
#include "Helpers.h"
#include "PathAdapter.h"
#include "KnowledgeCache.h"
#include "PathPointAdapter.h"
#include "OnComponentComputer_ABC.h"
#include "Decision/DEC_AgentFunctions.h"
#include "Decision/DEC_FireFunctions.h"
#include "Decision/DEC_PathFind_Manager.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_InterfaceMoving.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RoleInterface_HumanFactors.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Entities/Agents/Units/HumanFactors/PHY_Morale.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_Knowledge_AgentComposante.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_KS_Perception.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "Tools/MIL_Tools.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include <core/Facade.h>
#include <core/Model.h>
#include <core/UserData.h>
#include <core/Convert.h>
#include <module_api/Hook.h>
#include <boost/bind.hpp>
#include <boost/lambda/core.hpp>
#include <boost/make_shared.hpp>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( sword::RolePion_Decision )

namespace sword
{
    template< typename Archive >
    void save_construct_data( Archive& archive, const sword::RolePion_Decision* role, const unsigned int /*version*/ )
    {
        MIL_AgentPion* pion = role->pEntity_;
        const core::Model* model = &role->model_;
        const sword::Sink* sink = &role->sink_;
        unsigned int gcPause = role->gcPause_;
        unsigned int gcMult = role->gcMult_;
        sword::DEC_Logger* logger = role->logger_;
        archive << pion
                << model
                << sink
                << gcPause
                << gcMult
                << logger;
    }

    template< typename Archive >
    void load_construct_data( Archive& archive, sword::RolePion_Decision* role, const unsigned int /*version*/ )
    {
        MIL_AgentPion* pion;
        core::Model* model;
        Sink* sink;
        unsigned int gcPause;
        unsigned int gcMult;
        DEC_Logger* logger;
        archive >> pion
                >> model
                >> sink
                >> gcPause
                >> gcMult
                >> logger;
        ::new( role )sword::RolePion_Decision( *pion, *model, gcPause, gcMult, logger, *sink );
    }
}

using namespace sword;

// movement
DECLARE_HOOK( ComputeAgentFutureObjectCollision, bool, ( const SWORD_Model* model, const SWORD_Model* entity, const KnowledgeCache* objectsToTest, \
                                                         double& rDistance, const SWORD_Model** pObject ) ) \
DECLARE_HOOK( CreatePath, size_t, ( const SWORD_Model* model, const MT_Vector2D& vPosEnd, int pathType ) )
DECLARE_HOOK( CreatePathList, size_t, ( const SWORD_Model* model, std::vector< boost::shared_ptr< MT_Vector2D > >& points, int pathType ) )
DECLARE_HOOK( GetAgentFuturePosition, MT_Vector2D, ( const SWORD_Model* entity, double rTime, bool bBoundOnPath ) )
DECLARE_HOOK( GetPathPoints, void, ( unsigned int entity, void(*callback)( size_t point, void* userData ), void* userData ) )
DECLARE_HOOK( IsAgentMovingOnPath, bool, ( unsigned int entity, size_t path ) )
DECLARE_HOOK( PathGetLastPointOfPath, void, ( size_t path, MT_Vector2D* point ) )

// perception
DECLARE_HOOK( GetPerceptionId, int, () )
DECLARE_HOOK( IsPointVisible, bool, ( const SWORD_Model* model, const SWORD_Model* entity, const MT_Vector2D* point ) )
DECLARE_HOOK( AgentHasRadar, bool, ( const SWORD_Model* entity, size_t radarType ) )

// fire
DECLARE_HOOK( GetDangerosity, double, ( const SWORD_Model* firer, const SWORD_Model* target, bool(*filter)( const SWORD_Model* component ), double distance, bool checkAmmo ) )
DECLARE_HOOK( GetMaxRangeToFireOn, double, ( const SWORD_Model* firer, const SWORD_Model* target, bool(*filter)( const SWORD_Model* component ), double rWantedPH, int dotation ) )
DECLARE_HOOK( GetMinRangeToFireOn, double, ( const SWORD_Model* firer, const SWORD_Model* target, bool(*filter)( const SWORD_Model* component ), double rWantedPH ) )
DECLARE_HOOK( GetMaxRangeToFireOnWithPosture, double, ( const SWORD_Model* firer, const SWORD_Model* target, bool(*filter)( const SWORD_Model* component ), double rWantedPH ) )
DECLARE_HOOK( GetMinRangeToFireOnWithPosture, double, ( const SWORD_Model* firer, const SWORD_Model* target, bool(*filter)( const SWORD_Model* component ), double rWantedPH ) )
DECLARE_HOOK( GetMaxRangeToFire, double, ( const SWORD_Model* firer, bool(*filter)( const SWORD_Model* component ), double rWantedPH ) )
DECLARE_HOOK( GetMaxRangeToIndirectFire, double, ( const SWORD_Model* firer, bool(*filter)( const SWORD_Model* component ), int dotation, bool checkAmmo ) )
DECLARE_HOOK( GetMinRangeToIndirectFire, double, ( const SWORD_Model* firer, bool(*filter)( const SWORD_Model* component ), int dotation, bool checkAmmo ) )
DECLARE_HOOK( GetForceRatio, double, ( const SWORD_Model* model, const SWORD_Model* entity ) )
DECLARE_HOOK( GetDangerousEnemies, void, ( const SWORD_Model* model, const SWORD_Model* entity, void(*visitor)( const SWORD_Model* knowledge, void* userData ), void* userData ) )
DECLARE_HOOK( ComputeForceRatio, double, ( const SWORD_Model* model, const SWORD_Model* entity, bool(*filter)( const SWORD_Model* knowledge, void* userData ), void* userData ) )
DECLARE_HOOK( GetAmmunitionForIndirectFire, int, ( const SWORD_Model* model, const SWORD_Model* firer, const char* type, const MT_Vector2D* target ) )

// -----------------------------------------------------------------------------
// Name: RolePion_Decision constructor
// Created: SLI 2012-02-01
// -----------------------------------------------------------------------------
RolePion_Decision::RolePion_Decision( MIL_AgentPion& pion, const core::Model& model, unsigned int gcPause, unsigned int gcMult, sword::DEC_Logger* logger, Sink& sink )
    : DEC_RolePion_Decision( pion, gcPause, gcMult, logger )
    , sink_ ( sink )
    , model_( model )
{
    RegisterFunctions();
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision destructor
// Created: SLI 2012-02-01
// -----------------------------------------------------------------------------
RolePion_Decision::~RolePion_Decision()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::Reload
// Created: SLI 2012-07-16
// -----------------------------------------------------------------------------
void RolePion_Decision::Reload( bool doInitBrain )
{
    DEC_RolePion_Decision::Reload( doInitBrain );
    RegisterFunctions();
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::load
// Created: SLI 2012-07-17
// -----------------------------------------------------------------------------
template< typename Archive >
void RolePion_Decision::load( Archive& archive, const unsigned int )
{
    archive >> boost::serialization::base_object< DEC_RolePion_Decision >( *this );
    RegisterFunctions();
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::save
// Created: SLI 2012-07-17
// -----------------------------------------------------------------------------
template< typename Archive >
void RolePion_Decision::save( Archive& archive, const unsigned int ) const
{
    archive << boost::serialization::base_object< DEC_RolePion_Decision >( *this );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterCommand
// Created: SLI 2012-04-03
// -----------------------------------------------------------------------------
template< typename Result, typename Function >
void RolePion_Decision::RegisterCommand( const char* const name, Function fun )
{
    RegisterFunction( name, boost::function< Result >( boost::bind( fun, boost::ref( sink_ ), boost::ref( GetPion() ) ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterCommand
// Created: SLI 2012-04-03
// -----------------------------------------------------------------------------
template< typename Result, typename Function, typename Arg1 >
void RolePion_Decision::RegisterCommand( const char* const name, Function fun, Arg1 arg1 )
{
    RegisterFunction( name, boost::function< Result >( boost::bind( fun, boost::ref( sink_ ), boost::ref( GetPion() ), arg1 ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterCommand
// Created: SLI 2012-04-03
// -----------------------------------------------------------------------------
template< typename Result, typename Function, typename Arg1, typename Arg2 >
void RolePion_Decision::RegisterCommand( const char* const name, Function fun, Arg1 arg1, Arg2 arg2 )
{
    RegisterFunction( name, boost::function< Result >( boost::bind( fun, boost::ref( sink_ ), boost::ref( GetPion() ), arg1, arg2 ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterCommand
// Created: SLI 2012-04-03
// -----------------------------------------------------------------------------
template< typename Result, typename Function, typename Arg1, typename Arg2, typename Arg3 >
void RolePion_Decision::RegisterCommand( const char* const name, Function fun, Arg1 arg1, Arg2 arg2, Arg3 arg3 )
{
    RegisterFunction( name, boost::function< Result >( boost::bind( fun, boost::ref( sink_ ), boost::ref( GetPion() ), arg1, arg2, arg3 ) ) );
}

namespace
{
    unsigned int StartCommand( Sink& sink, MIL_AgentPion& pion, const std::string& name, core::Model& parameters )
    {
        boost::shared_ptr< PHY_Action_ABC > action( new sword::Action( sink, pion, name, parameters ) );
        pion.RegisterAction( action );
        return action->GetId();
    }
    unsigned int StopAction( MIL_AgentPion& pion, unsigned int action )
    {
        pion.UnregisterAction( action );
        return 0u;
    }
    void PauseAction( const MIL_AgentPion& pion, unsigned int action )
    {
        boost::shared_ptr< PHY_Action_ABC > pAction = pion.GetAction( action );
        if( pAction )
            pAction->Suspend();
    }
    void ResumeAction( const MIL_AgentPion& pion, unsigned int action )
    {
        boost::shared_ptr< PHY_Action_ABC > pAction = pion.GetAction( action );
        if( pAction )
            pAction->Resume();
    }
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterControlActions
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void RolePion_Decision::RegisterControlActions()
{
    RegisterFunction( "DEC__StopAction",
        boost::function< unsigned int( unsigned int ) >( boost::bind( &StopAction, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_PauseAction",
        boost::function< void( unsigned int ) >( boost::bind( &PauseAction, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ReprendAction",
        boost::function< void( unsigned int ) >( boost::bind( &ResumeAction, boost::ref( GetPion() ), _1 ) ) );
}

namespace
{
    std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > NextObjectOnPath( const MIL_ObjectFilter& filter, const MIL_Agent_ABC& agent, const core::Model& model, bool bCheckBypassed = true )
    {
        double rDistanceCollision = 0;
        KnowledgeCache cache;
        if( DEC_BlackBoard_CanContainKnowledgeObject* container = agent.GetKnowledgeGroup()->GetKnowledgeObjectContainer() )
            container->GetObjectsAtInteractionHeight( cache.objectsToAvoid_, agent, filter, bCheckBypassed );
        std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > result;
        const SWORD_Model* pObject = 0;
        if( cache.objectsToAvoid_.empty() || ! GET_HOOK( ComputeAgentFutureObjectCollision )( core::Convert( &model ), core::Convert( &model[ "entities" ][ agent.GetID() ] ), &cache, rDistanceCollision, &pObject ) )
        {
            result.first = false;
            return result;
        }
        if( !pObject )
            throw MASA_EXCEPTION( "invalid parameter." );
        boost::shared_ptr< DEC_Knowledge_Object > pObjectColliding = (*core::Convert( pObject ))[ "data" ].GetUserData< boost::shared_ptr< DEC_Knowledge_Object > >();
        if( !pObjectColliding || !pObjectColliding->IsValid() )
            throw MASA_EXCEPTION( "invalid parameter." );
        result.first = true;
        result.second.first = pObjectColliding;
        result.second.second = MIL_Tools::ConvertSimToMeter( rDistanceCollision );
        return result;
    }
    std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > GetNextObjOnPath( const MIL_Agent_ABC& agent, const core::Model& model, const std::vector< std::string >& params, bool bCheckBypassed = true )
    {
        if( params.empty() )
        {
            MIL_DangerousObjectFilter filter;
            return NextObjectOnPath( filter, agent, model, bCheckBypassed );
        }
        else
        {
            MIL_ObjectFilter filter( params );
            return NextObjectOnPath( filter, agent, model, bCheckBypassed );
        }
    }
    std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > GetNextObjectOnPath( const MIL_Agent_ABC& agent, const core::Model& model, boost::shared_ptr< DEC_Knowledge_Object > /*oId*/, const std::vector< std::string >& params )
    {
        return GetNextObjOnPath( agent, model, params );
    }
    std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > GetNextObjectOnPathWithBypassed( const MIL_Agent_ABC& agent, const core::Model& model, boost::shared_ptr< DEC_Knowledge_Object > /*oId*/, const std::vector< std::string >& params )
    {
        return GetNextObjOnPath( agent, model, params, false );
    }
    boost::shared_ptr< MT_Vector2D > ExtrapolatePosition( const MIL_AgentPion& agent, const core::Model& model, const double time, bool bBoundOnPath )
    {
        return boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( GET_HOOK( GetAgentFuturePosition )( core::Convert( &model[ "entities" ][ agent.GetID() ] ), MIL_Tools::ConvertMinutesToSim( time ), bBoundOnPath ) ) );
    }
    std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > GetNextRemovableObjectOnPath( const core::Model& model, const DEC_Decision_ABC& agent, boost::shared_ptr< DEC_Knowledge_Object > oId )
    {
        class RemovableFromPathObjectFilter : public MIL_ObjectFilter
        {
        public:
            RemovableFromPathObjectFilter( MIL_Agent_ABC& agent )
                : agent_( agent )
            {}
            virtual bool Test ( const MIL_ObjectType_ABC& type ) const
            {
                CanRemoveFromPathComputer computer( type );
                agent_.Execute< OnComponentComputer_ABC >( computer );
                return computer.canRemove_;
            }
        private:
            MIL_Agent_ABC& agent_;
            class CanRemoveFromPathComputer : public OnComponentComputer_ABC
            {
            public:
                CanRemoveFromPathComputer( const MIL_ObjectType_ABC& type )
                    : type_( type )
                    , canRemove_( false )
                {}
                virtual void ApplyOnComponent( PHY_ComposantePion& component )
                {
                    if( !canRemove_ && component.CanRemoveFromPath( type_, false ) ) // $$$$ BCI 2011-06-20: comment savoir si l'objet est miné ou pas?
                        canRemove_ = true;
                }
                bool canRemove_;
            private:
                const MIL_ObjectType_ABC& type_;
            };
        };
        RemovableFromPathObjectFilter filter( agent.GetPion() );
        return NextObjectOnPath( filter, agent.GetPion(), model );
    }
    bool IsMovingOnPath( const MIL_AgentPion& agent, const DEC_Path_ABC* pPath )
    {
        return pPath ? GET_HOOK( IsAgentMovingOnPath )( agent.GetID(), pPath->GetID() ) : false;
    }
    boost::shared_ptr< DEC_Path_ABC > CreatePathToPoint( MIL_AgentPion& agent, const core::Model& model, MT_Vector2D* pEnd, int pathType )
    {
        assert( pEnd );
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        return PathAdapter::Get( GET_HOOK( CreatePath )( core::Convert( &entity ), *pEnd, pathType ) );
    }
    boost::shared_ptr< DEC_Path_ABC > CreatePathToPointList( MIL_AgentPion& agent, const core::Model& model, std::vector< boost::shared_ptr< MT_Vector2D > > listPt, int pathType  )
    {
        assert( !listPt.empty() );
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        return PathAdapter::Get( GET_HOOK( CreatePathList )( core::Convert( &entity ), listPt, pathType ) );
    }
    boost::shared_ptr< DEC_Path_ABC > CreatePathToPointBM( MIL_AgentPion& agent, const core::Model& model, boost::shared_ptr< MT_Vector2D > end, int pathType )
    {
        return CreatePathToPoint( agent, model, end.get(), pathType );
    }
    int GetPathState( DEC_Path_ABC* pPath )
    {
        if( !pPath )
            throw MASA_EXCEPTION( "invalid parameter." );
        return pPath->GetState();
    }
    boost::shared_ptr< MT_Vector2D > GetLastPointOfPath( const DEC_Path_ABC* pPath )
    {
        boost::shared_ptr< MT_Vector2D > result( new MT_Vector2D() );
        if( !pPath )
            return result;
        GET_HOOK( PathGetLastPointOfPath )( pPath->GetID(), result.get() );
        return result;
    }
    unsigned int StartMovement( Sink& sink, MIL_AgentPion& pion, const boost::shared_ptr< DEC_Path_ABC >& path )
    {
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "path" ] = path->GetID();
        return StartCommand( sink, pion, "move", parameters );
    }
    void Orientate( Sink& sink, MIL_AgentPion& pion, boost::shared_ptr< MT_Vector2D > direction )
    {
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "direction/x" ] = direction->rX_;
        parameters[ "direction/y" ] = direction->rY_;
        sink.PostCommand( "orientate", parameters );
    }
    unsigned int StartTirDirect( Sink& sink, MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Agent > pEnemy, double percentage, int firingMode, int firingType, int ammoDotationClass, bool bFireOnlyOnMajorComposantes )
    {
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "enemy" ] = pEnemy->GetID();
        parameters[ "percentage" ] = std::max( 0., std::min< double >( 1., percentage ) );
        parameters[ "mode" ] = firingMode;
        parameters[ "type" ] = firingType;
        parameters[ "major" ] = bFireOnlyOnMajorComposantes;
        parameters[ "dotation" ] = ammoDotationClass;
        return StartCommand( sink, pion, "direct fire", parameters );
    }
    unsigned int StartTirPopulation( Sink& sink, MIL_AgentPion& pion, unsigned int populationKnowledgeID, const std::string& ammoDotationClass )
    {
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "population" ] = populationKnowledgeID;
        parameters[ "percentage" ] = 1; // $$$$ MCO 2012-09-21: yes it's a percentage between 0 and 1...
        parameters[ "mode" ] = 0; // $$$$ MCO 2012-09-21: eFiringModeNormal
        parameters[ "type" ] = 0; // $$$$ MCO 2012-10-02: eFireUsingAllComposantes
        const PHY_AmmoDotationClass* pClass = PHY_AmmoDotationClass::Find( ammoDotationClass );
        parameters[ "dotation" ] = pClass ? pClass->GetID() : 0;
        return StartCommand( sink, pion, "direct fire population", parameters );
    }
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterActions
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void RolePion_Decision::RegisterActions()
{
    RegisterCommand< unsigned int( const boost::shared_ptr< DEC_Path_ABC >& ) >( "DEC_StartDeplacement", &StartMovement, _1 );
    RegisterCommand< void( boost::shared_ptr< MT_Vector2D > ) >( "DEC_Orientate", &Orientate, _1 );
    RegisterFunction( "DEC_StartTirDirect",
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Agent >, double, int, int ) >( boost::bind( &StartTirDirect, boost::ref( sink_ ), boost::ref( GetPion() ), _1, _2, _3, 0, _4, false ) ) ); // $$$$ MCO 2012-09-14: 0 => eFireUsingAllComposantes
    RegisterFunction( "DEC_StartTirDirectDebarques",
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Agent >, double, int ) >( boost::bind( &StartTirDirect, boost::ref( sink_ ), boost::ref( GetPion() ), _1, _2, _3, 1, -1, false ) ) ); // $$$$ MCO 2012-09-14: 1 => eFireUsingOnlyComposantesLoadable
    RegisterFunction( "DEC_StartTirDirectTransporteurs",
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Agent >, double, int ) >( boost::bind( &StartTirDirect, boost::ref( sink_ ), boost::ref( GetPion() ), _1, _2, _3, 2, -1, false ) ) ); // $$$$ MCO 2012-09-14: 2 = eFireUsingOnlyComposantesCarrier
    RegisterFunction( "DEC__StartTirDirectSurComposantesMajeures",
        boost::function< unsigned int( int, boost::shared_ptr< DEC_Knowledge_Agent >, double, int ) >( boost::bind( &StartTirDirect, boost::ref( sink_ ), boost::ref( GetPion() ), _2, _3, 0, _4, _1, true ) ) ); // $$$$ MCO 2012-09-14: 0 => eFireUsingAllComposantes
    RegisterFunction( "DEC__StartTirSurPopulation",
        boost::function< unsigned int( unsigned int, const std::string& ) >( boost::bind( &StartTirPopulation, boost::ref( sink_ ), boost::ref( GetPion() ), _1, _2 ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterPath
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void RolePion_Decision::RegisterPath()
{
    RegisterFunction( "DEC_CreerItineraire",
        boost::function< boost::shared_ptr< DEC_Path_ABC >( MT_Vector2D*, int ) >( boost::bind( &CreatePathToPoint, boost::ref( GetPion() ), boost::ref( model_ ), _1, _2 ) ) );
    RegisterFunction( "DEC_CreerItineraireListe",
        boost::function< boost::shared_ptr< DEC_Path_ABC >( std::vector< boost::shared_ptr< MT_Vector2D > >, int ) >( boost::bind( &CreatePathToPointList, boost::ref( GetPion() ), boost::ref( model_ ), _1, _2 ) ) );
    RegisterFunction( "DEC_CreerItineraireBM",
        boost::function< boost::shared_ptr< DEC_Path_ABC >( boost::shared_ptr< MT_Vector2D >, int ) >( boost::bind( &CreatePathToPointBM, boost::ref( GetPion() ), boost::ref( model_ ), _1, _2 ) ) );
    RegisterFunction( "DEC_Itineraire_EstEnMouvementSur",
        boost::function< bool ( const DEC_Path_ABC* ) >( boost::bind( &IsMovingOnPath, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Itineraire_ExtrapolerPosition" ,
        boost::function< boost::shared_ptr< MT_Vector2D >( const double, bool ) >( boost::bind( &::ExtrapolatePosition, boost::ref( GetPion() ), boost::ref( model_ ), _1, _2 ) ) );
    RegisterFunction( "DEC_GetNextObjectOnPath",
        boost::function< std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > >( boost::shared_ptr< DEC_Knowledge_Object >, float, const std::vector< std::string >& ) >(
            boost::bind( &GetNextObjectOnPath, boost::ref( GetPion() ), boost::cref( model_ ), _1, _3 ) ) );
    RegisterFunction( "DEC_GetNextObjectOnPathWithBypassed",
        boost::function< std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > >( boost::shared_ptr< DEC_Knowledge_Object >, float, const std::vector< std::string >& ) >(
            boost::bind( &GetNextObjectOnPathWithBypassed, boost::ref( GetPion() ), boost::cref( model_ ), _1, _3 ) ) );
    RegisterFunction( "DEC_GetNextRemovableObjectOnPath",
        boost::function< std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > >( const DEC_Decision_ABC&, boost::shared_ptr< DEC_Knowledge_Object >, float ) >(
            boost::bind( &GetNextRemovableObjectOnPath, boost::cref( model_ ), _1, _2 ) ) );
    RegisterFunction( "DEC_Itineraire_Etat",
        boost::function< int ( DEC_Path_ABC* ) >( boost::bind( &GetPathState, _1 ) ) );
}

namespace
{
    void ApplyFunction( std::size_t point, void* userData )
    {
        (*static_cast< boost::function< void( std::size_t point ) >* >( userData ))( point );
    }
    void FillPoints( std::vector< boost::shared_ptr< PathPointAdapter > >& path, unsigned int entity, std::size_t point )
    {
        path.push_back( boost::make_shared< PathPointAdapter >( entity, point ) );
    }
    std::vector< boost::shared_ptr< PathPointAdapter > > GetPointsCategory( MIL_AgentPion& agent )
    {
        std::vector< boost::shared_ptr< PathPointAdapter > > result;
        boost::function< void( std::size_t ) > callback =
            boost::bind( &FillPoints, boost::ref( result ), agent.GetID(), _1 );
        GET_HOOK( GetPathPoints )( agent.GetID(), &ApplyFunction, &callback );
        return result;
    }
}

//-----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterRepresentations
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void RolePion_Decision::RegisterRepresentations()
{
    RegisterFunction( "DEC_GetPointsCategory", boost::bind( &GetPointsCategory, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_RemoveFromPointsCategory", 
        boost::function< void( boost::shared_ptr< PathPointAdapter > )>( boost::bind( &PathPointAdapter::RemoveFromPointsCategory, _1 ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterRepPoints
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void RolePion_Decision::RegisterRepPoints()
{
    RegisterFunction( "DEC_GetRepPoint",
        boost::function< MT_Vector2D*( boost::shared_ptr< PathPointAdapter > )>( boost::bind( &PathPointAdapter::GetRepPoint, _1 ) ) );
    RegisterFunction( "DEC_IsAvantPoint",
        boost::function< bool( boost::shared_ptr< PathPointAdapter > )>( boost::bind( &PathPointAdapter::IsAvantPoint, _1 ) ) );
    RegisterFunction( "DEC_IsPoint",
        boost::function< bool( boost::shared_ptr< PathPointAdapter > )>( boost::bind( &PathPointAdapter::IsPoint, _1 ) ) );
    RegisterFunction( "DEC_GetTypePoint",
        boost::function< int( boost::shared_ptr< PathPointAdapter > )>( boost::bind( &PathPointAdapter::GetTypePoint, _1 ) ) );
    RegisterFunction( "DEC_GetDestPoint",
        boost::function< boost::shared_ptr< PathPointAdapter >( boost::shared_ptr< PathPointAdapter > )>( boost::bind( &PathPointAdapter::GetDestPoint, _1 ) ) );
    RegisterFunction( "DEC_GetTypeLimaPoint",
        boost::function< int( boost::shared_ptr< PathPointAdapter > )>( boost::bind( &PathPointAdapter::GetTypeLimaPoint, _1 ) ) );
    RegisterFunction( "DEC_GetLimaPoint",
        boost::function< unsigned int( boost::shared_ptr< PathPointAdapter > )>( boost::bind( &PathPointAdapter::GetLimaPoint, _1 ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterItinerary
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void RolePion_Decision::RegisterItinerary()
{
    RegisterFunction( "DEC_Itineraire_DernierPoint",
        boost::function< boost::shared_ptr< MT_Vector2D >( const DEC_Path_ABC* ) >( boost::bind( &::GetLastPointOfPath, _1 ) ) );
    RegisterFunction( "DEC_Itineraire_ExtrapolerPosition",
        boost::function< boost::shared_ptr< MT_Vector2D >( double, bool )>( boost::bind( &::ExtrapolatePosition, boost::ref( GetPion() ), boost::cref( model_ ), _1, _2 ) ) );
    RegisterMethod( "DEC_Itineraire_DernierPoint", &RolePion_Decision::GetLastPointOfPath );
    RegisterMethod( "DEC_Itineraire_ExtrapolerPosition", &RolePion_Decision::ExtrapolatePosition );
    RegisterMethod( "GetType", &PathPointAdapter::GetDIAType );
    RegisterMethod( "Getpoint_", &PathPointAdapter::GetPos );
}

SWORD_USER_DATA_EXPORT( const MIL_UrbanObject_ABC* )
SWORD_USER_DATA_EXPORT( boost::shared_ptr< TER_Localisation > )

namespace
{
    void TogglePerception( Sink& sink, MIL_AgentPion& pion, const std::string& perception, bool isActivated )
    {
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "perception" ] = perception;
        parameters[ "activated" ] = isActivated;
        sink.PostCommand( "toggle perception", parameters );
    }
    void ToggleRadar( Sink& sink, MIL_AgentPion& pion, bool isActivated, int radar )
    {
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "activated" ] = isActivated;
        parameters[ "radar-class" ] = radar;
        sink.PostCommand( "toggle radar", parameters );
    }
    int EnableRadar( Sink& sink, MIL_AgentPion& pion, int radar, boost::shared_ptr< TER_Localisation > localisation ) // $$$$ _RC_ SLI 2012-03-28: DRY with ToggleRadar
    {
        if( !localisation )
            throw MASA_EXCEPTION( "invalid localization parameter while enabling radar." );
        const int perceptionId = GET_HOOK( GetPerceptionId )();
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "activated" ] = true;
        parameters[ "radar-class" ] = radar;
        parameters[ "perception-id" ] = perceptionId;
        parameters[ "localization/data" ].SetUserData( localisation );
        sink.PostCommand( "toggle localized radar", parameters );
        return perceptionId;
    }
    int EnableLocalizedRadarOnPoint( Sink& sink, MIL_AgentPion& pion, int radar, boost::shared_ptr< MT_Vector2D > point )
    {
        if( !point )
            throw MASA_EXCEPTION( "invalid localization parameter while enabling radar." );
        return EnableRadar( sink, pion, radar, boost::make_shared< TER_Localisation >( TER_Localisation::ePoint, boost::assign::list_of( *point ) ) );
    }
    int EnableLocalizedRadar( Sink& sink, MIL_AgentPion& pion, int radar, const TER_Localisation* localisation )
    {
        if( !localisation )
            throw MASA_EXCEPTION( "invalid localization parameter while enabling radar." );
        return EnableRadar( sink, pion, radar, boost::make_shared< TER_Localisation >( *localisation ) );
    }
    void DisableLocalizedRadar( Sink& sink, MIL_AgentPion& pion, int radar, int perceptionId )
    {
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "activated" ] = false;
        parameters[ "radar-class" ] = radar;
        parameters[ "perception-id" ] = perceptionId;
        sink.PostCommand( "toggle localized radar", parameters );
    }
    int EnableLocalizedDetectionId( Sink& sink, MIL_AgentPion& pion, const std::string& perception, const core::Model& localisation, int perceptionId )
    {
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "activated" ] = true;
        parameters[ "perception" ] = perception;
        parameters[ "perception-id" ] = perceptionId;
        parameters[ "localization" ] = localisation;
        sink.PostCommand( "toggle localized perception", parameters );
        return perceptionId;
    }
    int EnableSharedLocalizedDetection( Sink& sink, MIL_AgentPion& pion, const std::string& perception, boost::shared_ptr< TER_Localisation > localisation )
    {
        if( !localisation )
            throw MASA_EXCEPTION( "invalid localization parameter while enabling localized detection." );
        core::Model parameter;
        parameter[ "data" ].SetUserData( localisation );
        return EnableLocalizedDetectionId( sink, pion, perception, parameter, GET_HOOK( GetPerceptionId )() );
    }
    int EnableUrbanLocalizedDetection( Sink& sink, MIL_AgentPion& pion, const std::string& perception, const MIL_UrbanObject_ABC* block )
    {
        if( !block )
            throw MASA_EXCEPTION( "invalid urban block parameter while enabling localized detection." );
        core::Model parameter;
        parameter[ "data" ].SetUserData( boost::make_shared< TER_Localisation >( block->GetLocalisation() ) );
        parameter[ "block/data" ].SetUserData( block );
        return EnableLocalizedDetectionId( sink, pion, perception, parameter, GET_HOOK( GetPerceptionId )() );
    }
    int EnableAlatLocalizedDetection( Sink& sink, MIL_AgentPion& pion, const std::string& perception, const TER_Localisation* localisation )
    {
        if( !localisation )
            throw MASA_EXCEPTION( "invalid localization parameter while enabling localized detection." );
        core::Model parameter;
        parameter[ "data" ].SetUserData( boost::make_shared< TER_Localisation >( *localisation ) );
        return EnableLocalizedDetectionId( sink, pion, perception, parameter, 0 );
    }
    int EnableAlatMonitoring( Sink& sink, MIL_AgentPion& pion, const TER_Localisation* localisation )
    {
        if( !localisation )
            throw MASA_EXCEPTION( "invalid localization parameter while enabling ALAT monitoring detection." );
        const unsigned int perceptionId = GET_HOOK( GetPerceptionId )();
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "activated" ] = true;
        parameters[ "perception-id" ] = perceptionId;
        parameters[ "localization/data" ].SetUserData( boost::make_shared< TER_Localisation >( *localisation ) );
        sink.PostCommand( "toggle alat monitoring", parameters );
        return perceptionId;
    }
    void DisableLocalizedDetection( Sink& sink, MIL_AgentPion& pion, const std::string& perception, int perceptionId )
    {
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "activated" ] = false;
        parameters[ "perception" ] = perception;
        parameters[ "perception-id" ] = perceptionId;
        sink.PostCommand( "toggle localized perception", parameters );
    }
    int EnableRecoOnLocation( Sink& sink, MIL_AgentPion& pion, const TER_Localisation* localisation, boost::optional< float > growthSpeed )
    {
        if( !localisation )
            throw MASA_EXCEPTION( "invalid localization parameter while enabling reco on location detection." );
        const int perceptionId = GET_HOOK( GetPerceptionId )();
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "activated" ] = true;
        parameters[ "perception-id" ] = perceptionId;
        parameters[ "has-growth-speed" ] = static_cast< bool >( growthSpeed );
        parameters[ "growth-speed" ] = growthSpeed ? *growthSpeed : 0;
        parameters[ "localization/data" ].SetUserData( boost::make_shared< TER_Localisation >( *localisation ) );
        sink.PostCommand( "toggle reco", parameters );
        return perceptionId;
    }
    void DisableIdentifiedCommand( Sink& sink, MIL_AgentPion& pion, const std::string& command, int perceptionId )
    {
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "activated" ] = false;
        parameters[ "perception-id" ] = perceptionId;
        sink.PostCommand( command, parameters );
    }
    int EnableObjectDetection( Sink& sink, MIL_AgentPion& pion, boost::shared_ptr< TER_Localisation > localisation, const MT_Vector2D* center, double speed )
    {
        if( !localisation )
            throw MASA_EXCEPTION( "invalid localization parameter while enabling object detection on location." );
        const int perceptionId = GET_HOOK( GetPerceptionId )();
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "activated" ] = true;
        parameters[ "perception-id" ] = perceptionId;
        parameters[ "center/x" ] = center->rX_;
        parameters[ "center/y" ] = center->rY_;
        parameters[ "growth-speed" ] = speed;
        parameters[ "localization/data" ].SetUserData( localisation );
        sink.PostCommand( "toggle object detection", parameters );
        return perceptionId;
    }
    int EnableRecognitionPoint( Sink& sink, MIL_AgentPion& pion, MT_Vector2D* center, double size, double growthSpeed )
    {
        if( !center )
            throw MASA_EXCEPTION( "invalid center parameter while enabling recognition on point." );
        const int perceptionId = GET_HOOK( GetPerceptionId )();
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "activated" ] = true;
        parameters[ "perception-id" ] = perceptionId;
        parameters[ "center/x" ] = center->rX_;
        parameters[ "center/y" ] = center->rY_;
        parameters[ "growth-speed" ] = MIL_Tools::ConvertSpeedMosToSim( growthSpeed );
        parameters[ "max-radius" ] = size;
        sink.PostCommand( "toggle recognition point", parameters );
        return perceptionId;
    }
    template< typename T >
    void SwitchVisionMode( Sink& sink, MIL_AgentPion& pion, const std::string& mode, T location )
    {
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "mode" ] = mode;
        parameters[ "location/x" ] = location ? location->rX_ : 0.;
        parameters[ "location/y" ] = location ? location->rY_ : 0.;
        sink.PostCommand( "vision", parameters );
    }
    bool IsPointVisible( MIL_AgentPion& pion, const core::Model& model, MT_Vector2D* pPt )
    {
        const core::Model& entity = model[ "entities" ][ pion.GetID() ];
        return GET_HOOK( IsPointVisible )( core::Convert( &model ), core::Convert( &entity ), pPt );
    }
    void IdentifyAllAgentsInZone( Sink& sink, MIL_AgentPion& pion, const TER_Localisation* localisation )
    {
        if( !localisation )
            throw MASA_EXCEPTION( "invalid localization parameter while identifying all agents in zone." );
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "localization/data" ].SetUserData( boost::make_shared< TER_Localisation >( *localisation ) );
        sink.PostCommand( "identify all agents in zone", parameters );
    }
    bool AgentHasRadar( const core::Model& model, const DEC_Decision_ABC* agent, int typeRadar )
    {
        if( !agent )
            throw MASA_EXCEPTION( "Invalid pion in AgentHasRadar" );
        const core::Model& entity = model[ "entities" ][ agent->GetPion().GetID() ];
        return GET_HOOK( AgentHasRadar )( core::Convert( &entity ), typeRadar );
    }
    double GetPerception( const MIL_AgentPion& pion, boost::shared_ptr< MT_Vector2D > pPoint, boost::shared_ptr< MT_Vector2D > pTarget )
    {
        if( !pPoint || !pTarget )
            return 0.;
        return pion.GetRole< PHY_RoleInterface_Perceiver >().GetPerception( *pPoint, *pTarget );
    }
    bool HasNoDelayedPerceptions( const MIL_AgentPion& pion )
    {
        return !pion.GetKnowledge().GetKsPerception().HasDelayedPerceptions();
    }
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterPerception
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void RolePion_Decision::RegisterPerception()
{
    RegisterCommand< void() >                                      ( "DEC_Perception_ActiverCoupsDeSonde", &TogglePerception, "scan", true );
    RegisterCommand< void() >                                      ( "DEC_Perception_DesactiverCoupsDeSonde", &TogglePerception, "scan", false );
    RegisterCommand< void() >                                      ( "DEC_Perception_ActiverSenseurs", &TogglePerception, "sensor", true );
    RegisterCommand< void() >                                      ( "DEC_Perception_DesactiverSenseurs", &TogglePerception, "sensor", false );
    RegisterCommand< void() >                                      ( "DEC_Perception_ActiverObserveurTir", &TogglePerception, "fire-observer", true );
    RegisterCommand< void() >                                      ( "DEC_Perception_DesactiverObserveurTir", &TogglePerception, "fire-observer", false );
    RegisterCommand< void() >                                      ( "DEC_Perception_ActiverModeEnregistrement", &TogglePerception, "record-mode", true );
    RegisterCommand< void() >                                      ( "DEC_Perception_DesactiverModeEnregistrement", &TogglePerception, "record-mode", false );
    RegisterCommand< void( int ) >                                 ( "DEC_Perception_ActiverRadar", &ToggleRadar, true, _1 );
    RegisterCommand< void( int ) >                                 ( "DEC_Perception_DesactiverRadar", &ToggleRadar, false, _1 );
    RegisterCommand< int( int, const TER_Localisation* ) >         ( "DEC_Perception_ActiverRadarSurLocalisation", &EnableLocalizedRadar, _1, _2 );
    RegisterCommand< int( int, boost::shared_ptr< MT_Vector2D > ) >( "DEC_Perception_ActiverRadarSurPointPtr", &EnableLocalizedRadarOnPoint, _1, _2 );
    RegisterCommand< void( int, int ) >                            ( "DEC_Perception_DesactiverRadarSurLocalisation", &DisableLocalizedRadar, _1, _2 );
    RegisterCommand< int( boost::shared_ptr< TER_Localisation> ) > ( "DEC_Perception_ActiverPerceptionTirsIndirect", &EnableSharedLocalizedDetection, "flying-shell/zones", _1 );
    RegisterCommand< void( int ) >                                 ( "DEC_Perception_DesactiverPerceptionTirsIndirect", &DisableLocalizedDetection, "flying-shell/zones", _1 );
    RegisterCommand< int( const TER_Localisation* ) >              ( "DEC_Perception_ActiverReconnaissanceLocalisation", &EnableRecoOnLocation, _1, boost::optional< float >() );
    RegisterCommand< int( const TER_Localisation*, float ) >       ( "DEC_Perception_ActivateLocationProgressiveRecce", &EnableRecoOnLocation, _1, _2 );
    RegisterCommand< void( int ) >                                 ( "DEC_Perception_DesactiverReconnaissanceLocalisation", &DisableIdentifiedCommand, "toggle reco", _1 );
    RegisterCommand< int( MIL_UrbanObject_ABC* ) >                 ( "DEC_Perception_ActiverReconnaissanceDansBlocUrbain", &EnableUrbanLocalizedDetection, "urban", _1 );
    RegisterCommand< void( int ) >                                 ( "DEC_Perception_DesactiverReconnaissanceDansBlocUrbain", &DisableLocalizedDetection, "urban", _1 );
    RegisterCommand< int( boost::shared_ptr< TER_Localisation >,
                          const MT_Vector2D*, double ) >           ( "DEC_Perception_ActiverDetectionObjetLocalisation", &EnableObjectDetection, _1, _2, _3 );
    RegisterCommand< void( int ) >                                 ( "DEC_Perception_DesactiverDetectionObjetLocalisation", &DisableIdentifiedCommand, "toggle object detection", _1 );
    RegisterCommand< int( MT_Vector2D*, double, double ) >         ( "DEC_Perception_ActiverReconnaissancePoint", &EnableRecognitionPoint, _1, _2, _3 );
    RegisterCommand< void( int ) >                                 ( "DEC_Perception_DesactiverReconnaissancePoint", &DisableIdentifiedCommand, "toggle recognition point", _1 );
    RegisterCommand< void( boost::shared_ptr< MT_Vector2D > ) >    ( "DEC_Perception_VisionVerrouilleeSurDirection", &SwitchVisionMode< boost::shared_ptr< MT_Vector2D > >, "direction", _1 );
    RegisterCommand< void( const MT_Vector2D* ) >                  ( "DEC_Perception_VisionVerrouilleeSurPoint", &SwitchVisionMode< const MT_Vector2D* >, "location", _1 );
    RegisterCommand< void( boost::shared_ptr< MT_Vector2D > ) >    ( "DEC_Perception_VisionVerrouilleeSurPointPtr", &SwitchVisionMode< boost::shared_ptr< MT_Vector2D > >, "location", _1 );
    RegisterCommand< void() >                                      ( "DEC_Perception_VisionNormale", &SwitchVisionMode< boost::shared_ptr< MT_Vector2D > >, "normal", boost::make_shared< MT_Vector2D >() );
    RegisterCommand< void( const TER_Localisation* ) >             ( "DEC_Connaissances_IdentifierToutesUnitesDansZone", &IdentifyAllAgentsInZone, _1 );
    RegisterFunction( "DEC_Perception_PointEstVisible", boost::function< bool( MT_Vector2D* ) >( boost::bind( &IsPointVisible, boost::ref( GetPion() ), boost::ref( model_ ), _1 ) ) );
    RegisterFunction( "DEC_Agent_ARadar", boost::function< bool( const DEC_Decision_ABC*, int ) >( boost::bind( &AgentHasRadar, boost::cref( model_ ), _1, _2 ) ) );
    RegisterFunction( "DEC_GetPerception", boost::function< double( boost::shared_ptr< MT_Vector2D >, boost::shared_ptr< MT_Vector2D > ) >( boost::bind( &GetPerception, boost::cref( GetPion() ), _1, _2 ) ) );
    // ALAT
    RegisterCommand< int( const TER_Localisation* ) >              ( "DEC_ALAT_ActiverReconnaissance", &EnableAlatLocalizedDetection, "alat/reco", _1 );
    RegisterCommand< void() >                                      ( "DEC_ALAT_DesactiverReconnaissance", &DisableLocalizedDetection, "alat/reco", 0u ); // $$$$ _RC_ SLI 2012-07-12: no perception id
    RegisterCommand< int( const TER_Localisation* ) >              ( "DEC_Perception_ActiverSurveillance", &EnableAlatMonitoring, _1 );
    RegisterCommand< void( int ) >                                 ( "DEC_Perception_DesactiverSurveillance", &DisableLocalizedDetection, "alat/monitoring", _1 );
    RegisterFunction( "DEC_ALAT_ReconnaissanceNonVuTerminee", boost::bind( &HasNoDelayedPerceptions, boost::cref( GetPion() ) ) );
}

namespace
{
    bool CanFire( const SWORD_Model* component )
    {
        return GET_DATA( component, PHY_ComposantePion ).CanFire();
    }
    bool IsMajor( const SWORD_Model* component )
    {
        return (*core::Convert( component ))[ "major" ];
    }
    bool CanMajorFire( const SWORD_Model* component )
    {
        return IsMajor( component ) && CanFire( component );
    }
    int GetDotationIdentifier( const PHY_DotationCategory* dotation )
    {
        if( dotation )
            return dotation->GetMosID();
        return -1;
    }
    double GetMaxRangeToFireOnEnemy( const MIL_AgentPion& agent, const core::Model& model, boost::shared_ptr< DEC_Knowledge_Agent > target, float rWantedPH, const PHY_DotationCategory* dotation )
    {
        if( ! target || ! target->IsValid() )
            return -1;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        const unsigned int id = entity[ "knowledges" ];
        const core::Model& knowledge = model[ "knowledges" ][ id ][ "agents" ][ target->GetID() ];
        return GET_HOOK( GetMaxRangeToFireOn )( core::Convert( &entity ), core::Convert( &knowledge ), &CanFire, rWantedPH, GetDotationIdentifier( dotation ) );
    }
    double GetMinRangeToFireOnEnemy( const MIL_AgentPion& agent, const core::Model& model, boost::shared_ptr< DEC_Knowledge_Agent > target, float rWantedPH )
    {
        if( ! target || ! target->IsValid() )
            return -1;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        const unsigned int id = entity[ "knowledges" ];
        const core::Model& knowledge = model[ "knowledges" ][ id ][ "agents" ][ target->GetID() ];
        const double range = GET_HOOK( GetMinRangeToFireOn )( Convert( &entity ), Convert( &knowledge ), &CanFire, rWantedPH );
        if( range == std::numeric_limits< double >::max() )
            return -1;
        return range;
    }
    double GetMaxRangeToFireOnEnemyActualPosture( const MIL_AgentPion& agent, const core::Model& model, boost::shared_ptr< DEC_Knowledge_Agent > target, float rWantedPH )
    {
        if( ! target || ! target->IsValid() )
            return -1;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        const unsigned int id = entity[ "knowledges" ];
        const core::Model& knowledge = model[ "knowledges" ][ id ][ "agents" ][ target->GetID() ];
        return GET_HOOK( GetMaxRangeToFireOnWithPosture )( core::Convert( &entity ), core::Convert( &knowledge ), &CanFire, rWantedPH );
    }
    double GetMinRangeToFireOnEnemyActualPosture( const MIL_AgentPion& agent, const core::Model& model, boost::shared_ptr< DEC_Knowledge_Agent > target, float rWantedPH )
    {
        if( ! target || ! target->IsValid() )
            return -1;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        const unsigned int id = entity[ "knowledges" ];
        const core::Model& knowledge = model[ "knowledges" ][ id ][ "agents" ][ target->GetID() ];
        const double range = GET_HOOK( GetMinRangeToFireOnWithPosture )( core::Convert( &entity ), core::Convert( &knowledge ), &CanFire, rWantedPH );
        if( range == std::numeric_limits< double >::max() )
            return -1;
        return range;
    }
    bool CanFireWhenUnloaded( const SWORD_Model* component )
    {
        return GET_DATA( component, PHY_ComposantePion ).CanFireWhenUnloaded();
    }
    double GetMaxRangeToFireOnEnemyWhenUnloaded( const MIL_AgentPion& agent, const core::Model& model, boost::shared_ptr< DEC_Knowledge_Agent > target, float rWantedPH )
    {
        if( ! target || ! target->IsValid() )
            return -1;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        const unsigned int id = entity[ "knowledges" ];
        const core::Model& knowledge = model[ "knowledges" ][ id ][ "agents" ][ target->GetID() ];
        return GET_HOOK( GetMaxRangeToFireOn )( core::Convert( &entity ), core::Convert( &knowledge ), &CanFireWhenUnloaded, rWantedPH, 0 );
    }
    bool True( const SWORD_Model* /*component*/ )
    {
        return true;
    }
    double GetMaxRangeToBeFiredByEnemy( const MIL_AgentPion& agent, const core::Model& model, boost::shared_ptr< DEC_Knowledge_Agent > enemy, float rWantedPH )
    {
        if( ! enemy || ! enemy->IsValid() )
            return -1;
        const core::Model& target = model[ "entities" ][ agent.GetID() ];
        const unsigned int id = target[ "knowledges" ];
        const core::Model& firer = model[ "knowledges" ][ id ][ "agents" ][ enemy->GetID() ];
        return GET_HOOK( GetMaxRangeToFireOn )( core::Convert( &firer ), core::Convert( &target ), &True, rWantedPH, 0 );
    }
    double GetMaxRangeToFire( const MIL_Agent_ABC& agent, const core::Model& model, float rWantedPH )
    {
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        const double range = GET_HOOK( GetMaxRangeToFire )( core::Convert( &entity ), &True, rWantedPH );
        if( range == std::numeric_limits< double >::max() )
            return -1;
        return range;
    }
    double GetMaxRangeToIndirectFire( const MIL_Agent_ABC& agent, const core::Model& model, const PHY_DotationCategory* dotation )
    {
        if( ! dotation )
            return -1;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        return GET_HOOK( GetMaxRangeToIndirectFire )( core::Convert( &entity ), &CanFire, GetDotationIdentifier( dotation ), true );
    }
    double GetMinRangeToIndirectFire( const MIL_Agent_ABC& agent, const core::Model& model, const PHY_DotationCategory* dotation )
    {
        if( ! dotation )
            return -1;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        const double range = GET_HOOK( GetMinRangeToIndirectFire )( core::Convert( &entity ), &CanFire, GetDotationIdentifier( dotation ), true );
        if( range == std::numeric_limits< double >::max() )
            return -1;
        return range;
    }
    double GetMaxRangeToIndirectFireWithoutAmmoCheck( const MIL_Agent_ABC& agent, const core::Model& model, const PHY_DotationCategory* dotation )
    {
        if( ! dotation )
            return -1;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        return GET_HOOK( GetMaxRangeToIndirectFire )( core::Convert( &entity ), &CanFire, GetDotationIdentifier( dotation ), false );
    }
    double GetMinRangeToIndirectFireWithoutAmmoCheck( const MIL_Agent_ABC& agent, const core::Model& model, const PHY_DotationCategory* dotation )
    {
        if( ! dotation )
            return -1;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        const double range = GET_HOOK( GetMinRangeToIndirectFire )( core::Convert( &entity ), &CanFire, GetDotationIdentifier( dotation ), false );
        if( range == std::numeric_limits< double >::max() )
            return -1;
        return range;
    }
    bool NullFilter( const SWORD_Model* /*component*/ )
    {
        return true;
    }
    double GetMaxRangeToIndirectFireWithoutAmmo( const MIL_Agent_ABC& agent, const core::Model& model )
    {
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        const double range = GET_HOOK( GetMaxRangeToIndirectFire )( core::Convert( &entity ), &NullFilter, 0, false );
        if( range == std::numeric_limits< double >::max() )
            return -1;
        return range;
    }
    double GetTheoricMaxRangeToIndirectFire( const MIL_Agent_ABC& agent, const core::Model& model, const PHY_DotationCategory* dotation )
    {
        if( ! dotation )
            return -1;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        return GET_HOOK( GetMaxRangeToIndirectFire )( core::Convert( &entity ), &NullFilter, GetDotationIdentifier( dotation ), true );
    }
    double GetTheoricMinRangeToIndirectFire( const MIL_Agent_ABC& agent, const core::Model& model, const PHY_DotationCategory* dotation )
    {
        if( ! dotation )
            return -1;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        const double range = GET_HOOK( GetMinRangeToIndirectFire )( core::Convert( &entity ), &NullFilter, GetDotationIdentifier( dotation ), true );
        if( range == std::numeric_limits< double >::max() )
            return -1;
        return range;
    }
    const PHY_DotationCategory* GetAmmunitionForIndirectFire( const MIL_Agent_ABC& agent, const core::Model& model, int indirectFireDotationClassID, const MT_Vector2D* pTarget )
    {
        const PHY_IndirectFireDotationClass* pClass = PHY_IndirectFireDotationClass::Find( indirectFireDotationClassID );
        if( ! pClass )
            return 0;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        int dotation = GET_HOOK( GetAmmunitionForIndirectFire )( core::Convert( &model ), core::Convert( &entity ), pClass->GetName().c_str(), pTarget );
        return PHY_DotationType::FindDotationCategory( static_cast< unsigned int >( dotation ) );
    }
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterFire
// Created: MCO 2012-05-10
// -----------------------------------------------------------------------------
void RolePion_Decision::RegisterFire()
{
    RegisterFunction( "DEC_Tir_PorteeMaxPourTirerSurUnite",
        boost::function< double( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &GetMaxRangeToFireOnEnemy, boost::ref( GetPion() ), boost::ref( model_ ), _1, _2, static_cast< PHY_DotationCategory* >( 0 ) ) ) );
    RegisterFunction( "DEC_Tir_PorteeMaxPourTirerSurUniteAvecMunition",
        boost::function< double( boost::shared_ptr< DEC_Knowledge_Agent >, float, const PHY_DotationCategory* ) >( boost::bind( &GetMaxRangeToFireOnEnemy, boost::ref( GetPion() ), boost::ref( model_ ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMinPourTirerSurUnite",
        boost::function< double( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &GetMinRangeToFireOnEnemy, boost::ref( GetPion() ), boost::ref( model_ ), _1, _2 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMaxPourTirerSurUnitePosturesReelles",
        boost::function< double( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &GetMaxRangeToFireOnEnemyActualPosture, boost::ref( GetPion() ), boost::ref( model_ ), _1, _2 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMinPourTirerSurUnitePosturesReelles",
        boost::function< double( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &GetMinRangeToFireOnEnemyActualPosture, boost::ref( GetPion() ), boost::ref( model_ ), _1, _2 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMaxPourTirerDebarqueSurUnite",
        boost::function< double( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &GetMaxRangeToFireOnEnemyWhenUnloaded, boost::ref( GetPion() ), boost::ref( model_ ), _1, _2 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMaxPourEtreTireParUnite",
        boost::function< double( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &GetMaxRangeToBeFiredByEnemy, boost::ref( GetPion() ), boost::ref( model_ ), _1, _2 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMaxPourTirer",
        boost::function< double( float ) >( boost::bind( &GetMaxRangeToFire, boost::ref( GetPion() ), boost::ref( model_ ), _1 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMaxTirIndirect",
        boost::function< double( const PHY_DotationCategory* ) >( boost::bind( &GetMaxRangeToIndirectFire, boost::ref( GetPion() ), boost::ref( model_ ), _1 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMinTirIndirect",
        boost::function< double( const PHY_DotationCategory* ) >( boost::bind( &GetMinRangeToIndirectFire, boost::ref( GetPion() ), boost::ref( model_ ), _1 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMaxTirIndirect_SansTesterEtatMunitions",
        boost::function< double( const PHY_DotationCategory* ) >( boost::bind( &GetMaxRangeToIndirectFireWithoutAmmoCheck, boost::ref( GetPion() ), boost::ref( model_ ), _1 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMinTirIndirect_SansTesterEtatMunitions",
        boost::function< double( const PHY_DotationCategory* ) >( boost::bind( &GetMinRangeToIndirectFireWithoutAmmoCheck, boost::ref( GetPion() ), boost::ref( model_ ), _1 ) ) );
    RegisterFunction( "DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition",
        boost::bind( &GetMaxRangeToIndirectFireWithoutAmmo, boost::ref( GetPion() ), boost::ref( model_ ) ) );
    RegisterFunction( "DEC_Tir_PorteeTheoriqueMaxTirIndirect",
        boost::function< double( const PHY_DotationCategory* ) >( boost::bind( &GetTheoricMaxRangeToIndirectFire, boost::ref( GetPion() ), boost::ref( model_ ), _1 ) ) );
    RegisterFunction( "DEC_Tir_PorteeTheoriqueMinTirIndirect",
        boost::function< double( const PHY_DotationCategory* ) >( boost::bind( &GetTheoricMinRangeToIndirectFire, boost::ref( GetPion() ), boost::ref( model_ ), _1 ) ) );
    RegisterFunction( "DEC_Tir_LancerFumigeneSurConnaissance", // $$$$ MCO 2012-06-25: should be a command
        boost::function< double( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_FireFunctions::ThrowSmokeOnKnowledgeAgent, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Tir_MunitionPourTirIndirect",
        boost::function< const PHY_DotationCategory* ( int, const MT_Vector2D* ) >( boost::bind( &GetAmmunitionForIndirectFire, boost::ref( GetPion() ), boost::ref( model_ ), _1, _2 ) ) );
    RegisterFunction( "DEC_Pion_InterdireMunition",
        boost::function< void( const std::vector< const PHY_DotationCategory* >& ) >( boost::bind( &DEC_FireFunctions::ForbidAmmunition, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Pion_AutoriserMunition",
        boost::function< void( const std::vector< const PHY_DotationCategory* >& ) >( boost::bind( &DEC_FireFunctions::AllowAmmunition, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Pion_AutoriserToutesMunitions",
        boost::bind( &DEC_FireFunctions::AllowAllAmmunitions, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Tir_LancerFumigeneSurPosition", &DEC_FireFunctions::ThrowSmokeOnPosition );
}

namespace
{
    double ComputeDistance( boost::shared_ptr< DEC_Knowledge_Agent > pTargetKnowledge, boost::shared_ptr< MT_Vector2D > position )
    {
        const MT_Vector3D sourcePosition( position->rX_, position->rY_, 0 );
        const MT_Vector3D targetPosition( pTargetKnowledge->GetPosition().rX_, pTargetKnowledge->GetPosition().rY_, pTargetKnowledge->GetAltitude() );
        return sourcePosition.Distance( targetPosition );
    }
    double ComputeDistance( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, const MIL_Agent_ABC& target )
    {
        const PHY_RoleInterface_Location& targetLocation = target.GetRole< PHY_RoleInterface_Location >();
        const MT_Vector3D vTargetPosition( targetLocation.GetPosition().rX_, targetLocation.GetPosition().rY_, targetLocation.GetAltitude() );
        const MT_Vector3D vDataPosition( pKnowledge->GetPosition().rX_, pKnowledge->GetPosition().rY_, pKnowledge->GetAltitude() );
        return vTargetPosition.Distance( vDataPosition );
    }
    double ComputeDistance( boost::shared_ptr< DEC_Knowledge_Agent > pTargetKnowledge, boost::shared_ptr< DEC_Knowledge_Agent > pSourceKnowledge )
    {
        const MT_Vector3D sourcePosition( pSourceKnowledge->GetPosition().rX_, pSourceKnowledge->GetPosition().rY_, pSourceKnowledge->GetAltitude() );
        const MT_Vector3D targetPosition( pTargetKnowledge->GetPosition().rX_, pTargetKnowledge->GetPosition().rY_, pTargetKnowledge->GetAltitude() );
        return sourcePosition.Distance( targetPosition );
    }
    const core::Model& GetKnowledge( const core::Model& model, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
    {
        return model[ "knowledges" ][ pKnowledge->GetGroupID() ][ "agents" ][ pKnowledge->GetID() ];
    }
    double GetPotentialAttrition( const MIL_AgentPion& agent, const core::Model& model, boost::shared_ptr< DEC_Knowledge_Agent > pTargetKnowledge, boost::shared_ptr< MT_Vector2D > position )
    {
        if( ! pTargetKnowledge || ! pTargetKnowledge->IsValid() )
            return 0;
        if( pTargetKnowledge->GetMaxPerceptionLevel() < PHY_PerceptionLevel::recognized_
            || agent.GetArmy().IsAFriend( *pTargetKnowledge->GetArmy() ) == eTristate_True
            || pTargetKnowledge->IsSurrendered()
            || agent.GetRole< surrender::PHY_RoleInterface_Surrender >().IsSurrendered() )
            return 0;
        const double distance = ComputeDistance( pTargetKnowledge, position );
        const core::Model& enemy = GetKnowledge( model, pTargetKnowledge );
        const core::Model& firer = model[ "entities" ][ agent.GetID() ];
        return GET_HOOK( GetDangerosity )( core::Convert( &firer ), core::Convert( &enemy ), &CanMajorFire, distance, false );
    }
    double GetDangerosity( const MIL_AgentPion& target, const core::Model& model, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
    {
        if( ! pKnowledge || ! pKnowledge->IsValid() )
            return 0;
        // For DIA, the dangerosity value is 1 <= dangerosity <= 2 // $$$$ MCO 2012-06-29: is it ?
        if( pKnowledge->GetMaxPerceptionLevel() < PHY_PerceptionLevel::recognized_
            || pKnowledge->IsAFriend( target.GetArmy() ) == eTristate_True
            || pKnowledge->IsSurrendered() )
            return 1;
        const double distance = ComputeDistance( pKnowledge, target );
        const core::Model& firer = GetKnowledge( model, pKnowledge );
        const core::Model& enemy = model[ "entities" ][ target.GetID() ];
        double dangerosity = GET_HOOK( GetDangerosity )( core::Convert( &firer ), core::Convert( &enemy ), &IsMajor, distance, false );
        pKnowledge->DegradeDangerosity( dangerosity );
        return 1 + dangerosity;
    }
    double GetDangerosityOnPion( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, const core::Model& model, const DEC_Decision_ABC* pTarget )
    {
        if( ! pTarget )
            throw MASA_EXCEPTION( "invalid parameter." );
        return GetDangerosity( pTarget->GetPion(), model, pKnowledge );
    }
    double GetDangerosityOnKnowledge( boost::shared_ptr< DEC_Knowledge_Agent > pSource, const core::Model& model, boost::shared_ptr< DEC_Knowledge_Agent > pTarget )
    {
        if( ! pSource || ! pSource->IsValid()|| ! pTarget || ! pTarget->IsValid() )
            return 0;
        if( pSource->GetMaxPerceptionLevel() < PHY_PerceptionLevel::recognized_ )
            return 0;
        const double distance = ComputeDistance( pSource, pTarget );
        const core::Model& firer = GetKnowledge( model, pSource );
        const core::Model& enemy = GetKnowledge( model, pTarget );
        // For DIA, the dangerosity value is 1 <= dangerosity <= 2 // $$$$ MCO 2012-08-22: right...
        return 1 + GET_HOOK( GetDangerosity )( core::Convert( &firer ), core::Convert( &enemy ), &CanMajorFire, distance, false );
    }
    bool IsInUrbanBlock( const SWORD_Model* knowledge, void* userData )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > agent = GET_DATA( knowledge, boost::shared_ptr< DEC_Knowledge_Agent > );
        MIL_UrbanObject_ABC* pUrbanObject = static_cast< MIL_UrbanObject_ABC* >( userData );
        return pUrbanObject && agent->IsInUrbanBlock( *pUrbanObject );
    }
    double GetUrbanRapForLocal( const MIL_AgentPion& agent, const core::Model& model, MIL_UrbanObject_ABC* pUrbanObject )
    {
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        double rRapForValue = GET_HOOK( ComputeForceRatio )( core::Convert( &model ), core::Convert( &entity ), &IsInUrbanBlock, pUrbanObject );
        // Add bonus if the pion is posted in this urbanbloc // $$$$ MCO 2012-08-27: use a callback or a hook
        const MIL_UrbanObject_ABC* urbanBlock = agent.GetRole< PHY_RoleInterface_UrbanLocation >().GetCurrentUrbanBlock();
        if( pUrbanObject && urbanBlock && pUrbanObject == urbanBlock && agent.GetRole< PHY_RoleInterface_Posture >().IsInstalled() ) // $$$$ _RC_ LGY 2011-02-24: == sur les ID
            rRapForValue *= 1.2;
        rRapForValue = std::max( 0.2, std::min( 5., rRapForValue ) ); // $$$$ MCO 2012-08-27: hard-coded min and max actually equal to the ones in Knowledge_RapFor_ABC.cpp in fire module
        return rRapForValue;
    }
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterKnowledge
// Created: MCO 2012-05-14
// -----------------------------------------------------------------------------
void RolePion_Decision::RegisterKnowledge()
{
    RegisterFunction( "DEC_ConnaissanceAgent_AttritionPotentielle",
        boost::function< double( boost::shared_ptr< DEC_Knowledge_Agent >, boost::shared_ptr< MT_Vector2D > ) >( boost::bind( &GetPotentialAttrition, boost::cref( GetPion() ), boost::cref( model_ ), _1, _2 ) ) );
    RegisterFunction( "DEC_ConnaissanceAgent_Dangerosite",
        boost::function< double( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &GetDangerosity, boost::cref( GetPion() ), boost::cref( model_ ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceAgent_DangerositeSurPion",
        boost::function< double( boost::shared_ptr< DEC_Knowledge_Agent >, const DEC_Decision_ABC* ) >( boost::bind( &GetDangerosityOnPion, _1, boost::cref( model_ ), _2 ) ) );
    RegisterFunction( "DEC_ConnaissanceAgent_DangerositeSurConnaissance",
        boost::function< double( boost::shared_ptr< DEC_Knowledge_Agent >, boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &GetDangerosityOnKnowledge, _1, boost::cref( model_ ), _2 ) ) );
    RegisterFunction( "DEC_ConnaissanceBlocUrbain_RapForLocal",
        boost::function< double( MIL_UrbanObject_ABC* ) >( boost::bind( &GetUrbanRapForLocal, boost::cref( GetPion() ), boost::cref( model_ ), _1 ) ) );
}

namespace
{
    double GetRapForLocalAgent( const DEC_Decision_ABC* agent, const core::Model& model )
    {
        if( ! agent )
            return 0;
        const core::Model& entity = model[ "entities" ][ agent->GetPion().GetID() ];
        return GET_HOOK( GetForceRatio )( Convert( &model ), Convert( &entity ) );
    }
    double GetRapForLocal( const MIL_AgentPion& agent, const core::Model& model )
    {
        const PHY_Morale& morale = agent.GetRole< PHY_RoleInterface_HumanFactors >().GetMorale();
        if( &morale == &PHY_Morale::fanatique_ )
            return 5.0; // $$$$ _RC_ SLG 2010-05-28: Facteur moral max
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        return GET_HOOK( GetForceRatio )( Convert( &model ), Convert( &entity ) );
    }
    void Populate( const SWORD_Model* knowledge, void* userData )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > agent = GET_DATA( knowledge, boost::shared_ptr< DEC_Knowledge_Agent > );
        static_cast< T_ConstKnowledgeAgentVector* >( userData )->push_back( agent );
    }
    T_ConstKnowledgeAgentVector GetDangerousEnemies( const MIL_AgentPion& agent, const core::Model& model )
    {
        T_ConstKnowledgeAgentVector enemies;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        GET_HOOK( GetDangerousEnemies )( Convert( &model ), Convert( &entity ), &Populate, &enemies );
        return enemies;
    }
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterAgentKnowledgeFunctions
// Created: MCO 2012-05-15
// -----------------------------------------------------------------------------
void RolePion_Decision::RegisterAgentKnowledgeFunctions()
{
    RegisterFunction( "DEC_Agent_RapportDeForceLocal",
        boost::function< double( const DEC_Decision_ABC* ) >( boost::bind( &GetRapForLocalAgent, _1, boost::cref( model_ ) ) ) );
    RegisterFunction( "DEC_RapportDeForceLocal", boost::bind( &GetRapForLocal, boost::ref( GetPion() ), boost::cref( model_ ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesEnnemiesDangereuses", boost::bind( &GetDangerousEnemies, boost::ref( GetPion() ), boost::cref( model_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterFunctions
// Created: SLI 2012-02-01
// -----------------------------------------------------------------------------
void RolePion_Decision::RegisterFunctions()
{
    RegisterControlActions();
    RegisterActions();
    RegisterPath();
    RegisterRepresentations();
    RegisterRepPoints();
    RegisterItinerary();
    RegisterPerception();
    RegisterFire();
    RegisterKnowledge();
    RegisterAgentKnowledgeFunctions();
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::GetLastPointOfPath
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > RolePion_Decision::GetLastPointOfPath( const double time, bool bBoundOnPath ) const
{
    return ::ExtrapolatePosition( GetPion(), model_, time, bBoundOnPath );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::ExtrapolatePosition
// Created: MCO 2012-02-03
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > RolePion_Decision::ExtrapolatePosition( const double time, bool bBoundOnPath ) const
{
    return ::ExtrapolatePosition( GetPion(), model_, time, bBoundOnPath );
}
