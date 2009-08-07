// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_IntelligenceFunctions.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Orders/MIL_IntelligenceOrder.h"
#include "Entities/Orders/MIL_IntelligenceOrdersVisitor_ABC.h"
#include "Entities/Orders/MIL_OrderManager_ABC.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "Entities/Automates/MIL_Automate.h"

#include <boost/bind.hpp>

namespace
{
    template< typename T >
    struct IntelligenceStatusAccumulator : public MIL_IntelligenceOrdersVisitor_ABC
    {
        IntelligenceStatusAccumulator( const T& boundaries, bool loaded )
            : boundaries_( &boundaries ), loaded_( loaded ), count_( 0 ), sum_( 0 ) {}

        virtual void Visit( const MIL_IntelligenceOrder& intelligence )
        {
            if( intelligence.IsEnemy() && intelligence.IsInside( *boundaries_ ) )
            {
                const unsigned int factor = (unsigned int)std::pow( 4.f, int( intelligence.GetLevel() ) );
                count_ += factor;
                if( intelligence.IsEmbarked() == loaded_ )
                    sum_ += factor;
            }
        }

        const T* boundaries_;
        const bool loaded_;
        unsigned int count_;
        unsigned int sum_;
    };

    template< typename T >
    float ComputeEnemiesRatio( const MIL_Automate& caller, const T& boundaries, bool embarked )
    {
        IntelligenceStatusAccumulator< T > accu( boundaries, embarked );
        caller.GetOrderManager().Accept( accu );
        return accu.count_ == 0 ? 0.f : float( accu.sum_ ) / float( accu.count_ );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_IntelligenceFunctions::ComputeUnloadedEnemiesRatio
// Created: SBO 2007-11-13
// -----------------------------------------------------------------------------
float DEC_IntelligenceFunctions::ComputeUnloadedEnemiesRatio( const MIL_Automate& caller, const TER_Localisation* location )
{
    assert( location );
    return ComputeEnemiesRatio( caller, *location, false );
}

// -----------------------------------------------------------------------------
// Name: DEC_IntelligenceFunctions::ComputeLoadedEnemiesRatio
// Created: SBO 2007-11-13
// -----------------------------------------------------------------------------
float DEC_IntelligenceFunctions::ComputeLoadedEnemiesRatio( const MIL_Automate& caller, const TER_Localisation* location )
{
    assert( location );
    return ComputeEnemiesRatio( caller, *location, true );
}

// -----------------------------------------------------------------------------
// Name: DEC_IntelligenceFunctions::ComputeFuseauUnloadedEnemiesRatio
// Created: LDC 2009-07-06
// -----------------------------------------------------------------------------
float DEC_IntelligenceFunctions::ComputeFuseauUnloadedEnemiesRatio( const MIL_Automate& caller, const MIL_Fuseau* limits )
{
    assert( limits );
    return ComputeEnemiesRatio( caller, *limits, false );
}

// -----------------------------------------------------------------------------
// Name: DEC_IntelligenceFunctions::ComputeFuseauLoadedEnemiesRatio
// Created: LDC 2009-07-06
// -----------------------------------------------------------------------------
float DEC_IntelligenceFunctions::ComputeFuseauLoadedEnemiesRatio( const MIL_Automate& caller, const MIL_Fuseau* limits )
{
    assert( limits );
    return ComputeEnemiesRatio( caller, *limits, true );
}

namespace
{
    template< typename T >
    struct CompareBoundariesEnemies
    {
        CompareBoundariesEnemies( const MIL_Automate& caller, bool loaded ) : pCaller_( &caller ), loaded_( loaded ) {}
        bool operator()( boost::shared_ptr< T > lhs, boost::shared_ptr< T > rhs )
        {
            return ComputeEnemiesRatio( *pCaller_, *lhs, loaded_ ) < ComputeEnemiesRatio( *pCaller_, *rhs, loaded_ );
        }
        const MIL_Automate* pCaller_;
        const bool loaded_;
    };    
}

// -----------------------------------------------------------------------------
// Name: DEC_IntelligenceFunctions::SortZonesAccordingToUnloadedEnemies
// Created: LDC 2009-07-06
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< TER_Localisation > > DEC_IntelligenceFunctions::SortZonesAccordingToUnloadedEnemies( const MIL_Automate& caller, const std::vector< boost::shared_ptr< TER_Localisation > >& locations )
{
    std::vector< boost::shared_ptr< TER_Localisation > > result( locations );
    std::sort( result.begin(), result.end(), CompareBoundariesEnemies< TER_Localisation >( caller, false ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_IntelligenceFunctions::SortAccordingToUnloadedEnemies
// Created: SBO 2007-11-13
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MIL_Fuseau > > DEC_IntelligenceFunctions::SortAccordingToUnloadedEnemies( const MIL_Automate& caller, const std::vector< boost::shared_ptr< MIL_Fuseau > >& limits )
{
    std::vector< boost::shared_ptr< MIL_Fuseau > > result( limits );
    std::sort( result.begin(), result.end(), CompareBoundariesEnemies< MIL_Fuseau >( caller, false ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_IntelligenceFunctions::SortAccordingToLoadedEnemies
// Created: SBO 2007-11-13
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MIL_Fuseau > > DEC_IntelligenceFunctions::SortAccordingToLoadedEnemies( const MIL_Automate& caller, const std::vector< boost::shared_ptr< MIL_Fuseau > >& limits )
{
    std::vector< boost::shared_ptr< MIL_Fuseau > > result( limits );
    std::sort( result.begin(), result.end(), CompareBoundariesEnemies< MIL_Fuseau >( caller, true ) );
    return result;
}

namespace
{
    template< typename Filter, typename Handler >
    struct FlankIntelligenceFinder : public MIL_IntelligenceOrdersVisitor_ABC
    {
        FlankIntelligenceFinder( const MIL_Fuseau& zone, const MIL_Automate& automat, const Filter& filter, Handler& handler )
            : leftFlank_ ( automat.GetOrderManager().GetFuseau().IsLeftFlank ( zone ) )
            , rightFlank_( automat.GetOrderManager().GetFuseau().IsRightFlank( zone ) )
            , zone_      ( &zone )
            , filter_    ( filter )
            , handler_   ( &handler )
            , result_    ( false )
        {
        }

        virtual void Visit( const MIL_IntelligenceOrder& intelligence )
        {
            if( filter_( intelligence ) )
            {
                const bool onFlank = intelligence.IsOnFlank( *zone_, leftFlank_, rightFlank_ );
                result_ = result_ || onFlank;
                (*handler_)( intelligence );
            }
        }

        bool leftFlank_;
        bool rightFlank_;
        const MIL_Fuseau* zone_;
        Filter filter_;
        Handler* handler_;
        bool result_;
    };

    struct NoopHandler
    {
        void operator()( const MIL_IntelligenceOrder& /*intelligence*/ ) {}
    };

    struct ClosestIntelligenceHandler
    {
        ClosestIntelligenceHandler( const MT_Vector2D& origin ) 
            : origin_  ( origin )
            , closest_ ( 0 )
            , distance_( std::numeric_limits< float >::max() ) {}
        
        void operator()( const MIL_IntelligenceOrder& intelligence )
        {
            const MT_Float distance = intelligence.SquareDistance( origin_ );
            if( distance < distance_ )
            {
                closest_ = &intelligence;
                distance_ = distance;
            }
        }

        MT_Vector2D origin_;
        const MIL_IntelligenceOrder* closest_;
        MT_Float distance_;
    };

    template< typename Filter >
    bool IsOnFlank( const MIL_Fuseau& zone, const MIL_Automate& automat, const Filter& filter )
    {
        NoopHandler handler;
        FlankIntelligenceFinder< Filter, NoopHandler > finder( zone, automat, filter, handler );
        automat.GetOrderManager().Accept( finder );
        return finder.result_;
    }

    template< typename Filter >
    MT_Vector2D ComputeClosestDirection( const MT_Vector2D& origin, const MIL_Fuseau& zone, const MIL_Automate& automat, const Filter& filter )
    {
        ClosestIntelligenceHandler handler( origin );
        FlankIntelligenceFinder< Filter, ClosestIntelligenceHandler > finder( zone, automat, filter, handler );
        automat.GetOrderManager().Accept( finder );
        if( handler.closest_ )
            return handler.closest_->ComputeDirection( handler.origin_ );
        if( finder.leftFlank_ )
            return automat.GetOrderManager().GetDirDanger().Rotated90();
        return automat.GetOrderManager().GetDirDanger().Rotated90ClockWise();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_IntelligenceFunctions::IsFriendOnFlank
// Created: SBO 2007-12-06
// -----------------------------------------------------------------------------
bool DEC_IntelligenceFunctions::IsFriendOnFlank( const MIL_Automate& caller, const MIL_Fuseau* limits )
{
    return limits && IsOnFlank( *limits, caller, boost::bind( &MIL_IntelligenceOrder::IsFriend, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_IntelligenceFunctions::ComputeCoverDirection
// Created: SBO 2007-12-06
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_IntelligenceFunctions::ComputeCoverDirection( const MIL_Automate& caller, const MT_Vector2D* origin, const MIL_Fuseau* limits )
{
    if( limits && origin )
        return boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( ComputeClosestDirection( *origin, *limits, caller, boost::bind( &MIL_IntelligenceOrder::IsEnemy, _1 ) ) ) );
    return boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( caller.GetOrderManager().GetDirDanger() ) );
}
