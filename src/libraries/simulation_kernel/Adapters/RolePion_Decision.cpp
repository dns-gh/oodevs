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
#include "PathAdapter.h"
#include "KnowledgeCache.h"
#include "PathPointAdapter.h"
#include "OnComponentComputer_ABC.h"
#include "Decision/DEC_AgentFunctions.h"
#include "Decision/DEC_FireFunctions.h"
#include "Decision/DEC_PathFind_Manager.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_InterfaceMoving.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Knowledge/DEC_Knowledge_AgentComposante.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Army.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Tools/MIL_Tools.h"
#include <core/Facade.h>
#include <core/Model.h>
#include <core/UserData.h>
#include <core/Convert.h>
#include <module_api/Hook.h>
#include <directia/brain/Brain.h>
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
        unsigned int gcPause = role->gcPause_;
        unsigned int gcMult = role->gcMult_;
        const sword::Sink* sink = &role->sink_;
        archive << role->pEntity_
                << sink
                << gcPause
                << gcMult;
    }

    template< typename Archive >
    void load_construct_data( Archive& archive, sword::RolePion_Decision* role, const unsigned int /*version*/ )
    {
        MIL_AgentPion* pion;
        Sink* sink;
        unsigned int gcPause;
        unsigned int gcMult;
        archive >> pion
                >> sink
                >> gcPause
                >> gcMult;
        ::new( role )sword::RolePion_Decision( *pion, gcPause, gcMult, *sink );
    }
}

using namespace sword;

// movement
DECLARE_HOOK( ComputeAgentFutureObjectCollision, bool, ( const SWORD_Model* entity, const KnowledgeCache& objectsToTest, double& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject ) ) \
DECLARE_HOOK( CreatePath, boost::shared_ptr< sword::movement::Path_ABC >, ( const SWORD_Model* model, const MT_Vector2D& vPosEnd, int pathType ) )
DECLARE_HOOK( CreatePathList, boost::shared_ptr< sword::movement::Path_ABC >, ( const SWORD_Model* model, std::vector< boost::shared_ptr< MT_Vector2D > >& points, int pathType ) )
DECLARE_HOOK( GetAgentFuturePosition, MT_Vector2D, ( const SWORD_Model* entity, double rTime, bool bBoundOnPath ) )
DECLARE_HOOK( GetPathPoints, void, ( unsigned int entity, void(*callback)( boost::shared_ptr< sword::movement::PathPoint > point, void* userData ), void* userData ) )
DECLARE_HOOK( IsAgentMovingOnPath, bool, ( unsigned int entity, boost::shared_ptr< sword::movement::Path_ABC > path ) )
DECLARE_HOOK( PathGetLastPointOfPath, boost::shared_ptr< MT_Vector2D >, ( boost::shared_ptr< sword::movement::Path_ABC > pPath ) )

// perception
DECLARE_HOOK( GetPerceptionId, int, () )

