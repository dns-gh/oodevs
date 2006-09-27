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

class MIL_AgentPion;
class MIL_Automate;
class MIL_Fuseau;
class DEC_RolePion_Decision;

// =============================================================================
// Created: NLD 2003-08-20
// =============================================================================
class DEC_GeometryFunctions
{
public:
    //! @name Functions
    //@{
    // Creation / destruction
    static void CreateLocalisation     ( DIA_Call_ABC& );
    static void CreateListPoint        ( DIA_Call_ABC& );
    static void CreatePoint            ( DIA_Call_ABC& );  
    static void CreateDirection        ( DIA_Call_ABC& );
    static void CreateOrthoDirection   ( DIA_Call_ABC& );

    static void ReverseDirection             ( DIA_Call_ABC& );
    static void RotateDirection              ( DIA_Call_ABC& );
    static void CopyAndReverseDirection      ( DIA_Call_ABC& );
    static void CopyAndRotateDirection       ( DIA_Call_ABC& );
    static void IsPointInsideLocalisation    ( DIA_Call_ABC& );
    static void CompareLocalisations         ( DIA_Call_ABC& );
    static void CompareDirection             ( DIA_Call_ABC& );
    static void ComparePositions             ( DIA_Call_ABC& );
    static void TranslatePosition            ( DIA_Call_ABC& );
    static void TranslatePositionInDirection ( DIA_Call_ABC& );
    static void Distance                     ( DIA_Call_ABC& );
    static void ConvertPointToLocalisation   ( DIA_Call_ABC& );
    static void CreateCircleLocalisation     ( DIA_Call_ABC& );
    static void ComputeMeanDirection         ( DIA_Call_ABC& );
    static void ComputeAgentsBarycenter      ( DIA_Call_ABC& ); //$$$ Pourri
    static void ComputeDistanceFromMiddleLine( DIA_Call_ABC& );
    static void ComputeLocalisationBarycenter( DIA_Call_ABC& );
    static void ComputeCoverPosition         ( DIA_Call_ABC& );
    static void ComputeRandomPointOnCircle   ( DIA_Call_ABC& );
    static void ComputeRandomPointInCircle   ( DIA_Call_ABC& );
    static void IsPionCoordinated            ( DIA_Call_ABC& );
    static void ComputeLocalisationArea      ( DIA_Call_ABC& );
    static void SplitListPoints              ( DIA_Call_ABC& );
    static void SplitPath                    ( DIA_Call_ABC& );
                           
                           static void ComputeChantierPosition                          ( DIA_Call_ABC& call, const MIL_AgentPion& );
                           static void StartComputingFrontAndBackLines                  ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
                           static void StopComputingFrontAndBackLines                   ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
                           static void ComputeDistanceFromFrontLine                     ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
                           static void ComputeDistanceFromBackLine                      ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
                           static void ComputeNearestFuseauEntryPoint                   ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent   );
                           static void ComputeSupportPosition                           ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent   );
                           static void ComputeAmbushPosition                            ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent   );
                           static void ComputeSafetyPosition                            ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent   );
                           static void ComputeSafetyPositionWithPopulation              ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent   );
                           static void ComputeSafetyPositionWithObjective               ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent   );
    template< typename T > static void ComputeDestPoint                                 ( DIA_Call_ABC& call, const T&              caller       );
                           static void ComputeDestPointForPion                          ( DIA_Call_ABC& call, MIL_Automate& callerAutomate       );
    template< typename T > static void ComputeStartPoint                                ( DIA_Call_ABC& call, const T&              caller       );
    template< typename T > static void ComputeObstaclePosition                          ( DIA_Call_ABC& call, const T&              caller       );
                           static void ComputePointsBeforeLima                          ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    template< typename T > static void ComputePointBeforeLima                           ( DIA_Call_ABC& call, const T& caller                    );
    template< typename T > static void SplitLocalisationInParts                         ( DIA_Call_ABC& call, const T& caller                    );
    template< typename T > static void SplitLocalisationInSurfaces                      ( DIA_Call_ABC& call, const T& caller                    );
    template< typename T > static void SplitLocalisationInSections                      ( DIA_Call_ABC& call, const T& caller                    );
    template< typename T > static void ComputeLocalisationBarycenterInFuseau            ( DIA_Call_ABC& call, const T& caller                    );
                           static void ComputeLocalisationPointsForPionsInFuseau        ( DIA_Call_ABC& call, const MIL_Automate& callerAutomate );
    template< typename T > static void ComputeNearestLocalisationPointInFuseau          ( DIA_Call_ABC& call, const T& caller                    );
    template< typename T > static void ComputeNearestUnclippedLocalisationPointInFuseau ( DIA_Call_ABC& call, const T& caller                    );
    template< typename T > static void IsPointInFuseau                                  ( DIA_Call_ABC& call, const T& caller                    );
                           static void ComputeKnowledgeAgentBarycenter                  ( DIA_Call_ABC& call, const MIL_AgentPion& caller        );
    template< typename T > static void ComputeFrontestAgent                             ( DIA_Call_ABC& call, const T& callerAgent );
    template< typename T > static void ComputeBackestAgent                              ( DIA_Call_ABC& call, const T& callerAgent );

    // ASA
    static void ComputePosDeploiementASANasseDBG    ( DIA_Call_ABC& );
    static void ComputePosDeploiementASAOmni        ( DIA_Call_ABC&, const MIL_Automate& );
    static void ComputePosDeploiementASANasse       ( DIA_Call_ABC&, const MIL_Automate& );
    static void ComputePosDeploiementASADoubleRideau( DIA_Call_ABC&, const MIL_Automate& );
    
    // Interception
    static void GetInterceptionPosition( DIA_Call_ABC& call, const MIL_AgentPion& );
    static bool GetInterceptionPoint   ( const MT_Vector2D& vToInterceptPosition, const MT_Vector2D& vToInterceptSpeed, const MT_Vector2D& vInterceptingPosition, MT_Float rInterceptingSpeed, MT_Vector2D& result );
    //@}

private:
    static const MT_Vector2D& GetPosition( const MIL_Automate&  );
    static const MT_Vector2D& GetPosition( const MIL_AgentPion& );

private:
    static bool ClipLocalisationInFuseau( const TER_Localisation& localisation, const MIL_Fuseau& fuseau, TER_Localisation& clippedLocalisation );
    static void SplitLocalisation       ( const TER_Localisation& localisation, uint nNbrParts, DIA_Variable_ABC& diaReturnCode, DIA_Variable_ObjectList& result );
    static void SplitLocalisation       ( const TER_Localisation& localisation, MT_Vector2D vOrigin,  const MT_Vector2D& vDirection, MT_Float rSectionLength, DIA_Variable_ObjectList& diaObjectList );

    //! @name Tools
    //@{
    static DEC_RolePion_Decision* GetFrontestPion( const T_ObjectVector& pions, const MT_Vector2D& vDirection );
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
