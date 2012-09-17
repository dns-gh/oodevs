// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "PerceptionView.h"
#include "PerceptionLevel.h"
#include "PerceptionObserver_ABC.h"
#include "PerceptionSurfaceAgent.h"
#include "PerceptionSurfaceObject.h"
#include "SurfacesAgent_ABC.h"
#include "SurfacesAgentVisitor_ABC.h"
#include "SurfacesObject_ABC.h"
#include "SurfacesObjectVisitor_ABC.h"
#include "TargetPerceptionVisitor_ABC.h"
#include "PerceptionComputer.h"
#include "ZURBPerceptionComputer.h"
#include "ZOPerceptionComputer.h"
#include "wrapper/Hook.h"
#include "wrapper/View.h"
#include "wrapper/Event.h"
#include "simulation_kernel/Entities/Orders/MIL_Report.h" // $$$$ MCO : for enums

DECLARE_HOOK( IsInCity, bool, ( const SWORD_Model* entity ) )
DECLARE_HOOK( BelongsToKnowledgeGroup, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) )
DECLARE_HOOK( IsAgentPerceptionDistanceHacked, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) )
DECLARE_HOOK( IsObjectPerceptionDistanceHacked, bool, ( const SWORD_Model* perceiver, const MIL_Object_ABC* object ) )
DECLARE_HOOK( IsPopulationFlowPerceptionDistanceHacked, bool, ( const SWORD_Model* perceiver, const MIL_PopulationFlow* flow ) )
DECLARE_HOOK( IsPopulationConcentrationPerceptionDistanceHacked, bool, ( const SWORD_Model* perceiver, const MIL_PopulationConcentration* concentration ) )
DECLARE_HOOK( GetHackedPerceptionLevel, int, ( const SWORD_Model* perceiver, const SWORD_Model* target ) )
DECLARE_HOOK( GetObjectPerceptionLevel, int, ( const SWORD_Model* perceiver, const MIL_Object_ABC* object ) )
DECLARE_HOOK( GetPopulationFlowPerceptionLevel, int, ( const SWORD_Model* perceiver, const MIL_PopulationFlow* flow ) )
DECLARE_HOOK( GetPopulationConcentrationPerceptionLevel, int, ( const SWORD_Model* perceiver, const MIL_PopulationConcentration* concentration ) )
DECLARE_HOOK( CanBeSeen, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) )
DECLARE_HOOK( CanObjectBePerceived, bool, ( const MIL_Object_ABC* object ) )
DECLARE_HOOK( CanPopulationFlowBePerceived, bool, ( const MIL_PopulationFlow* flow ) )
DECLARE_HOOK( CanPopulationConcentrationBePerceived, bool, ( const MIL_PopulationConcentration* concentration ) )
DECLARE_HOOK( IsCivilian, bool, ( const SWORD_Model* agent ) )
DECLARE_HOOK( IsAgentNewlyPerceived, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target, int level ) )
DECLARE_HOOK( IsPopulationFlowNewlyPerceived, bool, ( const SWORD_Model* perceiver, const MIL_PopulationFlow* flow, int level ) )
DECLARE_HOOK( IsPopulationConcentrationNewlyPerceived, bool, ( const SWORD_Model* perceiver, const MIL_PopulationConcentration* concentration, int level ) )
DECLARE_HOOK( IsObjectUniversal, bool, ( const MIL_Object_ABC* object ) )
DECLARE_HOOK( GetPerceptionRandom, double, () )

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: PerceptionView constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PerceptionView::PerceptionView( const wrapper::View& /*model*/, const wrapper::View& entity, PerceptionObserver_ABC& observer )
    : identifier_( entity[ "identifier" ] )
    , observer_  ( observer )
    , wasInCity_ ( GET_HOOK( IsInCity )( entity ) ) // TODO SLG : Passer par algo car dépendance de role en perceiver et urbanlocation
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionView destructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PerceptionView::~PerceptionView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionView::Compute
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionView::Compute( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const MT_Vector2D& vPoint ) const
{
    PerceptionComputer< SurfacesAgentVisitor_ABC, PerceptionSurfaceAgent, MT_Vector2D > computer( perceiver, vPoint );
    if( IsEnabled( perceiver ) )
        surfaces.Apply( computer );
    return computer.GetLevel();
}

