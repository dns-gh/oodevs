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

    void ComputeEnemiesRatio( DIA_Call_ABC& call, const MIL_Automate& caller, bool embarked )
    {
        if( DEC_Tools::CheckTypeFuseau( call.GetParameter( 0 ) ) )
        {
            if( const MIL_Fuseau* fuseau = call.GetParameter( 0 ).ToUserPtr( fuseau ) )
                call.GetResult().SetValue( ComputeEnemiesRatio( caller, *fuseau, embarked ) );
        }
        else if( DEC_Tools::CheckTypeLocalisation( call.GetParameter( 0 ) ) )
        {
            if( const TER_Localisation* location = call.GetParameter( 0 ).ToUserPtr( location ) )
                call.GetResult().SetValue( ComputeEnemiesRatio( caller, *location, embarked ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_IntelligenceFunctions::ComputeUnloadedEnemiesRatio
// Created: SBO 2007-11-13
// -----------------------------------------------------------------------------
void DEC_IntelligenceFunctions::ComputeUnloadedEnemiesRatio( DIA_Call_ABC& call, const MIL_Automate& caller )
{
    ComputeEnemiesRatio( call, caller, false );
}

// -----------------------------------------------------------------------------
// Name: DEC_IntelligenceFunctions::ComputeLoadedEnemiesRatio
// Created: SBO 2007-11-13
// -----------------------------------------------------------------------------
void DEC_IntelligenceFunctions::ComputeLoadedEnemiesRatio( DIA_Call_ABC& call, const MIL_Automate& caller )
{
    ComputeEnemiesRatio( call, caller, true );
}

namespace
{
    struct CompareBoundariesEnemies
    {
        CompareBoundariesEnemies( const MIL_Automate& caller, bool loaded ) : pCaller_( &caller ), loaded_( loaded ) {}

        bool operator()( DIA_Variable_ABC* dia1, DIA_Variable_ABC* dia2 )
        {
            if( DEC_Tools::CheckTypeFuseau( *dia1 ) && DEC_Tools::CheckTypeFuseau( *dia2 ) )
            {
                MIL_Fuseau* pFuseau1 = dia1->ToUserPtr( pFuseau1 );
                MIL_Fuseau* pFuseau2 = dia2->ToUserPtr( pFuseau2 );
                return ComputeEnemiesRatio( *pCaller_, *pFuseau1, loaded_ )
                     < ComputeEnemiesRatio( *pCaller_, *pFuseau2, loaded_ );
            }
            else if( DEC_Tools::CheckTypeLocalisation( *dia1 ) && DEC_Tools::CheckTypeLocalisation( *dia2 ) )
            {
                TER_Localisation* location1 = dia1->ToUserPtr( location1 );
                TER_Localisation* location2 = dia2->ToUserPtr( location2 );
                return ComputeEnemiesRatio( *pCaller_, *location1, loaded_ )
                     < ComputeEnemiesRatio( *pCaller_, *location2, loaded_ );
            }
            return false;
        }

        const MIL_Automate* pCaller_;
        bool loaded_;
    };    
}

// -----------------------------------------------------------------------------
// Name: DEC_IntelligenceFunctions::SortAccordingToUnloadedEnemies
// Created: SBO 2007-11-13
// -----------------------------------------------------------------------------
void DEC_IntelligenceFunctions::SortAccordingToUnloadedEnemies( DIA_Call_ABC& call, const MIL_Automate& caller )
{
    assert( DEC_Tools::CheckTypeListeFuseaux( call.GetParameter( 0 ) ) || DEC_Tools::CheckTypeListeLocalisations( call.GetParameter( 0 ) ) );
    call.GetResult() = call.GetParameter( 0 );   
    T_ObjectVariableVector& boundariesList = const_cast< T_ObjectVariableVector& >( static_cast< DIA_Variable_ObjectList& >( call.GetResult() ).GetContainer() );
    std::sort( boundariesList.begin(), boundariesList.end(), CompareBoundariesEnemies( caller, false ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_IntelligenceFunctions::SortAccordingToLoadedEnemies
// Created: SBO 2007-11-13
// -----------------------------------------------------------------------------
void DEC_IntelligenceFunctions::SortAccordingToLoadedEnemies( DIA_Call_ABC& call, const MIL_Automate& caller )
{
    assert( DEC_Tools::CheckTypeListeFuseaux( call.GetParameter( 0 ) ) || DEC_Tools::CheckTypeListeLocalisations( call.GetParameter( 0 ) ) );
    call.GetResult() = call.GetParameter( 0 );
    T_ObjectVariableVector& boundariesList = const_cast< T_ObjectVariableVector& >( static_cast< DIA_Variable_ObjectList& >( call.GetResult() ).GetContainer() );
    std::sort( boundariesList.begin(), boundariesList.end(), CompareBoundariesEnemies( caller, true ) );
}

namespace
{
    template< typename Filter, typename Handler >
    struct FlankIntelligenceFinder : public MIL_IntelligenceOrdersVisitor_ABC
    {
        FlankIntelligenceFinder( const MIL_Fuseau& zone, const MIL_Automate& automat, const Filter& filter, Handler& handler )
            : leftFlank_ ( automat.GetFuseau().IsLeftFlank ( zone ) )
            , rightFlank_( automat.GetFuseau().IsRightFlank( zone ) )
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
            return automat.GetDirDanger().Rotated90();
        return automat.GetDirDanger().Rotated90ClockWise();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_IntelligenceFunctions::IsEnemyOnFlank
// Created: SBO 2007-12-06
// -----------------------------------------------------------------------------
void DEC_IntelligenceFunctions::IsEnemyOnFlank( DIA_Call_ABC& call, const MIL_Automate& caller )
{
    assert( DEC_Tools::CheckTypeFuseau( call.GetParameter( 0 ) ) );
    const MIL_Fuseau* zone = call.GetParameter( 0 ).ToUserPtr( zone );
    call.GetResult().SetValue( zone && ::IsOnFlank( *zone, caller, boost::bind( &MIL_IntelligenceOrder::IsEnemy, _1 ) ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_IntelligenceFunctions::IsFriendOnFlank
// Created: SBO 2007-12-06
// -----------------------------------------------------------------------------
void DEC_IntelligenceFunctions::IsFriendOnFlank( DIA_Call_ABC& call, const MIL_Automate& caller )
{
    assert( DEC_Tools::CheckTypeFuseau( call.GetParameter( 0 ) ) );
    const MIL_Fuseau* zone = call.GetParameter( 0 ).ToUserPtr( zone );
    call.GetResult().SetValue( zone && ::IsOnFlank( *zone, caller, boost::bind( &MIL_IntelligenceOrder::IsFriend, _1 ) ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_IntelligenceFunctions::ComputeCoverDirection
// Created: SBO 2007-12-06
// -----------------------------------------------------------------------------
void DEC_IntelligenceFunctions::ComputeCoverDirection( DIA_Call_ABC& call, const MIL_Automate& caller )
{
    assert( DEC_Tools::CheckTypePoint ( call.GetParameter( 0 ) ) );
    assert( DEC_Tools::CheckTypeFuseau( call.GetParameter( 1 ) ) );
    const MT_Vector2D* origin = call.GetParameter( 0 ).ToUserPtr( origin );
    const MIL_Fuseau*  zone   = call.GetParameter( 1 ).ToUserPtr( zone );
    if( zone && origin )
    {
        MT_Vector2D* result = new MT_Vector2D( ComputeClosestDirection( *origin, *zone, caller, boost::bind( &MIL_IntelligenceOrder::IsEnemy, _1 ) ) ); // $$$$ SBO 2007-12-07: RAM
        call.GetResult().SetValue( (void*)result, &DEC_Tools::GetTypeDirection(), 1 );
    }
    else
    {
        // $$$$ SBO 2008-01-11: should not happen as a zone parameter is supposed to be provided
        MT_Vector2D* result = new MT_Vector2D( caller.GetDirDanger() ); // $$$$ SBO 2008-01-11: RAM
        call.GetResult().SetValue( (void*)result, &DEC_Tools::GetTypeDirection(), 1 );
    }
}
