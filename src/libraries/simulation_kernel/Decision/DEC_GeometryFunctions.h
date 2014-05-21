// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#ifndef __DEC_GeometryFunctions_h_
#define __DEC_GeometryFunctions_h_

#include "Decision/DEC_Objective.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MIL_ObjectFactory.h"
#include "Entities/Objects/TerrainHeuristicCapacity.h"
#include "Tools/MIL_Tools.h"
#include "simulation_terrain/TER_World.h"

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

class DEC_Decision_ABC;
class DEC_FrontAndBackLinesComputer;
class DEC_Knowledge_Agent;
class DEC_Objective;
class MIL_AgentPion;
class MIL_Automate;
class MIL_Fuseau;
class MIL_LimaOrder;
class MT_Vector2D;
class TER_Localisation;
class MIL_UrbanObject_ABC;

// =============================================================================
// Created: NLD 2003-08-20
// =============================================================================
class DEC_GeometryFunctions
{
public:
    //! @name Functions
    //@{
    // Creation / destruction
    static boost::shared_ptr< TER_Localisation > CreateLocalisation ();
    static std::vector< boost::shared_ptr< MT_Vector2D > > CreateListPoint();
    static boost::shared_ptr< MT_Vector2D > CreatePoint();
    static boost::shared_ptr< MT_Vector2D > CreatePointFromLatLong( double lat, double lng );
    static boost::shared_ptr< MT_Vector2D > CreatePointFromXY( double x, double y );
    static boost::shared_ptr< MT_Vector2D > CopyPoint( boost::shared_ptr< MT_Vector2D > point );
    static boost::shared_ptr< MT_Vector2D > CreateDirection( MT_Vector2D* pPosSource, MT_Vector2D* pPosDest );
    static boost::shared_ptr< MT_Vector2D > CreateVector( MT_Vector2D* pPosSource, MT_Vector2D* pPosDest );
    static boost::shared_ptr< MT_Vector2D > CreateOrthoDirection( MT_Vector2D* pDir, bool bCounterClockwise );

    static float ComputeDistance( boost::shared_ptr< MT_Vector2D > pos1, boost::shared_ptr< MT_Vector2D > pos2 );
    static void ReverseDirection( boost::shared_ptr< MT_Vector2D > pDir );
    static boost::shared_ptr< MT_Vector2D > CopyAndReverseDirection( const MT_Vector2D* );
    static boost::shared_ptr< MT_Vector2D > CopyAndRotateDirection ( const MT_Vector2D*, const double angle );
    static bool CompareLocalisations( TER_Localisation* pLocalisation1, TER_Localisation* pLocalisation2 );
    static bool IsPointInsideLocalisation( MT_Vector2D* pPoint, TER_Localisation* pLocalisation );
    static bool ComparePositions(  MT_Vector2D* p1, MT_Vector2D* p2 );
    static boost::shared_ptr< MT_Vector2D > TranslatePosition( MT_Vector2D* p1, MT_Vector2D* p2, double d );
    static boost::shared_ptr< MT_Vector2D > TranslatePositionInDirection( MT_Vector2D* p1, MT_Vector2D* p2, double d );
    static boost::shared_ptr< MT_Vector2D > TranslatePositionInVector( MT_Vector2D* position, MT_Vector2D* offset );
    static double Distance( const MT_Vector2D* p1, const MT_Vector2D* p2 );
    static double Distance3D( const MT_Vector2D* p1, float altitude1, const MT_Vector2D* p2, float altitude2 );
    static boost::shared_ptr< TER_Localisation > ConvertPointToLocalisation( const MT_Vector2D* pPos );
    static boost::shared_ptr< TER_Localisation > CreateCircleLocalisation( const MT_Vector2D* pCenter, const double );
    static boost::shared_ptr< TER_Localisation > CreateLineLocalisation( const MT_Vector2D* pPoint1, const MT_Vector2D* pPoint2 );
    static boost::shared_ptr< TER_Localisation > CreatePolylineLocalisation( const std::vector< boost::shared_ptr< MT_Vector2D > >& points );
    static boost::shared_ptr< TER_Localisation > CreatePolygonLocalisation( const std::vector< boost::shared_ptr< MT_Vector2D > >& points );
    static boost::shared_ptr< TER_Localisation > CreateScaledLocalisation( TER_Localisation* location, double length );
    static boost::shared_ptr< MT_Vector2D > ComputeMeanDirection( const std::vector< MT_Vector2D* >& selection );
    static boost::shared_ptr< MT_Vector2D > ComputeAgentsBarycenter( const std::vector< DEC_Decision_ABC* >& selection ); //$$$ Pourri
    static float ComputeDistanceFromMiddleLine( const std::vector< DEC_Decision_ABC*>& selPions, DEC_Decision_ABC* pReferencePion );
    static boost::shared_ptr< MT_Vector2D > ComputeLocalisationBarycenter( TER_Localisation* pLocalisation );
    static std::vector< boost::shared_ptr< MT_Vector2D > > ListLocalisationPoints( TER_Localisation* pLocalisation );
    // Returns the convex hull of a list of locations. Shapes other than lines and polygons are first converted into their bounding box.
    static boost::shared_ptr< TER_Localisation > ComputeConvexHull( const std::vector< TER_Localisation* >& locations );

