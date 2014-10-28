// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Agent_Path.h"
#include "DEC_Agent_PathClass.h"
#include "DEC_Agent_PathfinderRule.h"
#include "DEC_AgentContext.h"
#include "DEC_PathComputer_ABC.h"
#include "DEC_PathSection.h"
#include "Decision/DEC_GeometryFunctions.h"
#include "Decision/DEC_PathType.h"
#include "Decision/DEC_Rep_PathPoint_Front.h"
#include "Decision/DEC_Rep_PathPoint_Special.h"
#include "Decision/DEC_Rep_PathPoint_Lima.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/MIL_AgentType_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Objects/TrafficabilityCapacity.h"
#include "Entities/Orders/MIL_AutomateOrderManager.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "Entities/Orders/MIL_PionOrderManager.h"
#include "MT_Tools/MT_Logger.h"
#include <boost/make_shared.hpp>

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path constructor
// Created: JDY 03-04-10
//-----------------------------------------------------------------------------
DEC_Agent_Path::DEC_Agent_Path( MIL_Agent_ABC& queryMaker, const T_PointVector& points, const DEC_PathType& pathType,
    const boost::shared_ptr< DEC_PathComputer_ABC >& computer )
    : DEC_PathResult     ( pathType )
    , queryMaker_        ( queryMaker )
    , pathClass_         ( DEC_Agent_PathClass::GetPathClass( pathType, queryMaker ) )
    , initialWaypoints_  ( points )
    , nextWaypoints_     ( points.empty() ? points.begin() : points.begin() + 1, points.end() )
    , context_           ( new DEC_AgentContext( queryMaker, pathClass_, points, false ) )
    , computer_          ( computer )
{
    const bool refine = queryMaker.GetType().GetUnitType().CanFly() && !queryMaker.IsAutonomous();
    const bool useStrictClosest = !queryMaker.GetAutomate().GetOrderManager().GetFuseau().IsNull();
    if( points.empty() )
        throw MASA_EXCEPTION( "List of points is empty in population path initialization" );
    for( auto it = points.begin(); it != points.end() - 1; ++it )
    {
        std::unique_ptr< TerrainRule_ABC > rule( new DEC_Agent_PathfinderRule( context_, *it, *(it + 1) ) );
        computer_->RegisterPathSection( *new DEC_PathSection( *computer_, std::move( rule ), *it, *(it + 1), refine, useStrictClosest ) );
    }
    queryMaker.RegisterPath( *this );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
DEC_Agent_Path::~DEC_Agent_Path()
{
    for( auto it = resultList_.begin(); it != resultList_.end(); ++it )
        if( ( *it )->GetType() != DEC_PathPoint::eTypePointPath )
            ( *it )->RemoveFromDIA( *it );
    queryMaker_.UnregisterPath( *this );
}

void DEC_Agent_Path::Cancel()
{
    computer_->Cancel();
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path::IsPointAvantIn
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
bool DEC_Agent_Path::IsPointAvantIn( const TerrainData& nObjectTypesBefore, const TerrainData& nObjectTypesToNextPoint, const TerrainData& nTypeTerrain ) const
{
    // Test : entr�e dans environnement
    return !( nObjectTypesBefore.ContainsOne( nTypeTerrain ) ) && ( nObjectTypesToNextPoint.ContainsOne( nTypeTerrain ) );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path::IsPointAvantOut
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
bool DEC_Agent_Path::IsPointAvantOut( const TerrainData& nObjectTypesBefore, const TerrainData& nObjectTypesToNextPoint, const TerrainData& nTypeTerrain ) const
{
    // Test : sortie d'environnement
    return ( nObjectTypesBefore.ContainsOne( nTypeTerrain ) ) && !( nObjectTypesToNextPoint.ContainsOne(  nTypeTerrain ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::IsPointAvant
// Created: AGE 2005-02-04
// -----------------------------------------------------------------------------
bool DEC_Agent_Path::IsPointAvant( const TerrainData& nObjectTypesBefore, const TerrainData& nObjectTypesToNextPoint, const TerrainData& nTypeTerrain ) const
{
    return IsPointAvantIn ( nObjectTypesBefore, nObjectTypesToNextPoint, nTypeTerrain )
        || IsPointAvantOut( nObjectTypesBefore, nObjectTypesToNextPoint, nTypeTerrain );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetPreviousPathPointOnDifferentLocation
// Created: JVT 2005-07-08
// -----------------------------------------------------------------------------
DEC_Agent_Path::T_PathPoints::iterator DEC_Agent_Path::GetPreviousPathPointOnDifferentLocation( T_PathPoints::iterator itCurrent )
{
    if( itCurrent == resultList_.end() )
        throw MASA_EXCEPTION( "Current path point is invalid" );
    const MT_Vector2D& vPosition = ( *itCurrent )->GetPos();
    while ( itCurrent != resultList_.begin() && ( *itCurrent )->GetPos() == vPosition )
        --itCurrent;
    return itCurrent;
}

#pragma warning( push )
#pragma warning( disable : 4127 ) // conditional expression is constant

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::InsertPointAvant
// Created: NLD 2005-08-11
// -----------------------------------------------------------------------------
void DEC_Agent_Path::InsertPointAvant( const boost::shared_ptr< DEC_Rep_PathPoint > spottedPathPoint, T_PathPoints::iterator itCurrent )
{
    double rDistanceLeft = spottedPathPoint->GetTypePoint() == DEC_Rep_PathPoint::eTypePointLima ?
                             queryMaker_.GetType().GetDistanceAvantLima() :
                             queryMaker_.GetType().GetDistanceAvantPoint( spottedPathPoint->GetTypeTerrain() );
    try
    {
        while( true )
        {
            const MT_Vector2D& vCurrentPos = ( *itCurrent )->GetPos();
            if( itCurrent == resultList_.begin() )
            {
                auto pNewPoint = boost::make_shared< DEC_Rep_PathPoint_Front >( vCurrentPos, spottedPathPoint );
                resultList_.insert( ++itCurrent, pNewPoint );
                break;
            }

            // remise en position des it�rateurs
            auto itPrev = GetPreviousPathPointOnDifferentLocation( itCurrent );
            itCurrent = itPrev;
            ++itCurrent;
            if( ( *itCurrent )->GetPos() != vCurrentPos )
                throw MASA_EXCEPTION( "Current position is invalid" );

            // calcul de la distance "mang�e" par le parcours de ce segment
            const MT_Vector2D& vPreviousPos = ( *itPrev )->GetPos();
            MT_Vector2D vTmp = vPreviousPos - vCurrentPos;
            const double vTmpMag = vTmp.Magnitude();

            rDistanceLeft -= vTmpMag;
            if( rDistanceLeft == 0. )
            {
                // Positionnement du point avant au m�me endroit qu'un autre point
                auto pNewPoint = boost::make_shared< DEC_Rep_PathPoint_Front >( vPreviousPos, spottedPathPoint );
                resultList_.insert( itPrev, pNewPoint );
                break;
            }
            else if( rDistanceLeft < 0. )
            {
                // Positionnement du point avant
                vTmp /= vTmpMag;
                vTmp *= rDistanceLeft;
                vTmp += vPreviousPos;
                auto pNewPoint = boost::make_shared< DEC_Rep_PathPoint_Front >( vTmp, spottedPathPoint );
                resultList_.insert( itCurrent, pNewPoint );
                break;
            }
            // tant pis, �a sera peut-�tre pour le prochain segment
            itCurrent = itPrev;
        }
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
    }
}

#pragma warning( pop )

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path::InsertPointAvant
// Created: JVT 02-12-04
//----------------------------------------------------------------------------
void DEC_Agent_Path::InsertPointAvant( const boost::shared_ptr< DEC_Rep_PathPoint > spottedPathPoint, T_PathPoints::iterator itCurrent, double& rDistSinceLastPointAvant )
{
    static const double rDist = 2000.;
    if( rDistSinceLastPointAvant > rDist )
    {
        InsertPointAvant( spottedPathPoint, itCurrent );
        rDistSinceLastPointAvant = 0.;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::InsertPoint
// Created: NLD 2005-08-10
// -----------------------------------------------------------------------------
bool DEC_Agent_Path::InsertPoint( boost::shared_ptr< DEC_PathPoint > spottedPathPoint, T_PathPoints::iterator itCurrent, double& rDistSinceLastPoint )
{
    static double rDist = 500.;
    if( rDistSinceLastPoint > rDist )
    {
        resultList_.insert( itCurrent, spottedPathPoint );
        rDistSinceLastPoint = 0.;
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::InsertPointAndPointAvant
// Created: NLD 2005-08-10
// -----------------------------------------------------------------------------
void DEC_Agent_Path::InsertPointAndPointAvant( boost::shared_ptr< DEC_Rep_PathPoint > spottedPathPoint, T_PathPoints::iterator itCurrent, double& rDistSinceLastPoint, double& rDistSinceLastPointAvant )
{
    if( InsertPoint( spottedPathPoint, itCurrent, rDistSinceLastPoint ) )
        InsertPointAvant( spottedPathPoint, itCurrent, rDistSinceLastPointAvant );
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path::InsertPointAvants
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
void DEC_Agent_Path::InsertPointAvants()
{
    double rDistSinceLastPointAvant = std::numeric_limits< double >::max();
    double rDistSinceLastPoint = std::numeric_limits< double >::max();
    TerrainData nObjectTypesBefore;
    DEC_PathPoint* pPrevPoint = 0;
    for( auto it = resultList_.begin(); it != resultList_.end(); ++it )
    {
        DEC_PathPoint& current = **it;
        if( pPrevPoint )
        {
            if( rDistSinceLastPointAvant != std::numeric_limits< double >::max() )
                rDistSinceLastPointAvant += pPrevPoint->GetPos().Distance( current.GetPos() );
            if( rDistSinceLastPoint != std::numeric_limits< double >::max() )
                rDistSinceLastPoint += pPrevPoint->GetPos().Distance( current.GetPos() );
        }

        // On ne teste les points avants que sur les points du path find original
        if( current.GetType() != DEC_PathPoint::eTypePointPath )
        {
            pPrevPoint = &current;
            continue;
        }

        TerrainData nObjectTypesToNextPoint = current.GetObjectTypesToNextPoint();
        if( nObjectTypesBefore == TerrainData() )
        {
            pPrevPoint = &current;
            nObjectTypesBefore = nObjectTypesToNextPoint;
            continue;
        }

        // Village
        if( IsPointAvantIn( nObjectTypesBefore, nObjectTypesToNextPoint, TerrainData::Urban() ) )
            InsertPointAndPointAvant( boost::make_shared< DEC_Rep_PathPoint_Special >( ( *it )->GetPos(), DEC_Rep_PathPoint_Special::eTypePointParticulierVillage, TerrainData::Urban() ), it, rDistSinceLastPoint, rDistSinceLastPointAvant );

        // Urban
        else if( IsPointAvantOut( nObjectTypesBefore, nObjectTypesToNextPoint, TerrainData::Urban() ) )
            InsertPoint( boost::make_shared< DEC_Rep_PathPoint >( ( *it )->GetPos(), DEC_Rep_PathPoint::eTypePointCCT, TerrainData::Urban() ), it, rDistSinceLastPoint );

        // Forest
        else if( IsPointAvant( nObjectTypesBefore, nObjectTypesToNextPoint, TerrainData::Forest() ) )
            InsertPointAndPointAvant( boost::make_shared< DEC_Rep_PathPoint >( ( *it )->GetPos(), DEC_Rep_PathPoint::eTypePointCCT, TerrainData::Forest() ), it, rDistSinceLastPoint, rDistSinceLastPointAvant );

        // Cross roads
        else if( current.GetObjectTypes().ContainsOne( TerrainData::Crossroad() ) )
            InsertPointAndPointAvant( boost::make_shared< DEC_Rep_PathPoint_Special >( ( *it )->GetPos(), DEC_Rep_PathPoint_Special::eTypePointParticulierCarrefour, TerrainData::Crossroad() ), it, rDistSinceLastPoint, rDistSinceLastPointAvant );

        // Pont
        else if( IsPointAvantIn( nObjectTypesBefore, nObjectTypesToNextPoint, TerrainData::Bridge() )
                || ( !current.GetObjectTypes().ContainsOne( TerrainData::SmallRiver() ) && current.GetObjectTypes().ContainsOne( TerrainData::Bridge() ) && !nObjectTypesBefore.ContainsOne( TerrainData::Bridge() ) && !nObjectTypesToNextPoint.ContainsOne( TerrainData::Bridge() ) )
                )
            InsertPointAndPointAvant( boost::make_shared< DEC_Rep_PathPoint_Special >( ( *it )->GetPos(), DEC_Rep_PathPoint_Special::eTypePointParticulierPont, TerrainData::Bridge() ), it, rDistSinceLastPoint, rDistSinceLastPointAvant );

        nObjectTypesBefore = nObjectTypesToNextPoint;
        pPrevPoint = &current;
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path::InsertLima
// Created: JVT 02-12-05
//-----------------------------------------------------------------------------
void DEC_Agent_Path::InsertLima( const MIL_LimaOrder& lima )
{
    boost::shared_ptr< DEC_PathPoint > pLastPoint;
    for( auto it = resultList_.begin(); it != resultList_.end(); ++it )
    {
        boost::shared_ptr< DEC_PathPoint > pCurrentPoint = *it;
        if( !pCurrentPoint )
        {
            MT_LOG_ERROR_MSG( "Current point is invalid" );
            break;
        }
        if( pLastPoint.get() && ( pCurrentPoint->GetPos() != pLastPoint->GetPos() ) )
        {
            MT_Line segment( pLastPoint->GetPos(), pCurrentPoint->GetPos() );
            MT_Vector2D posIntersect;
            if( lima.Intersect2D( segment, posIntersect ) )
            {
                for( auto itFunction = lima.GetFunctions().begin(); itFunction != lima.GetFunctions().end(); ++itFunction )
                {
                    auto pPoint = boost::make_shared< DEC_Rep_PathPoint_Lima >( posIntersect, TerrainData(), lima.GetID(), **itFunction );
                    auto itTmp = resultList_.insert( it, pPoint );
                    InsertPointAvant( pPoint, itTmp );
                }
            }
        }
        pLastPoint = pCurrentPoint;
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_Agent_Path::InsertLimas
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
void DEC_Agent_Path::InsertLimas()
{
    const T_LimaVector& limas = queryMaker_.GetOrderManager().GetLimas();
    for( auto it = limas.begin(); it != limas.end(); ++it )
        InsertLima( *it );
}

void DEC_Agent_Path::Finalize()
{
    if( !context_ )
        return;
    SetResult( computer_->GetResult() );
    context_.reset();
    // Points avants
    if( queryMaker_.GetRole< PHY_RoleInterface_Location >().GetHeight() == 0 )
        InsertPointAvants();
    // Limas
    InsertLimas();
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::NotifyPointReached
// Created: LDC 2012-01-18
// -----------------------------------------------------------------------------
void DEC_Agent_Path::NotifyPointReached( const T_PathPoints::const_iterator& itCurrentPathPoint )
{
    const T_PointVector& computedWaypoints = computer_->GetComputedWaypoints();
    if( nextWaypoints_.size() > 1 && computedWaypoints.size() > 1 &&
        static_cast< float >( (*itCurrentPathPoint)->GetPos().rX_ ) == static_cast< float >( computedWaypoints.front().rX_ ) &&
        static_cast< float >( (*itCurrentPathPoint)->GetPos().rY_ ) == static_cast< float >( computedWaypoints.front().rY_ ) )
    {
        nextWaypoints_.erase( nextWaypoints_.begin() );
        computer_->RemoveComputedWaypoint();
    }
    DEC_PathResult::NotifyPointReached( itCurrentPathPoint );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetPathClass
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
const DEC_Agent_PathClass& DEC_Agent_Path::GetPathClass() const
{
    return pathClass_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetNextWaypoints
// Created: CMA 2012-02-20
// -----------------------------------------------------------------------------
const T_PointVector& DEC_Agent_Path::GetNextWaypoints() const
{
    return nextWaypoints_;
}

DEC_Path_ABC::E_State DEC_Agent_Path::GetState() const
{
    return computer_->GetState();
}

const MT_Vector2D& DEC_Agent_Path::GetLastWaypoint() const
{
    return computer_->GetLastWaypoint();
}

double DEC_Agent_Path::GetLength() const
{
    return computer_->GetLength();
}
