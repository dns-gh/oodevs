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
#include <boost/bind.hpp>

DECLARE_HOOK( IsInCity, bool, ( const SWORD_Model* entity ) )
DECLARE_HOOK( BelongsToKnowledgeGroup, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) )
DECLARE_HOOK( IsAgentPerceptionDistanceHacked, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) )
DECLARE_HOOK( IsObjectPerceptionDistanceHacked, bool, ( const SWORD_Model* perceiver, const SWORD_Model* object ) )
DECLARE_HOOK( IsPopulationElementPerceptionDistanceHacked, bool, ( const SWORD_Model* perceiver, const SWORD_Model* element ) )
DECLARE_HOOK( GetHackedPerceptionLevel, int, ( const SWORD_Model* perceiver, const SWORD_Model* target ) )
DECLARE_HOOK( GetObjectPerceptionLevel, int, ( const SWORD_Model* perceiver, const SWORD_Model* object ) )
DECLARE_HOOK( GetPopulationElementPerceptionLevel, int, ( const SWORD_Model* perceiver, const SWORD_Model* element ) )
DECLARE_HOOK( CanBeSeen, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) )
DECLARE_HOOK( CanObjectBePerceived, bool, ( const SWORD_Model* object ) )
DECLARE_HOOK( CanPopulationElementBePerceived, bool, ( const SWORD_Model* element ) )
DECLARE_HOOK( IsCivilian, bool, ( const SWORD_Model* agent ) )
DECLARE_HOOK( IsAgentNewlyPerceived, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target, int level ) )
DECLARE_HOOK( IsPopulationElementNewlyPerceived, bool, ( const SWORD_Model* perceiver, const SWORD_Model* element, int level ) )
DECLARE_HOOK( IsObjectUniversal, bool, ( const SWORD_Model* object ) )
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
// Name: PerceptionView::ComputePoint
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionView::ComputePoint( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const MT_Vector2D& vPoint ) const
{
    PerceptionComputer< SurfacesAgentVisitor_ABC, PerceptionSurfaceAgent > computer( boost::bind( &PerceptionSurfaceAgent::ComputePointPerception, _1, perceiver, vPoint ) );
    if( IsEnabled( perceiver ) )
        surfaces.Apply( computer );
    return computer.GetLevel();
}

