//*****************************************************************************
// 
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Path.cpp $
// $Author: Age $
// $Modtime: 16/06/05 15:23 $
// $Revision: 28 $
// $Workfile: DEC_Path.cpp $
// 
//*****************************************************************************

#include "MIL_pch.h"

#if 0

#include "DEC_Population_Path.h"

//#include "DEC_PathSection.h"
//#include "DEC_PathPoint.h"
//#include "DEC_PathFind_Manager.h"
//#include "Decision/Knowledge/DEC_Rep_PathPoint_Front.h"
//#include "Decision/Knowledge/DEC_Rep_PathPoint_Special.h"
//#include "Decision/Knowledge/DEC_Rep_PathPoint_Lima.h"
//#include "Entities/Agents/MIL_AgentPion.h"
//#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
//#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
//#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
//#include "Entities/Automates/MIL_Automate.h"
//#include "Entities/Objects/MIL_RealObjectTypeFilter.h"
//#include "Entities/Orders/Lima/MIL_Lima.h"
//#include "Knowledge/DEC_Knowledge_Object.h"
//#include "Knowledge/DEC_Knowledge_Agent.h"
//#include "Knowledge/DEC_KS_AgentQuerier.h"
//#include "Network/NET_ASN_Messages.h"
//#include "Network/NET_ASN_Tools.h"
//#include "Tools/MIL_Tools.h"
//#include "TER/TER_World.h"

//-----------------------------------------------------------------------------
// Name: DEC_Path constructor
// Created: JVT 02-09-17
//-----------------------------------------------------------------------------
DEC_Path::DEC_Path( const MIL_Population& queryMaker, const MT_Vector2D& vPosEnd ) 
    : queryMaker_        ( queryMaker )
    , bSectionJustEnded_ ( false )
{
    T_PointVector pointsTmp;
    pointsTmp.push_back( queryMaker_.GetRole< PHY_RolePion_Location >().GetPosition() );
    pointsTmp.push_back( vPosEnd );
    Initialize( pointsTmp );
}

//-----------------------------------------------------------------------------
// Name: DEC_Path constructor
// Created: NLD 2005-06-30
// -----------------------------------------------------------------------------
DEC_Path::DEC_Path( const DEC_Path& rhs )
    : nID_               ( (++nIDIdx_) != nInvalidID_ ? nIDIdx_ : ++nIDIdx_ )
    , queryMaker_        ( rhs.queryMaker_ )
    , fuseau_            () //$$$ Debile
    , automateFuseau_    () //$$$ Debile
    , vDirDanger_        ( rhs.vDirDanger_ )
 //   , unitSpeeds_        ( rhs.unitSpeeds_ ) $$$ TODO
    , unitSpeeds_        ( queryMaker_.GetRole< PHY_RoleAction_Moving >() )
    , rMaxSlope_         ( rhs.rMaxSlope_ )
    , pathType_          ( rhs.pathType_ )
    , bDecPointsInserted_( false )
    , bSectionJustEnded_ ( false )
{
    fuseau_         = rhs.fuseau_;
    automateFuseau_ = rhs.automateFuseau_;

    T_PointVector pointsTmp;
    rhs.GetPathPoints( pointsTmp );
    Initialize( pointsTmp );
}

//-----------------------------------------------------------------------------
// Name: DEC_Path destructor
// Created: DFT 02-03-04
// Last modified: JVT 02-09-17
//-----------------------------------------------------------------------------
DEC_Path::~DEC_Path()
{
    // $$$$ NLD 2005-09-29: const_cast pour DIA
    DEC_RolePion_Decision& roleDecision = const_cast< MIL_AgentPion& >( queryMaker_ ).GetRole< DEC_RolePion_Decision >();
    for( IT_PathPointList itPoint = resultList_.begin(); itPoint != resultList_.end(); ++itPoint )
    {
        (*itPoint)->RemoveFromDIA( roleDecision );
        delete *itPoint;
    }
    resultList_.clear();
    fuseau_        .Reset();
    automateFuseau_.Reset();
}

//=============================================================================
// INIT
//=============================================================================

 // -----------------------------------------------------------------------------
