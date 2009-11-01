// *****************************************************************************
//
// $Created: NLD 2003-08-20 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_GeometryFunctions.cpp $
// $Author: Nld $
// $Modtime: 7/06/05 9:59 $
// $Revision: 46 $
// $Workfile: DEC_GeometryFunctions.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "DEC_GeometryFunctions.h"
#include "DEC_FunctionsTools.h"
#include "DEC_FrontAndBackLinesComputer.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Decision/DEC_Tools.h"
#include "Decision/Path/DEC_PathPoint.h"
#include "Decision/Path/Agent/DEC_Agent_Path.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "Tools/MIL_Tools.h"
#include "simulation_terrain/TER_Localisation.h"
#include "MT_Tools/MT_Random.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"

#define PRECISION 0.0000001

// =============================================================================
// TOUT POURRI
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeAgentsBarycenter
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeAgentsBarycenter( const std::vector< DEC_Decision_ABC* >& selection )
{
    boost::shared_ptr< MT_Vector2D > pResult( new MT_Vector2D );
    for( std::vector< DEC_Decision_ABC* >::const_iterator it = selection.begin(); it != selection.end(); ++it )
    {
        DEC_Decision_ABC* pKnow = *it;
        assert( pKnow );

        *pResult += pKnow->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition();
    }

    if( selection.size() != 0 )
        *pResult = (*pResult / (MT_Float)selection.size());

    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDestPointForPion
// Created: JVT 2004-12-06
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeDestPointForPion( MIL_Automate& callerAutomate, DEC_Decision_ABC* pPion )
{
    assert( pPion );
    assert( std::find( callerAutomate.GetPions().begin(), callerAutomate.GetPions().end(), &pPion->GetPion() ) != callerAutomate.GetPions().end() );

    return ComputeDestPoint( pPion->GetPion() );
}

// =============================================================================
// CREATION / DESTRUCTION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CreateLocalisation
// Created: JVT 2005-05-11
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::CreateLocalisation()
{
    boost::shared_ptr< TER_Localisation > pLoc( new TER_Localisation() );
    return pLoc;
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CreateListPoint
// Created: NLD 2003-09-15
//-----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_GeometryFunctions::CreateListPoint()
{
    return std::vector< boost::shared_ptr< MT_Vector2D > >();
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CreatePoint
// Created: AGN 03-03-14
//-----------------------------------------------------------------------------
// static
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::CreatePoint()
{
    boost::shared_ptr< MT_Vector2D > pVect( new MT_Vector2D() );
    return pVect;
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CreateDirection
// Created: NLD 2003-05-13
//-----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::CreateDirection( MT_Vector2D* pPosSource, MT_Vector2D* pPosDest )
{
    assert( pPosSource );
    assert( pPosDest   );

    boost::shared_ptr< MT_Vector2D > pResult;
    if( *pPosSource == *pPosDest )
    {
        MT_Vector2D* pVect = new MT_Vector2D( 0., 1. );
        pResult.reset( pVect );
    }
    else
    {
        MT_Vector2D* pVect = new MT_Vector2D( (*pPosDest) - (*pPosSource) );
        pVect->Normalize();
        pResult.reset( pVect );
    }
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CreateOrthoDirection
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::CreateOrthoDirection( MT_Vector2D* pDir, bool bCounterClockwise )
{
    assert( pDir );
    assert( MT_IsZero( pDir->SquareMagnitude() - 1. ) );

    boost::shared_ptr< MT_Vector2D > pResult( new MT_Vector2D( *pDir ) );

    if( bCounterClockwise )
        pResult->Rotate90();
    else
        pResult->Rotate90ClockWise();

    return pResult;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ReverseDirection
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ReverseDirection( boost::shared_ptr< MT_Vector2D > pDir )
{
    assert( pDir.get() );
    *pDir *= -1.;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CopyAndReverseDirection
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::CopyAndReverseDirection( const MT_Vector2D* pDir )
{
    assert( pDir );

    boost::shared_ptr< MT_Vector2D > pNewDir( new MT_Vector2D( *pDir ) );
    *pNewDir *= -1.;
    return pNewDir;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CopyAndRotateDirection
// Created: NLD 2005-03-18
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::CopyAndRotateDirection( const MT_Vector2D* pDir, const MT_Float angle )
{
    assert( pDir );

    boost::shared_ptr< MT_Vector2D > pNewDir( new MT_Vector2D( *pDir ) );
    const MT_Float rAngle = - ( angle * MT_PI / 180. );
    pNewDir->Rotate( rAngle );
    return pNewDir;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeLocalisationPointsForPionsInFuseau
/**
*   Calcule les points dans une localisation pour un nombre de pions donné, répartis
*   par rapport au barycentre de l'objet clippé dans le fuseau, et sur la droite
*   perpendiculaire à la direction donnée en paramètre, en respectant un distance
*   maximale entre les points.
*/
// Created: NLD 2003-08-20
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_GeometryFunctions::ComputeLocalisationPointsForPionsInFuseau( const std::vector< DEC_Decision_ABC* >& pions, TER_Localisation* pLocalisation, MT_Vector2D* pDirDanger, MT_Float rDistMaxBtwPoints )
{
    assert( !pions.empty() );

    std::vector< boost::shared_ptr< MT_Vector2D > > result;

    // 1. Vérifie si les pions sont bien dans le même fuseau
    const MIL_Fuseau& fuseau = ( **pions.begin() ).GetPion().GetOrderManager().GetFuseau();
    std::vector< DEC_Decision_ABC* >::const_iterator itPion = pions.begin();
    for( ++itPion; itPion != pions.end(); ++itPion )
    {
        if( ( **itPion ).GetPion().GetOrderManager().GetFuseau() != fuseau ) //$$$ beark 
            return result;
    }

    // 2. Clippe le polygone dans le fuseau
    T_PointVector clippedPointVector;
    pLocalisation->GetPointsClippedByPolygon( fuseau, clippedPointVector );
    if( clippedPointVector.empty() )
        return result;

    // 3. Barycentre polygone clippé
    MT_Vector2D vBarycenter = MT_ComputeBarycenter( clippedPointVector );
    assert( fuseau.IsInside( vBarycenter ) );
    assert( TER_World::GetWorld().IsValidPosition( vBarycenter ) );

    // 4. Droite perpendiculaire dir dangereuse
    MT_Vector2D vDirTmp( *pDirDanger );
    vDirTmp.Normalize();
    MT_Float rTmp = vDirTmp.rX_;
    vDirTmp.rX_   = vDirTmp.rY_;
    vDirTmp.rY_   = -rTmp;
    MT_Droite lineForPositions( vBarycenter, vDirTmp + vBarycenter );

    // 5. Extrémités dans fuseau
    MT_Vector2D vLeftPoint;
    MT_Vector2D vRightPoint;
    fuseau.ComputeExtremities( lineForPositions, vLeftPoint, vRightPoint );

    // 6. Extrémités réélles dans objet clippé - fait que si l'objet est un polygone
    if( clippedPointVector.size() > 2 )
    {
        MT_Line segmentLeftSide ( vBarycenter, vLeftPoint  );
        MT_Line segmentRightSide( vBarycenter, vRightPoint );

        TER_DistanceLess cmp( vBarycenter );
        T_PointSet collisionSet( cmp );
        collisionSet.clear();
        segmentLeftSide.Intersect2D( clippedPointVector, collisionSet, PRECISION );
        if( !collisionSet.empty() )
            vLeftPoint = *(--collisionSet.end());
        collisionSet.clear();
        segmentRightSide.Intersect2D( clippedPointVector, collisionSet, PRECISION );
        if( !collisionSet.empty() )
            vRightPoint = *(--collisionSet.end());
    }

    // 7. Répartition des points
    result.reserve( pions.size() + 1 );

    // Nombre impaire => un pion est au barycentre
    if( pions.size() % 2 )
    {
        boost::shared_ptr< MT_Vector2D > point( new MT_Vector2D( vBarycenter ) );
        result.push_back( point );
    }

    uint nNbrPointsPerSide = pions.size() / 2;

    // Left side
    MT_Vector2D vDir( vLeftPoint - vBarycenter );
    MT_Float rIncr = std::min( vDir.Magnitude() / ( nNbrPointsPerSide + 1 ), rDistMaxBtwPoints );
    vDir.Normalize();
    for( uint i = 1; i <= nNbrPointsPerSide; ++i )
    {   
        boost::shared_ptr< MT_Vector2D > point( new MT_Vector2D( vBarycenter + vDir * ( rIncr * i ) ) );
        result.push_back( point );
    }

    // Right side
    vDir = MT_Vector2D( vRightPoint - vBarycenter );
    rIncr = std::min( vDir.Magnitude() / ( nNbrPointsPerSide + 1 ), rDistMaxBtwPoints );
    vDir.Normalize();
    for( uint j = 1; j <= nNbrPointsPerSide; ++j )
    {
        boost::shared_ptr< MT_Vector2D > point( new MT_Vector2D( vBarycenter + vDir * ( rIncr * j ) ) );
        result.push_back( point );
    }

    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::IsPointInsideLocalisation
// Created: NLD 2004-05-17
// -----------------------------------------------------------------------------
bool DEC_GeometryFunctions::IsPointInsideLocalisation( MT_Vector2D* pPoint, TER_Localisation* pLocalisation )
{
    return pLocalisation->IsInside( *pPoint );
}


// =============================================================================
// GEOMETRY - POINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ConvertPointToLocalisation
// Created: NLD 2003-08-19
// -----------------------------------------------------------------------------
// static
boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::ConvertPointToLocalisation( const MT_Vector2D* pPos )
{
    boost::shared_ptr< TER_Localisation > pLoc( new TER_Localisation() );
    pLoc->Reset( *pPos );
    return pLoc;
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::Distance
// Created: NLD 2002-12-24
//-----------------------------------------------------------------------------
MT_Float DEC_GeometryFunctions::Distance( const MT_Vector2D* p1, const MT_Vector2D* p2 )
{
    assert( p1 != 0 && p2 != 0 );
    return MIL_Tools::ConvertSimToMeter( p1->Distance( *p2 ) );
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::TranslatePosition
// Created: JDY 03-01-23
//-----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::TranslatePosition( MT_Vector2D* p1, MT_Vector2D* p2, MT_Float d )
{
    assert( p1 && p2 );

    boost::shared_ptr< MT_Vector2D > res( new MT_Vector2D() );
    if( (*p1) == (*p2) )
        *res = *p1;
    else
        *res=*p1+ d*(*p2-*p1).Normalized();

    return res;
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::TranslatePositionInDirection
// Created: AGN 03-04-11
//-----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::TranslatePositionInDirection( MT_Vector2D* p1, MT_Vector2D* p2, MT_Float d )
{
    assert( p1 && p2 );
    assert( MT_IsZero( p2->SquareMagnitude() - 1. ) );

    boost::shared_ptr< MT_Vector2D > res( new MT_Vector2D() );
    *res = *p1 + d * (*p2);

    return res;
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComparePositions
// Created: AGN 03-01-02
//-----------------------------------------------------------------------------
bool DEC_GeometryFunctions::ComparePositions( MT_Vector2D* p1, MT_Vector2D* p2 )
{
    assert( p1 && p2 );

    // $$$ JVT : Débile : si il y a besoin d'une "weldvalue" elle doit être prise en compte
    //           directement dans l'objet vecteur et dans ses operateurs ( en l'occurence == )
    static const MT_Float rWeldValue = TER_World::GetWorld().GetWeldValue();

    return( p1->Distance( *p2 ) <= rWeldValue );
}

// =============================================================================
// CALCULS DE POSITIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeSupportPosition
// Created: NLD 2003-10-15
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeSupportPosition( const MIL_AgentPion& callerAgent, DEC_Decision_ABC* pAgentToSupport, MT_Float rDist )
{
    assert( pAgentToSupport != 0 );

    const MT_Vector2D& vUnitToSupportPos = pAgentToSupport->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition ();
    const MIL_Fuseau& fuseau             = callerAgent.GetOrderManager().GetFuseau();

    MT_Vector2D  vDirLooked;
    pAgentToSupport->GetPion().GetRole< PHY_RoleInterface_Perceiver >().GetMainPerceptionDirection( vDirLooked );

    vDirLooked.Rotate90ClockWise();

    boost::shared_ptr< MT_Vector2D > pResult( new MT_Vector2D() );

    // $$$$ Position dans le fuseau : devrait être générique
    MT_Vector2D vSupportPos1( vUnitToSupportPos + vDirLooked * rDist );
    if( fuseau.IsInside( vSupportPos1 ) )
    {
        *pResult = vSupportPos1;
        return pResult;
    }

    MT_Vector2D vSupportPos2( vUnitToSupportPos + vDirLooked * -rDist );
    if( fuseau.IsInside( vSupportPos2 ) )
    {
        *pResult = vSupportPos2;
        return pResult;
    }

    MT_Line line1( vUnitToSupportPos, vSupportPos1 );
    MT_Line line2( vUnitToSupportPos, vSupportPos2 );

    TER_DistanceLess cmpLeft( vUnitToSupportPos );
    T_PointSet collisions( cmpLeft );

    fuseau.IntersectWithBorder( line1, collisions );
    fuseau.IntersectWithBorder( line2, collisions );
    if( collisions.empty() )
    {
        *pResult = vSupportPos1; //$$$ POURRI
        return pResult;
    }

    *pResult = *collisions.begin();
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeAmbushPosition
// Created: NLD 2003-11-05
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeAmbushPosition( const MIL_AgentPion& callerAgent, MT_Vector2D* pAmbushPosition, MT_Vector2D* pRetreatPosition, MT_Float rDist )
{
    assert( pAmbushPosition  );
    assert( pRetreatPosition );

    boost::shared_ptr< MT_Vector2D > pResult( new MT_Vector2D() );

    MT_Vector2D vDirAmbushPos = callerAgent.GetOrderManager().GetDirDanger();
    vDirAmbushPos.Rotate90ClockWise();
    vDirAmbushPos.Normalize();

    MT_Vector2D vAmbushPos1( *pAmbushPosition + vDirAmbushPos *  rDist );
    MT_Vector2D vAmbushPos2( *pAmbushPosition + vDirAmbushPos * -rDist );

    if( pRetreatPosition->SquareDistance( vAmbushPos1 ) < pRetreatPosition->SquareDistance( vAmbushPos2 ) )
        *pResult = vAmbushPos1;
    else
        *pResult = vAmbushPos2;
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeSafetyPosition
// Created: NLD 2004-04-28 (Copied from shit's JDY code)
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeSafetyPosition( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledgeEnemy, MT_Float rMinDistance )
{
    boost::shared_ptr< MT_Vector2D > pResult;
    if( pKnowledgeEnemy && pKnowledgeEnemy->IsValid() )
    {
        // Position de l'ennemi
        const MT_Vector2D& vEnemyPos  = pKnowledgeEnemy->GetPosition();

        MT_Vector2D vDirEniToAmi = ( callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition() - vEnemyPos).Normalize();
        if( vDirEniToAmi.IsZero() )
            vDirEniToAmi = -callerAgent.GetOrderManager().GetDirDanger();

        MT_Vector2D vSafetyPos = vEnemyPos + vDirEniToAmi * rMinDistance;

        TER_World::GetWorld().ClipPointInsideWorld( vSafetyPos );

        pResult.reset( new MT_Vector2D( vSafetyPos ) );
    }
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeSafetyPositionWithPopulation
// Created: SBO 2005-12-16
// Modified: RPD 2009-08-04
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeSafetyPositionWithPopulation( const MIL_AgentPion& callerAgent, uint nPopulationKnowledgeID, MT_Float rMinDistance )
{
    boost::shared_ptr< MT_Vector2D > pResult;
    DEC_Knowledge_Population* pKnowledge = callerAgent.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID( nPopulationKnowledgeID );
    if( pKnowledge )
        pResult.reset( new MT_Vector2D( pKnowledge->GetSafetyPosition( callerAgent, rMinDistance ) ) );
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeSafetyPositionWithObjective
// Created: NLD 2004-04-28
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeSafetyPositionWithObjective( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledgeEnnemy, MT_Float rMinMeterDistance, MT_Vector2D* pObjective )
{
    boost::shared_ptr< MT_Vector2D > pResult;
    if( pKnowledgeEnnemy.get() && pKnowledgeEnnemy->IsValid() )
    {
        assert( pObjective );

        MT_Float     rMinDistance = MIL_Tools::ConvertMeterToSim( rMinMeterDistance );

        const MT_Vector2D& vEnnemiPos  = pKnowledgeEnnemy->GetPosition();

        MT_Vector2D vDirEniToAmi       = ( callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition() - vEnnemiPos).Normalize();
        MT_Vector2D vDirEniToObjective = ( *pObjective - vEnnemiPos ).Normalize();

        MT_Vector2D vSafetyPos;
        if( vDirEniToAmi.IsZero() )
            vSafetyPos = vEnnemiPos + vDirEniToObjective * rMinDistance;
        else
        {
            const MT_Float rAngle = Angle( vDirEniToAmi, vDirEniToObjective );
            vSafetyPos = vEnnemiPos + vDirEniToAmi.Rotate( rAngle  * -0.5 ) * rMinDistance;
        }

        TER_World::GetWorld().ClipPointInsideWorld( vSafetyPos );

        pResult.reset( new MT_Vector2D( vSafetyPos ) );
    }
    return pResult;
}


//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeNearestFuseauEntryPoint
// Created: NLD 2003-04-24
//-----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeNearestFuseauEntryPoint( const MIL_AgentPion& callerAgent )
{
    boost::shared_ptr< MT_Vector2D > pResult( new MT_Vector2D() );
    callerAgent.GetOrderManager().GetFuseau().ComputeEntryPoint( callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition(), *pResult );
    return pResult;
}

// =============================================================================
// ASA
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputePosDeploiementASAOmni
// Created: JVT 2005-02-15
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_GeometryFunctions::ComputePosDeploiementASAOmni( const MIL_Automate& automat, int positionCount, const MT_Vector2D* center, float radius )
{
    assert( center );
    std::vector< boost::shared_ptr< MT_Vector2D > > result;
    if( positionCount > 0 )
    {
        const MT_Float    rAngle = 2. * MT_PI / positionCount;
        MT_Vector2D vDir = automat.GetOrderManager().GetDirDanger() * MIL_Tools::ConvertMeterToSim( radius );
        result.reserve( positionCount );
        while( positionCount-- )
        {
            result.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( *center + vDir ) ) );
            vDir.Rotate( rAngle );
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputePosDeploiementASANasse
// Created: JVT 2005-02-15
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_GeometryFunctions::ComputePosDeploiementASANasse( int positionCount, const MT_Vector2D* center, float angle, float initialDistance, float spacing, const MT_Vector2D* direction )
{
    assert( direction && center && MT_IsZero( direction->SquareMagnitude() - 1. ) );

    std::vector< boost::shared_ptr< MT_Vector2D > > result;
    if( positionCount <= 0 )
        return result;
    const MT_Float semiAngle = angle * MT_PI / 360.; // ( / 360. = * 0.5 / 180., car demi-angle );
    initialDistance = ( float ) MIL_Tools::ConvertMeterToSim( initialDistance );
    spacing         = ( float ) MIL_Tools::ConvertMeterToSim( spacing );
    MT_Vector2D vCenter = *center + initialDistance * *direction;

    const MT_Vector2D vSupport1( direction->Rotated(  semiAngle ) * spacing );
    const MT_Vector2D vSupport2( direction->Rotated( -semiAngle ) * spacing );

    result.reserve( positionCount );
    if( positionCount % 2 )
    {    // cas impair : on a en plus un point au centre
        result.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( vCenter ) ) );
        if( !--positionCount )
            return result;
    }

    assert( positionCount >= 2 && !( positionCount % 2 ) );
    result.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( vCenter + vSupport1 ) ) );
    result.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( vCenter + vSupport2 ) ) );
    for( positionCount -= 2; positionCount; positionCount -= 2 )
    {
        result.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( **( result.rbegin() + 1 ) + vSupport1 ) ) );
        result.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( **( result.rbegin() + 1 ) + vSupport2 ) ) );
    }
    return result;
 }

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputePosDeploiementASADoubleRideau
// Created: JVT 2005-02-15
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_GeometryFunctions::ComputePosDeploiementASADoubleRideau( int positionCount, const MT_Vector2D* center, float initialDistance, float lineSpacing, float pointSpacing, const MT_Vector2D* direction )
{
    assert( direction && center && MT_IsZero( direction->SquareMagnitude() - 1. ) );

    std::vector< boost::shared_ptr< MT_Vector2D > > result;
    if( positionCount <= 0 )
        return result;

    initialDistance = ( float ) MIL_Tools::ConvertMeterToSim( initialDistance );
    lineSpacing     = ( float ) MIL_Tools::ConvertMeterToSim( lineSpacing );
    pointSpacing    = ( float ) MIL_Tools::ConvertMeterToSim( pointSpacing );
    MT_Vector2D vDirection = *direction;

    MT_Vector2D vSupport1( vDirection.Rotated90() );
    MT_Vector2D vSupport2( -vSupport1 );

    result.reserve( positionCount );
    result.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( *center + initialDistance * vDirection + ( pointSpacing * .5 ) * vSupport1 ) ) );
    if( !--positionCount )
        return result;
    result.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( *center + initialDistance * vDirection + ( pointSpacing * .5 ) * vSupport2 ) ) );

    vDirection *= lineSpacing;
    vSupport1  *= pointSpacing;
    vSupport2  *= pointSpacing;

    while( --positionCount )
    {
        result.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( **( result.rbegin() + 1 ) + vDirection ) ) );
        if( !--positionCount ) break;
        // $$$$ _RC_ SBO 2009-07-31: Really? seems like the same point is pushed twice
        result.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( **( result.rbegin() + 1 ) + vDirection ) ) );
        if( !--positionCount ) break;
        result.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( **( result.rbegin() + 3 ) + vSupport1 ) ) );
        if( !--positionCount ) break;
        result.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( **( result.rbegin() + 3 ) + vSupport2 ) ) );
    }
    return result;
}


// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputePointsBeforeLima
// Created: NLD 2004-05-25
// Modified: RPD 2009-08-04
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_GeometryFunctions::ComputePointsBeforeLima( const MIL_Automate& callerAutomate, uint nLimaID, MT_Float rDistBeforeLima, uint nNbrPoints )
{
    MIL_LimaOrder* pLima = callerAutomate.GetOrderManager().FindLima( nLimaID );

    std::vector< boost::shared_ptr< MT_Vector2D > > result;
    if( pLima != 0 )
    {
        T_PointVector tempVector;
        callerAutomate.GetOrderManager().GetFuseau().ComputePointsBeforeLima( *pLima, rDistBeforeLima, nNbrPoints, tempVector );
        for( CIT_PointVector it = tempVector.begin(); it != tempVector.end(); ++it )
        {
            boost::shared_ptr< MT_Vector2D > point( new MT_Vector2D( *it ) );
            result.push_back( point );
        }
    }
    return result;
}

// =============================================================================
// LIGNES AVANT / ARRIERE /MIDDLE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDistanceFromMiddleLine
// Created: NLD 2003-10-01
// -----------------------------------------------------------------------------
float DEC_GeometryFunctions::ComputeDistanceFromMiddleLine( const std::vector< DEC_Decision_ABC*>& selPions, DEC_Decision_ABC* pReferencePion )
{
    if( selPions.empty() )
        return 0.f;

    // Barycenter of the pions given
    MT_Vector2D vBarycenter;
    for( std::vector< DEC_Decision_ABC*>::const_iterator itPion = selPions.begin(); itPion != selPions.end(); ++itPion )
        vBarycenter += (*itPion)->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition();
    vBarycenter /= selPions.size();

    const MT_Vector2D& vReferencePionPosition = pReferencePion->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition();

    //
    const MIL_Fuseau& fuseau = pReferencePion->GetPion().GetOrderManager().GetFuseau();
    MT_Float rDist;
    if( fuseau.IsNull() )
        rDist = vReferencePionPosition.Distance( vBarycenter );
    else
    {
        // Project it on the fuseau global direction
        MT_Vector2D vProjectedPionsBarycenter_ = fuseau.GetGlobalDirection().ClosestPointOnLine( vBarycenter );
        rDist = vReferencePionPosition.Distance( vProjectedPionsBarycenter_ );
    }
    return MIL_Tools::ConvertSimToMeter( rDist );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::StartComputingFrontAndBackLines
// Created: NLD 2004-10-19
// -----------------------------------------------------------------------------
DEC_FrontAndBackLinesComputer* DEC_GeometryFunctions::StartComputingFrontAndBackLines( const MIL_Automate& callerAutomate, const std::vector< DEC_Decision_ABC* >& pionDecisionList )
{
    std::vector< MIL_AgentPion*> pions;
    pions.reserve( pionDecisionList.size() );
    for( std::vector< DEC_Decision_ABC* >::const_iterator it = pionDecisionList.begin(); it != pionDecisionList.end(); ++it )
        pions.push_back( &static_cast< DEC_Decision_ABC& >( **it ).GetPion() );
    DEC_FrontAndBackLinesComputer* pComputer = new DEC_FrontAndBackLinesComputer( callerAutomate, pions );
    return pComputer;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::StartComputingAutomatFrontAndBackLines
// Created: LDC 2009-07-06
// -----------------------------------------------------------------------------
DEC_FrontAndBackLinesComputer* DEC_GeometryFunctions::StartComputingAutomatFrontAndBackLines( const MIL_Automate& callerAutomate, const std::vector< DEC_Decision_ABC* >& automatDecisionList )
{
    std::vector< MIL_Automate* > automats;
    automats.reserve( automatDecisionList.size() );
    for( std::vector< DEC_Decision_ABC* >::const_iterator it = automatDecisionList.begin(); it != automatDecisionList.end(); ++it )
        automats.push_back( &static_cast< DEC_Decision_ABC& >( **it ).GetAutomate() );
    DEC_FrontAndBackLinesComputer* pComputer = new DEC_FrontAndBackLinesComputer( callerAutomate, automats );
    return pComputer;
}                           

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::StopComputingFrontAndBackLines
// Created: NLD 2004-10-19
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::StopComputingFrontAndBackLines( DEC_FrontAndBackLinesComputer* /*pComputer*/ )
{
//    delete pComputer;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDistanceFromFrontLine
// Created: NLD 2003-09-29
// -----------------------------------------------------------------------------
float DEC_GeometryFunctions::ComputeDistanceFromFrontLine( DEC_FrontAndBackLinesComputer* pComputer, DEC_Decision_ABC* pPion )
{
    assert( pComputer );

    MT_Float rDist = 0;
    assert( pPion );
    rDist = pComputer->ComputeDistanceFromFrontLine( pPion->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition() );

    return( MIL_Tools::ConvertSimToMeter( rDist ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDistanceAutomatFromFrontLine
// Created: LDC 2009-07-06
// -----------------------------------------------------------------------------
float DEC_GeometryFunctions::ComputeDistanceAutomatFromFrontLine( DEC_FrontAndBackLinesComputer* pComputer, DEC_Decision_ABC* pAutomate )
{
    assert( pComputer );

    MT_Float rDist = 0;
    assert( pAutomate );
    MT_Vector2D barycenter;
    if( pAutomate->GetAutomate().GetAlivePionsBarycenter( barycenter ) )
        rDist = pComputer->ComputeDistanceFromFrontLine( barycenter );

    return MIL_Tools::ConvertSimToMeter( rDist );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDistanceFromBackLine
// Created: NLD 2003-09-29
// -----------------------------------------------------------------------------
float DEC_GeometryFunctions::ComputeDistanceFromBackLine( DEC_FrontAndBackLinesComputer* pComputer, DEC_Decision_ABC* pPion )
{
    assert( pComputer );

    MT_Float rDist = 0;
    assert( pPion );
    rDist = pComputer->ComputeDistanceFromBackLine( pPion->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition() );

    return MIL_Tools::ConvertSimToMeter( rDist );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDistanceAutomatFromBackLine
// Created: LDC 2009-07-06
// Modified: RPD 2009-08-04
// -----------------------------------------------------------------------------
float DEC_GeometryFunctions::ComputeDistanceAutomatFromBackLine( const MIL_Automate& /*callerAutomate*/, DEC_FrontAndBackLinesComputer* pComputer, DEC_Decision_ABC* pAutomate )
{
    assert( pComputer );
    assert( pAutomate );
    MT_Float rDist = 0;
    MT_Vector2D barycenter;
    if( pAutomate->GetAutomate().GetAlivePionsBarycenter( barycenter ) )
        rDist = pComputer->ComputeDistanceFromBackLine( barycenter );

    return MIL_Tools::ConvertSimToMeter( rDist );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ClipLocalisationInFuseau
// Created: JVT 2004-11-03
// -----------------------------------------------------------------------------
bool DEC_GeometryFunctions::ClipLocalisationInFuseau( const TER_Localisation& localisation, const MIL_Fuseau& fuseau, TER_Localisation& clippedLocalisation )
{
    // Clippe le polygone dans le fuseau
    T_PointVector clippedPointVector;
    localisation.GetPointsClippedByPolygon( fuseau, clippedPointVector );

    if( clippedPointVector.empty() )
        return false;

    clippedLocalisation.Reset( clippedPointVector ); //$$$ NAZE
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::SplitLocalisation
// Created: JVT 2004-11-03
// -----------------------------------------------------------------------------
unsigned int DEC_GeometryFunctions::SplitLocalisation( const TER_Localisation& localisation, uint nNbrParts, std::vector< boost::shared_ptr< TER_Localisation > >& result )
{
    assert( nNbrParts > 0 );

    result.clear();
    localisation.Split( nNbrParts, result );

    if( result.size() != nNbrParts )
        return eWarning_DecoupageIncomplet;
    else
        return eNoError;
}


// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::SplitLocalisation
// Created: JVT 2004-11-04
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< TER_Localisation > > DEC_GeometryFunctions::SplitLocalisation( const TER_Localisation& localisation, MT_Vector2D vOrigin, const MT_Vector2D& vDirection, MT_Float rSectionLength )
{
    T_LocalisationPtrVector splitLocVector;

    const MT_Vector2D vTranslation   = rSectionLength * vDirection;
    const MT_Vector2D vLineDirection = vDirection.Rotated90();

    MT_Droite backBound  ( vOrigin, vOrigin + vLineDirection );
    vOrigin += vTranslation;
    MT_Droite frontBound ( vOrigin, vOrigin + vLineDirection );

    while( true )
    {
        T_PointVector points;
        localisation.GetPointsClippedBetweenTwoLines( backBound, frontBound, points );

        if( points.empty() )
            break;

        splitLocVector.push_back( boost::shared_ptr< TER_Localisation >( new TER_Localisation( TER_Localisation::ePolygon, points ) ) );

        vOrigin += vTranslation;
        backBound  = frontBound;
        frontBound.MT_Droite::MT_Droite( vOrigin, vOrigin + vLineDirection );
    };

    return splitLocVector;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeLocalisationBarycenter
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeLocalisationBarycenter( TER_Localisation* pLocalisation )
{
    boost::shared_ptr< MT_Vector2D > pBarycenter( new MT_Vector2D( MT_ComputeBarycenter( pLocalisation->GetPoints() ) ) );
    assert( TER_World::GetWorld().IsValidPosition( *pBarycenter ) );
    return pBarycenter;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeMeanDirection
// Created: JVT 2005-01-25
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeMeanDirection( const std::vector< MT_Vector2D* >& selection )
{
    boost::shared_ptr< MT_Vector2D > pResult( new MT_Vector2D( 0., 0. ) );
    
    if( selection.empty() )
    {
        assert( !"Should not be called when empty !" );
        pResult->rX_ = 0.;
        pResult->rY_ = 1.;
    }
    else
    {
        for( std::vector< MT_Vector2D* >::const_iterator it = selection.begin(); it != selection.end(); ++it )
            *pResult += *(*it);

        if( pResult->IsZero() )
            *pResult = *selection.front();
        else
            pResult->Normalize();
    }

    return pResult;
}


// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeKnowledgeAgentBarycenter
// Created: JVT 2005-01-17
// Modified: RPD 2009-08-05
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeKnowledgeAgentBarycenter( const MIL_AgentPion& /*caller*/, const std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > vKnowledges )
{
    boost::shared_ptr< MT_Vector2D > pResult( new MT_Vector2D( 0., 0. ) );

    uint nNbr = 0;
    for( std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >::const_iterator it = vKnowledges.begin(); it != vKnowledges.end(); ++it )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge = *it;
        if( pKnowledge && pKnowledge->IsValid() )
        {
            *pResult += pKnowledge->GetPosition();
            ++ nNbr;
        }
    }

    if( nNbr )
        *pResult /= (MT_Float)nNbr;

    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::GetFrontestPion
// Created: JVT 2005-02-11
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_GeometryFunctions::GetFrontestPion( const std::vector< DEC_Decision_ABC* >& pions, const MT_Vector2D* pDirection )
{
    const MT_Line     support( MT_Vector2D( 0., 0. ), *pDirection );
    DEC_Decision_ABC* pResult = 0;
    MT_Float          rSquareDistResult = -1.;
           
    for ( std::vector< DEC_Decision_ABC* >::const_iterator it = pions.begin(); it != pions.end(); ++it )
    {
        DEC_Decision_ABC* pKnow = *it;
        assert( pKnow );
        
        MT_Vector2D vProjectedPoint;
        MT_Float    rSquareDist;
        
        const bool bInDirection = support.ProjectPointOnLine( GetPosition( pKnow->GetPion() ), vProjectedPoint ) >= 0.;
        
        rSquareDist = vProjectedPoint.rX_ * vProjectedPoint.rX_ + vProjectedPoint.rY_ * vProjectedPoint.rY_;
        if ( !pResult || ( bInDirection &&  rSquareDist > rSquareDistResult ) || ( !bInDirection && rSquareDist < rSquareDistResult )  )
        {
            rSquareDistResult = rSquareDist;
            pResult           = pKnow;
        }        
    }
    
    return pResult;
}
// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeBackestAgent
// Created: JVT 2004-12-20
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_GeometryFunctions::ComputeBackestAgent( const std::vector< DEC_Decision_ABC* >& pions, const MT_Vector2D* pDirection )
{    
    MT_Vector2D vNewDirection = *pDirection;
    vNewDirection *= -1.;
    return GetFrontestPion( pions, &vNewDirection );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeCoverPosition
// Created: JVT 2005-02-11
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeCoverPosition( const std::vector< DEC_Decision_ABC* >& pions, MT_Vector2D* pDirection, float distance )
{
    boost::shared_ptr< MT_Vector2D > result;

    if( pions.empty() )
    {
        assert( !"La liste ne doit pas etre vide" );
        return result;
    }
    assert( pDirection );

    // calcul de la première ligne de support ( perpendiculaire à la direction passant par le pion le plus avancé )
    const DEC_Decision_ABC* pFrontestPion = GetFrontestPion( pions, pDirection );
    assert( pFrontestPion );
    const MT_Vector2D& vFrontestPionPosition = pFrontestPion->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition();
    const MT_Droite support1( vFrontestPionPosition, vFrontestPionPosition + pDirection->Rotated90() );

    // calcul de la seconde ligne de support ( dans la direction passant par le barycentre des pions )
    MT_Vector2D vOrigin;
    for( std::vector< DEC_Decision_ABC* >::const_iterator it = pions.begin(); it != pions.end(); ++it )
        vOrigin += ( *it )->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition();
    vOrigin /= (MT_Float)pions.size();
    const MT_Droite support2( vOrigin, vOrigin + *pDirection );

    // calcul du point de couverture
    result.reset( new MT_Vector2D() );
    support1.Intersect2D( support2, *result );

    *result += ( *pDirection * MIL_Tools::ConvertMeterToSim( distance ) );

    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeRandomPointOnCircle
// Created: JVT 2005-02-16
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeRandomPointOnCircle( MT_Vector2D* pCenter, float radius )
{
    static MT_Random rand;

    boost::shared_ptr< MT_Vector2D > pResult( new MT_Vector2D( 0., 1. ) );
    pResult->Rotate( rand.rand_io( 0., 2. * MT_PI ) );
    *pResult *= ( MIL_Tools::ConvertMeterToSim( radius ) );
    *pResult += *pCenter;
    TER_World::GetWorld().ClipPointInsideWorld( *pResult );
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeRandomPointInCircle
// Created: NLD 2004-04-28
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeRandomPointInCircle( MT_Vector2D* pCenter, float radius )
{
    static MT_Random randomGenerator;
    MT_Float     rRadius_ = MIL_Tools::ConvertMeterToSim( radius );
    assert( pCenter );

    // retrieve a random position in the circle (vCenter_,rRadius_)
    MT_Float rAlpha = randomGenerator.rand_ii( -MT_PI, MT_PI );
    MT_Float rMod   = randomGenerator.rand_oi();

    boost::shared_ptr< MT_Vector2D > pRandomPosition( new MT_Vector2D( *pCenter ) );
    (*pRandomPosition) += MT_Vector2D( rMod * rRadius_ * cos( rAlpha ), rMod * rRadius_ * sin( rAlpha ) );

    TER_World::GetWorld().ClipPointInsideWorld( *pRandomPosition );
    return pRandomPosition;
}

// =============================================================================
// Interception
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::GetInterceptionPoint
// Created: JVT 2005-02-17
// -----------------------------------------------------------------------------
bool DEC_GeometryFunctions::GetInterceptionPoint( const MT_Vector2D& vToInterceptPosition, const MT_Vector2D& vToInterceptSpeed, const MT_Vector2D& vInterceptingPosition, MT_Float rInterceptingSpeed, MT_Vector2D& result )
{
    // Soit les équations suivantes ( inconnues = vxnB, vynB (vercteur direction normé de B) et t (temps de l'interception) ) :
    // vToInterceptPosition.rX_ + vToInterceptSpeed.rX_ * t = vInterceptingPosition.rX_ + rInterceptingSpeed * vxnB * t
    // vToInterceptPosition.rY_ + vToInterceptSpeed.rY_ * t = vInterceptingPosition.rY_ + rInterceptingSpeed * vynB * t
    //                            vxnB * vxnB + vynB * vynB = 1
    //
    // Les solutions pour t ( seule variable qui nous intéresse ) sont les racines de rA * X^2  + rB * X + rC avec :
    const MT_Float rA = vToInterceptSpeed.rX_ * vToInterceptSpeed.rX_ + vToInterceptSpeed.rY_ * vToInterceptSpeed.rY_ - rInterceptingSpeed * rInterceptingSpeed;
    const MT_Float rB = 2. * ( vToInterceptPosition.rY_ * vToInterceptSpeed.rY_ + vToInterceptPosition.rX_ * vToInterceptSpeed.rX_ - vToInterceptSpeed.rY_ * vInterceptingPosition.rY_ - vToInterceptSpeed.rX_ * vInterceptingPosition.rX_ );
    const MT_Float rC = vToInterceptPosition.rY_ * vToInterceptPosition.rY_ + vInterceptingPosition.rY_ * vInterceptingPosition.rY_ - 2. * vToInterceptPosition.rY_ * vInterceptingPosition.rY_ + vInterceptingPosition.rX_ * vInterceptingPosition.rX_ - 2. * vToInterceptPosition.rX_ * vInterceptingPosition.rX_ + vToInterceptPosition.rX_ * vToInterceptPosition.rX_;

    const MT_Float delta = rB * rB - 4. * rA * rC;

    if( rA == 0. || delta < 0. )
        return false;

    MT_Float t = ( std::sqrt( delta ) - rB ) / ( 2. * rA );

    if( t < 0. && ( t = ( std::sqrt( delta ) + rB ) / ( -2. * rA ) ) < 0. )
        return false;

    result.rX_ = vToInterceptPosition.rX_ + vToInterceptSpeed.rX_ * t;
    result.rY_ = vToInterceptPosition.rY_ + vToInterceptSpeed.rY_ * t;

    return TER_World::GetWorld().IsValidPosition( result );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::GetInterceptionPosition
// Created: JVT 2005-02-17
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::GetInterceptionPosition( const MIL_AgentPion& /*caller*/, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, MT_Vector2D* pInterceptingPosition, MT_Float rSpeed )
{
    assert( pInterceptingPosition );
    boost::shared_ptr< MT_Vector2D > point;
    if( pKnowledge && pKnowledge->IsValid() )
    {
        const MT_Float rInterceptingSpeed = MIL_Tools::ConvertSpeedMosToSim( 3.6 /*m.s-1 => km.h-1*/ * rSpeed );
        MT_Vector2D vInterceptionPosition;
        if( GetInterceptionPoint( pKnowledge->GetPosition(), pKnowledge->GetDirection() * pKnowledge->GetSpeed(), *pInterceptingPosition, rInterceptingSpeed, vInterceptionPosition ) )
            point.reset( new MT_Vector2D( vInterceptionPosition ) );
    }
    return point;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CreateCircleLocalisation
// Created: NLD 2005-02-23
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::CreateCircleLocalisation( const MT_Vector2D* pCenter, const MT_Float rRadius )
{
    boost::shared_ptr< TER_Localisation > pResult( new TER_Localisation( *pCenter, rRadius ) );
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ListUncoordinatedPawns
// Created: NLD 2005-03-18
// -----------------------------------------------------------------------------
namespace
{
    bool IsPionCoordinated( const MT_Vector2D& vPosToTest, const std::vector< DEC_Decision_ABC* >& coordinatedPions, const MT_Float rMinDist )
    {
        for( std::vector< DEC_Decision_ABC* >::const_iterator itC = coordinatedPions.begin(); itC != coordinatedPions.end(); ++itC )
        {
            const MT_Vector2D& vPosTmp = (**itC).GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition();
            if( vPosToTest.Distance( vPosTmp ) <= rMinDist )
                return true;
        }
        return false;
    }
}

std::vector< DEC_Decision_ABC* > DEC_GeometryFunctions::ListUncoordinatedPawns( DEC_Decision_ABC* pion, const std::vector< DEC_Decision_ABC* >& unCoordinatedPions, float minDist )
{
    const MT_Float rMinDist = MIL_Tools::ConvertMeterToSim( minDist );
    std::vector< DEC_Decision_ABC* > notCoordinatedPions( unCoordinatedPions );
    std::vector< DEC_Decision_ABC* > coordinatedPions;
    coordinatedPions.push_back( pion );

    bool bDummy = true;
    while( bDummy )
    {
        bDummy = false;
        for( std::vector< DEC_Decision_ABC* >::iterator itNC = notCoordinatedPions.begin(); itNC != notCoordinatedPions.end(); )
        {
            const MT_Vector2D& vPosToTest = (**itNC).GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition();
            if( ::IsPionCoordinated( vPosToTest, coordinatedPions, rMinDist ) )
            {
                bDummy = true;
                coordinatedPions.push_back( *itNC );
                itNC = notCoordinatedPions.erase( itNC );
            }
            else
                ++itNC;
        }
    }
    return notCoordinatedPions;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::SplitListPoints
// Created: NLD 2005-04-04
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_GeometryFunctions::SplitListPoints( const std::vector< MT_Vector2D* >& listPoints, unsigned int nNbrParts )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > result;
    if( listPoints.size() >= 2 )
    {
        std::vector< MT_Vector2D > points;
        for( std::vector< MT_Vector2D* >::const_iterator it = listPoints.begin(); it != listPoints.end(); ++it )
            points.push_back( **it );
        MT_Polyline polyLine( points );

        if( nNbrParts <= 0 )
        {
            boost::shared_ptr< MT_Vector2D > point( new MT_Vector2D( polyLine.GetPointAt( polyLine.Magnitude() / 2. ) ) );
            result.push_back(  point );
        }
        else
        {
            result.reserve( nNbrParts + 1 );
            const MT_Float rPartSize = polyLine.Magnitude() / nNbrParts;
            MT_Float rDist = 0.;
            for( uint i = 0; i < nNbrParts + 1; ++i, rDist+= rPartSize )
            {
                boost::shared_ptr< MT_Vector2D > point( new MT_Vector2D( polyLine.GetPointAt( rDist ) ) );
                result.push_back( point );
            }
        }
    }
    return result;
}

namespace
{
    float ComputeClosedTerrainRatio( const TER_Localisation& location )
    {
        uint nForestSurface = 0, nEmptySurface  = 0, nUrbanSurface  = 0;
        MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetVisionObjectsInSurface( location, nEmptySurface, nForestSurface, nUrbanSurface );
        return float( nForestSurface + nUrbanSurface ) / float( nForestSurface + nUrbanSurface + nEmptySurface );
    }

    float ComputeOpenTerrainRatio( const TER_Localisation& location )
    {
        return 1.f - ComputeClosedTerrainRatio( location );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeClosedTerrainRatioInZone
// Created: NLD 2007-04-13
// -----------------------------------------------------------------------------
float DEC_GeometryFunctions::ComputeClosedTerrainRatioInZone( const TER_Localisation* location )
{
    return location ? ComputeClosedTerrainRatio( *location ) : 0.f;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeClosedTerrainRatioInFuseau
// Created: LDC 2009-07-06
// -----------------------------------------------------------------------------
float DEC_GeometryFunctions::ComputeClosedTerrainRatioInFuseau( const MIL_Fuseau* pFuseau )
{
    return pFuseau ? pFuseau->ComputeClosedTerrainRatio() : 0.f;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeOpenTerrainRatioInZone
// Created: SBO 2007-11-14
// -----------------------------------------------------------------------------
float DEC_GeometryFunctions::ComputeOpenTerrainRatioInZone( const TER_Localisation* location )
{
    return location ? ComputeOpenTerrainRatio( *location ) : 0.f;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeOpenTerrainRatioInFuseau
// Created: LDC 2009-07-06
// -----------------------------------------------------------------------------
float DEC_GeometryFunctions::ComputeOpenTerrainRatioInFuseau( const MIL_Fuseau* pFuseau )
{
    return pFuseau ? pFuseau->ComputeOpenTerrainRatio() : 0.f;
}

namespace {

    bool CompareTerrainOpening( TER_Localisation* location1, TER_Localisation* location2 )
    {
        return ComputeOpenTerrainRatio( *location1 ) < ComputeOpenTerrainRatio( *location2 );
    }

    bool CompareFuseauxOpening( MIL_Fuseau* pFuseau1, MIL_Fuseau* pFuseau2 )
    {
        return pFuseau1->ComputeOpenTerrainRatio() < pFuseau2->ComputeOpenTerrainRatio();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::SortZonesAccordingToTerrainOpening
// Created: NLD 2007-04-13
// -----------------------------------------------------------------------------
std::vector< TER_Localisation* > DEC_GeometryFunctions::SortZonesAccordingToTerrainOpening( const std::vector< TER_Localisation* >& list )
{
    std::vector< TER_Localisation* > result( list );
    std::sort( result.begin(), result.end(), CompareTerrainOpening );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::SortFuseauxAccordingToTerrainOpening
// Created: LDC 2009-07-06
// -----------------------------------------------------------------------------
std::vector< MIL_Fuseau* > DEC_GeometryFunctions::SortFuseauxAccordingToTerrainOpening( const std::vector< MIL_Fuseau* >& list )
{
    std::vector< MIL_Fuseau* > result = list;
    std::sort( result.begin(), result.end(), CompareFuseauxOpening );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ConvertFuseauToLocalisation
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::ConvertFuseauToLocalisation( const MIL_Fuseau* pFuseau )
{
    assert( pFuseau );

    boost::shared_ptr< TER_Localisation > pLocalisation( new TER_Localisation( *pFuseau ) );
    return pLocalisation;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::_ComputeAutomatesBarycenter
// Created: NLD 2007-04-29
// -----------------------------------------------------------------------------
MT_Vector2D DEC_GeometryFunctions::_ComputeAutomatesBarycenter( const std::vector< DEC_Decision_ABC* >& automates )
{
    MT_Vector2D barycenter;
    uint nNbrElt = 0;
    for( std::vector< DEC_Decision_ABC* >::const_iterator it = automates.begin(); it != automates.end(); ++it )
    {
        MT_Vector2D tmp;
        if( reinterpret_cast< DEC_Decision_ABC& >( **it ).GetAutomate().GetAlivePionsBarycenter( tmp ) )// $$$$ LDC: Remove DIA_TypedObjects
        {
            barycenter += tmp;
            ++ nNbrElt;
        }
    }
    if( nNbrElt > 0 )
        barycenter /= nNbrElt;
    return barycenter;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::GetNextObjectiveInFuseau
// Created: NLD 2007-05-16
// -----------------------------------------------------------------------------
DEC_Objective* DEC_GeometryFunctions::GetNextObjectiveInFuseau( const MIL_Fuseau* pFuseau, const MT_Vector2D* pRefPoint, const std::vector< DEC_Objective* >& objectives )
{
    DEC_Objective* result = 0;

    if( pFuseau )
    {
        MT_Float rDist = std::numeric_limits< MT_Float >::max();

        for( std::vector< DEC_Objective* >::const_iterator it = objectives.begin(); it != objectives.end(); ++it )
        {
            DEC_Objective* pObjective = *it;
            if( !pObjective->IsFlagged() && pFuseau->IsInside( pObjective->ComputerBarycenter() ) )
            {
                const MT_Float rTmp = pFuseau->ComputeAverageDistanceFromObjective( *pObjective, *pRefPoint );
                if( rTmp < rDist )
                {
                    result = pObjective;
                    rDist = rTmp;
                }
            }
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeAreaInZone
// Created: SBO 2007-12-07
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::ComputeAreaInZone( const MIL_Fuseau* zone, const MT_Vector2D* center )
{
    boost::shared_ptr< TER_Localisation > result;
    if( zone && center )
    {
        result.reset( new TER_Localisation( *center, zone->Distance( *center, true ) ) );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeAutomatDelayFromSchedule
// Created: NLD 2007-04-29
// -----------------------------------------------------------------------------
float DEC_GeometryFunctions::ComputeAutomatDelayFromSchedule( const MIL_Fuseau* pFuseau, const std::vector< DEC_Decision_ABC* >& automates, const MIL_LimaOrder* pLima )
{   
    // Calcul distance entre barycentre automates et element schedulé
    MT_Float rDistanceFromScheduled = std::numeric_limits< MT_Float >::max();
    uint     nSchedule              = 0;
    if( pLima ) 
    {
        rDistanceFromScheduled = pFuseau->ComputeAverageDistanceFromLima( *pLima, _ComputeAutomatesBarycenter( automates ) );
        nSchedule = pLima->GetSchedule();
    }

    return ComputeDelayFromSchedule( pFuseau, automates, ( float ) rDistanceFromScheduled, nSchedule );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDelayFromSchedule
// Created: LDC 2009-07-06
// -----------------------------------------------------------------------------
float DEC_GeometryFunctions::ComputeDelayFromSchedule( const MIL_Fuseau* pFuseau, const std::vector< DEC_Decision_ABC* >& automates, float rDistanceFromScheduled, int nSchedule )
{
    assert( pFuseau );

    // Calcul vitesse moyenne de l'automate
    MT_Float rSpeed = std::numeric_limits< MT_Float >::max();
    for( std::vector< DEC_Decision_ABC* >::const_iterator it = automates.begin(); it != automates.end(); ++it )
    {
        const MIL_Automate& automate = ( **it ).GetAutomate();
        rSpeed = std::min( rSpeed, automate.GetAlivePionsMaxSpeed() );
    }

    if( rDistanceFromScheduled == std::numeric_limits< MT_Float >::max() || rSpeed == 0. )
        return 0.f;
    else
    {
        const MT_Float rTimeToGoToElement = 1.439 * rDistanceFromScheduled / rSpeed; //$$$ Deplacer la formule magique (Cf. PHY_ComposantePion où elle existe aussi...)
        const MT_Float rTimeLeeway        = 1.439 * 2000. / rSpeed;

        // Valeur de retour : = 0 : en avance, ou à 2km de la lima
        //                    = 1 : en retard
        //              entre les 2 : marge de sécurité

        const MT_Float rDelay = nSchedule - ( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() + rTimeToGoToElement );
        if( rDelay < 0 )
            return 1.f;
        else
            return (float)( 1.f - std::min( 1., rDelay / rTimeLeeway ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::GetCrossroads
// Created: MGD 2009-08-19
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::GetCrossroads( directia::ScriptRef& knowledgeCreateFunction, float x, float y, float z, float radius, const directia::ScriptRef& table )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > points = TER_PathFindManager::GetPathFindManager().FindCrossroadsWithinCircle( MT_Vector2D( x, y ), radius );
    std::vector< std::vector< MT_Float > > positions;
    for( std::vector< boost::shared_ptr< MT_Vector2D > >::iterator it = points.begin(); it != points.end(); it++ )
    {
        positions.push_back( (*it)->ToStdVector() );
    }
    knowledgeCreateFunction( table, std::string( "net.masagroup.sword.military.world.Point" ), points, positions, true );//@TODO MGD replace "none" by another function
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeBarycenter
// Created: MGD 2009-10-30
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeBarycenter( const TER_Localisation* localisation )
{
    boost::shared_ptr< MT_Vector2D > result;
    result.reset( new MT_Vector2D( localisation->ComputeBarycenter() ) );
    return result;
}