    static std::vector< boost::shared_ptr< MT_Vector2D > > ComputeUrbanBlockLocalisations( MIL_UrbanObject_ABC* pUrbanObject );

    static boost::shared_ptr< MT_Vector2D > ComputeTrafficableLocalisationBarycenter( MIL_AgentPion& pion, TER_Localisation* pLocalisation );
    static std::vector< boost::shared_ptr< MT_Vector2D > > ComputeTrafficableLocalisation( const MT_Vector2D& point );

    static bool IsUrbanBlockTrafficable( const MT_Vector2D& point, double weight );
    static boost::shared_ptr< MT_Vector2D > ComputeCoverPosition( const std::vector< DEC_Decision_ABC* >& pions, MT_Vector2D* pDirection, float distance );
    static boost::shared_ptr< MT_Vector2D > ComputeRandomPointOnCircle( MT_Vector2D* pCenter, float radius );
    static boost::shared_ptr< MT_Vector2D > ComputeRandomPointInCircle( MT_Vector2D* pCenter, float radius );
    static boost::shared_ptr< MT_Vector2D > ComputeRandomPointInZone( const TER_Localisation* location );
    static std::vector< DEC_Decision_ABC* > ListUncoordinatedPawns( DEC_Decision_ABC* pion, const std::vector< DEC_Decision_ABC* >& unCoordinatedPions, float minDist );
    static std::vector< boost::shared_ptr< MT_Vector2D > >  SplitListPoints( const std::vector< MT_Vector2D* >& listPoints, unsigned int nNbrParts );
    static float ComputeClosedTerrainRatioInZone( const TER_Localisation* location );
    static float ComputeOpenTerrainRatioInZone( const TER_Localisation* location );
    static float ComputeClosedTerrainRatioInFuseau( const MIL_Fuseau* );
    static float ComputeOpenTerrainRatioInFuseau( const MIL_Fuseau* pFuseau);
    static std::vector< TER_Localisation* > SortZonesAccordingToTerrainOpening( const std::vector< TER_Localisation* >& list );
    static std::vector< MIL_Fuseau* > SortFuseauxAccordingToTerrainOpening( const std::vector< MIL_Fuseau* >& list );
    static boost::shared_ptr< TER_Localisation > ConvertFuseauToLocalisation( const MIL_Fuseau* pFuseau );
    static DEC_Objective* GetNextObjectiveInFuseau( const MIL_Fuseau* pFuseau, const MT_Vector2D* pRefPoint, const std::vector< DEC_Objective* >& objectives );
    static boost::shared_ptr< TER_Localisation > ComputeAreaInZone( const MIL_Fuseau* zone, const MT_Vector2D* center );
    static double ComputeAreaSize( TER_Localisation* pLocalisation );
    template< typename T > static boost::shared_ptr< MT_Vector2D > GetLeavingAreaPosition( const T& caller, TER_Localisation* pLocalisation );
    static boost::shared_ptr< MT_Vector2D > ComputeNearestBorder( const MT_Vector2D* position, TER_Localisation* pLocalisation );
    static double ComputeAreaDiameter( TER_Localisation* pLocalisation );
    static bool IsPointInUrbanBlock( const MT_Vector2D& point, const MIL_UrbanObject_ABC* pUrbanBlock );
    static bool IsPointInUrbanBlockTrafficable( MIL_AgentPion& pion, const MT_Vector2D& point );
    static bool IsPointInUrbanBlockTrafficableForPlatoon( DEC_Decision_ABC* pion, const MT_Vector2D& point );
    static bool IsPointInCity( const MT_Vector2D& point );
    static std::vector< boost::shared_ptr< MT_Vector2D > > SplitLocalisation( const TER_Localisation* location, unsigned int nbr );
    static std::vector< boost::shared_ptr< TER_Localisation > > SplitLocalisation( const TER_Localisation& localisation, MT_Vector2D vOrigin, const MT_Vector2D& vDirection, double rSectionLength );
    static boost::shared_ptr< TER_Localisation > ClipLocalisation( const TER_Localisation& localisation, const MIL_Fuseau& fuseau );

