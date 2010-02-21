//*****************************************************************************
// 
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_PathResult.cpp $
// $Author: Age $
// $Modtime: 16/06/05 15:23 $
// $Revision: 28 $
// $Workfile: DEC_PathResult.cpp $
// 
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_PathResult.h"
#include "DEC_PathPoint.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/protocol.h"

// -----------------------------------------------------------------------------
// Name: DEC_PathResult constructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
DEC_PathResult::DEC_PathResult()
    : DEC_Path_ABC      ()
    , bSectionJustEnded_( false )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: DEC_PathResult destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
DEC_PathResult::~DEC_PathResult()
{
    for( CIT_PathPointList itPoint = resultList_.begin(); itPoint != resultList_.end(); ++itPoint )
        delete *itPoint;
    resultList_.clear();
}

//=============================================================================
// TOOLS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: DEC_PathResult::GetClosestPointOnPath
// Created: AGN 03-01-13
//-----------------------------------------------------------------------------
MT_Vector2D DEC_PathResult::GetPointOnPathCloseTo( const MT_Vector2D& posToTest ) const
{
    assert( !resultList_.empty() );
    CIT_PathPointList itStart = resultList_.begin();
    CIT_PathPointList itEnd   = resultList_.begin();
    ++itEnd;

    MT_Vector2D result( (*itStart)->GetPos() );
    MT_Float rDistance = std::numeric_limits< MT_Float >::max();
    for( itStart = resultList_.begin(); itEnd != resultList_.end(); ++itStart, ++itEnd )
    {
        MT_Line vLine( (*itStart)->GetPos(), (*itEnd)->GetPos() );
        MT_Vector2D vClosest = vLine.ClosestPointOnLine( posToTest );

        MT_Float rCurrentDistance = vClosest.SquareDistance( posToTest );

        if( rCurrentDistance < rDistance )
        {
            rDistance = rCurrentDistance;
            result = vClosest;
        }
    }
    assert( !_isnan( result.rX_ ) );
    assert( !_isnan( result.rY_ ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::GetNextKeyOnPath
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
DEC_PathResult::CIT_PathPointList DEC_PathResult::GetCurrentKeyOnPath( const MT_Vector2D& vPos ) const
{
    if( resultList_.empty() )
        return resultList_.end();

    static const MT_Float rWeldValue = TER_World::GetWorld().GetWeldValue();

    if( resultList_.size() == 1 )
    {
        if( vPos.Distance( resultList_.front()->GetPos() ) <= rWeldValue )
            return resultList_.begin();
        return resultList_.end();
    }

    CIT_PathPointList itEnd = resultList_.begin();
    ++itEnd;
    
    for( CIT_PathPointList itStart = resultList_.begin(); itEnd != resultList_.end(); ++itStart, ++itEnd )
    {
        MT_Line vLine( (*itStart)->GetPos(), (*itEnd)->GetPos() );

        if( vLine.IsInside( vPos, rWeldValue ) ) //$$$ DE LA MERDE EN BOITE
            return itStart;
    }
    return resultList_.end();
}

//-----------------------------------------------------------------------------
// Name: DEC_PathResult::InternalGetFuturePosition
// Created: JVT 03-09-25
//-----------------------------------------------------------------------------
MT_Vector2D DEC_PathResult::InternalGetFuturePosition( const CIT_PathPointList& itCurrentPos, MT_Float rDist, bool bBoundOnPath ) const
{
    assert( itCurrentPos != resultList_.end() );

    // recherche du prochain point sur le path
    // on passe tous les points sp�ciaux, car il n'y a des changement de direction que sur les PathPoint_Point
    CIT_PathPointList itNextPos = itCurrentPos;
    while ( ++itNextPos != resultList_.end() && (*itNextPos)->GetType() != DEC_PathPoint::eTypePointPath )
        ;
    
    const MT_Vector2D& vCurrentPos = (*itCurrentPos)->GetPos();
    if( itNextPos == resultList_.end() )
    {   
        const MT_Vector2D& vStartPos = resultList_.front()->GetPos();
        const MT_Vector2D& vEndPos   = resultList_.back ()->GetPos();

        if( bBoundOnPath )
            return vEndPos;
        else
            return vEndPos + ( vEndPos - vStartPos ).Normalize() * rDist; // on suit la direction g�n�rale du d�placement
    }

    MT_Float rLength = vCurrentPos.Distance( (*itNextPos)->GetPos() );
    if ( rLength >= rDist )
        return vCurrentPos + ( (*itNextPos)->GetPos() - vCurrentPos ).Normalize() * rDist;

    // parcours sur le segment suivant
    return InternalGetFuturePosition( itNextPos, rDist - rLength, bBoundOnPath );
}

//-----------------------------------------------------------------------------
// Name: DEC_PathResult::GetFuturePosition
// Created: JVT 03-09-25
//-----------------------------------------------------------------------------
MT_Vector2D DEC_PathResult::GetFuturePosition( const MT_Vector2D& vStartPos, MT_Float rDist, bool bBoundOnPath ) const
{
    CIT_PathPointList itCurrentPathPoint = GetCurrentKeyOnPath( vStartPos );
    if( itCurrentPathPoint == resultList_.end() )
        return vStartPos;
    
    CIT_PathPointList itNextPathPoint = itCurrentPathPoint;
    ++itNextPathPoint;

    // Recherche du premier pathpoint  
    if ( itNextPathPoint == resultList_.end() )
    {
        const MT_Vector2D& vEndPos   = resultList_.back ()->GetPos();
        return vEndPos == vStartPos ? vEndPos : vStartPos + ( vEndPos - vStartPos ).Normalize() * rDist;
    }

    MT_Float rLength = vStartPos.Distance( (*itNextPathPoint)->GetPos() );
    if ( rLength >= rDist )
        return vStartPos + ( (*itNextPathPoint)->GetPos() - vStartPos ).Normalize() * rDist;

    // parcours sur les points
    return InternalGetFuturePosition( itNextPathPoint, rDist - rLength, bBoundOnPath );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::ComputeFutureObjectCollision
// Created: NLD 2003-10-08
// -----------------------------------------------------------------------------
bool DEC_PathResult::ComputeFutureObjectCollision( const MT_Vector2D& vStartPos, const T_KnowledgeObjectVector& objectsToTest, MT_Float& rDistance, boost::shared_ptr< DEC_Knowledge_Object >& pObject ) const
{
    rDistance = std::numeric_limits< MT_Float >::max();
    pObject.reset();

    E_State nPathState = GetState();
    if( nPathState != eValid && nPathState != ePartial )
        return false;

    assert( !resultList_.empty() );
    CIT_PathPointList itCurrentPathPoint = GetCurrentKeyOnPath( vStartPos );
    if( itCurrentPathPoint == resultList_.end() )
        return false;
        
    CIT_PathPointList itNextPathPoint = itCurrentPathPoint;
    ++itNextPathPoint;
       
    std::multimap< MT_Float, boost::shared_ptr< DEC_Knowledge_Object > > objectsOnPathMap;

    // Determination de tous les objets connus avec lesquels il va y avoir collision dans le d�placement en cours
    for( CIT_KnowledgeObjectVector itKnowledge = objectsToTest.begin(); itKnowledge != objectsToTest.end(); ++itKnowledge )
    {
        boost::shared_ptr< DEC_Knowledge_Object > pKnowledge = *itKnowledge;
        
        const MT_Vector2D* pPrevPos = &vStartPos;

        for( CIT_PathPointList itPathPoint = itNextPathPoint; itPathPoint != resultList_.end(); ++itPathPoint )
        {
            MT_Line lineTmp( *pPrevPos, (*itPathPoint)->GetPos() );

            TER_DistanceLess colCmp( *pPrevPos );
            T_PointSet collisions( colCmp );

            if( pKnowledge->GetLocalisation().Intersect2D( lineTmp, collisions ) )
            {
                assert( !collisions.empty() );
                //$$$ Distance fausse (distance en ligne droite)
                const MT_Float rColDist = vStartPos.Distance( *collisions.begin() );
                objectsOnPathMap.insert( std::make_pair( rColDist, pKnowledge ) );
                break;
            }
            pPrevPos = &(*itPathPoint)->GetPos();
        }
    }

    if( objectsOnPathMap.empty() )
        return false;

    rDistance = objectsOnPathMap.begin()->first;
    pObject   = objectsOnPathMap.begin()->second;
    return true;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::Serialize
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void DEC_PathResult::Serialize( Common::MsgPath& asn ) const
{
    assert( !resultList_.empty() );
    asn.mutable_location()->set_type( Common::MsgLocation::line );
    for( CIT_PathPointList it = resultList_.begin(); it != resultList_.end(); ++it )
        NET_ASN_Tools::WritePoint( (*it)->GetPos(), *asn.mutable_location()->mutable_coordinates()->add_elem() );
}

// =============================================================================
// RESULT 
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::AddResultPoint
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void DEC_PathResult::AddResultPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint )
{
    if( bSectionJustEnded_ )
    {
        // Pop last point
        delete resultList_.back();
        resultList_.pop_back();
        bSectionJustEnded_ = false;
    }
    resultList_.push_back( new DEC_PathPoint( vPos, nObjectTypes, nObjectTypesToNextPoint ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathResult::NotifySectionEnded
// Created: AGE 2005-09-01
// -----------------------------------------------------------------------------
void DEC_PathResult::NotifySectionEnded()
{
    bSectionJustEnded_ = true;
}