// -----------------------------------------------------------------------------
// Name: PerceptionView::Compute
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionView::Compute( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const wrapper::View& target ) const
{
    TransferPerception( perceiver, surfaces );
    if( GET_HOOK( BelongsToKnowledgeGroup )( perceiver, target ) )
        return PerceptionLevel::identified_;

    if( !IsEnabled( perceiver ) )
        return PerceptionLevel::notSeen_;
    
    const ZOPerceptionComputer computer;
    const PerceptionLevel& result = computer.ComputePerception( perceiver, surfaces, target );
    if( result == PerceptionLevel::notSeen_ || !GET_HOOK( IsInCity )( perceiver ) )
        return result;
    else
    {
        CIT_PerceptionTickMap it = perceptionsUnderway_.find( target[ "identifier" ] );
        unsigned int tick = 0;
        double roll = 0.f;
        if( it != perceptionsUnderway_.end() )
        {
            tick = it->second.first;
            roll = it->second.second;
        }
        else
            roll = static_cast< double >( GET_HOOK( GetPerceptionRandom )() );
        perceptionsBuffer_[ target[ "identifier" ] ] = std::pair< unsigned int, double >( tick + 1, roll );
        const ZURBPerceptionComputer computer( roll, tick );
        const PerceptionLevel& urbanResult = computer.ComputePerception( perceiver, surfaces, target );
        return result < urbanResult ? result : urbanResult;
    }
}