    static DEC_FrontAndBackLinesComputer* StartComputingFrontAndBackLines( const MIL_Automate& callerAutomate, const std::vector< DEC_Decision_ABC* >& pionDecisionList );
    static DEC_FrontAndBackLinesComputer* StartComputingFrontAndBackLinesForPlatoon( const DEC_Decision_ABC* pAutomate, const std::vector< DEC_Decision_ABC* >& pionDecisionList );
    static DEC_FrontAndBackLinesComputer* StartComputingAutomatFrontAndBackLines( const MIL_Automate& callerAutomate, const std::vector< DEC_Decision_ABC* >& automatDecisionList );
    static void StopComputingFrontAndBackLines( DEC_FrontAndBackLinesComputer* pComputer );
    static float ComputeDistanceFromFrontLine( DEC_FrontAndBackLinesComputer* pComputer, DEC_Decision_ABC* pPion );
    static float ComputeDistanceAutomatFromFrontLine( DEC_FrontAndBackLinesComputer* pComputer, DEC_Decision_ABC* pAutomate );
    static float ComputeDistanceFromBackLine( DEC_FrontAndBackLinesComputer* pComputer, DEC_Decision_ABC* pPion );
    static float ComputeDistanceAutomatFromBackLine( const MIL_Automate& callerAutomate, DEC_FrontAndBackLinesComputer* pComputer, DEC_Decision_ABC* pAutomate );
    static boost::shared_ptr< MT_Vector2D > ComputeNearestFuseauEntryPoint( const MIL_AgentPion& callerAgent );
    static boost::shared_ptr< MT_Vector2D > ComputeSupportPosition( const MIL_AgentPion& callerAgent, DEC_Decision_ABC* pAgentToSupport, double rDist );
    static boost::shared_ptr< MT_Vector2D > ComputeAmbushPosition ( const MIL_AgentPion& callerAgent, MT_Vector2D* pAmbushPosition, MT_Vector2D* pRetreatPosition, double rDist );
    static boost::shared_ptr< MT_Vector2D > ComputeSafetyPosition( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledgeEnemy, double rMinDistance );
    static boost::shared_ptr< MT_Vector2D > ComputeSafetyPositionWithPopulation( const MIL_AgentPion& callerAgent, unsigned int nPopulationKnowledgeID, double rMinDistance );
    static boost::shared_ptr< MT_Vector2D > ComputeSafetyPositionWithObjective( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledgeEnnemy, double rMinMeterDistance, MT_Vector2D* pObjective );
    template< typename T > static boost::shared_ptr< MT_Vector2D > ComputeDestPoint( const T& caller );
    static boost::shared_ptr< MT_Vector2D > ComputeDestPointForPion( MIL_Automate& callerAutomate, DEC_Decision_ABC* pPion );
    static boost::shared_ptr< MT_Vector2D > ComputeDestPointForFuseau( MIL_Fuseau& fuseau );
    template< typename T > static boost::shared_ptr< MT_Vector2D > ComputeStartPoint( const T& caller );
    static boost::shared_ptr< MT_Vector2D > ComputeObstaclePositionForUnit(
        const MIL_AgentPion& pion, MT_Vector2D* pCenter, const std::string& type, double rRadius );
    static boost::shared_ptr< MT_Vector2D > ComputeObstaclePositionForAutomat(
        const MIL_Automate& automat, MT_Vector2D* pCenter, const std::string& type, double rRadius );
    static std::vector< boost::shared_ptr< MT_Vector2D > > ComputePointsBeforeLima ( const MIL_Automate& callerAutomate, unsigned int nLimaID, double rDistBeforeLima, unsigned int nNbrPoints );
    template< typename T > static boost::shared_ptr< MT_Vector2D > ComputePointBeforeLima( const T& caller, int phaseLine, float distanceBefore );
    template< typename T > static boost::shared_ptr< MT_Vector2D > ComputePointBeforeLimaInFuseau( const T& caller, unsigned int limaID, double rDistBeforeLima, const MIL_Fuseau* pFuseau );
    template< typename T > static std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int > SplitLocalisationInParts( const T& caller, TER_Localisation* pLocalisation, unsigned int nNbrParts, const MT_Vector2D* direction );
    static std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int > SplitPionLocalisationInParts( DEC_Decision_ABC* caller, TER_Localisation* pLocalisation, unsigned int nNbrParts, const MT_Vector2D* direction );
    template< typename T > static std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int > SplitLocalisationInSurfaces( const T& caller, TER_Localisation* pLocalisation, const double rAverageArea, MT_Vector2D* direction );
    template< typename T > static std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int > RecursiveSplitLocalisationInSurfaces( const T& caller, TER_Localisation* pLocalisation, const double rAverageArea );
    template< typename T > static std::vector< boost::shared_ptr< TER_Localisation > > SplitLocalisationInSections( const T& caller, const double rSectionLength );
    template< typename T > static boost::shared_ptr< MT_Vector2D > ComputeLocalisationBarycenterInFuseau( const T& caller, TER_Localisation* pLocalisation );
    static std::vector< boost::shared_ptr< MT_Vector2D > > ComputeLocalisationPointsForPionsInFuseau( const std::vector< DEC_Decision_ABC* >& pions, TER_Localisation* pLocalisation, MT_Vector2D* pDirDanger, double rDistMaxBtwPoints );
    template< typename T > static boost::shared_ptr< MT_Vector2D > ComputeNearestLocalisationPointInFuseau( const T& caller, const TER_Localisation* location );
    template< typename T > static boost::shared_ptr< MT_Vector2D > ComputeNearestUnclippedLocalisationPointInFuseau ( const T& caller, const TER_Localisation* pLocation );
    template< typename T > static bool IsPointInFuseau( const T& caller, MT_Vector2D* pVect );
    static boost::shared_ptr< MT_Vector2D > ComputeKnowledgeAgentBarycenter ( const MIL_AgentPion& caller, const std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& vKnowledges );
    static DEC_Decision_ABC* GetFrontestPion( const std::vector< DEC_Decision_ABC* >& pions, const MT_Vector2D* pDirection );
    static DEC_Decision_ABC* ComputeBackestAgent( const std::vector< DEC_Decision_ABC* >& pions, const MT_Vector2D* pDirection );
    static float ComputeAutomatDelayFromSchedule( const MIL_Fuseau* pFuseau, const std::vector< DEC_Decision_ABC* >& automates, const MIL_LimaOrder* pLima );
    template< typename T > static float ComputeDelayFromScheduleAndObjectives( const T& caller, const MIL_Fuseau* pFuseau, const std::vector< DEC_Decision_ABC* >& automates, const std::vector< DEC_Objective* >& objectives );
    static double ComputeAdvanceAlongFuseau( MIL_Automate& callerAutomate, DEC_Decision_ABC* pion );
    static double ComputePositionAdvanceAlongFuseau( const DEC_Decision_ABC& pCaller, MT_Vector2D* point );
    static double ComputePositionAdvanceAlongDangerDirection( DEC_Decision_ABC* pPion, MT_Vector2D* point );
    static boost::shared_ptr< MT_Vector2D > GetPointAlongFuseau( const MIL_Fuseau* pFuseau, double advance );
    static std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > > GetPointsOnLimasInFuseau( MIL_Automate& callerAutomate, int limaType, int divider );

