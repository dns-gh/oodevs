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
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeCrossingSite.h"

class MIL_Agent_ABC;
class DEC_Knowledge_Object;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_KnowledgeObjectFunctions
{
public:
    //! @name Functions
    //@{
    static float GetSiteFranchissementWidth       ( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static std::string GetType                    ( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static const TER_Localisation* GetLocalisation( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static int  IsBypassed                        ( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static bool IsReservedObstacleActivated       ( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static bool IsReservedObstacle                ( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static bool IsKnowledgeValid                  ( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );

    static int  QueueForDecontamination           ( MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static bool CanBeAnimated                     ( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static float GetConstructionLevel             ( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge);
    static float GetValorizationLevel             ( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge);
    static float GetAnimationLevel                ( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge);
    static bool CanBeOccupied                     ( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static void DecontaminateZone                 ( const MIL_Agent_ABC& callerAgent, const TER_Localisation* location );
    static int  DamageObject                      (       MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, float factor, const PHY_DotationCategory* dotation );

    static bool CanBeBypassed                     ( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );

    static void SetExitingPopulationDensity       ( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, float density );
    static void ResetExitingPopulationDensity     ( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );

    static void Recon                             ( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static bool IsRecon                           ( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );

    static int  EquipLogisticRoute                ( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static bool IsStockSupplied                   ( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    
    static int   IsAnEnemy                        ( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static int   IsAFriend                        ( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static int   GetCurrentPerceptionLevel        ( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static float EstimatedWorkTime                ( MIL_Agent_ABC& pion, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    //@}
};

#endif // __DEC_KnowledgeObjectFunctions_h_