// fire
DECLARE_HOOK( GetDangerosity, double, ( const SWORD_Model* firer, const SWORD_Model* target, bool(*filter)( const SWORD_Model* component ), float rDistBtwSourceAndTarget, bool checkAmmo ) )
DECLARE_HOOK( GetMaxRangeToFireOn, double, ( const SWORD_Model* firer, const SWORD_Model* target, bool(*filter)( const SWORD_Model* component ), double rWantedPH, const char* dotation ) )
DECLARE_HOOK( GetMinRangeToFireOn, double, ( const SWORD_Model* firer, const SWORD_Model* target, bool(*filter)( const SWORD_Model* component ), double rWantedPH ) )
DECLARE_HOOK( GetMaxRangeToFireOnWithPosture, double, ( const SWORD_Model* firer, const SWORD_Model* target, bool(*filter)( const SWORD_Model* component ), double rWantedPH ) )
DECLARE_HOOK( GetMinRangeToFireOnWithPosture, double, ( const SWORD_Model* firer, const SWORD_Model* target, bool(*filter)( const SWORD_Model* component ), double rWantedPH ) )
DECLARE_HOOK( GetMaxRangeToFire, double, ( const SWORD_Model* firer, bool(*filter)( const SWORD_Model* component ), double rWantedPH ) )
DECLARE_HOOK( GetMaxRangeToIndirectFire, double, ( const SWORD_Model* firer, bool(*filter)( const SWORD_Model* component ), const char* dotation, bool checkAmmo ) )
DECLARE_HOOK( GetMinRangeToIndirectFire, double, ( const SWORD_Model* firer, bool(*filter)( const SWORD_Model* component ), const char* dotation, bool checkAmmo ) )
DECLARE_HOOK( GetForceRatio, double, ( const SWORD_Model* model, const SWORD_Model* entity ) )
DECLARE_HOOK( GetDangerousEnemies, void, ( const SWORD_Model* model, const SWORD_Model* entity, void(*visitor)( const SWORD_Model* knowledge, void* userData ), void* userData ) )
DECLARE_HOOK( GetAmmunitionForIndirectFire, const char*, ( const SWORD_Model* model, const SWORD_Model* firer, const char* type, const MT_Vector2D& target ) )

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::Initialize
// Created: SLI 2012-02-01
// -----------------------------------------------------------------------------
void RolePion_Decision::Initialize( core::Facade& facade )
{
    USE_HOOK( ComputeAgentFutureObjectCollision, facade );
    USE_HOOK( CreatePath, facade );
    USE_HOOK( CreatePathList, facade );
    USE_HOOK( GetAgentFuturePosition, facade );
    USE_HOOK( GetPathPoints, facade );
    USE_HOOK( IsAgentMovingOnPath, facade );
    USE_HOOK( PathGetLastPointOfPath, facade );
    USE_HOOK( GetPerceptionId, facade );
    USE_HOOK( GetDangerosity, facade );
    USE_HOOK( GetMaxRangeToFireOn, facade );
    USE_HOOK( GetMinRangeToFireOn, facade );
    USE_HOOK( GetMaxRangeToFireOnWithPosture, facade );
    USE_HOOK( GetMinRangeToFireOnWithPosture, facade );
    USE_HOOK( GetMaxRangeToFire, facade );
    USE_HOOK( GetMaxRangeToIndirectFire, facade );
    USE_HOOK( GetMinRangeToIndirectFire, facade );
    USE_HOOK( GetForceRatio, facade );
    USE_HOOK( GetDangerousEnemies, facade );
    USE_HOOK( GetAmmunitionForIndirectFire, facade );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision constructor
// Created: SLI 2012-02-01
// -----------------------------------------------------------------------------
RolePion_Decision::RolePion_Decision( MIL_AgentPion& pion, unsigned int gcPause, unsigned int gcMult, const Sink& sink )
    : DEC_RolePion_Decision( pion, gcPause, gcMult )
    , sink_  ( sink )
    , facade_( sink.GetFacade() )
    , model_ ( sink.GetModel() )
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
// Name: RolePion_Decision::serialize
// Created: SLI 2012-02-01
// -----------------------------------------------------------------------------
template< typename Archive >
void RolePion_Decision::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< DEC_RolePion_Decision >( *this );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterCommand
// Created: SLI 2012-04-03
// -----------------------------------------------------------------------------
template< typename Result, typename Function >
void RolePion_Decision::RegisterCommand( const std::string& name, Function fun )
{
    RegisterFunction( name, boost::function< Result >( boost::bind( fun, boost::ref( facade_ ), boost::ref( GetPion() ) ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterCommand
// Created: SLI 2012-04-03
// -----------------------------------------------------------------------------
template< typename Result, typename Function, typename Arg1 >
void RolePion_Decision::RegisterCommand( const std::string& name, Function fun, Arg1 arg1 )
{
    RegisterFunction( name, boost::function< Result >( boost::bind( fun, boost::ref( facade_ ), boost::ref( GetPion() ), arg1 ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterCommand
// Created: SLI 2012-04-03
// -----------------------------------------------------------------------------
template< typename Result, typename Function, typename Arg1, typename Arg2 >
void RolePion_Decision::RegisterCommand( const std::string& name, Function fun, Arg1 arg1, Arg2 arg2 )
{
    RegisterFunction( name, boost::function< Result >( boost::bind( fun, boost::ref( facade_ ), boost::ref( GetPion() ), arg1, arg2 ) ) );
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterCommand
// Created: SLI 2012-04-03
// -----------------------------------------------------------------------------
template< typename Result, typename Function, typename Arg1, typename Arg2, typename Arg3 >
void RolePion_Decision::RegisterCommand( const std::string& name, Function fun, Arg1 arg1, Arg2 arg2, Arg3 arg3 )
{
    RegisterFunction( name, boost::function< Result >( boost::bind( fun, boost::ref( facade_ ), boost::ref( GetPion() ), arg1, arg2, arg3 ) ) );
}

namespace core
{
    template<>
    class UserData< boost::shared_ptr< DEC_Path_ABC > > : public UserData_ABC
    {
    public:
        //! @name Constructors/Destructor
        //@{
        UserData( boost::shared_ptr< DEC_Path_ABC > userData )
            : userData_( boost::dynamic_pointer_cast< PathAdapter >( userData )->Get() )
        {}
        //@}

        //! @name Operations
        //@{
        virtual void* Get() const
        {
            return static_cast< void* >( const_cast< boost::shared_ptr< movement::Path_ABC >* >( &userData_ ) );
        }
        //@}

    private:
        //! @name Member data
        //@{
        boost::shared_ptr< movement::Path_ABC > userData_;
        //@}
    };
}

namespace
{
    std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > NextObjectOnPath( const MIL_ObjectFilter& filter, const MIL_Agent_ABC& agent, const core::Model& model )
    {
        boost::shared_ptr< DEC_Knowledge_Object > pObjectColliding;
        double rDistanceCollision = 0;
        KnowledgeCache cache;
        agent.GetArmy().GetKnowledge().GetObjectsAtInteractionHeight( cache.objectsToAvoid_, agent, filter );
        std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > result;
        if( cache.objectsToAvoid_.empty() || ! GET_HOOK( ComputeAgentFutureObjectCollision )( core::Convert( &model[ "entities" ][ agent.GetID() ] ), cache, rDistanceCollision, pObjectColliding ) )
        {
            result.first = false;
            return result;
        }
        if( !pObjectColliding || !pObjectColliding->IsValid() )
            throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
        result.first = true;
        result.second.first = pObjectColliding;
        result.second.second = MIL_Tools::ConvertSimToMeter( rDistanceCollision );
        return result;
    }

    std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > > GetNextObjectOnPath( const MIL_Agent_ABC& agent, const core::Model& model, boost::shared_ptr< DEC_Knowledge_Object > oId, const std::vector< std::string >& params )
    {
        if( params.empty() )
        {
            MIL_DangerousObjectFilter filter;
            return NextObjectOnPath( filter, agent, model );
        }
        else
        {
            MIL_ObjectFilter filter( params );
            return NextObjectOnPath( filter, agent, model );
        }
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
                    if( !canRemove_ && component.CanRemoveFromPath( type_, false ) ) // $$$$ BCI 2011-06-20: comment savoir si l'objet est min� ou pas?
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
        const PathAdapter* adapter = dynamic_cast< const PathAdapter* >( pPath );
        if( ! adapter )
            return false;
        return GET_HOOK( IsAgentMovingOnPath )( agent.GetID(), adapter->Get() );
    }
    boost::shared_ptr< DEC_Path_ABC > CreatePathToPoint( MIL_AgentPion& agent, const core::Model& model, MT_Vector2D* pEnd, int pathType )
    {
        assert( pEnd );
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        boost::shared_ptr< PathAdapter > pPath = PathAdapter::Add( entity, GET_HOOK( CreatePath )( core::Convert( &entity ), *pEnd, pathType ) );
        pPath->ComputePath();
        return pPath;
    }
    boost::shared_ptr< DEC_Path_ABC > CreatePathToPointList( MIL_AgentPion& agent, const core::Model& model, std::vector< boost::shared_ptr< MT_Vector2D > > listPt, int pathType  )
    {
        assert( !listPt.empty() );
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        boost::shared_ptr< PathAdapter > pPath = PathAdapter::Add( entity, GET_HOOK( CreatePathList )( core::Convert( &entity ), listPt, pathType ) );
        pPath->ComputePath();
        return pPath;
    }
    boost::shared_ptr< DEC_Path_ABC > CreatePathToPointBM( MIL_AgentPion& agent, const core::Model& model, boost::shared_ptr< MT_Vector2D > end, int pathType )
    {
        return CreatePathToPoint( agent, model, end.get(), pathType );
    }
    bool ShouldEmbark( MIL_AgentPion& agent, boost::shared_ptr< DEC_Path_ABC > path )
    {
        if( !path )
            throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
        const double length = path->GetLength();
        const double maxSpeedUnloaded = agent.GetRole< moving::PHY_RoleAction_InterfaceMoving >().GetTheoricMaxSpeed( false );
        if( maxSpeedUnloaded == 0 )
            return true;
        const double timeUnloaded = length / maxSpeedUnloaded;
        const double speed = agent.GetRole< moving::PHY_RoleAction_InterfaceMoving >().GetTheoricMaxSpeed( true );
        if( !speed )
            return false;
        const double timeLoaded = ( length / speed ) + DEC_AgentFunctions::GetLoadingTime( agent ) * 60 + DEC_AgentFunctions::GetUnloadingTime( agent ) * 60; //Conversion minutes into hours
        return ( timeLoaded < timeUnloaded );
    }
    int GetPathState( DEC_Path_ABC* pPath )
    {
        if( !pPath )
            throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
        return pPath->GetState();
    }
    boost::shared_ptr< MT_Vector2D > GetLastPointOfPath( const DEC_Path_ABC* pPath )
    {
        const PathAdapter* adapter = dynamic_cast< const PathAdapter* >( pPath );
        if( ! adapter )
            return boost::shared_ptr< MT_Vector2D >();
        return GET_HOOK( PathGetLastPointOfPath )( adapter->Get() );
    }

    template< typename T >
    unsigned int StartAction( core::Facade& facade, MIL_AgentPion& pion, const std::string& command, T arg )
    {
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "parameters" ].AddElement().SetUserData( arg );
        return facade.StartCommand( command, parameters );
    }
    unsigned int StopAction( core::Facade& facade, PHY_Actor& actor, unsigned int command )
    {
        facade.StopCommand( command );
        actor.UnregisterAction( command );
        return 0u;
    }
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterControlActions
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void RolePion_Decision::RegisterControlActions()
{
    RegisterCommand< unsigned int( unsigned int ) >( "DEC__StopAction", &StopAction, _1 );
    RegisterFunction( "DEC_PauseAction",
        boost::function< void( unsigned int ) >( boost::bind( &core::Facade::PauseCommand, &facade_, _1 ) ) );
    RegisterFunction( "DEC_ReprendAction",
        boost::function< void( unsigned int ) >( boost::bind( &core::Facade::ResumeCommand, &facade_, _1 ) ) );
}

namespace
{
    void Orientate( core::Facade& facade, MIL_AgentPion& pion, boost::shared_ptr< MT_Vector2D > direction )
    {
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "direction/x" ] = direction->rX_;
        parameters[ "direction/y" ] = direction->rY_;
        facade.PostCommand( "orientate", parameters );
    }
    unsigned int StartTirDirect( core::Facade& facade, MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Agent > pEnemy, double percentage, int firingMode, int ammoDotationClass )
    {
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "enemy" ] = pEnemy->GetAgentKnown().GetID();
        parameters[ "percentage" ] = percentage;
        parameters[ "mode" ] = firingMode;
        parameters[ "dotation" ] = ammoDotationClass;
        return facade.StartCommand( "direct fire command", parameters );
    }
}

// -----------------------------------------------------------------------------
// Name: RolePion_Decision::RegisterActions
// Created: SLI 2012-03-20
// -----------------------------------------------------------------------------
void RolePion_Decision::RegisterActions()
{
    RegisterCommand< unsigned int( boost::shared_ptr< DEC_Path_ABC > ) >( "DEC_StartDeplacement", &StartAction< boost::shared_ptr< DEC_Path_ABC > >, "move", _1 );
    RegisterCommand< void( boost::shared_ptr< MT_Vector2D > ) >( "DEC_Orientate", &Orientate, _1 );
    RegisterFunction( "DEC_StartTirDirect",
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Agent >, double, int, int ) >( boost::bind( &StartTirDirect, boost::ref( facade_ ), boost::ref( GetPion() ), _1, _2, _3, _4 ) ) );
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
    RegisterFunction( "DEC_GetNextRemovableObjectOnPath",
        boost::function< std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > >( const DEC_Decision_ABC&, boost::shared_ptr< DEC_Knowledge_Object >, float ) >(
        boost::bind( &GetNextRemovableObjectOnPath, boost::cref( model_ ), _1, _2 ) ) );
    RegisterFunction( "DEC_ShouldEmbark",
        boost::function< bool( boost::shared_ptr< DEC_Path_ABC > ) >( boost::bind( &ShouldEmbark, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Itineraire_Etat",
        boost::function< int ( DEC_Path_ABC* ) >( boost::bind( &GetPathState, _1 ) ) );
}

namespace sword
{
    namespace movement
    {
        class PathPoint;
    }
}
namespace
{
    void ApplyFunction( boost::shared_ptr< movement::PathPoint > point, void* userData )
    {
        (*static_cast< boost::function< void( boost::shared_ptr< movement::PathPoint > ) >* >( userData ))( point );
    }
    void FillPoints( std::vector< boost::shared_ptr< PathPointAdapter > >& path, unsigned int entity, boost::shared_ptr< movement::PathPoint > point )
    {
        path.push_back( AdaptPoint( entity, point ) );
    }
    std::vector< boost::shared_ptr< PathPointAdapter > > GetPointsCategory( MIL_AgentPion& agent )
    {
        std::vector< boost::shared_ptr< PathPointAdapter > > result;
        boost::function< void( boost::shared_ptr< movement::PathPoint > ) > callback =
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

namespace
{
    void TogglePerception( core::Facade& facade, MIL_AgentPion& pion, const std::string& perception, bool isActivated )
    {
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "perception" ] = perception;
        parameters[ "activated" ] = isActivated;
        facade.PostCommand( "toggle perception", parameters );
    }
    void ToggleRadar( core::Facade& facade, MIL_AgentPion& pion, bool isActivated, int radar )
    {
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "activated" ] = isActivated;
        parameters[ "radar-class" ] = radar;
        facade.PostCommand( "toggle radar", parameters );
    }
    template< typename T >
    int EnableLocalizedRadar( core::Facade& facade, MIL_AgentPion& pion, int radar, T localisation ) // $$$$ _RC_ SLI 2012-03-28: DRY with ToggleRadar
    {
        if( !localisation )
            throw std::runtime_error( __FUNCTION__ ": invalid localization parameter while enabling radar." );
        const int perceptionId = GET_HOOK( GetPerceptionId )();
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "activated" ] = true;
        parameters[ "radar-class" ] = radar;
        parameters[ "perception-id" ] = perceptionId;
        parameters[ "localization" ].SetUserData( localisation );
        facade.PostCommand( "toggle localized radar", parameters );
        return perceptionId;
    }
    int EnableLocalizedRadarOnPoint( core::Facade& facade, MIL_AgentPion& pion, int radar, boost::shared_ptr< MT_Vector2D > point )
    {
        if( !point )
            throw std::runtime_error( __FUNCTION__ ": invalid localization parameter while enabling radar." );
        return EnableLocalizedRadar< boost::shared_ptr< TER_Localisation > >( facade, pion, radar, boost::make_shared< TER_Localisation >( TER_Localisation::ePoint, boost::assign::list_of( *point ) ) );
    }
    void DisableLocalizedRadar( core::Facade& facade, MIL_AgentPion& pion, int radar, int perceptionId )
    {
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "activated" ] = false;
        parameters[ "radar-class" ] = radar;
        parameters[ "perception-id" ] = perceptionId;
        facade.PostCommand( "toggle localized radar", parameters );
    }
    template< typename T >
    int EnableLocalizedDetection( core::Facade& facade, MIL_AgentPion& pion, const std::string& perception, T localisation )
    {
        if( !localisation )
            throw std::runtime_error( __FUNCTION__ ": invalid localization parameter while enabling flying shell detection." );
        const int perceptionId = GET_HOOK( GetPerceptionId )();
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "activated" ] = true;
        parameters["perception" ] = perception;
        parameters[ "perception-id" ] = perceptionId;
        parameters[ "localization" ].SetUserData( localisation );
        facade.PostCommand( "toggle localized perception", parameters );
        return perceptionId;
    }
    void DisableLocalizedDetection( core::Facade& facade, MIL_AgentPion& pion, const std::string& perception, int perceptionId )
    {
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "activated" ] = false;
        parameters[ "perception" ] = perception;
        parameters[ "perception-id" ] = perceptionId;
        facade.PostCommand( "toggle localized perception", parameters );
    }
    int EnableRecoOnLocation( core::Facade& facade, MIL_AgentPion& pion, const TER_Localisation* localisation, boost::optional< float > growthSpeed )
    {
        if( !localisation )
            throw std::runtime_error( __FUNCTION__ ": invalid localization parameter while enabling reco on location detection." );
        const int perceptionId = GET_HOOK( GetPerceptionId )();
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "activated" ] = true;
        parameters[ "perception-id" ] = perceptionId;
        parameters[ "has-growth-speed" ] = static_cast< bool >( growthSpeed );
        parameters[ "growth-speed" ] = growthSpeed ? *growthSpeed : 0;
        parameters[ "localization" ].SetUserData( localisation );
        facade.PostCommand( "toggle reco", parameters );
        return perceptionId;
    }
    void DisableIdentifiedCommand( core::Facade& facade, MIL_AgentPion& pion, const std::string& command, int perceptionId )
    {
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "activated" ] = false;
        parameters[ "perception-id" ] = perceptionId;
        facade.PostCommand( command, parameters );
    }
    int EnableObjectDetection( core::Facade& facade, MIL_AgentPion& pion, const TER_Localisation* localisation, const MT_Vector2D* center, double speed )
    {
        if( !localisation )
            throw std::runtime_error( __FUNCTION__ ": invalid localization parameter while enabling object detection on location." );
        const int perceptionId = GET_HOOK( GetPerceptionId )();
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "activated" ] = true;
        parameters[ "perception-id" ] = perceptionId;
        parameters[ "center/x" ] = center->rX_;
        parameters[ "center/y" ] = center->rY_;
        parameters[ "speed" ] = speed;
        parameters[ "localization" ].SetUserData( localisation );
        facade.PostCommand( "toggle object detection", parameters );
        return perceptionId;
    }
    int EnableRecognitionPoint( core::Facade& facade, MIL_AgentPion& pion, MT_Vector2D* center, double size, double growthSpeed )
    {
        if( !center )
            throw std::runtime_error( __FUNCTION__ ": invalid center parameter while enabling recognition on point." );
        const int perceptionId = GET_HOOK( GetPerceptionId )();
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "activated" ] = true;
        parameters[ "perception-id" ] = perceptionId;
        parameters[ "center/x" ] = center->rX_;
        parameters[ "center/y" ] = center->rY_;
        parameters[ "growth-speed" ] = MIL_Tools::ConvertSpeedMosToSim( growthSpeed );
        parameters[ "size" ] = size;
        facade.PostCommand( "toggle recognition point", parameters );
        return perceptionId;
    }
    template< typename T >
    void SwitchVisionMode( core::Facade& facade, MIL_AgentPion& pion, const std::string& mode, T location )
    {
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        parameters[ "mode" ] = mode;
        parameters[ "location/x" ] = location->rX_;
        parameters[ "location/y" ] = location->rY_;
        facade.PostCommand( "vision", parameters );
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
    RegisterCommand< void( int ) >                                 ( "DEC_Perception_ActiverRadar", &ToggleRadar, true, _1 );
    RegisterCommand< void( int ) >                                 ( "DEC_Perception_DesactiverRadar", &ToggleRadar, false, _1 );
    RegisterCommand< int( int, const TER_Localisation* ) >         ( "DEC_Perception_ActiverRadarSurLocalisation", &EnableLocalizedRadar< const TER_Localisation* >, _1, _2 );
    RegisterCommand< int( int, boost::shared_ptr< MT_Vector2D > ) >( "DEC_Perception_ActiverRadarSurPointPtr", &EnableLocalizedRadarOnPoint, _1, _2 );
    RegisterCommand< void( int, int ) >                            ( "DEC_Perception_DesactiverRadarSurLocalisation", &DisableLocalizedRadar, _1, _2 );
    RegisterCommand< int( const TER_Localisation* ) >              ( "DEC_Perception_ActiverPerceptionTirsIndirect", &EnableLocalizedDetection< const TER_Localisation* >, "flying-shell", _1 );
    RegisterCommand< void( int ) >                                 ( "DEC_Perception_DesactiverPerceptionTirsIndirect", &DisableLocalizedDetection, "flying-shell", _1 );
    RegisterCommand< int( const TER_Localisation* ) >              ( "DEC_Perception_ActiverReconnaissanceLocalisation", &EnableRecoOnLocation, _1, boost::optional< float >() );
    RegisterCommand< int( const TER_Localisation*, float ) >       ( "DEC_Perception_ActivateLocationProgressiveRecce", &EnableRecoOnLocation, _1, _2 );
    RegisterCommand< void( int ) >                                 ( "DEC_Perception_DesactiverReconnaissanceLocalisation", &DisableIdentifiedCommand, "toggle reco", _1 );
    RegisterCommand< int( UrbanObjectWrapper* ) >                  ( "DEC_Perception_ActiverReconnaissanceDansBlocUrbain", &EnableLocalizedDetection< UrbanObjectWrapper* >, "urban", _1 );
    RegisterCommand< void( int ) >                                 ( "DEC_Perception_DesactiverReconnaissanceDansBlocUrbain", &DisableLocalizedDetection, "urban", _1 );
    RegisterCommand< int( const TER_Localisation*,
                          const MT_Vector2D*, double ) >           ( "DEC_Perception_ActiverDetectionObjetLocalisation", &EnableObjectDetection, _1, _2, _3 );
    RegisterCommand< void( int ) >                                 ( "DEC_Perception_DesactiverDetectionObjetLocalisation", &DisableIdentifiedCommand, "toggle object detection", _1 );
    RegisterCommand< int( MT_Vector2D*, double, double ) >         ( "DEC_Perception_ActiverReconnaissancePoint", &EnableRecognitionPoint, _1, _2, _3 );
    RegisterCommand< void( int ) >                                 ( "DEC_Perception_DesactiverReconnaissancePoint", &DisableIdentifiedCommand, "toggle recognition point", _1 );
    RegisterCommand< void( boost::shared_ptr< MT_Vector2D > ) >    ( "DEC_Perception_VisionVerrouilleeSurDirection", &SwitchVisionMode< boost::shared_ptr< MT_Vector2D > >, "direction", _1 );
    RegisterCommand< void( const MT_Vector2D* ) >                  ( "DEC_Perception_VisionVerrouilleeSurPoint", &SwitchVisionMode< const MT_Vector2D* >, "location", _1 );
    RegisterCommand< void( boost::shared_ptr< MT_Vector2D > ) >    ( "DEC_Perception_VisionVerrouilleeSurPointPtr", &SwitchVisionMode< boost::shared_ptr< MT_Vector2D > >, "location", _1 );
    RegisterCommand< void() >                                      ( "DEC_Perception_VisionNormale", &SwitchVisionMode< boost::shared_ptr< MT_Vector2D > >, "normal", boost::make_shared< MT_Vector2D >() );
}

namespace
{
    bool CanFire( const SWORD_Model* component )
    {
        return (*core::Convert( component ))[ "component" ].GetUserData< PHY_ComposantePion >().CanFire();
    }
    bool IsMajor( const SWORD_Model* component )
    {
        return (*core::Convert( component ))[ "major" ];
    }
    bool CanMajorFire( const SWORD_Model* component )
    {
        return IsMajor( component ) && CanFire( component );
    }
    double GetMaxRangeToFireOnEnemy( const MIL_AgentPion& agent, const core::Model& model, boost::shared_ptr< DEC_Knowledge_Agent > target, float rWantedPH, const PHY_DotationCategory* dotation )
    {
        if( ! target || ! target->IsValid() )
            return -1;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        const unsigned int id = entity[ "knowledges" ];
        const core::Model& knowledge = model[ "knowledges" ][ id ][ target->GetAgentKnown().GetID() ];
        return GET_HOOK( GetMaxRangeToFireOn )( core::Convert( &entity ), core::Convert( &knowledge ), &CanFire, rWantedPH, dotation ? dotation->GetName().c_str() : 0 );
    }
    double GetMinRangeToFireOnEnemy( const MIL_AgentPion& agent, const core::Model& model, boost::shared_ptr< DEC_Knowledge_Agent > target, float rWantedPH )
    {
        if( ! target || ! target->IsValid() )
            return -1;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        const unsigned int id = entity[ "knowledges" ];
        const core::Model& knowledge = model[ "knowledges" ][ id ][ target->GetAgentKnown().GetID() ];
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
        const core::Model& knowledge = model[ "knowledges" ][ id ][ target->GetAgentKnown().GetID() ];
        return GET_HOOK( GetMaxRangeToFireOnWithPosture )( core::Convert( &entity ), core::Convert( &knowledge ), &CanFire, rWantedPH );
    }
    double GetMinRangeToFireOnEnemyActualPosture( const MIL_AgentPion& agent, const core::Model& model, boost::shared_ptr< DEC_Knowledge_Agent > target, float rWantedPH )
    {
        if( ! target || ! target->IsValid() )
            return -1;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        const unsigned int id = entity[ "knowledges" ];
        const core::Model& knowledge = model[ "knowledges" ][ id ][ target->GetAgentKnown().GetID() ];
        const double range = GET_HOOK( GetMinRangeToFireOnWithPosture )( core::Convert( &entity ), core::Convert( &knowledge ), &CanFire, rWantedPH );
        if( range == std::numeric_limits< double >::max() )
            return -1;
        return range;
    }
    bool CanFireWhenUnloaded( const SWORD_Model* component )
    {
        const PHY_ComposantePion& comp = (*core::Convert( component ))[ "component" ].GetUserData< PHY_ComposantePion >();
        return comp.IsLoadable() && comp.CanFireWhenUnloaded();
    }
    double GetMaxRangeToFireOnEnemyWhenUnloaded( const MIL_AgentPion& agent, const core::Model& model, boost::shared_ptr< DEC_Knowledge_Agent > target, float rWantedPH )
    {
        if( ! target || ! target->IsValid() )
            return -1;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        const unsigned int id = entity[ "knowledges" ];
        const core::Model& knowledge = model[ "knowledges" ][ id ][ target->GetAgentKnown().GetID() ];
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
        if( enemy->GetComposantes().empty() ) // $$$$ MCO 2012-06-06: a knowledge has either all or none of the components
            return 0;
        const core::Model& firer = model[ "entities" ][ enemy->GetAgentKnown().GetID() ];
        const core::Model& target = model[ "entities" ][ agent.GetID() ];
        return GET_HOOK( GetMaxRangeToFireOn )( core::Convert( &firer ), core::Convert( &target ), &True, rWantedPH, 0 );
    }
    double GetMaxRangeToFire( const MIL_Agent_ABC& agent, const core::Model& model, float rWantedPH )
    {
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        const double range = GET_HOOK( GetMaxRangeToFire )( core::Convert( &entity ), &IsMajor, rWantedPH );
        if( range == std::numeric_limits< double >::max() )
            return -1;
        return range;
    }
    double GetMaxRangeToIndirectFire( const MIL_Agent_ABC& agent, const core::Model& model, const PHY_DotationCategory* dotation )
    {
        if( ! dotation )
            return -1;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        return GET_HOOK( GetMaxRangeToIndirectFire )( core::Convert( &entity ), &CanFire, dotation->GetName().c_str(), true );
    }
    double GetMinRangeToIndirectFire( const MIL_Agent_ABC& agent, const core::Model& model, const PHY_DotationCategory* dotation )
    {
        if( ! dotation )
            return -1;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        const double range = GET_HOOK( GetMinRangeToIndirectFire )( core::Convert( &entity ), &CanFire, dotation->GetName().c_str(), true );
        if( range == std::numeric_limits< double >::max() )
            return -1;
        return range;
    }
    double GetMaxRangeToIndirectFireWithoutAmmoCheck( const MIL_Agent_ABC& agent, const core::Model& model, const PHY_DotationCategory* dotation )
    {
        if( ! dotation )
            return -1;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        return GET_HOOK( GetMaxRangeToIndirectFire )( core::Convert( &entity ), &CanFire, dotation->GetName().c_str(), false );
    }
    double GetMinRangeToIndirectFireWithoutAmmoCheck( const MIL_Agent_ABC& agent, const core::Model& model, const PHY_DotationCategory* dotation )
    {
        if( ! dotation )
            return -1;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        const double range = GET_HOOK( GetMinRangeToIndirectFire )( core::Convert( &entity ), &CanFire, dotation->GetName().c_str(), false );
        if( range == std::numeric_limits< double >::max() )
            return -1;
        return range;
    }
    bool NullFilter( const SWORD_Model* /*component*/ )
    {
        return true;
    }
    double GetTheoricMaxRangeToIndirectFire( const MIL_Agent_ABC& agent, const core::Model& model, const PHY_DotationCategory* dotation )
    {
        if( ! dotation )
            return -1;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        return GET_HOOK( GetMaxRangeToIndirectFire )( core::Convert( &entity ), &NullFilter, dotation->GetName().c_str(), true );
    }
    double GetTheoricMinRangeToIndirectFire( const MIL_Agent_ABC& agent, const core::Model& model, const PHY_DotationCategory* dotation )
    {
        if( ! dotation )
            return -1;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        const double range = GET_HOOK( GetMinRangeToIndirectFire )( core::Convert( &entity ), &NullFilter, dotation->GetName().c_str(), true );
        if( range == std::numeric_limits< double >::max() )
            return -1;
        return range;
    }
    const PHY_DotationCategory* GetAmmunitionForIndirectFire( const MIL_Agent_ABC& agent, const core::Model& model, int indirectFireDotationClassID, const MT_Vector2D* pTarget )
    {
        if( ! pTarget )
            return 0;
        const PHY_IndirectFireDotationClass* pClass = PHY_IndirectFireDotationClass::Find( indirectFireDotationClassID );
        if( ! pClass )
            return 0;
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        const char* dotation = GET_HOOK( GetAmmunitionForIndirectFire )( core::Convert( &model ), core::Convert( &entity ), pClass->GetName().c_str(), *pTarget );
        if( ! dotation )
            return 0;
        const PHY_DotationCategory* category = PHY_DotationType::FindDotationCategory( dotation );
        if( ! category )
            MT_LOG_ERROR_MSG( "Unknown dotation category in DirectFirePionEventListener : " << dotation );
        return category;
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
    RegisterFunction( "DEC_Tir_PorteeTheoriqueMaxTirIndirect",
        boost::function< double( const PHY_DotationCategory* ) >( boost::bind( &GetTheoricMaxRangeToIndirectFire, boost::ref( GetPion() ), boost::ref( model_ ), _1 ) ) );
    RegisterFunction( "DEC_Tir_PorteeTheoriqueMinTirIndirect",
        boost::function< double( const PHY_DotationCategory* ) >( boost::bind( &GetTheoricMinRangeToIndirectFire, boost::ref( GetPion() ), boost::ref( model_ ), _1 ) ) );
    RegisterFunction( "DEC_Tir_LancerFumigeneSurConnaissance", // $$$$ MCO 2012-06-25: should be a command
        boost::function< void( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_FireFunctions::ThrowSmokeOnKnowledgeAgent, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Tir_MunitionPourTirIndirect",
        boost::function< const PHY_DotationCategory* ( int, const MT_Vector2D* ) >( boost::bind( &GetAmmunitionForIndirectFire, boost::ref( GetPion() ), boost::ref( model_ ), _1, _2 ) ) );
    RegisterFunction( "DEC_Pion_InterdireMunition",
        boost::function< void( const PHY_DotationCategory* ) >( boost::bind( &DEC_FireFunctions::ForbidAmmunition, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Pion_AutoriserMunition",
        boost::function< void( const PHY_DotationCategory* ) >( boost::bind( &DEC_FireFunctions::AllowAmmunition, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Pion_AutoriserToutesMunitions",
        boost::bind( &DEC_FireFunctions::AllowAllAmmunitions, boost::ref( GetPion() ) ) );
}

namespace
{
    double GetDangerosity( const MIL_AgentPion& agent, const core::Model& model, const DEC_Knowledge_Agent& target, float rDistBtwSourceAndTarget, bool checkAmmo )
    {
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        const unsigned int id = entity[ "knowledges" ];
        const core::Model& knowledge = model[ "knowledges" ][ id ][ target.GetAgentKnown().GetID() ];
        return GET_HOOK( GetDangerosity )( core::Convert( &entity ), core::Convert( &knowledge ), &CanMajorFire, rDistBtwSourceAndTarget, checkAmmo );
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
        // Fight score
        const MT_Vector3D sourcePosition( position->rX_, position->rY_, 0);
        const MT_Vector3D targetPosition  ( pTargetKnowledge->GetPosition().rX_, pTargetKnowledge->GetPosition().rY_, pTargetKnowledge->GetAltitude() );
        const float rDistBtwSourceAndTarget = static_cast< float >( sourcePosition.Distance( targetPosition ) );
        double rDangerosity = GetDangerosity( agent, model, *pTargetKnowledge, rDistBtwSourceAndTarget, false );
        //DegradeDangerosity( rDangerosity );//@TODO MGD before commit
        return rDangerosity;
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
        const core::Model& entity = model[ "entities" ][ agent.GetID() ];
        return GET_HOOK( GetForceRatio )( Convert( &model ), Convert( &entity ) );
    }
    void Populate( const SWORD_Model* knowledge, void* userData )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > agent = (*core::Convert( knowledge ))[ "agent" ].GetUserData< boost::shared_ptr< DEC_Knowledge_Agent > >();
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
        boost::function< double( const DEC_Decision_ABC*, const core::Model& ) >( boost::bind( &GetRapForLocalAgent, _1, boost::cref( model_ ) ) ) );
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