    // ASA
    static std::vector< boost::shared_ptr< MT_Vector2D > > ComputePosDeploiementASAOmni( const MIL_Automate& automat, int positionCount, const MT_Vector2D* center, float radius );
    static std::vector< boost::shared_ptr< MT_Vector2D > > ComputePosDeploiementASANasse( int positionCount, const MT_Vector2D* center, float angle, float initialDistance, float spacing, const MT_Vector2D* direction );
    static std::vector< boost::shared_ptr< MT_Vector2D > > ComputePosDeploiementASADoubleRideau( int positionCount, const MT_Vector2D* center, float initialDistance, float lineSpacing, float pointSpacing, const MT_Vector2D* direction );

    // Interception
    static boost::shared_ptr< MT_Vector2D > GetInterceptionPosition( const MIL_AgentPion& caller, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, MT_Vector2D* pInterceptingPosition, double rSpeed );

    static bool GetInterceptionPoint( const MT_Vector2D& vToInterceptPosition, const MT_Vector2D& vToInterceptSpeed, const MT_Vector2D& vInterceptingPosition, double rInterceptingSpeed, MT_Vector2D& result );

    // BMArea
    static boost::shared_ptr< MT_Vector2D > ComputeBarycenter( const TER_Localisation* localisation );

    // Fuseau
    static double GetWidth( const MIL_Fuseau* pFuseau );
	static bool IsNull( const MIL_Fuseau* pFuseau );
    static std::vector< boost::shared_ptr< MT_Vector2D > > ComputeMiddlePointsInAOR( const MIL_Fuseau* pFuseau );
    static bool IsPointInFuseau_ParamFuseau( const MIL_Fuseau* pFuseau, const MT_Vector2D* pPoint );

