// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "PerceptionSurfaceAgent.h"
#include "SensorTypeAgent.h"
#include "SensorType.h"
#include "TargetPerceptionVisitor_ABC.h"
#include "PerceptionLevel.h"
#include "wrapper/Hook.h"
#include "wrapper/View.h"
#include "wrapper/Effect.h"
#include <boost/foreach.hpp>

DECLARE_HOOK( IsKnown, bool, ( const SWORD_Model* perceiver, const SWORD_Model* target ) )

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceAgent constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PerceptionSurfaceAgent::PerceptionSurfaceAgent()
    : vOrigin_    ()
    , pSensorType_( 0  )
    , rHeight_    ( 0. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceAgent constructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PerceptionSurfaceAgent::PerceptionSurfaceAgent( const SensorTypeAgent& sensorType, const MT_Vector2D& vOrigin, double rHeight )
    : vOrigin_    ( vOrigin    )
    , pSensorType_( &sensorType )
    , rHeight_    ( rHeight    )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceAgent destructor
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
PerceptionSurfaceAgent::~PerceptionSurfaceAgent()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceAgent::Apply
// Created: SLI 2012-05-30
// -----------------------------------------------------------------------------
void PerceptionSurfaceAgent::Apply( TargetPerceptionVisitor_ABC& visitor ) const
{
    BOOST_FOREACH( const T_PerceptionTickMap::value_type& perception, perceptionsUnderway_ )
        visitor.Notify( perception.first, perception.second );
}

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceAgent::AddDirection
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
void PerceptionSurfaceAgent::AddDirection( const MT_Vector2D& vDir )
{
    sectors_.push_back( MT_Sector( vOrigin_, vDir, pSensorType_->GetAngle() ) );
}

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceAgent::IsInside
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
bool PerceptionSurfaceAgent::IsInside( const MT_Vector2D& vPoint ) const
{
    if( vOrigin_.SquareDistance( vPoint ) <= pSensorType_->GetSquareProximityDistance() )
        return true;
    for( CIT_SectorVector it = sectors_.begin(); it != sectors_.end(); ++it )
        if( it->IsInSector( vPoint ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceAgent::ComputePointPerception
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionSurfaceAgent::ComputePointPerception( const wrapper::View& perceiver, const MT_Vector2D& vTargetPos ) const
{
    return pSensorType_->ComputePointPerception( perceiver, vTargetPos, rHeight_ );
}

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceAgent::ComputeAgentPerception
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionSurfaceAgent::ComputeAgentPerception( const wrapper::View& perceiver, const wrapper::View& target, const MT_Vector2D& vSourcePos, const MT_Vector2D& vTargetPos ) const
{
    if( GET_HOOK( IsKnown )( perceiver, target )
        || pSensorType_->CanScan() && perceiver[ "perceptions/peripherical-vision/activated" ]
        || IsInside( vTargetPos ) )
    {
        const PerceptionLevel& level = pSensorType_->ComputeAgentPerception( perceiver, target, vSourcePos, vTargetPos, rHeight_ );
        return GetLevelWithDelay( level, target[ "identifier" ] );
    }
    return PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceAgent::GetLevelWithDelay
// Created: LDC 2010-05-05
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionSurfaceAgent::GetLevelWithDelay( const PerceptionLevel& level, std::size_t target ) const
{
    int delay = pSensorType_->GetDelay();
    if( delay && level > PerceptionLevel::notSeen_ )
    {
        CIT_PerceptionTickMap it = perceptionsUnderway_.find( target );
        int tick = 0;
        if( it != perceptionsUnderway_.end() )
            tick = it->second;
        perceptionsBuffer_[ target ] = tick + 1;
        if( tick <= delay )
            return PerceptionLevel::notSeen_;
    }
    return level;
}

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceAgent::FinalizePerception
// Created: LDC 2010-05-04
// -----------------------------------------------------------------------------
void PerceptionSurfaceAgent::FinalizePerception()
{
    perceptionsUnderway_ = perceptionsBuffer_;
    perceptionsBuffer_.clear();
}

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceAgent::ComputeConcentrationPerception
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionSurfaceAgent::ComputeConcentrationPerception( const wrapper::View& perceiver, const wrapper::View& target ) const
{
    const PerceptionLevel& level = pSensorType_->ComputeConcentrationPerception( perceiver, target, rHeight_ );
    return GetLevelWithDelay( level, reinterpret_cast< std::size_t >( &target ) ); // $$$$ _RC_ SLI 2012-06-04: Use identifier instead
}

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceAgent::ComputePerceptionAccuracy
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
double PerceptionSurfaceAgent::ComputePerceptionAccuracy( const wrapper::View& perceiver, const wrapper::View& target ) const
{
    return pSensorType_->ComputePerceptionAccuracy( perceiver, target, rHeight_ );
}

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceAgent::ComputeFlowPerception
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
const PerceptionLevel& PerceptionSurfaceAgent::ComputeFlowPerception( const wrapper::View& perceiver, const wrapper::View& target, T_PointVector& shape ) const
{
    const PerceptionLevel& level = pSensorType_->ComputeFlowPerception( perceiver, target, rHeight_, shape );
    return GetLevelWithDelay( level, reinterpret_cast< std::size_t >( &target ) ); // $$$$ _RC_ SLI 2012-06-04: Use identifier instead
}

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceAgent::IsInitialized
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
bool PerceptionSurfaceAgent::IsInitialized()
{
    return pSensorType_ != 0;
}

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceAgent::TransferPerception
// Created: SLG 2010-05-07
// -----------------------------------------------------------------------------
void PerceptionSurfaceAgent::TransferPerception( const std::map< std::size_t, std::pair< unsigned int, double > >& urbanPerceptionMap ) const
{
    perceptionsBuffer_.clear();
    for( std::map< std::size_t, std::pair< unsigned int, double > >::const_iterator it = urbanPerceptionMap.begin(); it != urbanPerceptionMap.end(); ++it )
        perceptionsBuffer_[ it->first ] = it->second.first;
}

// -----------------------------------------------------------------------------
// Name: PerceptionSurfaceAgent::NotifyCone
// Created: SLI 2012-09-13
// -----------------------------------------------------------------------------
void PerceptionSurfaceAgent::NotifyCone( wrapper::Effect& effect ) const
{
    wrapper::Node cone = effect.AddElement();
    cone[ "origin/x" ] = vOrigin_.rX_;
    cone[ "origin/y" ] = vOrigin_.rY_;
    cone[ "height" ] = rHeight_;
    cone[ "sensor" ] = pSensorType_->GetType().GetName();
    for( CIT_SectorVector it = sectors_.begin(); it != sectors_.end(); ++it )
    {
        wrapper::Node sector = cone[ "sectors" ].AddElement();
        const MT_Vector2D& direction = it->GetDirection();
        sector[ "direction/x" ] = direction.GetX();
        sector[ "direction/y" ] = direction.GetY();
    }
}