namespace
{
    void PostReport( size_t identifier, MIL_Report::E_EngineReport code )
    {
        wrapper::Event event( "report" );
        event[ "entity" ] = identifier;
        event[ "code" ] = code;
        event.Post();
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionView::Execute
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PerceptionView::Execute( const wrapper::View& /*model*/, const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_AgentPtrVector& perceivableAgents )
{
    if( IsEnabled( perceiver ) )
    {
        bool civiliansEncountered = false;
        for ( T_AgentPtrVector::const_iterator itAgent = perceivableAgents.begin(); itAgent != perceivableAgents.end(); ++itAgent )
        {
            const wrapper::View& agent = *itAgent;
            if( GET_HOOK( BelongsToKnowledgeGroup )( perceiver, agent ) )
                continue;

            if( GET_HOOK( IsAgentPerceptionDistanceHacked )( perceiver, agent ) )
            {
                const PerceptionLevel& level = PerceptionLevel::FindPerceptionLevel( GET_HOOK( GetHackedPerceptionLevel )( perceiver, agent ) );
                observer_.NotifyPerception( agent, level );
            }
            else if( GET_HOOK( CanBeSeen )( perceiver, agent ) )
            {
                const PerceptionLevel& level = Compute( perceiver, surfaces, agent );
                observer_.NotifyPerception( agent, level );
                if( GET_HOOK( IsAgentNewlyPerceived )( perceiver, agent, level.GetID() ) 
                    && !civiliansEncountered && GET_HOOK( IsCivilian )( agent ) )
                {
                    PostReport( identifier_, MIL_Report::eReport_CiviliansEncountered );
                    civiliansEncountered = true;
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionView::Compute
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionView::Compute( const wrapper::View& perceiver, const SurfacesObject_ABC& surfaces, const DEC_Knowledge_Object& knowledge ) const
{
    PerceptionComputer< SurfacesObjectVisitor_ABC, PerceptionSurfaceObject, DEC_Knowledge_Object > computer( perceiver, knowledge );
    if( IsEnabled( perceiver ) )
        surfaces.Apply( computer );
    return computer.GetLevel();
}

// -----------------------------------------------------------------------------
// Name: PerceptionView::Compute
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionView::Compute( const wrapper::View& perceiver, const SurfacesObject_ABC& surfaces, const MIL_Object_ABC& target ) const
{
    if( !IsEnabled( perceiver ) || !GET_HOOK( CanObjectBePerceived )( &target ) )
        return PerceptionLevel::notSeen_;

    if( GET_HOOK( IsObjectUniversal )( &target ) )
        return PerceptionLevel::identified_;

    PerceptionComputer< SurfacesObjectVisitor_ABC, PerceptionSurfaceObject, MIL_Object_ABC > computer( perceiver, target );
    surfaces.Apply( computer );
    return computer.GetLevel();
}

// -----------------------------------------------------------------------------
// Name: PerceptionView::Execute
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PerceptionView::Execute( const wrapper::View& perceiver, const SurfacesObject_ABC& surfaces, const T_ObjectVector& perceivableObjects )
{
    if( IsEnabled( perceiver ) )
    {
        for( T_ObjectVector::const_iterator itObject = perceivableObjects.begin(); itObject != perceivableObjects.end(); ++itObject )
        {
            MIL_Object_ABC* object = *itObject;
            if ( GET_HOOK( IsObjectPerceptionDistanceHacked )( perceiver, object ) )
            {
                const PerceptionLevel& level = PerceptionLevel::FindPerceptionLevel( GET_HOOK( GetObjectPerceptionLevel )( perceiver, object ) );
                observer_.NotifyPerception( object, level );
            }
            else
                observer_.NotifyPerception( object, Compute( perceiver, surfaces, *object ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionView::Compute
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionView::Compute( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const MIL_PopulationFlow& flow, T_PointVector& shape ) const
{
    if( !IsEnabled( perceiver ) || !GET_HOOK( CanPopulationFlowBePerceived )( &flow ) )
        return PerceptionLevel::notSeen_;
    struct PerceptionComputer : public SurfacesAgentVisitor_ABC
    {
        PerceptionComputer( const wrapper::View& perceiver, const MIL_PopulationFlow& flow )
            : perceiver_   ( perceiver )
            , flow_        ( flow )
            , pBestSurface_( 0 )
            , rBestCost_   ( std::numeric_limits< double >::min() )
        {}
        virtual bool Notify( const PerceptionSurfaceAgent& surface )
        {        
            const double rCost = surface.ComputePerceptionAccuracy( perceiver_, flow_ );
            if( rCost > rBestCost_ )
            {
                rBestCost_    = rCost;
                pBestSurface_ = &surface;
            }
            return false;
        }
        const wrapper::View& perceiver_;
        const MIL_PopulationFlow& flow_;
        const PerceptionSurfaceAgent* pBestSurface_;
        double rBestCost_;
    } computer( perceiver, flow );
    surfaces.Apply( computer );
    if( !computer.pBestSurface_ )
        return PerceptionLevel::notSeen_;
    return computer.pBestSurface_->ComputePerception( perceiver, flow, shape );
}

// -----------------------------------------------------------------------------
// Name: PerceptionView::Execute
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
void PerceptionView::Execute( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_ConstPopulationFlowVector& perceivableFlows )
{
    if( IsEnabled( perceiver ) )
    {
        bool civiliansEncountered = false;
        for( T_ConstPopulationFlowVector::const_iterator it = perceivableFlows.begin(); it != perceivableFlows.end(); ++it )
        {
            MIL_PopulationFlow* flow = const_cast< MIL_PopulationFlow* >( *it ); // $$$ RC LDC Should propagate constness to called methods instead
            T_PointVector shape;
            const PerceptionLevel* level = &PerceptionLevel::notSeen_;
            if ( GET_HOOK( IsPopulationFlowPerceptionDistanceHacked )( perceiver, flow ) )
                level = &PerceptionLevel::FindPerceptionLevel( GET_HOOK( GetPopulationFlowPerceptionLevel )( perceiver, flow ) );
            else
                level = &Compute( perceiver, surfaces, *flow, shape );
            observer_.NotifyPerception( flow, *level, shape );
            civiliansEncountered |= GET_HOOK( IsPopulationFlowNewlyPerceived )( perceiver, flow, level->GetID() );
        }
        if( civiliansEncountered )
            PostReport( identifier_, MIL_Report::eReport_CiviliansEncountered );
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionView::Compute
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionView::Compute( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const MIL_PopulationConcentration& target ) const
{
    if( !IsEnabled( perceiver ) || !GET_HOOK( CanPopulationConcentrationBePerceived )( &target ) )
        return PerceptionLevel::notSeen_;

    PerceptionComputer< SurfacesAgentVisitor_ABC, PerceptionSurfaceAgent, MIL_PopulationConcentration > computer( perceiver, target );
    surfaces.Apply( computer );
    return computer.GetLevel();
}

// -----------------------------------------------------------------------------
// Name: PerceptionView::Execute
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
void PerceptionView::Execute( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_ConstPopulationConcentrationVector& perceivableConcentrations )
{
    if( IsEnabled( perceiver ) )
    {
        bool civiliansEncountered = false;
        for( T_ConstPopulationConcentrationVector::const_iterator it = perceivableConcentrations.begin(); it != perceivableConcentrations.end(); ++it )
        {
            MIL_PopulationConcentration* concentration = const_cast< MIL_PopulationConcentration* >( *it ); // $$$ RC LDC Should propagate constness to called methods instead
            const PerceptionLevel* level = &PerceptionLevel::notSeen_;
            if ( GET_HOOK( IsPopulationConcentrationPerceptionDistanceHacked )( perceiver, concentration ) )
                level = &PerceptionLevel::FindPerceptionLevel( GET_HOOK( GetPopulationConcentrationPerceptionLevel )( perceiver, concentration ) );
            else
                level = &Compute( perceiver, surfaces, *concentration );
            observer_.NotifyPerception( concentration, *level );
            civiliansEncountered |= GET_HOOK( IsPopulationConcentrationNewlyPerceived )( perceiver, concentration, level->GetID() );
        }
        if( civiliansEncountered )
            PostReport( identifier_, MIL_Report::eReport_CiviliansEncountered );
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionView::FinalizePerception
// Created: LDC 2010-05-05
// -----------------------------------------------------------------------------
void PerceptionView::FinalizePerception( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces )
{
    if( !GET_HOOK( IsInCity )( perceiver ) )
    {
        struct FinalizeVisitor : public SurfacesAgentVisitor_ABC
        {
            virtual bool Notify( const PerceptionSurfaceAgent& surface )
            {
                const_cast< PerceptionSurfaceAgent& >( surface ).FinalizePerception();
                return false;
            }
        } visitor;
        surfaces.Apply( visitor );
    }
    else
    {
        perceptionsUnderway_ = perceptionsBuffer_;
        perceptionsBuffer_.clear();
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionView::TransferPerception
// Created: SLG 2010-05-07
// -----------------------------------------------------------------------------
void PerceptionView::TransferPerception( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces ) const
{
    bool isInCity = GET_HOOK( IsInCity )( perceiver );
    if( isInCity && !wasInCity_ )
    {
        perceptionsBuffer_.clear();
        perceptionsUnderway_.clear();
        struct TargetPerceptionVisitor : public SurfacesAgentVisitor_ABC
                                       , public TargetPerceptionVisitor_ABC
        {
            explicit TargetPerceptionVisitor( T_PerceptionTickMap& perceptionsBuffer )
                : perceptionsBuffer_( perceptionsBuffer )
            {}
            virtual bool Notify( const PerceptionSurfaceAgent& surface )
            {
                surface.Apply( *this );
                return false;
            }
            virtual void Notify( std::size_t targetIdentifier, unsigned int delay )
            {
                T_PerceptionTickMap::iterator it = perceptionsBuffer_.find( targetIdentifier );
                if( it != perceptionsBuffer_.end() )
                    perceptionsBuffer_[ targetIdentifier ] = std::pair< unsigned int, double >( std::max( it->second.first, delay ), GET_HOOK( GetPerceptionRandom )() );
                else
                    perceptionsBuffer_[ targetIdentifier ] = std::pair< unsigned int, double >( delay, GET_HOOK( GetPerceptionRandom )() );
            }
            T_PerceptionTickMap& perceptionsBuffer_;
        } visitor( perceptionsBuffer_ );
        surfaces.Apply( visitor );
        wasInCity_ = true;
    }
    else if( !isInCity && wasInCity_ )
    {
        struct TransferVisitor : public SurfacesAgentVisitor_ABC
        {
            explicit TransferVisitor( T_PerceptionTickMap& perceptionsUnderway )
                : perceptionsUnderway_( perceptionsUnderway )
            {}
            virtual bool Notify( const PerceptionSurfaceAgent& surface )
            {
                surface.TransferPerception( perceptionsUnderway_ );
                return false;
            }
            T_PerceptionTickMap& perceptionsUnderway_;
        } visitor( perceptionsUnderway_ );
        surfaces.Apply( visitor );
        wasInCity_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionView::IsEnabled
// Created: SLI 2012-06-04
// -----------------------------------------------------------------------------
bool PerceptionView::IsEnabled( const wrapper::View& perceiver ) const
{
    return perceiver[ "perceptions/sensor/activated" ];
}
