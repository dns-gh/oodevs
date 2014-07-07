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
    static boost::shared_ptr< MT_Vector2D > ComputeListPointsBarycenter( const std::vector< boost::shared_ptr< MT_Vector2D > >& points );
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
    static boost::shared_ptr< MT_Vector2D > GetLeavingAreaPosition( const MIL_AgentPion& caller, TER_Localisation* pLocalisation );
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
    static boost::shared_ptr< MT_Vector2D > ComputeUnitDestPoint( const MIL_AgentPion& caller );
    static boost::shared_ptr< MT_Vector2D > ComputeAutomatDestPoint( const MIL_Automate& caller );

    static boost::shared_ptr< MT_Vector2D > ComputeDestPointForPion( MIL_Automate& callerAutomate, DEC_Decision_ABC* pPion );
    static boost::shared_ptr< MT_Vector2D > ComputeDestPointForFuseau( MIL_Fuseau& fuseau );
    static boost::shared_ptr< MT_Vector2D > ComputeUnitStartPoint( const MIL_AgentPion& caller );
    static boost::shared_ptr< MT_Vector2D > ComputeAutomatStartPoint( const MIL_Automate& caller );
    static boost::shared_ptr< MT_Vector2D > ComputeObstaclePositionForUnit(
        const MIL_AgentPion& pion, MT_Vector2D* pCenter, const std::string& type, double rRadius );
    static boost::shared_ptr< MT_Vector2D > ComputeObstaclePositionForAutomat(
        const MIL_Automate& automat, MT_Vector2D* pCenter, const std::string& type, double rRadius );
    static std::vector< boost::shared_ptr< MT_Vector2D > > ComputePointsBeforeLima ( const MIL_Automate& callerAutomate, unsigned int nLimaID, double rDistBeforeLima, unsigned int nNbrPoints );
    static boost::shared_ptr< MT_Vector2D > ComputePointBeforeUnitLima( const MIL_AgentPion& caller, int phaseLine, float distanceBefore );
    static boost::shared_ptr< MT_Vector2D > ComputePointBeforeAutomatLima( const MIL_Automate& caller, int phaseLine, float distanceBefore );

    static boost::shared_ptr< MT_Vector2D > ComputePointBeforeLimaInFuseau( const MIL_Automate& caller, unsigned int limaID, double rDistBeforeLima, const MIL_Fuseau* pFuseau );

    static std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int > SplitUnitLocalisationInParts( const MIL_AgentPion& caller, TER_Localisation* pLocalisation, unsigned int nNbrParts, const MT_Vector2D* direction );
    static std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int > SplitAutomatLocalisationInParts( const MIL_Automate& caller, TER_Localisation* pLocalisation, unsigned int nNbrParts, const MT_Vector2D* direction );

    static std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int > SplitPionLocalisationInParts( DEC_Decision_ABC* caller, TER_Localisation* pLocalisation, unsigned int nNbrParts, const MT_Vector2D* direction );

    static std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int > SplitUnitLocalisationInSurfaces( const MIL_AgentPion& caller, TER_Localisation* pLocalisation, const double rAverageArea, MT_Vector2D* direction );
    static std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int > RecursiveSplitUnitLocalisationInSurfaces( const MIL_AgentPion& caller, TER_Localisation* pLocalisation, const double rAverageArea );

    static std::vector< boost::shared_ptr< TER_Localisation > > SplitUnitLocalisationInSections( const MIL_AgentPion& caller, const double rSectionLength );
    static std::vector< boost::shared_ptr< TER_Localisation > > SplitAutomatLocalisationInSections( const MIL_Automate& caller, const double rSectionLength );

    static boost::shared_ptr< MT_Vector2D > ComputeUnitLocalisationBarycenterInFuseau( const MIL_AgentPion& caller, TER_Localisation* pLocalisation );
    static boost::shared_ptr< MT_Vector2D > ComputeAutomatLocalisationBarycenterInFuseau( const MIL_Automate& caller, TER_Localisation* pLocalisation );

    static std::vector< boost::shared_ptr< MT_Vector2D > > ComputeLocalisationPointsForPionsInFuseau( const std::vector< DEC_Decision_ABC* >& pions, TER_Localisation* pLocalisation, MT_Vector2D* pDirDanger, double rDistMaxBtwPoints );
    static boost::shared_ptr< MT_Vector2D > ComputeNearestLocalisationPointInUnitFuseau( const MIL_AgentPion& caller, const TER_Localisation* pLocation );
    static boost::shared_ptr< MT_Vector2D > ComputeNearestLocalisationPointInAutomatFuseau( const MIL_Automate& caller, const TER_Localisation* pLocation );

    static boost::shared_ptr< MT_Vector2D > ComputeNearestUnclippedLocalisationPointInUnitFuseau( const MIL_AgentPion& caller, const TER_Localisation* pLocation );
    static boost::shared_ptr< MT_Vector2D > ComputeNearestUnclippedLocalisationPointInAutomatFuseau( const MIL_Automate& caller, const TER_Localisation* pLocation );
    static bool IsPointInUnitFuseau( const MIL_AgentPion& caller, MT_Vector2D* pVect );
    static bool IsPointInAutomatFuseau( const MIL_Automate& caller, MT_Vector2D* pVect );

    static boost::shared_ptr< MT_Vector2D > ComputeKnowledgeAgentBarycenter ( const MIL_AgentPion& caller, const std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& vKnowledges );
    static DEC_Decision_ABC* GetFrontestPion( const std::vector< DEC_Decision_ABC* >& pions, const MT_Vector2D* pDirection );
    static DEC_Decision_ABC* ComputeBackestAgent( const std::vector< DEC_Decision_ABC* >& pions, const MT_Vector2D* pDirection );
    static float ComputeAutomatDelayFromSchedule( const MIL_Fuseau* pFuseau, const std::vector< DEC_Decision_ABC* >& automates, const MIL_LimaOrder* pLima );
    static float ComputeDelayFromScheduleAndObjectives( const MIL_Automate& caller, const MIL_Fuseau* pFuseau, const std::vector< DEC_Decision_ABC* >& automates, const std::vector< DEC_Objective* >& objectives );
    static double ComputeAdvanceAlongFuseau( MIL_Automate& callerAutomate, DEC_Decision_ABC* pion );
    static double ComputePositionAdvanceAlongFuseau( const DEC_Decision_ABC& pCaller, MT_Vector2D* point );
    static double ComputePositionAdvanceAlongDangerDirection( DEC_Decision_ABC* pPion, MT_Vector2D* point );
    static boost::shared_ptr< MT_Vector2D > GetPointAlongFuseau( const MIL_Fuseau* pFuseau, double advance );
    static std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > > GetPointsOnLimasInFuseau( MIL_Automate& callerAutomate, int limaType, int divider );
    static boost::shared_ptr< MT_Vector2D > ComputeEntryPointForFuseau( DEC_Decision_ABC* pPion, MIL_Fuseau& fuseau );

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
    // These static functions have no reasons to be kept in the header but their
    // declaration cannot be moved in the .cpp until the other private have been
    // moved to. And the others cannot be moved until the templates have been
    // moved too...
    template< typename T > static std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int > SplitLocalisationInParts( const T& caller, TER_Localisation* pLocalisation, unsigned int nNbrParts, const MT_Vector2D* direction );
    template< typename T > static std::vector< boost::shared_ptr< TER_Localisation > > SplitLocalisationInSections( const T& caller, const double rSectionLength );
    template< typename T > static boost::shared_ptr< MT_Vector2D > ComputeLocalisationBarycenterInFuseau( const T& caller, TER_Localisation* pLocalisation );
    template< typename T > static boost::shared_ptr< MT_Vector2D > ComputePointBeforeLima( const T& caller, int phaseLine, float distanceBefore );
    template< typename T > static boost::shared_ptr< MT_Vector2D > ComputeNearestLocalisationPointInFuseau( const T& caller, const TER_Localisation* location );
    template< typename T > static boost::shared_ptr< MT_Vector2D > ComputeNearestUnclippedLocalisationPointInFuseau ( const T& caller, const TER_Localisation* pLocation );

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

#endif // __DEC_GeometryFunctions_h_
