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
#include "DEC_FrontAndBackLinesComputer.h"
#include "MIL_AgentServer.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Roles/Terrain/PHY_RolePion_TerrainAnalysis.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "Entities/Orders/MIL_AutomateOrderManager.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "Entities/Orders/MIL_LimaFunction.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Tools/MIL_Tools.h"
#include "simulation_terrain/TER_Localisation.h"
#include "simulation_terrain/TER_ObjectManager.h"
#include "simulation_terrain/TER_World.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_Random.h"
#include <urban/GeometryAttribute.h>
#include <urban/Model.h>
#include <urban/TerrainObject_ABC.h>
#include <directia/brain/Brain.h>
#include <boost/foreach.hpp>

#define PRECISION 0.0000001

#define VECTOR_TO_POINT( point ) geometry::Point2f( static_cast< float >( ( point ).rX_ ), static_cast< float >( ( point ).rY_ ) )

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
    if( !selection.empty() )
        *pResult = (*pResult / static_cast< double >( selection.size() ) );
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDestPointForPion
// Created: JVT 2004-12-06
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeDestPointForPion( MIL_Automate& callerAutomate, DEC_Decision_ABC* pPion )
{
    if( !pPion )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    if( std::find( callerAutomate.GetPions().begin(), callerAutomate.GetPions().end(), &pPion->GetPion() ) == callerAutomate.GetPions().end() )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    return ComputeDestPoint( pPion->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDestPointForFuseau
// Created: LMT 2010-09-20
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeDestPointForFuseau( MIL_Fuseau& fuseau )
{
    boost::shared_ptr< MT_Vector2D > pResult( new MT_Vector2D() ); //$$$$ RAM
    fuseau.ComputeFurthestExtremityPoint( *pResult );
    return pResult;
}

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
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::CreatePoint()
{
    boost::shared_ptr< MT_Vector2D > pVect( new MT_Vector2D() );
    return pVect;
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CopyPoint
// Created: MGD 10-03-23
//-----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::CopyPoint( boost::shared_ptr< MT_Vector2D > point )
{
    if( !point )
        throw std::runtime_error( "Invalid position" );
    boost::shared_ptr< MT_Vector2D > pVect( new MT_Vector2D( *point ) );
    return pVect;
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CreateDirection
// Created: NLD 2003-05-13
//-----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::CreateDirection( MT_Vector2D* pPosSource, MT_Vector2D* pPosDest )
{
    if( !pPosSource || !pPosDest )
        throw std::runtime_error( "Invalid position" );

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
// Name: boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::CreateVector
// Created: LDC 2011-04-01
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::CreateVector( MT_Vector2D* pPosSource, MT_Vector2D* pPosDest )
{
    if( !pPosSource || !pPosDest )
        throw std::runtime_error( "Invalid position" );

    boost::shared_ptr< MT_Vector2D > pResult;
    MT_Vector2D* pVect = new MT_Vector2D( (*pPosDest) - (*pPosSource) );
    pResult.reset( pVect );
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CreateOrthoDirection
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::CreateOrthoDirection( MT_Vector2D* pDir, bool bCounterClockwise )
{
    if( !pDir || !( MT_IsZero( pDir->SquareMagnitude() - 1. ) ) )
        throw std::runtime_error( "Invalid direction" );

    boost::shared_ptr< MT_Vector2D > pResult( new MT_Vector2D( *pDir ) );

    if( bCounterClockwise )
        pResult->Rotate90();
    else
        pResult->Rotate90ClockWise();

    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDistance
// Created: MGD 2010-02-19
// -----------------------------------------------------------------------------
float DEC_GeometryFunctions::ComputeDistance( boost::shared_ptr< MT_Vector2D > pos1, boost::shared_ptr< MT_Vector2D > pos2 )
{
    if( !pos1 || !pos2 )
        throw std::runtime_error( "Null ptr in compute distance" );
    return (float)pos1->Distance( *pos2 );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ReverseDirection
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ReverseDirection( boost::shared_ptr< MT_Vector2D > pDir )
{
    if( !pDir )
        throw std::runtime_error( "Invalid direction" );
    *pDir *= -1.;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CopyAndReverseDirection
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::CopyAndReverseDirection( const MT_Vector2D* pDir )
{
    if( !pDir )
        throw std::runtime_error( "Invalid direction" );

    boost::shared_ptr< MT_Vector2D > pNewDir( new MT_Vector2D( *pDir ) );
    *pNewDir *= -1.;
    return pNewDir;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CopyAndRotateDirection
// Created: NLD 2005-03-18
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::CopyAndRotateDirection( const MT_Vector2D* pDir, const double angle )
{
    if( !pDir )
        throw std::runtime_error( "Invalid direction" );

    boost::shared_ptr< MT_Vector2D > pNewDir( new MT_Vector2D( *pDir ) );
    const double rAngle = - ( angle * MT_PI / 180. );
    pNewDir->Rotate( rAngle );
    return pNewDir;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CompareLocalisations
// Created: LDC 2011-08-08
// -----------------------------------------------------------------------------
bool DEC_GeometryFunctions::CompareLocalisations( TER_Localisation* pLocalisation1, TER_Localisation* pLocalisation2 )
{
    return *pLocalisation1 == *pLocalisation2;
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
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_GeometryFunctions::ComputeLocalisationPointsForPionsInFuseau( const std::vector< DEC_Decision_ABC* >& pions, TER_Localisation* pLocalisation, MT_Vector2D* pDirDanger, double rDistMaxBtwPoints )
{
    if( pions.empty() || !pLocalisation || !pDirDanger || !(*pions.begin()) )
        throw std::runtime_error( "Invalid arguments to ComputeLocalisationPointsForPionsInFuseau" );

    std::vector< boost::shared_ptr< MT_Vector2D > > result;

    // 1. Vérifie si les pions sont bien dans le même fuseau
    const MIL_Fuseau& fuseau = ( **pions.begin() ).GetPion().GetOrderManager().GetFuseau();
    std::vector< DEC_Decision_ABC* >::const_iterator itPion = pions.begin();
    for( ++itPion; itPion != pions.end(); ++itPion )
    {
        if( !(*itPion) )
            throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
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
    double rTmp = vDirTmp.rX_;
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

    std::size_t nNbrPointsPerSide = pions.size() / 2;

    // Left side
    MT_Vector2D vDir( vLeftPoint - vBarycenter );
    double rIncr = std::min( vDir.Magnitude() / ( nNbrPointsPerSide + 1 ), rDistMaxBtwPoints );
    vDir.Normalize();
    for( std::size_t i = 1; i <= nNbrPointsPerSide; ++i )
    {
        boost::shared_ptr< MT_Vector2D > point( new MT_Vector2D( vBarycenter + vDir * ( rIncr * i ) ) );
        result.push_back( point );
    }

    // Right side
    vDir = MT_Vector2D( vRightPoint - vBarycenter );
    rIncr = std::min( vDir.Magnitude() / ( nNbrPointsPerSide + 1 ), rDistMaxBtwPoints );
    vDir.Normalize();
    for( std::size_t j = 1; j <= nNbrPointsPerSide; ++j )
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
    return pLocalisation ? pLocalisation->IsInside( *pPoint ) : false;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ConvertPointToLocalisation
// Created: NLD 2003-08-19
// -----------------------------------------------------------------------------
// static
boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::ConvertPointToLocalisation( const MT_Vector2D* pPos )
{
    if( !pPos )
        throw std::runtime_error( "Invalid position" );
    boost::shared_ptr< TER_Localisation > pLoc( new TER_Localisation() );
    pLoc->Reset( *pPos );
    return pLoc;
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::Distance
// Created: NLD 2002-12-24
//-----------------------------------------------------------------------------
double DEC_GeometryFunctions::Distance( const MT_Vector2D* p1, const MT_Vector2D* p2 )
{
    if( !p1 || !p2 )
        throw std::runtime_error( "Computing distance with null point" );
    return MIL_Tools::ConvertSimToMeter( p1->Distance( *p2 ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::Distance3D
// Created: LDC 2011-08-05
// -----------------------------------------------------------------------------
double DEC_GeometryFunctions::Distance3D( const MT_Vector2D* p1, float altitude1, const MT_Vector2D* p2, float altitude2 )
{
    if( !p1 || !p2 )
        throw std::runtime_error( "Computing distance with null point" );

    MT_Vector3D p3d1( p1->rX_, p1->rY_, altitude1 );
    MT_Vector3D p3d2( p2->rX_, p2->rY_, altitude2 );
    return MIL_Tools::ConvertSimToMeter( p3d1.Distance( p3d2 ) );
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::TranslatePosition
// Created: JDY 03-01-23
//-----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::TranslatePosition( MT_Vector2D* p1, MT_Vector2D* p2, double d )
{
    if( !p1 || !p2 )
        throw std::runtime_error( "Invalid position" );

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
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::TranslatePositionInDirection( MT_Vector2D* p1, MT_Vector2D* p2, double d )
{
    if( !p1 || !p2 )
        throw std::runtime_error( "Invalid position" );
    if( !( MT_IsZero( p2->SquareMagnitude() - 1. ) ) )
        throw std::runtime_error( "Invalid magnitude" );

    boost::shared_ptr< MT_Vector2D > res( new MT_Vector2D() );
    *res = *p1 + d * (*p2);
    return res;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::TranslatePositionInVector
// Created: LDC 2011-04-01
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::TranslatePositionInVector( MT_Vector2D* position, MT_Vector2D* offset )
{
    if( !position || !offset )
        throw std::runtime_error( "Invalid position" );
    boost::shared_ptr< MT_Vector2D > res( new MT_Vector2D() );
    *res = *position + *offset;
    return res;
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComparePositions
// Created: AGN 03-01-02
//-----------------------------------------------------------------------------
bool DEC_GeometryFunctions::ComparePositions( MT_Vector2D* p1, MT_Vector2D* p2 )
{
    if( !p1 || !p2 )
        throw std::runtime_error( "Bad pointer given to DEC_Geometrie_PositionsEgales" );

    // $$$ JVT : Débile : si il y a besoin d'une "weldvalue" elle doit être prise en compte
    //           directement dans l'objet vecteur et dans ses operateurs ( en l'occurence == )
    static const double rWeldValue = TER_World::GetWorld().GetWeldValue();

    return( p1->Distance( *p2 ) <= rWeldValue );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeSupportPosition
// Created: NLD 2003-10-15
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeSupportPosition( const MIL_AgentPion& callerAgent, DEC_Decision_ABC* pAgentToSupport, double rDist )
{
    if( pAgentToSupport == 0 )
        throw std::runtime_error( "Invalid agent" );

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
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeAmbushPosition( const MIL_AgentPion& callerAgent, MT_Vector2D* pAmbushPosition, MT_Vector2D* pRetreatPosition, double rDist )
{
    if( !pAmbushPosition || !pRetreatPosition )
        throw std::runtime_error( "Invalid position" );;

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
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeSafetyPosition( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledgeEnemy, double rMinDistance )
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
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeSafetyPositionWithPopulation( const MIL_AgentPion& callerAgent, unsigned int nPopulationKnowledgeID, double rMinDistance )
{
    boost::shared_ptr< MT_Vector2D > pResult;
    DEC_Knowledge_Population* pKnowledge = callerAgent.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID( nPopulationKnowledgeID );
    if( pKnowledge )
    {
        pResult.reset( new MT_Vector2D( pKnowledge->GetSafetyPosition( callerAgent, rMinDistance ) ) );
        if( !callerAgent.GetRole< PHY_RolePion_TerrainAnalysis >().CanMoveOn( pResult ) )
            pResult.reset();
    }
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeSafetyPositionWithObjective
// Created: NLD 2004-04-28
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeSafetyPositionWithObjective( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledgeEnnemy, double rMinMeterDistance, MT_Vector2D* pObjective )
{
    boost::shared_ptr< MT_Vector2D > pResult;
    if( pKnowledgeEnnemy.get() && pKnowledgeEnnemy->IsValid() )
    {
        if( !pObjective )
            throw std::runtime_error( "Invalid position" );

        double     rMinDistance = MIL_Tools::ConvertMeterToSim( rMinMeterDistance );

        const MT_Vector2D& vEnnemiPos  = pKnowledgeEnnemy->GetPosition();

        MT_Vector2D vDirEniToAmi       = ( callerAgent.GetRole< PHY_RoleInterface_Location >().GetPosition() - vEnnemiPos).Normalize();
        MT_Vector2D vDirEniToObjective = ( *pObjective - vEnnemiPos ).Normalize();

        MT_Vector2D vSafetyPos;
        if( vDirEniToAmi.IsZero() )
            vSafetyPos = vEnnemiPos + vDirEniToObjective * rMinDistance;
        else
        {
            const double rAngle = Angle( vDirEniToAmi, vDirEniToObjective );
            vSafetyPos = vEnnemiPos + vDirEniToAmi.Rotate( rAngle  * -0.5 ) * rMinDistance;
        }

        TER_World::GetWorld().ClipPointInsideWorld( vSafetyPos );

        pResult.reset( new MT_Vector2D( vSafetyPos ) );
        if( !callerAgent.GetRole< PHY_RolePion_TerrainAnalysis >().CanMoveOn( pResult ) )
            pResult.reset();
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

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputePosDeploiementASAOmni
// Created: JVT 2005-02-15
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_GeometryFunctions::ComputePosDeploiementASAOmni( const MIL_Automate& automat, int positionCount, const MT_Vector2D* center, float radius )
{
    if( !center )
        throw std::runtime_error( "Invalid position" );
    std::vector< boost::shared_ptr< MT_Vector2D > > result;
    if( positionCount > 0 )
    {
        const double    rAngle = 2. * MT_PI / positionCount;
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
    if( !direction || !center || !( MT_IsZero( direction->SquareMagnitude() - 1. ) ) )
        throw std::runtime_error( "direction or center in ComputePosDeploiementASANasse" );

    std::vector< boost::shared_ptr< MT_Vector2D > > result;
    if( positionCount <= 0 )
        return result;
    const double semiAngle = angle * MT_PI / 360.; // ( / 360. = * 0.5 / 180., car demi-angle );
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
    if( !direction || !center || !( MT_IsZero( direction->SquareMagnitude() - 1. ) ) )
        throw std::runtime_error( "direction or center in ComputePosDeploiementASADoubleRideau" );

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
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_GeometryFunctions::ComputePointsBeforeLima( const MIL_Automate& callerAutomate, unsigned int nLimaID, double rDistBeforeLima, unsigned int nNbrPoints )
{
    MIL_LimaOrder* pLima = callerAutomate.GetOrderManager().FindLima( nLimaID );
    std::vector< boost::shared_ptr< MT_Vector2D > > result;
    if( pLima )
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

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDistanceFromMiddleLine
// Created: NLD 2003-10-01
// -----------------------------------------------------------------------------
float DEC_GeometryFunctions::ComputeDistanceFromMiddleLine( const std::vector< DEC_Decision_ABC*>& selPions, DEC_Decision_ABC* pReferencePion )
{
    if( selPions.empty() )
        return 0.f;
    if( !pReferencePion )
        throw std::runtime_error( "Invalid reference unit" );

    // Barycenter of the pions given
    MT_Vector2D vBarycenter;
    for( std::vector< DEC_Decision_ABC*>::const_iterator itPion = selPions.begin(); itPion != selPions.end(); ++itPion )
        vBarycenter += (*itPion)->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition();
    vBarycenter /= static_cast< double >( selPions.size() );

    const MT_Vector2D& vReferencePionPosition = pReferencePion->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition();

    //
    const MIL_Fuseau& fuseau = pReferencePion->GetPion().GetOrderManager().GetFuseau();
    double rDist;
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
    {
        if( !(*it) )
            throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
        pions.push_back( &static_cast< DEC_Decision_ABC& >( **it ).GetPion() );
    }
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
    {
        if( !(*it) )
            throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
        automats.push_back( &static_cast< DEC_Decision_ABC& >( **it ).GetAutomate() );
    }
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
    if( !pComputer || !pPion )
        throw std::runtime_error( "Invalid argument to ComputeDistanceFromFrontLine" );

    double rDist = 0;
    rDist = pComputer->ComputeDistanceFromFrontLine( pPion->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition() );

    return( MIL_Tools::ConvertSimToMeter( rDist ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDistanceAutomatFromFrontLine
// Created: LDC 2009-07-06
// -----------------------------------------------------------------------------
float DEC_GeometryFunctions::ComputeDistanceAutomatFromFrontLine( DEC_FrontAndBackLinesComputer* pComputer, DEC_Decision_ABC* pAutomate )
{
    if( !pComputer || !pAutomate )
        throw std::runtime_error( "Invalid argument to ComputeDistanceAutomatFromFrontLine" );

    double rDist = 0;
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
    if( !pComputer || !pPion )
        throw std::runtime_error( "Invalid argument to ComputeDistanceFromBackLine" );

    double rDist = 0;
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
    if( !pComputer || !pAutomate )
        throw std::runtime_error( "Invalid argument to ComputeDistanceAutomatFromBackLine" );
    double rDist = 0;
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
unsigned int DEC_GeometryFunctions::SplitLocalisation( const TER_Localisation& localisation, unsigned int nNbrParts, const MT_Vector2D* splitDirection, std::vector< boost::shared_ptr< TER_Localisation > >& result )
{
    result.clear();
    if( nNbrParts <= 0 )
        return eWarning_DecoupageIncomplet;

    localisation.Split( nNbrParts, result, splitDirection );

    if( result.size() != nNbrParts )
        return eWarning_DecoupageIncomplet;
    else
        return eNoError;
}


// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::SplitLocalisation
// Created: JVT 2004-11-04
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< TER_Localisation > > DEC_GeometryFunctions::SplitLocalisation( const TER_Localisation& localisation, MT_Vector2D vOrigin, const MT_Vector2D& vDirection, double rSectionLength )
{
    T_LocalisationPtrVector splitLocVector;

    const MT_Vector2D vTranslation   = rSectionLength * vDirection;
    const MT_Vector2D vLineDirection = vDirection.Rotated90();

    MT_Droite backBound  ( vOrigin, vOrigin + vLineDirection );
    vOrigin += vTranslation;
    MT_Droite frontBound ( vOrigin, vOrigin + vLineDirection );

    for( ;; )
    {
        T_PointVector points;
        localisation.GetPointsClippedBetweenTwoLines( backBound, frontBound, points );

        if( points.empty() )
            return splitLocVector;

        splitLocVector.push_back( boost::shared_ptr< TER_Localisation >( new TER_Localisation( TER_Localisation::ePolygon, points ) ) );

        vOrigin += vTranslation;
        backBound  = frontBound;
        frontBound.MT_Droite::MT_Droite( vOrigin, vOrigin + vLineDirection );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeLocalisationBarycenter
// Created: JVT 2005-01-17
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeLocalisationBarycenter( TER_Localisation* pLocalisation )
{
    if( !pLocalisation )
        throw std::runtime_error( "Invalid location" );
    boost::shared_ptr< MT_Vector2D > pBarycenter( new MT_Vector2D( MT_ComputeBarycenter( pLocalisation->GetPoints() ) ) );
    assert( TER_World::GetWorld().IsValidPosition( *pBarycenter ) );
    return pBarycenter;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeUrbanBlockLocalisations
// Created: LMT 2010-10-13
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_GeometryFunctions::ComputeUrbanBlockLocalisations( UrbanObjectWrapper* pUrbanObject )
{
	typedef std::vector< boost::shared_ptr< MT_Vector2D > > T_Vectors;
    T_Vectors result;
    if( pUrbanObject )
    {
        boost::shared_ptr< MT_Vector2D > position( new MT_Vector2D( pUrbanObject->GetLocalisation().ComputeBarycenter() ) );
        result.push_back( position );
        const T_Vectors& area = pUrbanObject->ComputeLocalisationsInsideBlock();
		for( T_Vectors::const_iterator it = area.begin(); it != area.end(); ++it )
			result.push_back( *it );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeTrafficableLocalisationBarycenter
// @return a position which the agent can reach inside an area, or the barycenter if no trafficable position can be found.
// Created: LMT 2010-10-13
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeTrafficableLocalisationBarycenter( MIL_AgentPion& pion, TER_Localisation* pLocalisation )
{
    if( !pLocalisation )
        throw std::runtime_error( "Compute barycenter of null position" );
    boost::shared_ptr< MT_Vector2D > pBarycenter( new MT_Vector2D( MT_ComputeBarycenter( pLocalisation->GetPoints() ) ) );
    if( pBarycenter.get() )
    {
        const urban::TerrainObject_ABC* object = MIL_AgentServer::GetWorkspace().GetUrbanModel().FindBlock( VECTOR_TO_POINT( *pBarycenter ) );
        if( object )
        {
            const UrbanObjectWrapper& terrainObject = MIL_AgentServer::GetWorkspace().GetEntityManager().GetUrbanObjectWrapper( *object );
            const double myWeight = pion.GetRole< PHY_RoleInterface_Composantes >().GetMajorComponentWeight();
            if( terrainObject.GetTrafficability() <= myWeight )
            {
                const float distance = 10.f; // $$$$ _RC_ LGY 2010-10-11: delta hardcoded
                const T_PointVector& points = terrainObject.GetLocalisation().GetPoints();
                const MT_Vector2D barycenter = terrainObject.GetLocalisation().ComputeBarycenter();
                for( CIT_PointVector it = points.begin(); it != points.end(); ++it )
                {
                    const MT_Vector2D point( *it + MT_Vector2D( *it - barycenter ).Normalize() * distance );
                    if( DEC_GeometryFunctions::IsUrbanBlockTrafficable( point, myWeight ) && pLocalisation->IsInside( point ) )
                    {
                        *pBarycenter = point;
                        return pBarycenter;
                    }
                }
            }
        }
    }
    return pBarycenter;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeTrafficableLocalisation
// @return positions which the agent can reach when the point is in an urban block
// Created: LMT 2010-11-03
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_GeometryFunctions::ComputeTrafficableLocalisation( const MT_Vector2D& point )
{
    if( const urban::TerrainObject_ABC* terrainObject = MIL_AgentServer::GetWorkspace().GetUrbanModel().FindBlock( VECTOR_TO_POINT( point ) ) )
    {
        UrbanObjectWrapper& wrapper = MIL_AgentServer::GetWorkspace().GetEntityManager().GetUrbanObjectWrapper( *terrainObject );
		std::vector< boost::shared_ptr< MT_Vector2D > > points = wrapper.ComputeLocalisationsInsideBlock();
		std::vector< boost::shared_ptr< MT_Vector2D > > trafficablePoints;
		for( std::vector< boost::shared_ptr< MT_Vector2D > >::const_iterator it = points.begin(); it != points.end(); ++it )
		{
            if( !(*it) )
                continue;
			if( ! DEC_GeometryFunctions::IsPointInUrbanBlock( **it, &wrapper ) )
				trafficablePoints.push_back( *it );
		}
        return trafficablePoints;
    }
    else
	{
		// $$$$ JSR 2011-06-13 optimiser pour ne pas passer un tableau d'un seul point
	    std::vector< boost::shared_ptr< MT_Vector2D > > result;
        result.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( point ) ));
	    return result;
	}
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::IsUrbanBlockTrafficable
// Created: LDC 2010-08-10
// -----------------------------------------------------------------------------
bool DEC_GeometryFunctions::IsUrbanBlockTrafficable( const MT_Vector2D& point, double weight )
{
    if( const urban::TerrainObject_ABC* object = MIL_AgentServer::GetWorkspace().GetUrbanModel().FindBlock( VECTOR_TO_POINT( point ) ) )
    {
        const UrbanObjectWrapper& terrainObject = MIL_AgentServer::GetWorkspace().GetEntityManager().GetUrbanObjectWrapper( *object );
        return terrainObject.GetTrafficability() > weight;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::IsPointInUrbanBlock
// Created: LMT 2011-03-09
// -----------------------------------------------------------------------------
bool DEC_GeometryFunctions::IsPointInUrbanBlock( const MT_Vector2D& point, const UrbanObjectWrapper* pUrbanBlock )
{
    return pUrbanBlock && pUrbanBlock->GetLocalisation().IsInside( point );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::IsPointInUrbanBlockTrafficable
// Created: LMT 2010-10-18
// -----------------------------------------------------------------------------
bool DEC_GeometryFunctions::IsPointInUrbanBlockTrafficable( MIL_AgentPion& pion, const MT_Vector2D& point )
{
    return DEC_GeometryFunctions::IsUrbanBlockTrafficable( point, pion.GetRole< PHY_RoleInterface_Composantes >().GetMajorComponentWeight( true ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::IsPointInCity
// Created: LDC 2010-11-18
// -----------------------------------------------------------------------------
bool DEC_GeometryFunctions::IsPointInCity( const MT_Vector2D& point )
{
    // $$$$ LDC RC: static because the cities can't be changed at run-time and there's no point recomputing the vector everytime.
    static std::vector< const urban::TerrainObject_ABC* > cities = MIL_AgentServer::GetWorkspace().GetUrbanModel().GetCities();
    geometry::Point2f geoPoint( static_cast< float >( point.rX_ ), static_cast< float >( point.rY_ ) );
    for( std::vector< const urban::TerrainObject_ABC* >::const_iterator it = cities.begin(); it != cities.end(); ++it )
    {
        if( !(*it) )
        {
            MT_LOG_ERROR_MSG( "Null pointer in urban block list in IsPointInCity" );
            continue;
        }
        const urban::GeometryAttribute* geom = ( *it )->Retrieve< urban::GeometryAttribute >();
        if( geom && geom->Geometry().IsInside( geoPoint ) )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeAreaSize
// Created: PSN 2010-06-24
// -----------------------------------------------------------------------------
double DEC_GeometryFunctions::ComputeAreaSize( TER_Localisation* pLocalisation )
{
    if( !pLocalisation )
        throw std::runtime_error( "Invalid location" );
    return MIL_Tools::ConvertSimToMeterSquare( pLocalisation->GetArea() );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeNearestBorder
// Created: LDC 2011-06-22
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeNearestBorder( const MT_Vector2D* position, TER_Localisation* pLocalisation )
{
    if( !pLocalisation )
        throw std::runtime_error( "Invalid location" );

    boost::shared_ptr< MT_Vector2D > pResult;
    MT_Vector2D vResult;
    if( pLocalisation->ComputeNearestOutsidePoint( *position, vResult ) )
        pResult.reset( new MT_Vector2D( vResult ) );

    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeAreaDiameter
// Created: LDC 2010-09-09
// -----------------------------------------------------------------------------
double DEC_GeometryFunctions::ComputeAreaDiameter( TER_Localisation* pLocalisation )
{
    if( !pLocalisation )
        throw std::runtime_error( "Invalid location" );
    return MIL_Tools::ConvertSimToMeterSquare( pLocalisation->GetLength() );
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
        {
            if( !(*it) )
                throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
            *pResult += *(*it);
        }

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

    unsigned int nNbr = 0;
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
        *pResult /= (double)nNbr;

    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::GetFrontestPion
// Created: JVT 2005-02-11
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_GeometryFunctions::GetFrontestPion( const std::vector< DEC_Decision_ABC* >& pions, const MT_Vector2D* pDirection )
{
    if( !pDirection )
        throw std::runtime_error( "Invalid direction" );
    const MT_Line     support( MT_Vector2D( 0., 0. ), *pDirection );
    DEC_Decision_ABC* pResult = 0;
    double          rSquareDistResult = -1.;

    for ( std::vector< DEC_Decision_ABC* >::const_iterator it = pions.begin(); it != pions.end(); ++it )
    {
        DEC_Decision_ABC* pKnow = *it;
        if( !pKnow )
            throw std::runtime_error( __FUNCTION__ ": invalid parameter." );

        MT_Vector2D vProjectedPoint;
        double    rSquareDist;

        const bool bInDirection = support.ProjectPointOnLine( GetPosition( pKnow->GetPion() ), vProjectedPoint ) >= 0.;

        rSquareDist = vProjectedPoint.rX_ * vProjectedPoint.rX_ + vProjectedPoint.rY_ * vProjectedPoint.rY_;
        if( !pResult || ( bInDirection &&  rSquareDist > rSquareDistResult ) || ( !bInDirection && rSquareDist < rSquareDistResult )  )
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
    if( !pDirection )
        throw std::runtime_error( "Invalid direction" );
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
    if( !pDirection )
        throw std::runtime_error( "Invalid direction" );

    // calcul de la première ligne de support ( perpendiculaire à la direction passant par le pion le plus avancé )
    const DEC_Decision_ABC* pFrontestPion = GetFrontestPion( pions, pDirection );
    if( !pFrontestPion )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    const MT_Vector2D& vFrontestPionPosition = pFrontestPion->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition();
    const MT_Droite support1( vFrontestPionPosition, vFrontestPionPosition + pDirection->Rotated90() );

    // calcul de la seconde ligne de support ( dans la direction passant par le barycentre des pions )
    MT_Vector2D vOrigin;
    for( std::vector< DEC_Decision_ABC* >::const_iterator it = pions.begin(); it != pions.end(); ++it )
    {
        if( !(*it) )
            throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
        vOrigin += ( *it )->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition();
    }
    vOrigin /= (double)pions.size();
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
    if( !pCenter )
        throw std::runtime_error( "Invalid center" );
    boost::shared_ptr< MT_Vector2D > pResult( new MT_Vector2D( 0., 1. ) );
    pResult->Rotate( MIL_Random::rand_io( 0., 2. * MT_PI ) );
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
    const double rRadius_ = MIL_Tools::ConvertMeterToSim( radius );
    if( !pCenter )
        throw std::runtime_error( "Invalid center" );

    // retrieve a random position in the circle (vCenter_,rRadius_)
    const double rAlpha = MIL_Random::rand_ii( -MT_PI, MT_PI );
    const double rMod   = MIL_Random::rand_oi();

    boost::shared_ptr< MT_Vector2D > pRandomPosition( new MT_Vector2D( *pCenter ) );
    (*pRandomPosition) += MT_Vector2D( rMod * rRadius_ * cos( rAlpha ), rMod * rRadius_ * sin( rAlpha ) );

    TER_World::GetWorld().ClipPointInsideWorld( *pRandomPosition );
    return pRandomPosition;
}


// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeRandomPointInZone
// Created: LDC 2011-06-21
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeRandomPointInZone( const TER_Localisation* location )
{
    if( !location )
        throw std::runtime_error( "Invalid location" );
    const MT_Rect& rect = location->GetBoundingBox();
    double xMin = rect.GetLeft();
    double xMax = rect.GetRight();
    double yMin = rect.GetBottom();
    double yMax = rect.GetTop();
    boost::shared_ptr< MT_Vector2D > result;
    result.reset( new MT_Vector2D( location->ComputeBarycenter() ) );
    unsigned int tries = 10;
    while ( --tries )
    {
        const double x = MIL_Random::rand_ii( xMin, xMax );
        const double y = MIL_Random::rand_ii( yMin, yMax );
        MT_Vector2D point( x, y );
        if( location->IsInside( point ) )
        {
            result.reset( new MT_Vector2D( point ) );
            break;
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::GetInterceptionPoint
// Created: JVT 2005-02-17
// -----------------------------------------------------------------------------
bool DEC_GeometryFunctions::GetInterceptionPoint( const MT_Vector2D& vToInterceptPosition, const MT_Vector2D& vToInterceptSpeed, const MT_Vector2D& vInterceptingPosition, double rInterceptingSpeed, MT_Vector2D& result )
{
    // Soit les équations suivantes ( inconnues = vxnB, vynB (vercteur direction normé de B) et t (temps de l'interception) ) :
    // vToInterceptPosition.rX_ + vToInterceptSpeed.rX_ * t = vInterceptingPosition.rX_ + rInterceptingSpeed * vxnB * t
    // vToInterceptPosition.rY_ + vToInterceptSpeed.rY_ * t = vInterceptingPosition.rY_ + rInterceptingSpeed * vynB * t
    //                            vxnB * vxnB + vynB * vynB = 1
    //
    // Les solutions pour t ( seule variable qui nous intéresse ) sont les racines de rA * X^2  + rB * X + rC avec :
    const double rA = vToInterceptSpeed.rX_ * vToInterceptSpeed.rX_ + vToInterceptSpeed.rY_ * vToInterceptSpeed.rY_ - rInterceptingSpeed * rInterceptingSpeed;
    const double rB = 2. * ( vToInterceptPosition.rY_ * vToInterceptSpeed.rY_ + vToInterceptPosition.rX_ * vToInterceptSpeed.rX_ - vToInterceptSpeed.rY_ * vInterceptingPosition.rY_ - vToInterceptSpeed.rX_ * vInterceptingPosition.rX_ );
    const double rC = vToInterceptPosition.rY_ * vToInterceptPosition.rY_ + vInterceptingPosition.rY_ * vInterceptingPosition.rY_ - 2. * vToInterceptPosition.rY_ * vInterceptingPosition.rY_ + vInterceptingPosition.rX_ * vInterceptingPosition.rX_ - 2. * vToInterceptPosition.rX_ * vInterceptingPosition.rX_ + vToInterceptPosition.rX_ * vToInterceptPosition.rX_;

    const double delta = rB * rB - 4. * rA * rC;

    if( rA == 0. || delta < 0. )
        return false;

    double t = ( std::sqrt( delta ) - rB ) / ( 2. * rA );

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
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::GetInterceptionPosition( const MIL_AgentPion& /*caller*/, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, MT_Vector2D* pInterceptingPosition, double rSpeed )
{
    if( !pInterceptingPosition )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    boost::shared_ptr< MT_Vector2D > point;
    if( pKnowledge && pKnowledge->IsValid() )
    {
        const double rInterceptingSpeed = MIL_Tools::ConvertSpeedMosToSim( 3.6 /*m.s-1 => km.h-1*/ * rSpeed );
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
boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::CreateCircleLocalisation( const MT_Vector2D* pCenter, const double rRadius )
{
    if( !pCenter )
        throw std::runtime_error( "Invalid center" );
    boost::shared_ptr< TER_Localisation > pResult( new TER_Localisation( *pCenter, rRadius ) );
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::CreateLineLocalisation
// Created: LDC 2011-03-29
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::CreateLineLocalisation( const MT_Vector2D* pPoint1, const MT_Vector2D* pPoint2 )
{
    if( !pPoint1 || !pPoint2 )
        throw std::runtime_error( "Invalid position" );
    T_PointVector pointVector;
    pointVector.push_back( *pPoint1 );
    pointVector.push_back( *pPoint2 );
    boost::shared_ptr< TER_Localisation > pResult( new TER_Localisation( TER_Localisation::eLine, pointVector ) );
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::CreatePolylineLocalisation
// Created: LDC 2011-03-29
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::CreatePolylineLocalisation( const std::vector< boost::shared_ptr< MT_Vector2D > >& points )
{
    std::vector< MT_Vector2D > pointsVector;
    for( std::vector< boost::shared_ptr< MT_Vector2D > >::const_iterator it = points.begin(); it != points.end(); ++it )
    {
        if( !(*it) )
            throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
        pointsVector.push_back( **it );
    }
    boost::shared_ptr< TER_Localisation > pResult( new TER_Localisation( TER_Localisation::eLine, pointsVector ) );
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::CreatePolygonLocalisation
// Created: LDC 2011-03-29
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::CreatePolygonLocalisation( const std::vector< boost::shared_ptr< MT_Vector2D > >& points )
{
    std::vector< MT_Vector2D > pointsVector;
    for( std::vector< boost::shared_ptr< MT_Vector2D > >::const_iterator it = points.begin(); it != points.end(); ++it )
    {
        if( !(*it) )
            throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
        pointsVector.push_back( **it );
    }
    boost::shared_ptr< TER_Localisation > pResult( new TER_Localisation( TER_Localisation::ePolygon, pointsVector ) );
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::CreateScaledLocalisation
// Created: LDC 2011-03-29
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::CreateScaledLocalisation( TER_Localisation* location, double length )
{    
    if( !location )
        throw std::runtime_error( "Invalid location" );
    boost::shared_ptr< TER_Localisation > pResult( new TER_Localisation( *location ) );
    pResult->Scale( length );
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< MT_Vector2D > > DEC_GeometryFunctions::ListLocalisationPoints
// Created: LDC 2011-03-29
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_GeometryFunctions::ListLocalisationPoints( TER_Localisation* pLocalisation )
{
    if( !pLocalisation )
        throw std::runtime_error( "Invalid location" );
    const std::vector< MT_Vector2D >& points = pLocalisation->GetPoints();
    std::vector< boost::shared_ptr< MT_Vector2D > > result;
    for( std::vector< MT_Vector2D >::const_iterator it = points.begin(); it != points.end(); ++it )
        result.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( *it ) ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ListUncoordinatedPawns
// Created: NLD 2005-03-18
// -----------------------------------------------------------------------------
namespace
{
    bool IsPionCoordinated( const MT_Vector2D& vPosToTest, const std::vector< DEC_Decision_ABC* >& coordinatedPions, const double rMinDist )
    {
        for( std::vector< DEC_Decision_ABC* >::const_iterator itC = coordinatedPions.begin(); itC != coordinatedPions.end(); ++itC )
        {
            if( !(*itC) )
                throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
            const MT_Vector2D& vPosTmp = (**itC).GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition();
            if( vPosToTest.Distance( vPosTmp ) <= rMinDist )
                return true;
        }
        return false;
    }
}

std::vector< DEC_Decision_ABC* > DEC_GeometryFunctions::ListUncoordinatedPawns( DEC_Decision_ABC* pion, const std::vector< DEC_Decision_ABC* >& unCoordinatedPions, float minDist )
{
    if( !pion )
        throw std::runtime_error( "Invalid unit" );
    const double rMinDist = MIL_Tools::ConvertMeterToSim( minDist );
    std::vector< DEC_Decision_ABC* > notCoordinatedPions( unCoordinatedPions );
    std::vector< DEC_Decision_ABC* > coordinatedPions;
    coordinatedPions.push_back( pion );

    bool bDummy = true;
    while( bDummy )
    {
        bDummy = false;
        for( std::vector< DEC_Decision_ABC* >::iterator itNC = notCoordinatedPions.begin(); itNC != notCoordinatedPions.end(); )
        {
            if( !(*itNC) )
                throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
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
// Name: DEC_GeometryFunctions::SplitLocation
// Created: NLD 2005-04-04
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_GeometryFunctions::SplitLocalisation( const TER_Localisation* location, unsigned int nbr )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > result;
    if( !location )
        return result;

    TER_Localisation::T_LocalisationPtrVector splittedLocations;
    location->Split( nbr, splittedLocations );
    BOOST_FOREACH( const boost::shared_ptr< TER_Localisation > splittedLocation, splittedLocations )
    {
        MT_Vector2D ptResult;
        splittedLocation->ComputeNearestPoint( splittedLocation->ComputeBarycenter(), ptResult );
        result.push_back( boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( ptResult ) ) );
    }

    if( result.size() < nbr )
    {
        MT_Vector2D ptResult;
        location->ComputeNearestPoint( location->ComputeBarycenter(), ptResult );
        result.insert( result.end(), nbr - result.size(), boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( ptResult ) ) );
    }
    return result;
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
        {
            if( !(*it) )
                throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
            points.push_back( **it );
        }
        MT_Polyline polyLine( points );

        if( nNbrParts <= 0 )
        {
            boost::shared_ptr< MT_Vector2D > point( new MT_Vector2D( polyLine.GetPointAt( polyLine.Magnitude() / 2. ) ) );
            result.push_back(  point );
        }
        else
        {
            result.reserve( nNbrParts + 1 );
            const double rPartSize = polyLine.Magnitude() / nNbrParts;
            double rDist = 0.;
            for( unsigned int i = 0; i < nNbrParts + 1; ++i, rDist+= rPartSize )
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
        unsigned int nForestSurface = 0, nEmptySurface  = 0, nUrbanSurface  = 0;
        MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetVisionObjectsInSurface( location, nEmptySurface, nForestSurface, nUrbanSurface );
        return static_cast< float >( nForestSurface + nUrbanSurface ) / static_cast< float >( nForestSurface + nUrbanSurface + nEmptySurface );
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
        return ( location1 ? ComputeOpenTerrainRatio( *location1 ) : 0. ) < ( location2 ? ComputeOpenTerrainRatio( *location2 ) : 0. );
    }

    bool CompareFuseauxOpening( MIL_Fuseau* pFuseau1, MIL_Fuseau* pFuseau2 )
    {
        return ( pFuseau1 ? pFuseau1->ComputeOpenTerrainRatio() : 0. ) < ( pFuseau2 ? pFuseau2->ComputeOpenTerrainRatio() : 0. );
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
    if( !pFuseau )
        throw std::runtime_error( "Invalid fuseau" );

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
    unsigned int nNbrElt = 0;
    for( std::vector< DEC_Decision_ABC* >::const_iterator it = automates.begin(); it != automates.end(); ++it )
    {
        MT_Vector2D tmp;
        if( !(*it) )
            throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
        if( ( **it ).GetAutomate().GetAlivePionsBarycenter( tmp ) )
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
        if( !pRefPoint )
            throw std::runtime_error( "Invalid position" );
        double rDist = std::numeric_limits< double >::max();

        for( std::vector< DEC_Objective* >::const_iterator it = objectives.begin(); it != objectives.end(); ++it )
        {
            DEC_Objective* pObjective = *it;
            if( !pObjective )
                throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
            if( !pObjective->IsFlagged() && pFuseau->IsInside( pObjective->ComputerBarycenter() ) )
            {
                const double rTmp = pFuseau->ComputeAverageDistanceFromObjective( *pObjective, *pRefPoint );
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
    double rDistanceFromScheduled = std::numeric_limits< double >::max();
    unsigned int     nSchedule              = 0;
    if( pLima )
    {
        rDistanceFromScheduled = pFuseau->ComputeAverageDistanceFromLima( *pLima, _ComputeAutomatesBarycenter( automates ) );
        nSchedule = pLima->GetSchedule();
    }

    return ComputeDelayFromSchedule( pFuseau, automates, ( float ) rDistanceFromScheduled, nSchedule );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeAdvanceAlongFuseau
// Created: LDC 2010-11-26
// -----------------------------------------------------------------------------
double DEC_GeometryFunctions::ComputeAdvanceAlongFuseau( MIL_Automate& callerAutomate, DEC_Decision_ABC* pion )
{
    if( !pion )
        throw std::runtime_error( "Invalid unit" );
    MT_Vector2D position = pion->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition();
    return callerAutomate.GetOrderManager().GetFuseau().ComputeAdvance( position );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputePositionAdvanceAlongFuseau
// Created: LDC 2010-12-08
// -----------------------------------------------------------------------------
double DEC_GeometryFunctions::ComputePositionAdvanceAlongFuseau( MIL_AgentPion& pion, MT_Vector2D* point )
{
    if( !point)
        return 0.;
    return pion.GetOrderManager().GetFuseau().ComputeAdvance( *point );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputePositionAdvanceAlongFuseauAutomat
// Created: DDA 2011-10-17
// -----------------------------------------------------------------------------
double DEC_GeometryFunctions::ComputePositionAdvanceAlongFuseauAutomat( MIL_Automate& callerAutomate, MT_Vector2D* point )
{
    if( !point)
        return 0.;
    return callerAutomate.GetOrderManager().GetFuseau().ComputeAdvance( *point );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::GetPointAlongFuseau
// Created: LDC 2010-10-18
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::GetPointAlongFuseau( const MIL_Fuseau* pFuseau, double advance )
{
    if( !pFuseau )
        throw std::runtime_error( "Invalid fuseau" );
    return boost::shared_ptr< MT_Vector2D >( new MT_Vector2D( pFuseau->GetPositionAtAdvance( advance ) ) );
}

namespace
{
    bool SortPredicate( const std::vector< boost::shared_ptr< MT_Vector2D > >& d1, const std::vector< boost::shared_ptr< MT_Vector2D > >& d2, const MT_Vector2D& direction )
    {
        return DotProduct( direction, *(d2[0]) - *(d1[0]) ) > 0.;
    }
}

// -----------------------------------------------------------------------------
// Name: std::vector< MT_Vector2D > > DEC_GeometryFunctions::GetPointsOnLimasInFuseau
// Created: LDC 2011-01-06
// -----------------------------------------------------------------------------
std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > > DEC_GeometryFunctions::GetPointsOnLimasInFuseau( MIL_Automate& callerAutomate, int limaType, int divider )
{
    std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > > result;
    if( divider > 0 )
    {
        const MIL_Fuseau& fuseau = callerAutomate.GetOrderManager().GetFuseau();
        const MT_Line line = fuseau.GetGlobalDirection();
        MT_Vector2D direction = line.GetPosEnd() - line.GetPosStart();
        const T_LimaVector& limas = callerAutomate.GetOrderManager().GetLimas();
        for( CIT_LimaVector it = limas.begin(); it != limas.end(); ++it )
        {
            const MIL_LimaOrder::T_LimaFunctions& functions = it->GetFunctions();
            for( MIL_LimaOrder::CIT_LimaFunctions fit = functions.begin(); fit != functions.end(); ++fit )
            {
                if( !(*fit) )
                    throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
                if( static_cast< int >( ( *fit )->GetID() ) == limaType )
                {
                    std::vector< MT_Vector2D > nextPoints;
                    // Divide lima inside fuseau in divider sections and pick one point per section.
                    fuseau.ComputePointsBeforeLima( *it, 0., divider, nextPoints );
                    std::vector< boost::shared_ptr< MT_Vector2D > > sharedVector;
                    for( std::vector< MT_Vector2D >::const_iterator vit = nextPoints.begin(); vit != nextPoints.end(); ++vit )
                    {
                        boost::shared_ptr< MT_Vector2D > point( new MT_Vector2D( *vit ) );
                        sharedVector.push_back( point );
                    }
                    result.push_back( sharedVector );
                    break;
                }
            }
        }
        std::sort( result.begin(), result.end(), boost::bind( &SortPredicate, _1, _2, direction ) );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDelayFromSchedule
// Created: LDC 2009-07-06
// -----------------------------------------------------------------------------
float DEC_GeometryFunctions::ComputeDelayFromSchedule( const MIL_Fuseau* pFuseau, const std::vector< DEC_Decision_ABC* >& automates, float rDistanceFromScheduled, int nSchedule )
{
    if( !pFuseau )
        throw std::runtime_error( "Invalid fuseau" );

    // Calcul vitesse moyenne de l'automate
    double rSpeed = std::numeric_limits< double >::max();
    for( std::vector< DEC_Decision_ABC* >::const_iterator it = automates.begin(); it != automates.end(); ++it )
    {
        if( !(*it) )
            throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
        const MIL_Automate& automate = ( **it ).GetAutomate();
        rSpeed = std::min( rSpeed, automate.GetAlivePionsMaxSpeed() );
    }

    if( rDistanceFromScheduled == std::numeric_limits< double >::max() || rSpeed == 0. )
        return 0.f;
    else
    {
        const double rTimeToGoToElement = 1.439 * rDistanceFromScheduled / rSpeed; //$$$ Deplacer la formule magique (Cf. PHY_ComposantePion où elle existe aussi...)
        const double rTimeLeeway        = 1.439 * 2000. / rSpeed;

        // Valeur de retour : = 0 : en avance, ou à 2km de la lima
        //                    = 1 : en retard
        //              entre les 2 : marge de sécurité

        const double rDelay = nSchedule - ( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() + rTimeToGoToElement );
        if( rDelay < 0 )
            return 1.f;
        return static_cast< float >( 1.f - std::min( 1., rDelay / rTimeLeeway ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeBarycenter
// Created: MGD 2009-10-30
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeBarycenter( const TER_Localisation* localisation )
{    
    if( !localisation )
        throw std::runtime_error( "Invalid location" );
    boost::shared_ptr< MT_Vector2D > result;
    result.reset( new MT_Vector2D( localisation->ComputeBarycenter() ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::GetWidth
// Created: DDA 2010-12-03
// -----------------------------------------------------------------------------

double DEC_GeometryFunctions::GetWidth( const MIL_Fuseau* pFuseau )
{
    if( !pFuseau )
        throw std::runtime_error( "Invalid fuseau" );
    return pFuseau->GetWidth();
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::IsPointInFuseau_ParamFuseau
// Created: EVH 2011-07-26
// -----------------------------------------------------------------------------
bool DEC_GeometryFunctions::IsPointInFuseau_ParamFuseau( const MIL_Fuseau* pFuseau, const MT_Vector2D* pPoint )
{
    if( !pFuseau )
        throw std::runtime_error( "Invalid fuseau" );
    return pFuseau->IsInside( *pPoint );
}