    //@}

private:
    static const MT_Vector2D& GetPosition( const MIL_Automate& );
    static const MT_Vector2D& GetPosition( const MIL_AgentPion& );
    static MT_Vector2D _ComputeAutomatesBarycenter( const std::vector< DEC_Decision_ABC* >& automates );

private:
    static bool ClipLocalisationInFuseau( const TER_Localisation& localisation, const MIL_Fuseau& fuseau, TER_Localisation& clippedLocalisation );
    static unsigned int SplitLocalisation( const TER_Localisation& localisation, unsigned int nNbrParts, const MT_Vector2D* splitDirection, std::vector< boost::shared_ptr< TER_Localisation > >& result );

    //! @name Tools
    //@{
    static float ComputeDelayFromSchedule( const MIL_Fuseau* pFuseau, const std::vector< DEC_Decision_ABC* >& automates, float rDistanceFromScheduled, int nSchedule );
    //@}

private:
    //! @name Types
    //@{
    enum E_LocalisationFunctionsReturnCode
    {
        eError_PionsPasDansMemeFuseau,
        eError_LocalisationPasDansFuseau,
        eWarning_DecoupageIncomplet,
        eNoError
    };

    typedef std::vector< boost::shared_ptr< TER_Localisation > > T_LocalisationPtrVector;
    //@}
};

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::SplitLocalisation
// Created: NLD 2003-08-21
// Modified: JVT 2004-11-03
// -----------------------------------------------------------------------------
template< typename T >
std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int > DEC_GeometryFunctions::SplitLocalisationInParts( const T& caller, TER_Localisation* pLocalisation, unsigned int nNbrParts, const MT_Vector2D* direction )
{
    if( !pLocalisation )
        throw MASA_EXCEPTION( "Null location when splitting location in parts" );

    std::vector< boost::shared_ptr< TER_Localisation > > result;

    TER_Localisation clippedLocalisation;
    unsigned int errCode = eError_LocalisationPasDansFuseau;
    if ( ClipLocalisationInFuseau( *pLocalisation, caller.GetOrderManager().GetFuseau(), clippedLocalisation ) )
        errCode = SplitLocalisation( clippedLocalisation, nNbrParts, direction, result );
    return std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int >( result, errCode );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::SplitLocalisationInSurfaces
// Created: JVT 2004-11-03
// -----------------------------------------------------------------------------
template< typename T >
std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int > DEC_GeometryFunctions::SplitLocalisationInSurfaces( const T& caller, TER_Localisation* pLocalisation, const double rAverageArea, MT_Vector2D* direction )
{
    if( !pLocalisation )
        throw MASA_EXCEPTION( "invalid parameter." );

    std::vector< boost::shared_ptr< TER_Localisation > > result;

    TER_Localisation clippedLocalisation;
    unsigned int errCode = eError_LocalisationPasDansFuseau;
    if ( ClipLocalisationInFuseau( *pLocalisation, caller.GetOrderManager().GetFuseau(), clippedLocalisation ) )
    {
        const unsigned int nNbrParts = std::max( (unsigned int)1, (unsigned int)( clippedLocalisation.GetArea() / rAverageArea ) );
        errCode = SplitLocalisation( clippedLocalisation, nNbrParts, direction, result );
    }
    return std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int >( result, errCode );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::RecursiveSplitLocalisationInSurfaces
// Created: BCI 2011-01-31
// -----------------------------------------------------------------------------
template< typename T >
std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int > DEC_GeometryFunctions::RecursiveSplitLocalisationInSurfaces( const T& caller, TER_Localisation* pLocalisation, const double rAverageArea )
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
template< typename T >
std::vector< boost::shared_ptr< TER_Localisation > > DEC_GeometryFunctions::SplitLocalisationInSections( const T& caller, const double rSectionLength )
{
    const MT_Line& globalDirection = caller.GetOrderManager().GetFuseau().GetGlobalDirection();
    MT_Vector2D vDirection( globalDirection.GetPosEnd() - globalDirection.GetPosStart() );
    vDirection.Normalize();

    return SplitLocalisation( TER_Localisation( TER_Localisation::ePolygon, caller.GetOrderManager().GetFuseau().GetBorderPoints() ), globalDirection.GetPosStart(), vDirection , rSectionLength );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeLocalisationBarycenterInFuseau
// Created: NLD 2003-09-17
// -----------------------------------------------------------------------------
template< typename T >
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeLocalisationBarycenterInFuseau( const T& caller, TER_Localisation* pLocalisation )
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

//-----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeDestPoint
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
template< typename T >
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeDestPoint( const T& caller )
{
    auto pResult = boost::make_shared< MT_Vector2D >(); //$$$$ RAM
    caller.GetOrderManager().GetFuseau().ComputeFurthestExtremityPoint( *pResult );
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeStartPoint
// Created: NLD 2004-05-24
// -----------------------------------------------------------------------------
template< typename T >
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeStartPoint( const T& caller )
{
    boost::shared_ptr< MT_Vector2D > pResult( new MT_Vector2D() ); //$$$$ RAM
    caller.GetOrderManager().GetFuseau().ComputeClosestExtremityPoint( *pResult );
    return pResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::GetLeavingAreaPosition
// Created: MGG 2011-01-20
// -----------------------------------------------------------------------------
template< typename T >
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::GetLeavingAreaPosition( const T& caller, TER_Localisation* pLocalisation )
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
template< typename T >
bool DEC_GeometryFunctions::IsPointInFuseau( const T& caller, MT_Vector2D* pVect )
{
    assert( pVect != 0 );
    if( !pVect )
        throw MASA_EXCEPTION( "Invalid point" );
    return caller.GetOrderManager().GetFuseau().IsInside( *pVect );
}

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputePointBeforeLima
// Created: JVT 2005-01-03
// -----------------------------------------------------------------------------
template< typename T >
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputePointBeforeLima( const T& caller, int phaseLine, float distanceBefore )
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

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_GeometryFunctions::ComputePointBeforeLimaInFuseau
// Created: SBO 2008-01-11
// Modified RPD 2009-08-06
// -----------------------------------------------------------------------------
template< typename T >
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputePointBeforeLimaInFuseau( const T& caller, unsigned int limaID, double rDistBeforeLima, const MIL_Fuseau* pFuseau )
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
template< typename T >
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeNearestLocalisationPointInFuseau( const T& caller, const TER_Localisation* pLocation )
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

// -----------------------------------------------------------------------------
// Name: DEC_GeometryFunctions::ComputeNearestUnclippedLocalisationPointInFuseau
// Created: GGR 2006-09-27
// Modified RPD 2009-08-06
// -----------------------------------------------------------------------------
template< typename T >
boost::shared_ptr< MT_Vector2D > DEC_GeometryFunctions::ComputeNearestUnclippedLocalisationPointInFuseau( const T& caller, const TER_Localisation* pLocation )
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

// -----------------------------------------------------------------------------
// Name: template< typename T > static void DEC_GeometryFunctions::ComputeDelayFromScheduleAndObjectives
// Created: LDC 2009-07-06
// -----------------------------------------------------------------------------
template< typename T >
float DEC_GeometryFunctions::ComputeDelayFromScheduleAndObjectives( const T& caller, const MIL_Fuseau* pFuseau, const std::vector< DEC_Decision_ABC* >& automates, const std::vector< DEC_Objective* >& objectives )
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
        rDistanceFromScheduled = pFuseau->ComputeAverageDistanceFromObjective( *pNextObjective, _ComputeAutomatesBarycenter( automates ) );
        nSchedule = pNextObjective->GetSchedule();
    }
    else if( pNextLima )
    {
        rDistanceFromScheduled = pFuseau->ComputeAverageDistanceFromLima( *pNextLima, _ComputeAutomatesBarycenter( automates ) );
        nSchedule = pNextLima->GetSchedule();
    }

    return ComputeDelayFromSchedule( pFuseau, automates, ( float ) rDistanceFromScheduled, nSchedule );
}

#endif // __DEC_GeometryFunctions_h_