// Name: DEC_Path::Initialize
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void DEC_Path::Initialize( const T_PointVector& points )
{
    InitializePathKnowledges( points );

//    if( ! fuseau_.IsNull() && !fuseau_.IsInside( vStartPoint ) )
//    {
//        MT_Vector2D vEntryPoint;
//        fuseau_.ComputeEntryPoint( vStartPoint, vEntryPoint );
//        points.insert( points.begin(), vEntryPoint );
//    }

    assert( !points.empty() );
    const MT_Vector2D* pLastPoint = 0;
    for( CIT_PointVector itPoint = points.begin(); itPoint != points.end(); ++itPoint )
    {
        if( pLastPoint )
        {
            DEC_PathSection* pSection = new DEC_PathSection( *this, pathType_, *pLastPoint, *itPoint );
            RegisterPathSection( *pSection );
        }
        pLastPoint = &*itPoint;
    }
}

// -----------------------------------------------------------------------------
// Name: IsObjectInsidePathPoint
// Created: NLD 2005-07-21
// -----------------------------------------------------------------------------
static
bool IsObjectInsidePathPoint( const DEC_Knowledge_Object& knowledge, const T_PointVector& pathPoints )
{
    const TER_Localisation& loc = knowledge.GetLocalisation();
    for( CIT_PointVector it = pathPoints.begin(); it != pathPoints.end(); ++it )
    {
        if( loc.IsInside( *it ) )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::InitializePathKnowledges
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
void DEC_Path::InitializePathKnowledges( const T_PointVector& pathPoints )
{
    // Agents
    T_KnowledgeAgentVector knowledgesAgent;
    queryMaker_.GetKSQuerier().GetEnemies( knowledgesAgent );
    for( CIT_KnowledgeAgentVector itKnowledgeAgent = knowledgesAgent.begin(); itKnowledgeAgent != knowledgesAgent.end(); ++itKnowledgeAgent )
    {
        const DEC_Knowledge_Agent& knowledge = **itKnowledgeAgent;
        if( fuseau_.IsInside( knowledge.GetPosition() ) )
            pathKnowledgeAgentVector_.push_back( DEC_Path_KnowledgeAgent( knowledge, queryMaker_ ) );
    }

    // Objects
    T_KnowledgeObjectVector knowledgesObject;    
    queryMaker_.GetKSQuerier().GetObjects( knowledgesObject );
    for( CIT_KnowledgeObjectVector itKnowledgeObject = knowledgesObject.begin(); itKnowledgeObject != knowledgesObject.end(); ++itKnowledgeObject )
    {
        const DEC_Knowledge_Object& knowledge = **itKnowledgeObject;
        //$$$ POURRI : Faire un DEC_Knowledge_Object::CanCollideWithXXX
        if(        !knowledge.IsPrepared() 
                && !knowledge.IsBypassed() 
                && queryMaker_.GetRole< PHY_RolePion_Location >().GetHeight() <= knowledge.GetMaxInteractionHeight() 
                && !IsObjectInsidePathPoint( knowledge, pathPoints ) ) //$$$ BOF
            pathKnowledgeObjectVector_.push_back( DEC_Path_KnowledgeObject( knowledge ) );
    }       
}

// =============================================================================
// DEC POINTS
// =============================================================================

//-----------------------------------------------------------------------------
// Name: DEC_Path::IsPointAvantIn
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
bool DEC_Path::IsPointAvantIn( const TerrainData& nObjectTypesBefore, const TerrainData& nObjectTypesToNextPoint, const TerrainData& nTypeTerrain ) const
{
    // Test : entrée dans environnement
    return !( nObjectTypesBefore.ContainsOne( nTypeTerrain ) ) && ( nObjectTypesToNextPoint.ContainsOne( nTypeTerrain ) );
}

//-----------------------------------------------------------------------------
// Name: DEC_Path::IsPointAvantOut
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
bool DEC_Path::IsPointAvantOut( const TerrainData& nObjectTypesBefore, const TerrainData& nObjectTypesToNextPoint, const TerrainData& nTypeTerrain ) const
{
    // Test : sortie d'environnement
    return ( nObjectTypesBefore.ContainsOne( nTypeTerrain ) ) && !( nObjectTypesToNextPoint.ContainsOne(  nTypeTerrain ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::IsPointAvant
// Created: AGE 2005-02-04
// -----------------------------------------------------------------------------
bool DEC_Path::IsPointAvant( const TerrainData& nObjectTypesBefore, const TerrainData& nObjectTypesToNextPoint, const TerrainData& nTypeTerrain ) const
{
    return IsPointAvantIn ( nObjectTypesBefore, nObjectTypesToNextPoint, nTypeTerrain )
        || IsPointAvantOut( nObjectTypesBefore, nObjectTypesToNextPoint, nTypeTerrain );
}

//-----------------------------------------------------------------------------
// Name: DEC_Path::GetPreviousPathPointOnDifferentLocation
// Created: JVT 2005-07-08
// -----------------------------------------------------------------------------
inline
DEC_Path::IT_PathPointList DEC_Path::GetPreviousPathPointOnDifferentLocation( IT_PathPointList itCurrent )
{
    assert( itCurrent != resultList_.end() );

    const MT_Vector2D& vPosition = (*itCurrent)->GetPos();
    
    while ( itCurrent != resultList_.begin() && (*itCurrent)->GetPos() == vPosition )
        --itCurrent;
        
    return itCurrent;
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::InsertPointAvant
// Created: NLD 2005-08-11
// -----------------------------------------------------------------------------
void DEC_Path::InsertPointAvant( DEC_Rep_PathPoint& spottedPathPoint, IT_PathPointList itCurrent )
{
    MT_Float rDistanceLeft = spottedPathPoint.GetTypePoint() == DEC_Rep_PathPoint::eTypePointLima ?
                             queryMaker_.GetType().GetDistanceAvantLima() :
                             queryMaker_.GetType().GetDistanceAvantPoint( spottedPathPoint.GetTypeTerrain() );

    while( true )
    {
        const MT_Vector2D& vCurrentPos = (*itCurrent)->GetPos();

        if( itCurrent == resultList_.begin() )
        {
            DEC_Rep_PathPoint_Front* pNewPoint = new DEC_Rep_PathPoint_Front( *this, vCurrentPos, spottedPathPoint );
            spottedPathPoint.GetVariable( DEC_Rep_PathPoint::nDIAavtIdx_ ).SetValue( *pNewPoint );
            resultList_.insert( ++itCurrent, pNewPoint );
            break;
        }

        // remise en position des itérateurs
        IT_PathPointList itPrev = GetPreviousPathPointOnDifferentLocation( itCurrent );

        itCurrent = itPrev; 
        ++itCurrent; 
        assert( (*itCurrent)->GetPos() == vCurrentPos );
        
        // calcul de la distance "mangée" par le parcours de ce segment
        const MT_Vector2D& vPreviousPos = (*itPrev)->GetPos();
              MT_Vector2D  vTmp         = vPreviousPos - vCurrentPos;
        const MT_Float     vTmpMag      = vTmp.Magnitude();
        
        rDistanceLeft -= vTmpMag;
        if( rDistanceLeft == 0. )
        {
            // Positionnement du point avant au même endroit qu'un autre point
            DEC_Rep_PathPoint_Front* pNewPoint = new DEC_Rep_PathPoint_Front( *this, vPreviousPos, spottedPathPoint );

            spottedPathPoint.GetVariable( DEC_Rep_PathPoint::nDIAavtIdx_ ).SetValue( *pNewPoint );
            resultList_.insert( itPrev, pNewPoint );
            break;
        }
        else if( rDistanceLeft < 0. )
        {
            // Positionnement du point avant 
            vTmp /= vTmpMag;
            vTmp *= rDistanceLeft;
            vTmp += vPreviousPos;

            DEC_Rep_PathPoint_Front* pNewPoint = new DEC_Rep_PathPoint_Front( *this, vTmp, spottedPathPoint );

            spottedPathPoint.GetVariable( DEC_Rep_PathPoint::nDIAavtIdx_ ).SetValue( *pNewPoint );
            resultList_.insert( itCurrent, pNewPoint );
            break;
        }

        // tant pis, ça sera peut-être pour le prochain segment
        itCurrent = itPrev;
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_Path::InsertPointAvant
// Created: JVT 02-12-04
//----------------------------------------------------------------------------
void DEC_Path::InsertPointAvant( DEC_Rep_PathPoint& spottedPathPoint, IT_PathPointList itCurrent, MT_Float& rDistSinceLastPointAvant )
{
    static MT_Float rDist = 2000.;
    if( rDistSinceLastPointAvant > rDist )
    {
        InsertPointAvant( spottedPathPoint, itCurrent );
        rDistSinceLastPointAvant = 0.;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::InsertPoint
// Created: NLD 2005-08-10
// -----------------------------------------------------------------------------
void DEC_Path::InsertPoint( DEC_Rep_PathPoint& spottedPathPoint, IT_PathPointList itCurrent, MT_Float& rDistSinceLastPoint )
{
    static MT_Float rDist = 500.;
    if( rDistSinceLastPoint > rDist )
    {
        resultList_.insert( itCurrent, &spottedPathPoint );
        rDistSinceLastPoint = 0.;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::Shit
// Created: NLD 2005-08-10
// -----------------------------------------------------------------------------
void DEC_Path::InsertPointAndPointAvant( DEC_Rep_PathPoint& spottedPathPoint, IT_PathPointList itCurrent, MT_Float& rDistSinceLastPoint, MT_Float& rDistSinceLastPointAvant )
{
    InsertPoint     ( spottedPathPoint, itCurrent, rDistSinceLastPoint      );
    InsertPointAvant( spottedPathPoint, itCurrent, rDistSinceLastPointAvant );
}

//-----------------------------------------------------------------------------
// Name: DEC_Path::InsertPointAvants
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
void DEC_Path::InsertPointAvants()
{
    MT_Float rDistSinceLastPointAvant = std::numeric_limits< MT_Float >::max();
    MT_Float rDistSinceLastPoint      = std::numeric_limits< MT_Float >::max();

    TerrainData nObjectTypesBefore;

    DEC_PathPoint* pPrevPoint = 0;

    for( IT_PathPointList itPoint = resultList_.begin(); itPoint != resultList_.end(); ++itPoint )
    {
        DEC_PathPoint& current = **itPoint;

        if( pPrevPoint )
        {
            if( rDistSinceLastPointAvant != std::numeric_limits< MT_Float >::max() )
                rDistSinceLastPointAvant += pPrevPoint->GetPos().Distance( current.GetPos() );
            if( rDistSinceLastPoint != std::numeric_limits< MT_Float >::max() )
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
            pPrevPoint         = &current;
            nObjectTypesBefore = nObjectTypesToNextPoint;
            continue;
        }
        
        // Village
        if( IsPointAvantIn( nObjectTypesBefore, nObjectTypesToNextPoint, TerrainData::Urban() ) )
            InsertPointAndPointAvant( *new DEC_Rep_PathPoint_Special( *this, (*itPoint)->GetPos(), DEC_Rep_PathPoint_Special::eTypePointParticulierVillage, TerrainData::Urban() ), itPoint, rDistSinceLastPoint, rDistSinceLastPointAvant );

        else if( IsPointAvantOut( nObjectTypesBefore, nObjectTypesToNextPoint, TerrainData::Urban() ) )
            InsertPoint( *new DEC_Rep_PathPoint( *this, (*itPoint)->GetPos(), DEC_Rep_PathPoint::eTypePointCCT, TerrainData::Urban() ), itPoint, rDistSinceLastPoint );

        // Forest
        else if( IsPointAvant( nObjectTypesBefore, nObjectTypesToNextPoint, TerrainData::Forest() ) )
            InsertPointAndPointAvant( *new DEC_Rep_PathPoint( *this, (*itPoint)->GetPos(), DEC_Rep_PathPoint::eTypePointCCT, TerrainData::Forest() ), itPoint, rDistSinceLastPoint, rDistSinceLastPointAvant );

        // Cross roads
        else if( current.GetObjectTypes().ContainsOne( TerrainData::Crossroad() ) )
            InsertPointAndPointAvant( *new DEC_Rep_PathPoint_Special( *this, (*itPoint)->GetPos(), DEC_Rep_PathPoint_Special::eTypePointParticulierCarrefour, TerrainData::Crossroad() ), itPoint, rDistSinceLastPoint, rDistSinceLastPointAvant );

        // Pont
        else if( IsPointAvantIn( nObjectTypesBefore, nObjectTypesToNextPoint, TerrainData::Bridge() ) 
                || ( !current.GetObjectTypes().ContainsOne( TerrainData::SmallRiver() ) && current.GetObjectTypes().ContainsOne( TerrainData::Bridge() ) && !nObjectTypesBefore.ContainsOne( TerrainData::Bridge() ) && !nObjectTypesToNextPoint.ContainsOne( TerrainData::Bridge() ) )
                )
            InsertPointAndPointAvant( *new DEC_Rep_PathPoint_Special( *this, (*itPoint)->GetPos(), DEC_Rep_PathPoint_Special::eTypePointParticulierPont, TerrainData::Bridge() ), itPoint, rDistSinceLastPoint, rDistSinceLastPointAvant );

        nObjectTypesBefore = nObjectTypesToNextPoint;

        pPrevPoint = &current;
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_Path::InsertLimas
// Created: JVT 02-12-04
//-----------------------------------------------------------------------------
void DEC_Path::InsertLimas()
{
    const T_LimaFlagedPtrMap& limaMap = queryMaker_.GetLimas();
    if( limaMap.empty() )
        return;

    for( CIT_LimaFlagedPtrMap itLima = limaMap.begin(); itLima != limaMap.end(); ++itLima )
        InsertLima( *itLima->first );
}


//-----------------------------------------------------------------------------
// Name: DEC_Path::InsertLima
// Created: JVT 02-12-05
//-----------------------------------------------------------------------------
void DEC_Path::InsertLima( const MIL_Lima& lima )
{
    DEC_PathPoint* pLastPoint = 0;

    for( IT_PathPointList itPoint = resultList_.begin(); itPoint != resultList_.end(); ++itPoint )
    {
        DEC_PathPoint* pCurrentPoint = *itPoint;
        assert ( pCurrentPoint );

        if( pLastPoint && ( pCurrentPoint->GetPos() != pLastPoint->GetPos() ) )
        {
            MT_Line segment( pLastPoint->GetPos(), pCurrentPoint->GetPos() );
            MT_Vector2D posIntersect;
            if ( lima.Intersect2D( segment, posIntersect ) )
            {
                DEC_Rep_PathPoint* pPoint = new DEC_Rep_PathPoint_Lima( *this, posIntersect, TerrainData(), lima );
                IT_PathPointList itTmp = resultList_.insert( itPoint, pPoint );
                InsertPointAvant( *pPoint, itTmp );
            }
        }
        pLastPoint = pCurrentPoint;
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_Path::InsertDecPoints
// Created: JDY 03-03-04
//-----------------------------------------------------------------------------
void DEC_Path::InsertDecPoints()
{
    if( bDecPointsInserted_ )
        return;

    bDecPointsInserted_ = true;

    // Points avants
    if( queryMaker_.GetRole< PHY_RolePion_Location >().GetHeight() == 0 )
        InsertPointAvants();

    // Limas
    InsertLimas();


    //$$$$$ TEST
    /*T_PointVector cock;
    std::cout << "========== BEGIN PATH ============" << std::endl;
    for( IT_PathPointList itPoint = resultList_.begin(); itPoint != resultList_.end(); ++itPoint )
    {
        DEC_PathPoint& current = **itPoint;

        current.Dump();

        if( current.GetType() == DEC_PathPoint::eTypePointSpecial )
            cock.push_back( current.GetPos() );
    }
    std::cout << "========== END PATH ============" << std::endl;
   

    NET_AS_MOSServerMsgMgr& msgMgr = MIL_AgentServer::GetWorkspace().GetAgentServer().GetMessageMgr();
    DIN::DIN_BufferedMessage dinMsg = msgMgr.BuildMessage();
    
    dinMsg << (uint32)GetQueryMaker().GetID();
    dinMsg << (uint32)cock.size();
    for( CIT_PointVector itPoint = cock.begin(); itPoint != cock.end(); ++itPoint )
        dinMsg << *itPoint;
    msgMgr.SendMsgDebugDrawPoints( dinMsg );
    //$$$$$ TEST*/
}

//=============================================================================
// TOOLS
//=============================================================================
//-----------------------------------------------------------------------------
// Name: DEC_Path::GetClosestPointOnPath
// Created: AGN 03-01-13
//-----------------------------------------------------------------------------
MT_Vector2D DEC_Path::GetPointOnPathCloseTo( const MT_Vector2D& posToTest ) const
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
// Name: DEC_Path::GetNextKeyOnPath
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
DEC_Path::CIT_PathPointList DEC_Path::GetCurrentKeyOnPath( const MT_Vector2D& vPos ) const
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

// -----------------------------------------------------------------------------
// Name: DEC_Path::ComputeFutureObjectCollisions
// Created: NLD 2003-10-08
// -----------------------------------------------------------------------------
void DEC_Path::ComputeFutureObjectCollisions( const MT_Vector2D& vStartPos, const T_KnowledgeObjectVector& objectsToTest, T_KnowledgeObjectMultimap& objectsOnPathMap ) const
{
    objectsOnPathMap.clear();

    CIT_PathPointList itCurrentPathPoint = GetCurrentKeyOnPath( vStartPos );
    if( itCurrentPathPoint == resultList_.end() )
        return;
    
    CIT_PathPointList itNextPathPoint = itCurrentPathPoint;
    ++itNextPathPoint;
    
    E_State nPathState = GetState();
    if( nPathState != eValid && nPathState != ePartial )
        return;

    assert( !resultList_.empty() );

    const PHY_RolePion_Location& roleLocation = queryMaker_.GetRole< PHY_RolePion_Location >();

    // Determination de tous les objets connus avec lesquels il va y avoir collision dans le déplacement en cours
    for( CIT_KnowledgeObjectVector itKnowledge = objectsToTest.begin(); itKnowledge != objectsToTest.end(); ++itKnowledge )
    {
        DEC_Knowledge_Object& knowledge = **itKnowledge;
        
        if( roleLocation.GetHeight() > knowledge.GetMaxInteractionHeight() )
            continue;

        const MT_Vector2D* pPrevPos = &vStartPos;

        for( DEC_Path::CIT_PathPointList itPathPoint = itNextPathPoint; itPathPoint != resultList_.end(); ++itPathPoint )
        {
			MT_Line lineTmp( *pPrevPos, (*itPathPoint)->GetPos() );

            TER_DistanceLess colCmp( *pPrevPos );
            T_PointSet collisions( colCmp );

            if( knowledge.GetLocalisation().Intersect2D( lineTmp, collisions ) )
            {
                assert( !collisions.empty() );
                if( !knowledge.GetLocalisation().IsInside( vStartPos ) )
                {
                    //$$$ Distance fausse (distance en ligne droite)
                    MT_Float rColDist = MIL_Tools::ConvertSimToMeter( vStartPos.Distance( *collisions.begin() ) );
                    objectsOnPathMap.insert( std::make_pair( rColDist, &knowledge ) );
                }
                break;
            }
            pPrevPos = &(*itPathPoint)->GetPos();
        }
    }
}

//-----------------------------------------------------------------------------
// Name: DEC_Path::InternalGetFuturePosition
// Created: JVT 03-09-25
//-----------------------------------------------------------------------------
MT_Vector2D DEC_Path::InternalGetFuturePosition( const CIT_PathPointList& itCurrentPos, MT_Float rDist, bool bBoundOnPath ) const
{
    assert( itCurrentPos != resultList_.end() );

    // recherche du prochain point sur le path
    // on passe tous les points spéciaux, car il n'y a des changement de direction que sur les PathPoint_Point
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
            return vEndPos + ( vEndPos - vStartPos ).Normalize() * rDist; // on suit la direction générale du déplacement
    }

    MT_Float rLength = vCurrentPos.Distance( (*itNextPos)->GetPos() );
    if ( rLength >= rDist )
        return vCurrentPos + ( (*itNextPos)->GetPos() - vCurrentPos ).Normalize() * rDist;

    // parcours sur le segment suivant
    return InternalGetFuturePosition( itNextPos, rDist - rLength, bBoundOnPath );
}


//-----------------------------------------------------------------------------
// Name: DEC_Path::GetFuturePosition
// Created: JVT 03-09-25
//-----------------------------------------------------------------------------
MT_Vector2D DEC_Path::GetFuturePosition( const MT_Vector2D& vStartPos, MT_Float rDist, bool bBoundOnPath ) const
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

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Path::Serialize
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
void DEC_Path::Serialize( ASN1T_Itineraire& asn ) const
{
    assert( !resultList_.empty() );

    ASN1T_CoordUTM* pASNCoordUTMSeq = new ASN1T_CoordUTM[ resultList_.size() ]; //$$ RAM

    asn.type               = EnumTypeLocalisation::line;
    asn.vecteur_point.n    = resultList_.size();
    asn.vecteur_point.elem = pASNCoordUTMSeq;

    uint i = 0;
    for( CIT_PathPointList itPathPoint = resultList_.begin(); itPathPoint != resultList_.end(); ++itPathPoint )
        NET_ASN_Tools::WritePoint( (*itPathPoint)->GetPos(), asn.vecteur_point.elem[i++] );
}


// =============================================================================
// PATH CALCULATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Path::Execute
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
void DEC_Path::Execute( TerrainPathfinder& pathfind )
{    
    if( MIL_AgentServer::GetWorkspace().GetConfig().UsePathDebug() )
    {
        MT_LOG_MESSAGE_MSG( "DEC_Path::Compute: " << this << " : computation begin" );
        MT_LOG_MESSAGE_MSG( "   Thread    : " << MIL_AgentServer::GetWorkspace().GetPathFindManager().GetCurrentThread() );
        MT_LOG_MESSAGE_MSG( "   Agent     : " << queryMaker_.GetID() );
        MT_LOG_MESSAGE_MSG( "   Path type : " << pathType_.ConvertPathTypeToString() );
        MT_LOG_MESSAGE_MSG( GetPathAsString() );
        profiler_.Start();
    }

    assert( resultList_.empty() );

    ///$$$ try/catch() à déplacer DEC_Path_ABC
    try
    {
        DEC_Path_ABC::Execute( pathfind );
    }
    catch( ... )
    {
        Cancel();
        DEC_Path_ABC::Execute( pathfind ); //$$$$ ???
        DecRef();
        throw;
    }

#ifndef NDEBUG
    for( CIT_PathPointList itPoint = resultList_.begin(); itPoint != resultList_.end(); )
    {
        DEC_PathPoint& point = **itPoint;

//        if( itPoint != resultList_.begin() )
//            assert( unitSpeeds_.IsPassable( point.GetObjectTypes() ) );
        ++itPoint;
        if( itPoint != resultList_.end() )
            assert( unitSpeeds_.GetMaxSpeed( point.GetObjectTypesToNextPoint() ) > 0 );
    }
#endif

    if( MIL_AgentServer::GetWorkspace().GetConfig().UsePathDebug() )
    {
        double rComputationTime = profiler_.Stop();

        std::stringstream stream;
        if ( ! resultList_.empty() )
            stream << "[" << resultList_.front()->GetPos() << "] -> [" << resultList_.back()->GetPos() << "]";
        MT_LOG_MESSAGE_MSG( "DEC_Path::Compute: " << this << 
                            ", Thread : "  << MIL_AgentServer::GetWorkspace().GetPathFindManager().GetCurrentThread() <<
                            ", Time : " << rComputationTime << 
                            ", State : " << GetStateAsString() <<
                            ", Result : " << stream.str() );
    }
    DecRef(); // We are no longer in the pathfind queue
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::AddResultPoint
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
void DEC_Path::AddResultPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint )
{
    if( bSectionJustEnded_ )
    {
        // Pop last point
        resultList_.pop_back();
        bSectionJustEnded_ = false;
    }
    
//    assert( resultList_.empty() || unitSpeeds_.IsPassable ( nObjectTypes ) );
    resultList_.push_back( new DEC_PathPoint( vPos, nObjectTypes, nObjectTypesToNextPoint ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Path::NotifySectionEnded
// Created: AGE 2005-09-01
// -----------------------------------------------------------------------------
void DEC_Path::NotifySectionEnded()
{
    bSectionJustEnded_ = true;
}


#endif