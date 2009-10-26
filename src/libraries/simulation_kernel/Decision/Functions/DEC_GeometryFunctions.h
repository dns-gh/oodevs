// *****************************************************************************
//
// $Created: NLD 2003-08-20 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_GeometryFunctions.h $
// $Author: Jvt $
// $Modtime: 23/05/05 15:48 $
// $Revision: 23 $
// $Workfile: DEC_GeometryFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_GeometryFunctions_h_
#define __DEC_GeometryFunctions_h_

#include "MIL.h"
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
    static boost::shared_ptr< MT_Vector2D > CreatePoint         ();  
    static boost::shared_ptr< MT_Vector2D > CreateDirection     ( MT_Vector2D* pPosSource, MT_Vector2D* pPosDest );
    static boost::shared_ptr< MT_Vector2D > CreateOrthoDirection( MT_Vector2D* pDir, bool bCounterClockwise );

    static void ReverseDirection                    ( boost::shared_ptr< MT_Vector2D > pDir );
    static boost::shared_ptr< MT_Vector2D > CopyAndReverseDirection( const MT_Vector2D* );
    static boost::shared_ptr< MT_Vector2D > CopyAndRotateDirection ( const MT_Vector2D*, const MT_Float angle );
    static bool IsPointInsideLocalisation           ( MT_Vector2D* pPoint, TER_Localisation* pLocalisation );
    static bool ComparePositions                    (  MT_Vector2D* p1, MT_Vector2D* p2 );
    static boost::shared_ptr< MT_Vector2D >      TranslatePosition           ( MT_Vector2D* p1, MT_Vector2D* p2, MT_Float d );
    static boost::shared_ptr< MT_Vector2D >      TranslatePositionInDirection( MT_Vector2D* p1, MT_Vector2D* p2, MT_Float d );
    static MT_Float       Distance                    ( const MT_Vector2D* p1, const MT_Vector2D* p2 );
    static boost::shared_ptr< TER_Localisation > ConvertPointToLocalisation  ( const MT_Vector2D* pPos );
    static boost::shared_ptr< TER_Localisation > CreateCircleLocalisation    ( const MT_Vector2D* pCenter, const MT_Float );
    static boost::shared_ptr< MT_Vector2D > ComputeMeanDirection( const std::vector< MT_Vector2D* >& selection );
    static boost::shared_ptr< MT_Vector2D > ComputeAgentsBarycenter( const std::vector< DEC_Decision_ABC* >& selection ); //$$$ Pourri
    static float ComputeDistanceFromMiddleLine( const std::vector< DEC_Decision_ABC*>& selPions, DEC_Decision_ABC* pReferencePion );
    static boost::shared_ptr< MT_Vector2D > ComputeLocalisationBarycenter( TER_Localisation* pLocalisation );
    static boost::shared_ptr< MT_Vector2D > ComputeCoverPosition( const std::vector< DEC_Decision_ABC* >& pions, MT_Vector2D* pDirection, float distance );
    static boost::shared_ptr< MT_Vector2D > ComputeRandomPointOnCircle( MT_Vector2D* pCenter, float radius );
    static boost::shared_ptr< MT_Vector2D > ComputeRandomPointInCircle( MT_Vector2D* pCenter, float radius );
    static std::vector< DEC_Decision_ABC* > ListUncoordinatedPawns          ( DEC_Decision_ABC* pion, const std::vector< DEC_Decision_ABC* >& unCoordinatedPions, float minDist );
    static std::vector< boost::shared_ptr< MT_Vector2D > >  SplitListPoints( const std::vector< MT_Vector2D* >& listPoints, unsigned int nNbrParts );
    static float ComputeClosedTerrainRatioInZone    ( const TER_Localisation* location );
    static float ComputeOpenTerrainRatioInZone      ( const TER_Localisation* location );
    static float ComputeClosedTerrainRatioInFuseau  ( const MIL_Fuseau* );
    static float ComputeOpenTerrainRatioInFuseau    ( const MIL_Fuseau* pFuseau);
    static std::vector< TER_Localisation* > SortZonesAccordingToTerrainOpening( const std::vector< TER_Localisation* >& list );
    static std::vector< MIL_Fuseau* > SortFuseauxAccordingToTerrainOpening( const std::vector< MIL_Fuseau* >& list );
    static boost::shared_ptr< TER_Localisation > ConvertFuseauToLocalisation( const MIL_Fuseau* pFuseau );
    static DEC_Objective* GetNextObjectiveInFuseau( const MIL_Fuseau* pFuseau, const MT_Vector2D* pRefPoint, const std::vector< DEC_Objective* >& objectives );
    static boost::shared_ptr< TER_Localisation > ComputeAreaInZone( const MIL_Fuseau* zone, const MT_Vector2D* center );
    
    static DEC_FrontAndBackLinesComputer* StartComputingFrontAndBackLines       ( const MIL_Automate& callerAutomate, const std::vector< DEC_Decision_ABC* >& pionDecisionList );
    static DEC_FrontAndBackLinesComputer* StartComputingAutomatFrontAndBackLines( const MIL_Automate& callerAutomate, const std::vector< DEC_Decision_ABC* >& automatDecisionList );
    static void StopComputingFrontAndBackLines                   ( DEC_FrontAndBackLinesComputer* pComputer );
    static float ComputeDistanceFromFrontLine                    ( DEC_FrontAndBackLinesComputer* pComputer, DEC_Decision_ABC* pPion );
    static float ComputeDistanceAutomatFromFrontLine             ( DEC_FrontAndBackLinesComputer* pComputer, DEC_Decision_ABC* pAutomate );
    static float ComputeDistanceFromBackLine                     ( DEC_FrontAndBackLinesComputer* pComputer, DEC_Decision_ABC* pPion );
    static float ComputeDistanceAutomatFromBackLine               ( const MIL_Automate& callerAutomate, DEC_FrontAndBackLinesComputer* pComputer, DEC_Decision_ABC* pAutomate );
    static boost::shared_ptr< MT_Vector2D > ComputeNearestFuseauEntryPoint( const MIL_AgentPion& callerAgent );
    static boost::shared_ptr< MT_Vector2D > ComputeSupportPosition( const MIL_AgentPion& callerAgent, DEC_Decision_ABC* pAgentToSupport, MT_Float rDist );
    static boost::shared_ptr< MT_Vector2D > ComputeAmbushPosition ( const MIL_AgentPion& callerAgent, MT_Vector2D* pAmbushPosition, MT_Vector2D* pRetreatPosition, MT_Float rDist );
    static boost::shared_ptr< MT_Vector2D > ComputeSafetyPosition( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledgeEnemy, MT_Float rMinDistance );
    static boost::shared_ptr< MT_Vector2D > ComputeSafetyPositionWithPopulation              ( const MIL_AgentPion& callerAgent, uint nPopulationKnowledgeID, MT_Float rMinDistance );
    static boost::shared_ptr< MT_Vector2D > ComputeSafetyPositionWithObjective( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledgeEnnemy, MT_Float rMinMeterDistance, MT_Vector2D* pObjective );
    template< typename T > static boost::shared_ptr< MT_Vector2D > ComputeDestPoint     ( const T& caller );
    static boost::shared_ptr< MT_Vector2D > ComputeDestPointForPion( MIL_Automate& callerAutomate, DEC_Decision_ABC* pPion );
    template< typename T > static boost::shared_ptr< MT_Vector2D > ComputeStartPoint    ( const T& caller );
    template< typename T > static boost::shared_ptr< MT_Vector2D > ComputeObstaclePosition( const T& caller, MT_Vector2D* pCenter, const std::string& type, MT_Float rRadius );
    static std::vector< boost::shared_ptr< MT_Vector2D > > ComputePointsBeforeLima ( const MIL_Automate& callerAutomate, uint nLimaID, MT_Float rDistBeforeLima, uint nNbrPoints );
    template< typename T > static boost::shared_ptr< MT_Vector2D > ComputePointBeforeLima( const T& caller, int phaseLine, float distanceBefore );
    template< typename T > static boost::shared_ptr< MT_Vector2D > ComputePointBeforeLimaInFuseau( const T& caller, uint limaID, MT_Float rDistBeforeLima, const MIL_Fuseau* pFuseau );
    template< typename T > static std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int > SplitLocalisationInParts   ( const T& caller, TER_Localisation* pLocalisation, unsigned int nNbrParts );
    template< typename T > static std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int > SplitLocalisationInSurfaces( const T& caller, TER_Localisation* pLocalisation, const MT_Float rAverageArea );
    template< typename T > static std::vector< boost::shared_ptr< TER_Localisation > > SplitLocalisationInSections( const T& caller, const MT_Float rSectionLength );
    template< typename T > static boost::shared_ptr< MT_Vector2D > ComputeLocalisationBarycenterInFuseau( const T& caller, TER_Localisation* pLocalisation );
    static std::vector< boost::shared_ptr< MT_Vector2D > > ComputeLocalisationPointsForPionsInFuseau( const std::vector< DEC_Decision_ABC* >& pions, TER_Localisation* pLocalisation, MT_Vector2D* pDirDanger, MT_Float rDistMaxBtwPoints );
    template< typename T > static boost::shared_ptr< MT_Vector2D > ComputeNearestLocalisationPointInFuseau( const T& caller, const TER_Localisation* location );
    template< typename T > static boost::shared_ptr< MT_Vector2D > ComputeNearestUnclippedLocalisationPointInFuseau ( const T& caller, const TER_Localisation* pLocation );
    template< typename T > static bool IsPointInFuseau           ( const T& caller, MT_Vector2D* pVect );
    static boost::shared_ptr< MT_Vector2D > ComputeKnowledgeAgentBarycenter ( const MIL_AgentPion& caller, const std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > vKnowledges );
    static DEC_Decision_ABC* GetFrontestPion    ( const std::vector< DEC_Decision_ABC* >& pions, const MT_Vector2D* pDirection );
    static DEC_Decision_ABC* ComputeBackestAgent( const std::vector< DEC_Decision_ABC* >& pions, const MT_Vector2D* pDirection );
    static float ComputeAutomatDelayFromSchedule                 ( const MIL_Fuseau* pFuseau, const std::vector< DEC_Decision_ABC* >& automates, const MIL_LimaOrder* pLima );
    template< typename T > static float ComputeDelayFromScheduleAndObjectives           ( const T& caller, const MIL_Fuseau* pFuseau, const std::vector< DEC_Decision_ABC* >& automates, const std::vector< DEC_Objective* >& objectives );

    // ASA
    static std::vector< boost::shared_ptr< MT_Vector2D > > ComputePosDeploiementASAOmni( const MIL_Automate& automat, int positionCount, const MT_Vector2D* center, float radius );
    static std::vector< boost::shared_ptr< MT_Vector2D > > ComputePosDeploiementASANasse( int positionCount, const MT_Vector2D* center, float angle, float initialDistance, float spacing, const MT_Vector2D* direction );
    static std::vector< boost::shared_ptr< MT_Vector2D > > ComputePosDeploiementASADoubleRideau( int positionCount, const MT_Vector2D* center, float initialDistance, float lineSpacing, float pointSpacing, const MT_Vector2D* direction );
    
    // Interception
    static boost::shared_ptr< MT_Vector2D > GetInterceptionPosition( const MIL_AgentPion& caller, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, MT_Vector2D* pInterceptingPosition, MT_Float rSpeed );

    static bool GetInterceptionPoint   ( const MT_Vector2D& vToInterceptPosition, const MT_Vector2D& vToInterceptSpeed, const MT_Vector2D& vInterceptingPosition, MT_Float rInterceptingSpeed, MT_Vector2D& result );

    // Terrain keypoint
    static std::vector< boost::shared_ptr< MT_Vector2D > > GetCrossroads(  const MT_Vector2D& vCenter, float radius );
    //@}

private:
    static const MT_Vector2D& GetPosition                ( const MIL_Automate&  );
    static const MT_Vector2D& GetPosition                ( const MIL_AgentPion& );
    static       MT_Vector2D  _ComputeAutomatesBarycenter( const std::vector< DEC_Decision_ABC* >& automates );

private:
    static bool ClipLocalisationInFuseau( const TER_Localisation& localisation, const MIL_Fuseau& fuseau, TER_Localisation& clippedLocalisation );
    static unsigned int SplitLocalisation( const TER_Localisation& localisation, uint nNbrParts, std::vector< boost::shared_ptr< TER_Localisation > >& result );
    static std::vector< boost::shared_ptr< TER_Localisation > > SplitLocalisation       ( const TER_Localisation& localisation, MT_Vector2D vOrigin,  const MT_Vector2D& vDirection, MT_Float rSectionLength );

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
    //@}
};

#include "DEC_GeometryFunctions.inl"

#endif // __DEC_GeometryFunctions_h_
