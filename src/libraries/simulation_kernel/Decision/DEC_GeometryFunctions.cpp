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
#include "MIL_Random.h"
#include "DEC_Decision_ABC.h"
#include "DEC_Objective.h"
#include "Brain.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Roles/Terrain/PHY_RolePion_TerrainAnalysis.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Objects/MIL_ObjectFactory.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/TerrainHeuristicCapacity.h"
#include "Entities/Orders/MIL_AutomateOrderManager.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "Entities/Orders/MIL_LimaFunction.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "Tools/MIL_Tools.h"
#include "Urban/MIL_UrbanCache.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Urban/UrbanPhysicalCapacity.h"
#include "simulation_terrain/TER_Localisation.h"
#include "simulation_terrain/TER_ObjectManager.h"
#include "simulation_terrain/TER_Analyzer.h"
#include "simulation_terrain/TER_World.h"
#include "MT_Tools/MT_Logger.h"
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/foreach.hpp>

#define PRECISION 0.0000001

namespace
{

enum E_LocalisationFunctionsReturnCode
{
    eError_PionsPasDansMemeFuseau,
    eError_LocalisationPasDansFuseau,
    eWarning_DecoupageIncomplet,
    eNoError
};

// -----------------------------------------------------------------------------
// Name: ClipLocalisationInFuseau
// Created: JVT 2004-11-03
// -----------------------------------------------------------------------------
bool ClipLocalisationInFuseau( const TER_Localisation& localisation, const MIL_Fuseau& fuseau, TER_Localisation& clippedLocalisation )
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
// Name: SplitLocalisation
// Created: JVT 2004-11-03
// -----------------------------------------------------------------------------
unsigned int SplitLocalisation( const TER_Localisation& localisation, unsigned int nNbrParts, const MT_Vector2D* splitDirection, std::vector< boost::shared_ptr< TER_Localisation > >& result )
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
// Name: DEC_GeometryFunctions::ComputeDelayFromSchedule
// Created: LDC 2009-07-06
// -----------------------------------------------------------------------------
float ComputeDelayFromSchedule( const MIL_Fuseau* pFuseau, const std::vector< DEC_Decision_ABC* >& automates, float rDistanceFromScheduled, int nSchedule )
{
    if( !pFuseau )
        throw MASA_EXCEPTION( "Invalid fuseau" );

    // Calcul vitesse moyenne de l'automate
    double rSpeed = std::numeric_limits< double >::max();
    for( std::vector< DEC_Decision_ABC* >::const_iterator it = automates.begin(); it != automates.end(); ++it )
    {
        if( !(*it) )
            throw MASA_EXCEPTION( "invalid parameter." );
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

        const double rDelay = nSchedule - ( MIL_Time_ABC::GetTime().GetCurrentTimeStep() + rTimeToGoToElement );
        if( rDelay < 0 )
            return 1.f;
        return static_cast< float >( 1.f - std::min( 1., rDelay / rTimeLeeway ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ComputeAutomatesBarycenter
// Created: NLD 2007-04-29
// -----------------------------------------------------------------------------
MT_Vector2D ComputeAutomatesBarycenter( const std::vector< DEC_Decision_ABC* >& automates )
{
    MT_Vector2D barycenter;
    unsigned int nNbrElt = 0;
    for( std::vector< DEC_Decision_ABC* >::const_iterator it = automates.begin(); it != automates.end(); ++it )
    {
        MT_Vector2D tmp;
        if( !(*it) )
            throw MASA_EXCEPTION( "invalid parameter." );
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
// Name: GetPosition
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
const MT_Vector2D& GetPosition( const MIL_AgentPion& pion )
{
    return pion.GetRole< PHY_RoleInterface_Location >().GetPosition();
}

// -----------------------------------------------------------------------------
// Name: GetPosition
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
const MT_Vector2D& GetPosition( const MIL_Automate& automate )
{
    return automate.GetPosition();
}

}  // namespace

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeAgentsBarycenter
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeAgentsBarycenter( const std::vector< DEC_Decision_ABC* >& selection )
{
    boost::shared_ptr< MT_Vector2D > pResult = boost::make_shared< MT_Vector2D >();
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
        throw MASA_EXCEPTION( "invalid parameter." );
    if( std::find( callerAutomate.GetPions().begin(), callerAutomate.GetPions().end(), &pPion->GetPion() ) == callerAutomate.GetPions().end() )
        throw MASA_EXCEPTION( "invalid parameter." );
    return ComputeUnitDestPoint( pPion->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDestPointForFuseau
// Created: LMT 2010-09-20
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeDestPointForFuseau( MIL_Fuseau& fuseau )
{
    boost::shared_ptr< MT_Vector2D > pResult = boost::make_shared< MT_Vector2D >(); //$$$$ RAM
    fuseau.ComputeFurthestExtremityPoint( *pResult );
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CreateLocalisation
// Created: JVT 2005-05-11
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::CreateLocalisation()
{
    boost::shared_ptr< TER_Localisation > pLoc = boost::make_shared< TER_Localisation >();
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
    return DEC_GeometryFunctions::CreatePointFromXY(0, 0);
}

boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::CreatePointFromLatLong(
        double lat, double lng )
{
    auto p = boost::make_shared< MT_Vector2D >();
    TER_World::GetWorld().MosToSimMgrsCoord( lat, lng, *p );
    return p;
}

boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::CreatePointFromXY(
        double x, double y )
{
    return boost::make_shared< MT_Vector2D >(x, y);
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CopyPoint
// Created: MGD 10-03-23
//-----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::CopyPoint( boost::shared_ptr< MT_Vector2D > point )
{
    if( !point )
        throw MASA_EXCEPTION( "Invalid position" );
    boost::shared_ptr< MT_Vector2D > pVect = boost::make_shared< MT_Vector2D >( *point );
    return pVect;
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CreateDirection
// Created: NLD 2003-05-13
//-----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::CreateDirection( MT_Vector2D* pPosSource, MT_Vector2D* pPosDest )
{
    if( !pPosSource || !pPosDest )
        throw MASA_EXCEPTION( "Invalid position" );

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
        throw MASA_EXCEPTION( "Invalid position" );

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
        throw MASA_EXCEPTION( "Invalid direction" );

    boost::shared_ptr< MT_Vector2D > pResult = boost::make_shared< MT_Vector2D >( *pDir );

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
        throw MASA_EXCEPTION( "Null ptr in compute distance" );
    return (float)pos1->Distance( *pos2 );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ReverseDirection
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
void DEC_GeometryFunctions::ReverseDirection( boost::shared_ptr< MT_Vector2D > pDir )
{
    if( !pDir )
        throw MASA_EXCEPTION( "Invalid direction" );
    *pDir *= -1.;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::CopyAndReverseDirection
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::CopyAndReverseDirection( const MT_Vector2D* pDir )
{
    if( !pDir )
        throw MASA_EXCEPTION( "Invalid direction" );

    boost::shared_ptr< MT_Vector2D > pNewDir = boost::make_shared< MT_Vector2D >( *pDir );
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
        throw MASA_EXCEPTION( "Invalid direction" );

    boost::shared_ptr< MT_Vector2D > pNewDir = boost::make_shared< MT_Vector2D >( *pDir );
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
        throw MASA_EXCEPTION( "Invalid arguments to ComputeLocalisationPointsForPionsInFuseau" );

    std::vector< boost::shared_ptr< MT_Vector2D > > result;

    // 1. Vérifie si les pions sont bien dans le même fuseau
    const MIL_Fuseau& fuseau = ( **pions.begin() ).GetPion().GetOrderManager().GetFuseau();
    std::vector< DEC_Decision_ABC* >::const_iterator itPion = pions.begin();
    for( ++itPion; itPion != pions.end(); ++itPion )
    {
        if( !(*itPion) )
            throw MASA_EXCEPTION( "invalid parameter." );
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
        boost::shared_ptr< MT_Vector2D > point = boost::make_shared< MT_Vector2D >( vBarycenter );
        result.push_back( point );
    }

    std::size_t nNbrPointsPerSide = pions.size() / 2;

    // Left side
    MT_Vector2D vDir( vLeftPoint - vBarycenter );
    double rIncr = std::min( vDir.Magnitude() / ( nNbrPointsPerSide + 1 ), rDistMaxBtwPoints );
    vDir.Normalize();
    for( std::size_t i = 1; i <= nNbrPointsPerSide; ++i )
    {
        boost::shared_ptr< MT_Vector2D > point = boost::make_shared< MT_Vector2D >( vBarycenter + vDir * ( rIncr * i ) );
        result.push_back( point );
    }

    // Right side
    vDir = MT_Vector2D( vRightPoint - vBarycenter );
    rIncr = std::min( vDir.Magnitude() / ( nNbrPointsPerSide + 1 ), rDistMaxBtwPoints );
    vDir.Normalize();
    for( std::size_t j = 1; j <= nNbrPointsPerSide; ++j )
    {
        boost::shared_ptr< MT_Vector2D > point = boost::make_shared< MT_Vector2D >( vBarycenter + vDir * ( rIncr * j ) );
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
    if( pLocalisation && pPoint )
        return pLocalisation->IsInside( *pPoint );
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ConvertPointToLocalisation
// Created: NLD 2003-08-19
// -----------------------------------------------------------------------------
// static
boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::ConvertPointToLocalisation( const MT_Vector2D* pPos )
{
    if( !pPos )
        throw MASA_EXCEPTION( "Invalid position" );
    boost::shared_ptr< TER_Localisation > pLoc = boost::make_shared< TER_Localisation >();
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
        throw MASA_EXCEPTION( "Computing distance with null point" );
    return MIL_Tools::ConvertSimToMeter( p1->Distance( *p2 ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::Distance3D
// Created: LDC 2011-08-05
// -----------------------------------------------------------------------------
double DEC_GeometryFunctions::Distance3D( const MT_Vector2D* p1, float altitude1, const MT_Vector2D* p2, float altitude2 )
{
    if( !p1 || !p2 )
        throw MASA_EXCEPTION( "Computing distance with null point" );

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
        throw MASA_EXCEPTION( "Invalid position" );

    boost::shared_ptr< MT_Vector2D > res = boost::make_shared< MT_Vector2D >();
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
        throw MASA_EXCEPTION( "Invalid position" );
    if( !( MT_IsZero( p2->SquareMagnitude() - 1. ) ) )
        throw MASA_EXCEPTION( "Invalid magnitude" );

    boost::shared_ptr< MT_Vector2D > res = boost::make_shared< MT_Vector2D >();
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
        throw MASA_EXCEPTION( "Invalid position" );
    boost::shared_ptr< MT_Vector2D > res = boost::make_shared< MT_Vector2D >();
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
        throw MASA_EXCEPTION( "Bad pointer given to DEC_Geometrie_PositionsEgales" );

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
        throw MASA_EXCEPTION( "Invalid agent" );

    const MT_Vector2D& vUnitToSupportPos = pAgentToSupport->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition ();
    const MIL_Fuseau& fuseau             = callerAgent.GetOrderManager().GetFuseau();

    MT_Vector2D  vDirLooked;
    pAgentToSupport->GetPion().GetRole< PHY_RoleInterface_Perceiver >().GetMainPerceptionDirection( vDirLooked );

    vDirLooked.Rotate90ClockWise();

    boost::shared_ptr< MT_Vector2D > pResult = boost::make_shared< MT_Vector2D >();

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
        throw MASA_EXCEPTION( "Invalid position" );;

    boost::shared_ptr< MT_Vector2D > pResult = boost::make_shared< MT_Vector2D >();

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
    auto bbKg = callerAgent.GetKnowledgeGroup()->GetKnowledge();
    if( bbKg )
    {
        boost::shared_ptr< DEC_Knowledge_Population > pKnowledge = bbKg->GetKnowledgePopulationFromID( nPopulationKnowledgeID );
        if( pKnowledge )
            return pKnowledge->GetSafetyPosition( callerAgent, rMinDistance );
    }
    return boost::shared_ptr< MT_Vector2D >();
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
            throw MASA_EXCEPTION( "Invalid position" );

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
        if( !callerAgent.GetRole< PHY_RolePion_TerrainAnalysis >().CanMoveOn( *pResult ) )
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
    boost::shared_ptr< MT_Vector2D > pResult = boost::make_shared< MT_Vector2D >();
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
        throw MASA_EXCEPTION( "Invalid position" );
    std::vector< boost::shared_ptr< MT_Vector2D > > result;
    if( positionCount > 0 )
    {
        const double    rAngle = 2. * MT_PI / positionCount;
        MT_Vector2D vDir = automat.GetOrderManager().GetDirDanger() * MIL_Tools::ConvertMeterToSim( radius );
        result.reserve( positionCount );
        while( positionCount-- )
        {
            result.push_back( boost::make_shared< MT_Vector2D >( *center + vDir ) );
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
        throw MASA_EXCEPTION( "direction or center in ComputePosDeploiementASANasse" );

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
        result.push_back( boost::make_shared< MT_Vector2D >( vCenter ) );
        if( !--positionCount )
            return result;
    }

    result.push_back( boost::make_shared< MT_Vector2D >( vCenter + vSupport1 ) );
    result.push_back( boost::make_shared< MT_Vector2D >( vCenter + vSupport2 ) );
    for( positionCount -= 2; positionCount; positionCount -= 2 )
    {
        result.push_back( boost::make_shared< MT_Vector2D >( **( result.rbegin() + 1 ) + vSupport1 ) );
        result.push_back( boost::make_shared< MT_Vector2D >( **( result.rbegin() + 1 ) + vSupport2 ) );
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
        throw MASA_EXCEPTION( "direction or center in ComputePosDeploiementASADoubleRideau" );

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
    result.push_back( boost::make_shared< MT_Vector2D >( *center + initialDistance * vDirection + ( pointSpacing * .5 ) * vSupport1 ) );
    if( !--positionCount )
        return result;
    result.push_back( boost::make_shared< MT_Vector2D >( *center + initialDistance * vDirection + ( pointSpacing * .5 ) * vSupport2 ) );

    vDirection *= lineSpacing;
    vSupport1  *= pointSpacing;
    vSupport2  *= pointSpacing;

    while( --positionCount )
    {
        result.push_back( boost::make_shared< MT_Vector2D >( **( result.rbegin() + 1 ) + vDirection ) );
        if( !--positionCount ) break;
        // $$$$ _RC_ SBO 2009-07-31: Really? seems like the same point is pushed twice
        result.push_back( boost::make_shared< MT_Vector2D >( **( result.rbegin() + 1 ) + vDirection ) );
        if( !--positionCount ) break;
        result.push_back( boost::make_shared< MT_Vector2D >( **( result.rbegin() + 3 ) + vSupport1 ) );
        if( !--positionCount ) break;
        result.push_back( boost::make_shared< MT_Vector2D >( **( result.rbegin() + 3 ) + vSupport2 ) );
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
        for( auto it = tempVector.begin(); it != tempVector.end(); ++it )
        {
            boost::shared_ptr< MT_Vector2D > point = boost::make_shared< MT_Vector2D >( *it );
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
        throw MASA_EXCEPTION( "Invalid reference unit" );

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
            throw MASA_EXCEPTION( "invalid parameter." );
        pions.push_back( &static_cast< DEC_Decision_ABC& >( **it ).GetPion() );
    }
    DEC_FrontAndBackLinesComputer* pComputer = new DEC_FrontAndBackLinesComputer( callerAutomate, pions );
    return pComputer;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::StartComputingFrontAndBackLinesForPlatoon
// Created: LMT 2012-07-31
// -----------------------------------------------------------------------------
DEC_FrontAndBackLinesComputer* DEC_GeometryFunctions::StartComputingFrontAndBackLinesForPlatoon( const DEC_Decision_ABC* pAutomate, const std::vector< DEC_Decision_ABC* >& pionDecisionList )
{
    std::vector< MIL_AgentPion*> pions;
    pions.reserve( pionDecisionList.size() );
    for( std::vector< DEC_Decision_ABC* >::const_iterator it = pionDecisionList.begin(); it != pionDecisionList.end(); ++it )
    {
        if( !(*it) )
            throw MASA_EXCEPTION( "invalid parameter." );
        pions.push_back( &static_cast< DEC_Decision_ABC& >( **it ).GetPion() );
    }
    DEC_FrontAndBackLinesComputer* pComputer = new DEC_FrontAndBackLinesComputer( pAutomate->GetAutomate(), pions );
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
            throw MASA_EXCEPTION( "invalid parameter." );
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
        throw MASA_EXCEPTION( "Invalid argument to ComputeDistanceFromFrontLine" );

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
        throw MASA_EXCEPTION( "Invalid argument to ComputeDistanceAutomatFromFrontLine" );

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
        throw MASA_EXCEPTION( "Invalid argument to ComputeDistanceFromBackLine" );

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
        throw MASA_EXCEPTION( "Invalid argument to ComputeDistanceAutomatFromBackLine" );
    double rDist = 0;
    MT_Vector2D barycenter;
    if( pAutomate->GetAutomate().GetAlivePionsBarycenter( barycenter ) )
        rDist = pComputer->ComputeDistanceFromBackLine( barycenter );

    return MIL_Tools::ConvertSimToMeter( rDist );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::ClipLocalisation
// Created: LDC 2013-02-11
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::ClipLocalisation( const TER_Localisation& localisation, const MIL_Fuseau& fuseau )
{
    boost::shared_ptr< TER_Localisation > result;
    TER_Localisation clippedResult;
    if( ClipLocalisationInFuseau( localisation, fuseau, clippedResult ) )
        result.reset( new TER_Localisation( clippedResult ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::SplitLocalisation
// Created: JVT 2004-11-04
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< TER_Localisation > > DEC_GeometryFunctions::SplitLocalisation( const TER_Localisation& localisation, MT_Vector2D vOrigin, const MT_Vector2D& vDirection, double rSectionLength )
{
    if( localisation.GetType() != TER_Localisation::ePolygon )
        throw MASA_EXCEPTION( "SplitLocalisation expects an input polygon not a " + localisation.GetTypeString() );

    TER_Localisation::T_LocalisationPtrVector splitLocVector;
    if( rSectionLength == 0 || vDirection.SquareMagnitude() == 0. )
        return splitLocVector;

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

        splitLocVector.push_back( boost::make_shared< TER_Localisation >( TER_Localisation::ePolygon, boost::cref( points ) ) );

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
        throw MASA_EXCEPTION( "Invalid location" );
    boost::shared_ptr< MT_Vector2D > pBarycenter = boost::make_shared< MT_Vector2D >( MT_ComputeBarycenter( pLocalisation->GetPoints() ) );
    assert( TER_World::GetWorld().IsValidPosition( *pBarycenter ) );
    return pBarycenter;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeUrbanBlockLocalisations
// Created: LMT 2010-10-13
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_GeometryFunctions::ComputeUrbanBlockLocalisations( MIL_UrbanObject_ABC* pUrbanObject )
{
    typedef std::vector< boost::shared_ptr< MT_Vector2D > > T_Vectors;
    T_Vectors result;
    if( pUrbanObject )
    {
        boost::shared_ptr< MT_Vector2D > position = boost::make_shared< MT_Vector2D >( pUrbanObject->GetLocalisation().ComputeBarycenter() );
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
        throw MASA_EXCEPTION( "Compute barycenter of null position" );
    boost::shared_ptr< MT_Vector2D > pBarycenter = boost::make_shared< MT_Vector2D >( MT_ComputeBarycenter( pLocalisation->GetPoints() ) );
    if( pBarycenter.get() )
    {
        const MIL_UrbanObject_ABC* object = MIL_AgentServer::GetWorkspace().GetUrbanCache().FindBlock( *pBarycenter );
        if( object )
        {
            const double myWeight = pion.GetRole< PHY_RoleInterface_Composantes >().GetMaxWeight();
            if( const UrbanPhysicalCapacity* pPhysical = object->Retrieve< UrbanPhysicalCapacity >() )
            {
                if( pPhysical->GetTrafficability() <= myWeight )
                {
                    const float distance = 10.f; // $$$$ _RC_ LGY 2010-10-11: delta hardcoded
                    const T_PointVector& points = object->GetLocalisation().GetPoints();
                    const MT_Vector2D barycenter = object->GetLocalisation().ComputeBarycenter();
                    for( auto it = points.begin(); it != points.end(); ++it )
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
    if( const MIL_UrbanObject_ABC* wrapper = MIL_AgentServer::GetWorkspace().GetUrbanCache().FindBlock( point ) )
    {
        const std::vector< boost::shared_ptr< MT_Vector2D > > points = wrapper->ComputeLocalisationsInsideBlock();
        std::vector< boost::shared_ptr< MT_Vector2D > > trafficablePoints;
        for( std::vector< boost::shared_ptr< MT_Vector2D > >::const_iterator it = points.begin(); it != points.end(); ++it )
        {
            if( !(*it) )
                continue;
            if( ! DEC_GeometryFunctions::IsPointInUrbanBlock( **it, wrapper ) )
                trafficablePoints.push_back( *it );
        }
        return trafficablePoints;
    }
    else
    {
        // $$$$ JSR 2011-06-13 optimiser pour ne pas passer un tableau d'un seul point
        std::vector< boost::shared_ptr< MT_Vector2D > > result;
        result.push_back( boost::make_shared< MT_Vector2D >( point ));
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::IsUrbanBlockTrafficable
// Created: LDC 2010-08-10
// -----------------------------------------------------------------------------
bool DEC_GeometryFunctions::IsUrbanBlockTrafficable( const MT_Vector2D& point, double weight )
{
    if( const MIL_UrbanObject_ABC* object = MIL_AgentServer::GetWorkspace().GetUrbanCache().FindBlock( point ) )
        if( const UrbanPhysicalCapacity* pPhysical = object->Retrieve< UrbanPhysicalCapacity >() )
            return pPhysical->GetTrafficability() > weight;
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::IsPointInUrbanBlock
// Created: LMT 2011-03-09
// -----------------------------------------------------------------------------
bool DEC_GeometryFunctions::IsPointInUrbanBlock( const MT_Vector2D& point, const MIL_UrbanObject_ABC* pUrbanBlock )
{
    return pUrbanBlock && pUrbanBlock->GetLocalisation().IsInside( point );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::IsPointInUrbanBlockTrafficable
// Created: LMT 2010-10-18
// -----------------------------------------------------------------------------
bool DEC_GeometryFunctions::IsPointInUrbanBlockTrafficable( MIL_AgentPion& pion, const MT_Vector2D& point )
{
    return DEC_GeometryFunctions::IsUrbanBlockTrafficable( point, pion.GetRole< PHY_RoleInterface_Composantes >().GetMaxWeight( true ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::IsPointInUrbanBlockTrafficableForPlatoon
// Created: LMT 2012-03-15
// -----------------------------------------------------------------------------
bool DEC_GeometryFunctions::IsPointInUrbanBlockTrafficableForPlatoon( DEC_Decision_ABC* pion, const MT_Vector2D& point )
{
    return DEC_GeometryFunctions::IsPointInUrbanBlockTrafficable( pion->GetPion(), point );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::IsPointInCity
// Created: LDC 2010-11-18
// -----------------------------------------------------------------------------
bool DEC_GeometryFunctions::IsPointInCity( const MT_Vector2D& point )
{
    const std::vector< const MIL_UrbanObject_ABC* >& cities = MIL_AgentServer::GetWorkspace().GetUrbanCache().GetCities();
    for( std::vector< const MIL_UrbanObject_ABC* >::const_iterator it = cities.begin(); it != cities.end(); ++it )
    {
        if( !(*it) )
        {
            MT_LOG_ERROR_MSG( "Null pointer in urban block list in IsPointInCity" );
            continue;
        }
        if( ( *it )->GetLocalisation().IsInside( point ) )
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
        throw MASA_EXCEPTION( "Invalid location" );
    return MIL_Tools::ConvertSimToMeterSquare( pLocalisation->GetArea() );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeNearestBorder
// Created: LDC 2011-06-22
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeNearestBorder( const MT_Vector2D* position, TER_Localisation* pLocalisation )
{
    if( !pLocalisation )
        throw MASA_EXCEPTION( "Invalid location" );

    boost::shared_ptr< MT_Vector2D > pResult;
    MT_Vector2D vResult;
    pLocalisation->ComputeNearestOutsidePoint( *position, vResult );
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
        throw MASA_EXCEPTION( "Invalid location" );
    return MIL_Tools::ConvertSimToMeterSquare( pLocalisation->GetLength() );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeMeanDirection
// Created: JVT 2005-01-25
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeMeanDirection( const std::vector< MT_Vector2D* >& selection )
{
    boost::shared_ptr< MT_Vector2D > pResult = boost::make_shared< MT_Vector2D >( 0., 0. );

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
                throw MASA_EXCEPTION( "invalid parameter." );
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
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeKnowledgeAgentBarycenter( const MIL_AgentPion& /*caller*/, const std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& vKnowledges )
{
    boost::shared_ptr< MT_Vector2D > pResult = boost::make_shared< MT_Vector2D >( 0., 0. );

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
// Name: DEC_GeometryFunctions::ComputeListPointsBarycenter
// Created: GGE 2014-06-12
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeListPointsBarycenter( const std::vector< boost::shared_ptr< MT_Vector2D > >& points )
{
    boost::shared_ptr< MT_Vector2D > pResult = boost::make_shared< MT_Vector2D >( 0., 0. );

    unsigned int nNbr = 0;
    for( std::vector< boost::shared_ptr< MT_Vector2D > >::const_iterator it = points.begin(); it != points.end(); ++it )
    {
        boost::shared_ptr< MT_Vector2D > pPoint = *it;
        if( pPoint )
        {
            *pResult += *pPoint;
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
        throw MASA_EXCEPTION( "Invalid direction" );
    const MT_Line     support( MT_Vector2D( 0., 0. ), *pDirection );
    DEC_Decision_ABC* pResult = 0;
    double          rSquareDistResult = -1.;

    for ( std::vector< DEC_Decision_ABC* >::const_iterator it = pions.begin(); it != pions.end(); ++it )
    {
        DEC_Decision_ABC* pKnow = *it;
        if( !pKnow )
            throw MASA_EXCEPTION( "invalid parameter." );

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
        throw MASA_EXCEPTION( "Invalid direction" );
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
        throw MASA_EXCEPTION( "Invalid direction" );

    // calcul de la première ligne de support ( perpendiculaire à la direction passant par le pion le plus avancé )
    const DEC_Decision_ABC* pFrontestPion = GetFrontestPion( pions, pDirection );
    if( !pFrontestPion )
        throw MASA_EXCEPTION( "invalid parameter." );
    const MT_Vector2D& vFrontestPionPosition = pFrontestPion->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition();
    const MT_Droite support1( vFrontestPionPosition, vFrontestPionPosition + pDirection->Rotated90() );

    // calcul de la seconde ligne de support ( dans la direction passant par le barycentre des pions )
    MT_Vector2D vOrigin;
    for( std::vector< DEC_Decision_ABC* >::const_iterator it = pions.begin(); it != pions.end(); ++it )
    {
        if( !(*it) )
            throw MASA_EXCEPTION( "invalid parameter." );
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
        throw MASA_EXCEPTION( "Invalid center" );
    boost::shared_ptr< MT_Vector2D > pResult = boost::make_shared< MT_Vector2D >( 0., 1. );
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
        throw MASA_EXCEPTION( "Invalid center" );

    // retrieve a random position in the circle (vCenter_,rRadius_)
    const double rAlpha = MIL_Random::rand_ii( -MT_PI, MT_PI );
    const double rMod   = MIL_Random::rand_oi();

    boost::shared_ptr< MT_Vector2D > pRandomPosition = boost::make_shared< MT_Vector2D >( *pCenter );
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
        throw MASA_EXCEPTION( "Invalid location" );
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
        throw MASA_EXCEPTION( "invalid parameter." );
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
        throw MASA_EXCEPTION( "Invalid center" );
    boost::shared_ptr< TER_Localisation > pResult = boost::make_shared< TER_Localisation >( *pCenter, rRadius );
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::CreateLineLocalisation
// Created: LDC 2011-03-29
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::CreateLineLocalisation( const MT_Vector2D* pPoint1, const MT_Vector2D* pPoint2 )
{
    if( !pPoint1 || !pPoint2 )
        throw MASA_EXCEPTION( "Invalid position" );
    T_PointVector pointVector;
    pointVector.push_back( *pPoint1 );
    pointVector.push_back( *pPoint2 );
    boost::shared_ptr< TER_Localisation > pResult = boost::make_shared< TER_Localisation >( TER_Localisation::eLine, pointVector );
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
            throw MASA_EXCEPTION( "invalid parameter." );
        pointsVector.push_back( **it );
    }
    boost::shared_ptr< TER_Localisation > pResult = boost::make_shared< TER_Localisation >( TER_Localisation::eLine, pointsVector );
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
            throw MASA_EXCEPTION( "invalid parameter." );
        pointsVector.push_back( **it );
    }
    boost::shared_ptr< TER_Localisation > pResult = boost::make_shared< TER_Localisation >( TER_Localisation::ePolygon, pointsVector );
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::CreateScaledLocalisation
// Created: LDC 2011-03-29
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::CreateScaledLocalisation( TER_Localisation* location, double length )
{
    if( !location )
        throw MASA_EXCEPTION( "Invalid location" );
    boost::shared_ptr< TER_Localisation > pResult = boost::make_shared< TER_Localisation >( *location );
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
        throw MASA_EXCEPTION( "Invalid location" );
    const std::vector< MT_Vector2D >& points = pLocalisation->GetPoints();
    std::vector< boost::shared_ptr< MT_Vector2D > > result;
    for( std::vector< MT_Vector2D >::const_iterator it = points.begin(); it != points.end(); ++it )
        result.push_back( boost::make_shared< MT_Vector2D >( *it ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::ComputeConvexHull
// Created: LDC 2013-09-23
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_Localisation > DEC_GeometryFunctions::ComputeConvexHull( const std::vector< TER_Localisation* >& locations )
{
    std::vector< MT_Vector2D > points;
    for( auto it = locations.cbegin(); it != locations.cend(); ++it )
    {
        const auto& next = *it;
        if( !next )
            continue;
        TER_Localisation::E_LocationType type = next->GetType();
        if( type != TER_Localisation::ePolygon && type != TER_Localisation::eLine )
        {
            MT_Rect rect = next->GetBoundingBox();
            points.push_back( rect.GetPointUpLeft() );
            points.push_back( rect.GetPointUpRight() );
            points.push_back( rect.GetPointDownRight() );
            points.push_back( rect.GetPointDownLeft() );
        }
        else
        {
            const T_PointVector& nextPoints = next->GetPoints();
            points.insert( points.end(), nextPoints.begin(), nextPoints.end() );
        }
    }
    if( points.empty() )
        return boost::make_shared< TER_Localisation >();
    auto result = boost::make_shared< TER_Localisation >( TER_Localisation::ePolygon, points );
    result->Convexify();
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
                throw MASA_EXCEPTION( "invalid parameter." );
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
        throw MASA_EXCEPTION( "Invalid unit" );
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
                throw MASA_EXCEPTION( "invalid parameter." );
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
        result.push_back( boost::make_shared< MT_Vector2D >( ptResult ) );
    }

    if( result.size() < nbr )
    {
        MT_Vector2D ptResult;
        location->ComputeNearestPoint( location->ComputeBarycenter(), ptResult );
        result.insert( result.end(), nbr - result.size(), boost::make_shared< MT_Vector2D >( ptResult ) );
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
                throw MASA_EXCEPTION( "invalid parameter." );
            points.push_back( **it );
        }
        MT_Polyline polyLine( points );

        if( nNbrParts <= 0 )
        {
            boost::shared_ptr< MT_Vector2D > point = boost::make_shared< MT_Vector2D >( polyLine.GetPointAt( polyLine.Magnitude() / 2. ) );
            result.push_back(  point );
        }
        else
        {
            result.reserve( nNbrParts + 1 );
            const double rPartSize = polyLine.Magnitude() / nNbrParts;
            double rDist = 0.;
            for( unsigned int i = 0; i < nNbrParts + 1; ++i, rDist+= rPartSize )
            {
                boost::shared_ptr< MT_Vector2D > point = boost::make_shared< MT_Vector2D >( polyLine.GetPointAt( rDist ) );
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
        throw MASA_EXCEPTION( "Invalid fuseau" );

    boost::shared_ptr< TER_Localisation > pLocalisation = boost::make_shared< TER_Localisation >( *pFuseau );
    return pLocalisation;
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
            throw MASA_EXCEPTION( "Invalid position" );
        double rDist = std::numeric_limits< double >::max();

        for( std::vector< DEC_Objective* >::const_iterator it = objectives.begin(); it != objectives.end(); ++it )
        {
            DEC_Objective* pObjective = *it;
            if( !pObjective )
                throw MASA_EXCEPTION( "invalid parameter." );
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
        rDistanceFromScheduled = pFuseau->ComputeAverageDistanceFromLima( *pLima, ComputeAutomatesBarycenter( automates ) );
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
        throw MASA_EXCEPTION( "Invalid unit" );
    MT_Vector2D position = pion->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition();
    return callerAutomate.GetOrderManager().GetFuseau().ComputeAdvance( position );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputePositionAdvanceAlongDangerDirection
// Created: LMT 2013-08-29
// -----------------------------------------------------------------------------
double DEC_GeometryFunctions::ComputePositionAdvanceAlongDangerDirection( DEC_Decision_ABC* pPion, MT_Vector2D* position )
{
    if( !position)
        return 0.;
    MT_Vector2D dirDanger = pPion->GetPion().GetOrderManager().GetDirDanger();
    MT_Vector2D result;
    MT_Line line( MT_Vector2D( 0,0 ), dirDanger );
    return line.ProjectPointOnLine( *position, result );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputePositionAdvanceAlongFuseau
// Created: LDC 2010-12-08
// -----------------------------------------------------------------------------
double DEC_GeometryFunctions::ComputePositionAdvanceAlongFuseau( const DEC_Decision_ABC& caller, MT_Vector2D* point )
{
    if( !point )
        return 0.;
    return caller.GetOrderManager().GetFuseau().ComputeAdvance( *point );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::GetPointAlongFuseau
// Created: LDC 2010-10-18
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::GetPointAlongFuseau( const MIL_Fuseau* pFuseau, double advance )
{
    if( !pFuseau )
        throw MASA_EXCEPTION( "Invalid fuseau" );
    return boost::make_shared< MT_Vector2D >( pFuseau->GetPositionAtAdvance( advance ) );
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
        for( auto it = limas.begin(); it != limas.end(); ++it )
        {
            if( !(it->IsFlagged()) )
            {
                for( auto fit = it->GetFunctions().begin(); fit != it->GetFunctions().end(); ++fit )
                {
                    if( !(*fit) )
                        throw MASA_EXCEPTION( "invalid parameter." );
                    if( static_cast< int >( ( *fit )->GetID() ) == limaType )
                    {
                        std::vector< MT_Vector2D > nextPoints;
                        // Divide lima inside fuseau in divider sections and pick one point per section.
                        fuseau.ComputePointsBeforeLima( *it, 0., divider, nextPoints );
                        std::vector< boost::shared_ptr< MT_Vector2D > > sharedVector;
                        for( auto vit = nextPoints.begin(); vit != nextPoints.end(); ++vit )
                            sharedVector.push_back( boost::make_shared< MT_Vector2D >( *vit ) );
                        if( !sharedVector.empty() )
                            result.push_back( sharedVector );
                        break;
                    }
                }
            }
        }
        std::sort( result.begin(), result.end(), boost::bind( &SortPredicate, _1, _2, direction ) );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeEntryPointForFuseau
// Created: GGE 2014-06-04
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeEntryPointForFuseau( DEC_Decision_ABC* pPion, MIL_Fuseau& fuseau )
{
    if( !pPion )
        throw MASA_EXCEPTION( "Invalid unit" );
    boost::shared_ptr< MT_Vector2D > pResult( new MT_Vector2D() );
    fuseau.ComputeEntryPoint( pPion->GetPion().GetRole< PHY_RoleInterface_Location >().GetPosition(), *pResult );
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeBarycenter
// Created: MGD 2009-10-30
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeBarycenter( const TER_Localisation* localisation )
{
    if( !localisation )
        throw MASA_EXCEPTION( "Invalid location" );
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
        throw MASA_EXCEPTION( "Invalid fuseau" );
    return pFuseau->GetWidth();
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::IsNull
// Created: NMI 2013-04-24
// -----------------------------------------------------------------------------

bool DEC_GeometryFunctions::IsNull( const MIL_Fuseau* pFuseau )
{
	if( !pFuseau )
        throw MASA_EXCEPTION( "Invalid fuseau" );
	return pFuseau->IsNull();
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeMiddlePointsInAOR
// Created: NMI 2013-11-14
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_GeometryFunctions::ComputeMiddlePointsInAOR( const MIL_Fuseau* pFuseau )
{
    if( !pFuseau )
        throw MASA_EXCEPTION( "Invalid fuseau" );

    T_PointVector middlePoints = pFuseau->GetMiddleLimit();
    std::vector< boost::shared_ptr< MT_Vector2D > > result;

    for( auto it = middlePoints.begin(); it != middlePoints.end(); ++it )
    {
        boost::shared_ptr< MT_Vector2D > point = boost::make_shared< MT_Vector2D >( *it );
        result.push_back( point );
    }

    return result;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::IsPointInFuseau_ParamFuseau
// Created: EVH 2011-07-26
// -----------------------------------------------------------------------------
bool DEC_GeometryFunctions::IsPointInFuseau_ParamFuseau( const MIL_Fuseau* pFuseau, const MT_Vector2D* pPoint )
{
    if( !pFuseau )
        throw MASA_EXCEPTION( "Invalid fuseau" );
    return pFuseau->IsInside( *pPoint );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeObstaclePosition
// Created: NLD 2003-09-18
// Modified RPD 2009-08-06
// -----------------------------------------------------------------------------
namespace
{
    boost::shared_ptr< MT_Vector2D > ComputeObstaclePosition( const MIL_Fuseau& fuseau,
            MT_Vector2D* pCenter, const std::string& type, double rRadius )
    {
        if( !pCenter )
            throw MASA_EXCEPTION( "Compute obstacle position with null center" );
        boost::shared_ptr< MT_Vector2D > pResultPos( new MT_Vector2D( *pCenter ) );
        const MIL_ObjectType_ABC& object = MIL_AgentServer::GetWorkspace().GetObjectFactory().FindType( type );
        const TerrainHeuristicCapacity* pCapacity = object.GetCapacity< TerrainHeuristicCapacity >();
        if ( !pCapacity )
            return pResultPos;
        const double rSquareRadius_ = rRadius * rRadius;
        int score = std::numeric_limits< int >::min();
        TER_World::GetWorld().GetAnalyzer().ApplyOnNodesWithinCircle( *pCenter, rRadius,
            [&]( const MT_Vector2D& pos, const TerrainData& nPassability )
            {
                const double rTestNodeSquareDistance = pCenter->SquareDistance( pos );
                if( rTestNodeSquareDistance > rSquareRadius_ || !fuseau.IsInside( pos ) )
                    return;
                const int nTestNodeScore = pCapacity->ComputePlacementScore( pos, nPassability );
                if( nTestNodeScore == -1 )
                    return;
                if( score == std::numeric_limits< int >::min()
                    || nTestNodeScore > score
                    || nTestNodeScore == score && rTestNodeSquareDistance < pCenter->SquareDistance( *pResultPos ) )
                {
                    score = nTestNodeScore;
                    *pResultPos = pos;
                }
            } );
        return pResultPos;
    }

}

boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeObstaclePositionForUnit(
    const MIL_AgentPion& pion, MT_Vector2D* pCenter, const std::string& type, double rRadius )
{
    return ::ComputeObstaclePosition( pion.GetOrderManager().GetFuseau(), pCenter,
            type, rRadius );
}

boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeObstaclePositionForAutomat(
    const MIL_Automate& automat, MT_Vector2D* pCenter, const std::string& type, double rRadius )
{
    return ::ComputeObstaclePosition( automat.GetOrderManager().GetFuseau(), pCenter,
            type, rRadius );
}

namespace
{

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::SplitLocalisation
// Created: NLD 2003-08-21
// Modified: JVT 2004-11-03
// -----------------------------------------------------------------------------
template< typename T >
std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int > SplitLocalisationInParts( const T& caller, TER_Localisation* pLocalisation, unsigned int nNbrParts, const MT_Vector2D* direction )
{
    if( !pLocalisation )
        throw MASA_EXCEPTION( "Null location when splitting location in parts" );

    std::vector< boost::shared_ptr< TER_Localisation > > result;

    TER_Localisation clippedLocalisation;
    unsigned int errCode = eError_LocalisationPasDansFuseau;
    if ( ClipLocalisationInFuseau( *pLocalisation, caller.GetOrderManager().GetFuseau(), clippedLocalisation ) )
        errCode = ::SplitLocalisation( clippedLocalisation, nNbrParts, direction, result );
    return std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int >( result, errCode );
}

} // namespace

std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int > DEC_GeometryFunctions::SplitUnitLocalisationInParts( const MIL_AgentPion& caller, TER_Localisation* pLocalisation, unsigned int nNbrParts, const MT_Vector2D* direction )
{
    return SplitLocalisationInParts< MIL_AgentPion >( caller, pLocalisation, nNbrParts, direction );
}

std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int > DEC_GeometryFunctions::SplitAutomatLocalisationInParts( const MIL_Automate& caller, TER_Localisation* pLocalisation, unsigned int nNbrParts, const MT_Vector2D* direction )
{
    return SplitLocalisationInParts< MIL_Automate >( caller, pLocalisation, nNbrParts, direction );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::SplitPionLocalisationInParts
// Created: LDC 2013-02-07
// -----------------------------------------------------------------------------
std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int > DEC_GeometryFunctions::SplitPionLocalisationInParts( DEC_Decision_ABC* caller, TER_Localisation* pLocalisation, unsigned int nNbrParts, const MT_Vector2D* direction )
{
    if( !caller )
         throw std::runtime_error( "Null unit when splitting location in parts" );
    return SplitLocalisationInParts< MIL_AgentPion >( caller->GetPion(), pLocalisation, nNbrParts, direction);
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::SplitUnitLocalisationInSurfaces
// Created: JVT 2004-11-03
// -----------------------------------------------------------------------------
std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int > DEC_GeometryFunctions::SplitUnitLocalisationInSurfaces( const MIL_AgentPion& caller, TER_Localisation* pLocalisation, const double rAverageArea, MT_Vector2D* direction )
{
    if( !pLocalisation )
        throw MASA_EXCEPTION( "invalid parameter." );

    std::vector< boost::shared_ptr< TER_Localisation > > result;

    TER_Localisation clippedLocalisation;
    unsigned int errCode = eError_LocalisationPasDansFuseau;
    if ( ClipLocalisationInFuseau( *pLocalisation, caller.GetOrderManager().GetFuseau(), clippedLocalisation ) )
    {
        const unsigned int nNbrParts = std::max( (unsigned int)1, (unsigned int)( clippedLocalisation.GetArea() / rAverageArea ) );
        errCode = ::SplitLocalisation( clippedLocalisation, nNbrParts, direction, result );
    }
    return std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int >( result, errCode );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::RecursiveSplitLocalisationInSurfaces
// Created: BCI 2011-01-31
// -----------------------------------------------------------------------------
std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int > DEC_GeometryFunctions::RecursiveSplitUnitLocalisationInSurfaces( const MIL_AgentPion& caller, TER_Localisation* pLocalisation, const double rAverageArea )
{
    if( !pLocalisation )
        throw MASA_EXCEPTION( "Null location when splitting location in surfaces" );

    typedef std::vector< boost::shared_ptr< TER_Localisation > > T_ResultVector;
    T_ResultVector result;

    TER_Localisation clippedLocalisation;
    unsigned int errCode = eError_LocalisationPasDansFuseau;
    if ( ClipLocalisationInFuseau( *pLocalisation, caller.GetOrderManager().GetFuseau(), clippedLocalisation ) )
    {
        const unsigned int nNbrParts = std::max( (unsigned int)1, (unsigned int)( clippedLocalisation.GetArea() / rAverageArea ) );
        if( nNbrParts < 4 )
            clippedLocalisation.Split( nNbrParts, result );
        else
        {
            auto loc = boost::make_shared< TER_Localisation >( clippedLocalisation );
            result.push_back( loc );
            for( unsigned int n = 1; n < nNbrParts; n *= 4 )
            {
                T_ResultVector splitted;
                MT_Vector2D* splitDirection = 0;
                for( T_ResultVector::const_iterator it = result.begin(); it != result.end(); ++it )
                {
                    if( !(*it) )
                        throw MASA_EXCEPTION( "invalid parameter." );
                    (*it)->Split( 4, splitted, splitDirection );
                }
                std::swap( result, splitted );
            }
        }
        if( result.size() != nNbrParts )
            errCode = eWarning_DecoupageIncomplet;
        else
            errCode = eNoError;
    }
    return std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int >( result, errCode );
}
// -----------------------------------------------------------------------------
// Name:DEC_GeometryFunctions::SplitLocalisationInSections
// Created: JVT 2004-11-04
// -----------------------------------------------------------------------------
namespace
{

template< typename T >
std::vector< boost::shared_ptr< TER_Localisation > > SplitLocalisationInSections( const T& caller, const double rSectionLength )
{
    const MT_Line& globalDirection = caller.GetOrderManager().GetFuseau().GetGlobalDirection();
    MT_Vector2D vDirection( globalDirection.GetPosEnd() - globalDirection.GetPosStart() );
    vDirection.Normalize();

    return DEC_GeometryFunctions::SplitLocalisation( TER_Localisation(
        TER_Localisation::ePolygon, caller.GetOrderManager().GetFuseau().GetBorderPoints() ),
        globalDirection.GetPosStart(), vDirection , rSectionLength );
}

} // namespace

std::vector< boost::shared_ptr< TER_Localisation > > DEC_GeometryFunctions::SplitUnitLocalisationInSections( const MIL_AgentPion& caller, const double rSectionLength )
{
    return SplitLocalisationInSections< MIL_AgentPion >( caller, rSectionLength );
}

std::vector< boost::shared_ptr< TER_Localisation > > DEC_GeometryFunctions::SplitAutomatLocalisationInSections( const MIL_Automate& caller, const double rSectionLength )
{
    return SplitLocalisationInSections< MIL_Automate >( caller, rSectionLength );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeLocalisationBarycenterInFuseau
// Created: NLD 2003-09-17
// -----------------------------------------------------------------------------
namespace
{

template< typename T >
boost::shared_ptr< MT_Vector2D > ComputeLocalisationBarycenterInFuseau( const T& caller, TER_Localisation* pLocalisation )
{
    if( !pLocalisation )
        throw MASA_EXCEPTION( "Null location when computing barycenter in fuseau" );
    // 1. Clippe le polygone dans le fuseau
    T_PointVector clippedPointVector;
    pLocalisation->GetPointsClippedByPolygon( caller.GetOrderManager().GetFuseau(), clippedPointVector );
    // 2. Barycentre polygone clippé
    MT_Vector2D vBarycenter = MT_ComputeBarycenter( clippedPointVector );

    boost::shared_ptr< MT_Vector2D > result;

    if( !clippedPointVector.empty() && caller.GetOrderManager().GetFuseau().IsInside( vBarycenter ) )
    {
        assert( TER_World::GetWorld().IsValidPosition( vBarycenter ) );
        // 3. Envoi du résulat à DIA
        result = boost::make_shared< MT_Vector2D >( vBarycenter );
    }
    return result;
}

}  // namespace

boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeUnitLocalisationBarycenterInFuseau( const MIL_AgentPion& caller, TER_Localisation* pLocalisation )
{
    return ComputeLocalisationBarycenterInFuseau< MIL_AgentPion >( caller, pLocalisation );
}

boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeAutomatLocalisationBarycenterInFuseau( const MIL_Automate& caller, TER_Localisation* pLocalisation )
{
    return ComputeLocalisationBarycenterInFuseau< MIL_Automate >( caller, pLocalisation );
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDestPoint
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeUnitDestPoint( const MIL_AgentPion& caller )
{
    auto pResult = boost::make_shared< MT_Vector2D >();
    caller.GetOrderManager().GetFuseau().ComputeFurthestExtremityPoint( *pResult );
    return pResult;
}

boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeAutomatDestPoint( const MIL_Automate& caller )
{
    auto pResult = boost::make_shared< MT_Vector2D >();
    caller.GetOrderManager().GetFuseau().ComputeFurthestExtremityPoint( *pResult );
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeStartPoint
// Created: NLD 2004-05-24
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeUnitStartPoint( const MIL_AgentPion& caller )
{
    auto pResult = boost::make_shared< MT_Vector2D >();
    caller.GetOrderManager().GetFuseau().ComputeClosestExtremityPoint( *pResult );
    return pResult;
}

boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeAutomatStartPoint( const MIL_Automate& caller )
{
    auto pResult = boost::make_shared< MT_Vector2D >();
    caller.GetOrderManager().GetFuseau().ComputeClosestExtremityPoint( *pResult );
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::GetLeavingAreaPosition
// Created: MGG 2011-01-20
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::GetLeavingAreaPosition( const MIL_AgentPion& caller, TER_Localisation* pLocalisation )
{
    if( !pLocalisation )
        throw MASA_EXCEPTION( "Invalid location" );

    TER_Localisation scale = *pLocalisation;
    scale.Scale( 500 );

    boost::shared_ptr< MT_Vector2D > pResult;
    MT_Vector2D vResult;
    scale.ComputeNearestOutsidePoint( GetPosition( caller ), vResult );
    pResult = boost::make_shared< MT_Vector2D >( vResult );

    return pResult;
}

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::IsPointInFuseau
// Created: AGN 03-03-11
//-----------------------------------------------------------------------------
bool DEC_GeometryFunctions::IsPointInUnitFuseau( const MIL_AgentPion& caller, MT_Vector2D* pVect )
{
    if( !pVect )
        throw MASA_EXCEPTION( "Invalid point" );
    return caller.GetOrderManager().GetFuseau().IsInside( *pVect );
}

bool DEC_GeometryFunctions::IsPointInAutomatFuseau( const MIL_Automate& caller, MT_Vector2D* pVect )
{
    if( !pVect )
        throw MASA_EXCEPTION( "Invalid point" );
    return caller.GetOrderManager().GetFuseau().IsInside( *pVect );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputePointBeforeLima
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
namespace
{

template< typename T >
boost::shared_ptr< MT_Vector2D > ComputePointBeforeLima( const T& caller, int phaseLine, float distanceBefore )
{
    boost::shared_ptr< MT_Vector2D > pResult;
    if( const MIL_LimaOrder* pLima = caller.GetOrderManager().FindLima( phaseLine ) )
    {
        MT_Vector2D vResult;
        if( caller.GetOrderManager().GetFuseau().ComputePointBeforeLima( *pLima, MIL_Tools::ConvertMeterToSim( distanceBefore ), vResult ) )
            pResult = boost::make_shared< MT_Vector2D >( vResult );
    }
    return pResult;
}

}  // namespace

boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputePointBeforeUnitLima( const MIL_AgentPion& caller, int phaseLine, float distanceBefore )
{
    return ComputePointBeforeLima< MIL_AgentPion >( caller, phaseLine, distanceBefore );
}

boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputePointBeforeAutomatLima( const MIL_Automate& caller, int phaseLine, float distanceBefore )
{
    return ComputePointBeforeLima< MIL_Automate >( caller, phaseLine, distanceBefore );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_GeometryFunctions::ComputePointBeforeLimaInFuseau
// Created: SBO 2008-01-11
// Modified RPD 2009-08-06
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputePointBeforeLimaInFuseau( const MIL_Automate& caller, unsigned int limaID, double rDistBeforeLima, const MIL_Fuseau* pFuseau )
{
    MIL_LimaOrder* pLima = caller.GetOrderManager().FindLima( limaID );

    boost::shared_ptr< MT_Vector2D > pResult;
    if(  pLima && pFuseau )
    {
        MT_Vector2D vResult;
        if ( pFuseau->ComputePointBeforeLima( *pLima, rDistBeforeLima, vResult ) )
            pResult = boost::make_shared< MT_Vector2D >( vResult );
    }
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeNearestLocalisationPointInFuseau
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
namespace
{

template< typename T >
boost::shared_ptr< MT_Vector2D > ComputeNearestLocalisationPointInFuseau( const T& caller, const TER_Localisation* pLocation )
{
    assert( pLocation );
    if( !pLocation )
        throw MASA_EXCEPTION( "Invalid location" );
    boost::shared_ptr< MT_Vector2D > pResult;
    TER_Localisation clipped;
    if( ClipLocalisationInFuseau( *pLocation, caller.GetOrderManager().GetFuseau(), clipped ) )
    {
        MT_Vector2D vResult;
        clipped.ComputeNearestPoint( GetPosition( caller ), vResult );
        pResult = boost::make_shared< MT_Vector2D >( vResult );
    }
    return pResult;
}

} // namespace

boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeNearestLocalisationPointInUnitFuseau( const MIL_AgentPion& caller, const TER_Localisation* pLocation )
{
    return ComputeNearestLocalisationPointInFuseau< MIL_AgentPion >( caller, pLocation );
}

boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeNearestLocalisationPointInAutomatFuseau( const MIL_Automate& caller, const TER_Localisation* pLocation )
{
    return ComputeNearestLocalisationPointInFuseau< MIL_Automate >( caller, pLocation );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeNearestUnclippedLocalisationPointInFuseau
// Created: GGR 2006-09-27
// Modified RPD 2009-08-06
// -----------------------------------------------------------------------------
namespace
{

template< typename T >
boost::shared_ptr< MT_Vector2D > ComputeNearestUnclippedLocalisationPointInFuseau( const T& caller, const TER_Localisation* pLocation )
{
    if( !pLocation )
        throw MASA_EXCEPTION( "Invalid location" );
    boost::shared_ptr< MT_Vector2D > pResult;

    TER_Localisation fuseauLocation = TER_Localisation( TER_Localisation::ePolygon, caller.GetOrderManager().GetFuseau().GetBorderPoints() );

    MT_Vector2D vResult;
    fuseauLocation.ComputeNearestPoint( pLocation->ComputeBarycenter(), vResult );
    pResult = boost::make_shared< MT_Vector2D >( vResult );
    return pResult;
}

}  // namespace

boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeNearestUnclippedLocalisationPointInUnitFuseau( const MIL_AgentPion& caller, const TER_Localisation* pLocation )
{
    return ComputeNearestUnclippedLocalisationPointInFuseau< MIL_AgentPion >( caller, pLocation );
}

boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeNearestUnclippedLocalisationPointInAutomatFuseau( const MIL_Automate& caller, const TER_Localisation* pLocation )
{
    return ComputeNearestUnclippedLocalisationPointInFuseau< MIL_Automate >( caller, pLocation );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_GeometryFunctions::ComputeDelayFromScheduleAndObjectives
// Created: LDC 2009-07-06
// -----------------------------------------------------------------------------
float DEC_GeometryFunctions::ComputeDelayFromScheduleAndObjectives( const MIL_Automate& caller, const MIL_Fuseau* pFuseau, const std::vector< DEC_Decision_ABC* >& automates, const std::vector< DEC_Objective* >& objectives )
{
    //
    typedef std::vector< DEC_Objective* >     T_ObjectiveVector;
    typedef T_ObjectiveVector::iterator       IT_ObjectiveVector;
    typedef T_ObjectiveVector::const_iterator CIT_ObjectiveVector;
    //
    if( !pFuseau )
        throw MASA_EXCEPTION( "invalid parameter." );

    const MIL_LimaOrder* pNextLima = caller.GetOrderManager().FindNextScheduledLima();

    const DEC_Objective* pNextObjective = 0;
    for( std::vector< DEC_Objective* >::const_iterator it = objectives.begin(); it != objectives.end(); ++it )
    {
        const DEC_Objective* pObjective = *it;
        if( !pObjective )
            throw MASA_EXCEPTION( "invalid parameter." );
        if( pObjective->GetSchedule() == 0 || pObjective->IsFlagged() )
            continue;

        if( ( !pNextObjective || pObjective->GetSchedule() < pNextObjective->GetSchedule() )
         && ( !pNextLima      || pObjective->GetSchedule() < pNextLima->GetSchedule() ) )
            pNextObjective = *it;
    }

    double rDistanceFromScheduled = std::numeric_limits< double >::max();
    unsigned int     nSchedule              = 0;
    if( pNextObjective )
    {
        rDistanceFromScheduled = pFuseau->ComputeAverageDistanceFromObjective( *pNextObjective, ComputeAutomatesBarycenter( automates ) );
        nSchedule = pNextObjective->GetSchedule();
    }
    else if( pNextLima )
    {
        rDistanceFromScheduled = pFuseau->ComputeAverageDistanceFromLima( *pNextLima, ComputeAutomatesBarycenter( automates ) );
        nSchedule = pNextLima->GetSchedule();
    }

    return ComputeDelayFromSchedule( pFuseau, automates, ( float ) rDistanceFromScheduled, nSchedule );
}

