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
    template< typename T > static float GetSiteFranchissementWidth      ( const T& caller, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    template< typename T > static std::string GetType                   ( const T& caller, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    template< typename T > static const TER_Localisation* GetLocalisation( const T& caller, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    template< typename T > static int  IsBypassed                       ( const T& caller, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    template< typename T > static bool IsReservedObstacleActivated      ( const T& caller, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    template< typename T > static bool IsReservedObstacle               ( const T& caller, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    template< typename T > static bool IsKnowledgeValid                 ( const T& caller, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );

                           static int  QueueForDecontamination          ( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
                           static bool CanBeAnimated                    ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
                           static bool CanBeOccupied                    ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
                           static void DecontaminateZone                ( const MIL_AgentPion& callerAgent, const TER_Localisation* location );
                           static int  DamageObject                     ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, float factor );
                           
                           static bool CanBeBypassed                    ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );

                           static void SetExitingPopulationDensity      ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, float density );
                           static void ResetExitingPopulationDensity    ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );

                           static void Recon                            ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    template< typename T > static bool IsRecon                          ( const T& caller, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );

                           static int  EquipLogisticRoute               ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    //@}
};

#include "DEC_KnowledgeObjectFunctions.inl"

#endif // __DEC_KnowledgeObjectFunctions_h_
