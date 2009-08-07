// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_KnowledgeObjectFunctions.h $
// $Author: Nld $
// $Modtime: 23/03/05 11:38 $
// $Revision: 11 $
// $Workfile: DEC_KnowledgeObjectFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_KnowledgeObjectFunctions_h_
#define __DEC_KnowledgeObjectFunctions_h_

#include "MIL.h"
#include "DEC_FunctionsTools.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeCrossingSite.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeSupplyRoute.h"

class MIL_AgentPion;
class DEC_Knowledge_Object;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_KnowledgeObjectFunctions
{
public:
    //! @name Functions
    //@{
    template< typename T > static float GetSiteFranchissementWidth      ( const T& caller, int knowledgeId );
    template< typename T > static std::string GetType                   ( const T& caller, int knowledegeId );
    template< typename T > static const TER_Localisation* GetLocalisation( const T& caller, int knowledegId );
    template< typename T > static int  IsBypassed                       ( const T& caller, int knowledgeId );
    template< typename T > static bool IsReservedObstacleActivated      ( const T& caller, int knowledgeId );
    template< typename T > static bool IsReservedObstacle               ( const T& caller, int knowledegeId );
    template< typename T > static bool IsKnowledgeValid                 ( const T& caller, unsigned int knowledgeId );

                           static int  QueueForDecontamination          ( MIL_AgentPion& callerAgent, int knowledgeId );
                           static bool CanBeAnimated                    ( const MIL_AgentPion& callerAgent, int knowledgeId );
                           static bool CanBeOccupied                    ( const MIL_AgentPion& callerAgent, int knowledgeId );
                           static void DecontaminateZone                ( const MIL_AgentPion& callerAgent, const TER_Localisation* location );
                           static int  DamageObject                     ( const MIL_AgentPion& callerAgent, int knowledgeId, float factor );
                           
                           static bool CanBeBypassed                    ( const MIL_AgentPion& callerAgent, int knowledgeId );

                           static void SetExitingPopulationDensity      ( const MIL_AgentPion& callerAgent, int knowledgeId, float density );
                           static void ResetExitingPopulationDensity    ( const MIL_AgentPion& callerAgent, int knowledgeId );

                           static void Recon                            ( const MIL_AgentPion& callerAgent, int knowledgeId );
    template< typename T > static bool IsRecon                          ( const T& caller, int knowledgeId );

                           static int  EquipLogisticRoute               ( const MIL_AgentPion& callerAgent, int knowledgeId );
    //@}
};

#include "DEC_KnowledgeObjectFunctions.inl"

#endif // __DEC_KnowledgeObjectFunctions_h_