// -----------------------------------------------------------------------------
// Name: PerceptionView::ComputeAgent
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionView::ComputeAgent( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const wrapper::View& target ) const
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
        const PerceptionLevel& urbanResult = computer.ComputePerception( model, perceiver, target );
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
// Name: PerceptionView::ExecuteAgents
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PerceptionView::ExecuteAgents( const wrapper::View& model, const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_AgentPtrVector& perceivableAgents )
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
                observer_.NotifyAgentPerception( agent, level );
            }
            else if( GET_HOOK( CanBeSeen )( perceiver, agent ) )
            {
                const PerceptionLevel& level = ComputeAgent( model, perceiver, surfaces, agent );
                observer_.NotifyAgentPerception( agent, level );
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
// Name: PerceptionView::ComputeObject
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionView::ComputeObject( const wrapper::View& perceiver, const SurfacesObject_ABC& surfaces, const wrapper::View& knowledgeObject ) const
{
    PerceptionComputer< SurfacesObjectVisitor_ABC, PerceptionSurfaceObject > computer( boost::bind( &PerceptionSurfaceObject::ComputeKnowledgeObjectPerception, _1, perceiver, knowledgeObject ) );
    if( IsEnabled( perceiver ) )
        surfaces.Apply( computer );
    return computer.GetLevel();
}

namespace
{
    const PerceptionLevel& ComputeObject( const wrapper::View& perceiver, const SurfacesObject_ABC& surfaces, const wrapper::View& target, bool isEnabled )
    {
        if( !isEnabled || !GET_HOOK( CanObjectBePerceived )( target ) )
            return PerceptionLevel::notSeen_;

        if( GET_HOOK( IsObjectUniversal )( target ) )
            return PerceptionLevel::identified_;

        PerceptionComputer< SurfacesObjectVisitor_ABC, PerceptionSurfaceObject > computer( boost::bind( &PerceptionSurfaceObject::ComputeObjectPerception, _1, perceiver, target ) );
        surfaces.Apply( computer );
        return computer.GetLevel();
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionView::ExecuteObjects
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
void PerceptionView::ExecuteObjects( const wrapper::View& /*model*/, const wrapper::View& perceiver, const SurfacesObject_ABC& surfaces, const T_ObjectVector& perceivableObjects )
{
    if( IsEnabled( perceiver ) )
    {
        for( T_ObjectVector::const_iterator itObject = perceivableObjects.begin(); itObject != perceivableObjects.end(); ++itObject )
        {
            const wrapper::View& object = *itObject;
            if ( GET_HOOK( IsObjectPerceptionDistanceHacked )( perceiver, object ) )
            {
                const PerceptionLevel& level = PerceptionLevel::FindPerceptionLevel( GET_HOOK( GetObjectPerceptionLevel )( perceiver, object ) );
                observer_.NotifyObjectPerception( object, level );
            }
            else
                observer_.NotifyObjectPerception( object, ::ComputeObject( perceiver, surfaces, object, IsEnabled( perceiver ) ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionView::Compute
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionView::ComputeFlow( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const wrapper::View& flow, T_PointVector& shape ) const
{
    if( !IsEnabled( perceiver ) || !GET_HOOK( CanPopulationElementBePerceived )( flow ) )
        return PerceptionLevel::notSeen_;
    struct PerceptionComputer : public SurfacesAgentVisitor_ABC
    {
        PerceptionComputer( const wrapper::View& perceiver, const wrapper::View& flow )
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
        const wrapper::View& flow_;
        const PerceptionSurfaceAgent* pBestSurface_;
        double rBestCost_;
    } computer( perceiver, flow );
    surfaces.Apply( computer );
    if( !computer.pBestSurface_ )
        return PerceptionLevel::notSeen_;
    return computer.pBestSurface_->ComputeFlowPerception( perceiver, flow, shape );
}

// -----------------------------------------------------------------------------
// Name: PerceptionView::ExecuteFLows
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
void PerceptionView::ExecuteFlows( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_ConstPopulationFlowVector& perceivableFlows )
{
    if( IsEnabled( perceiver ) )
    {
        bool civiliansEncountered = false;
        for( T_ConstPopulationFlowVector::const_iterator it = perceivableFlows.begin(); it != perceivableFlows.end(); ++it )
        {
            const wrapper::View& flow = *it;
            T_PointVector shape;
            const PerceptionLevel* level = &PerceptionLevel::notSeen_;
            if ( GET_HOOK( IsPopulationElementPerceptionDistanceHacked )( perceiver, flow ) )
                level = &PerceptionLevel::FindPerceptionLevel( GET_HOOK( GetPopulationElementPerceptionLevel )( perceiver, flow ) );
            else
                level = &ComputeFlow( perceiver, surfaces, flow, shape );
            observer_.NotifyFlowPerception( flow, *level, shape );
            civiliansEncountered |= GET_HOOK( IsPopulationElementNewlyPerceived )( perceiver, flow, level->GetID() );
        }
        if( civiliansEncountered )
            PostReport( identifier_, MIL_Report::eReport_CiviliansEncountered );
    }
}

// -----------------------------------------------------------------------------
// Name: PerceptionView::ComputeConcentration
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionView::ComputeConcentration( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const wrapper::View& target ) const
{
    if( !IsEnabled( perceiver ) || !GET_HOOK( CanPopulationElementBePerceived )( target ) )
        return PerceptionLevel::notSeen_;

    PerceptionComputer< SurfacesAgentVisitor_ABC, PerceptionSurfaceAgent > computer( boost::bind( &PerceptionSurfaceAgent::ComputeConcentrationPerception, _1, perceiver, target ) );
    surfaces.Apply( computer );
    return computer.GetLevel();
}

// -----------------------------------------------------------------------------
// Name: PerceptionView::ExecuteConcentrations
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
void PerceptionView::ExecuteConcentrations( const wrapper::View& perceiver, const SurfacesAgent_ABC& surfaces, const T_ConstPopulationConcentrationVector& perceivableConcentrations )
{
    if( IsEnabled( perceiver ) )
    {
        bool civiliansEncountered = false;
        for( T_ConstPopulationConcentrationVector::const_iterator it = perceivableConcentrations.begin(); it != perceivableConcentrations.end(); ++it )
        {
            const wrapper::View& concentration = *it;
            const PerceptionLevel* level = &PerceptionLevel::notSeen_;
            if ( GET_HOOK( IsPopulationElementPerceptionDistanceHacked )( perceiver, concentration ) )
                level = &PerceptionLevel::FindPerceptionLevel( GET_HOOK( GetPopulationElementPerceptionLevel )( perceiver, concentration ) );
            else
                level = &ComputeConcentration( perceiver, surfaces, concentration );
            observer_.NotifyConcentrationPerception( concentration, *level );
            civiliansEncountered |= GET_HOOK( IsPopulationElementNewlyPerceived )( perceiver, concentration, level->GetID() );
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
