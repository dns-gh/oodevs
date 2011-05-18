// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_FireFunctions.h $
// $Author: Jvt $
// $Modtime: 2/05/05 11:39 $
// $Revision: 4 $
// $Workfile: DEC_FireFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_FireFunctions_h_
#define __DEC_FireFunctions_h_

#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Decision/DEC_Decision_ABC.h"

class MIL_AgentPion;
class PHY_DotationCategory;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_FireFunctions
{
public:
    //! @name Functions
    //@{
    static float GetMaxRangeToFireOnEnemy( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH );
    static float GetMaxRangeToFireOnEnemyWithDotation( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH, const PHY_DotationCategory* dotation );
    static float GetMinRangeToFireOnEnemy( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH );
    static float GetMaxRangeToFireOnEnemyActualPosture( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH );
    static float GetMinRangeToFireOnEnemyActualPosture( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH );
    static float GetMaxRangeToFireOnEnemyWhenUnloaded( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH );
    static float GetMaxRangeToBeFiredByEnemy( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float rWantedPH );
    static float GetMaxRangeToFire( const MIL_Agent_ABC& callerAgent, float rWantedPH );
    static float GetMaxRangeToFireDecision( const DEC_Decision_ABC& callerAgent, float rWantedPH );
    static float GetMaxRangeToIndirectFire( const MIL_AgentPion& callerAgent, const PHY_DotationCategory* pDotationCategory );
    static float GetMinRangeToIndirectFire( const MIL_AgentPion& callerAgent, const PHY_DotationCategory* pDotationCategory );
    static float GetMaxRangeToIndirectFireWithoutAmmoCheck( const MIL_AgentPion& callerAgent, const PHY_DotationCategory* pDotationCategory  );
    static float GetMinRangeToIndirectFireWithoutAmmoCheck( const MIL_AgentPion& callerAgent, const PHY_DotationCategory* pDotationCategory  );
    static void ThrowSmokeOnKnowledgeAgent( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pTarget );
    static const PHY_DotationCategory* GetMunitionForIndirectFire( MIL_AgentPion& callerAgent, int indirectFireDotationClassID, const MT_Vector2D* pTarget );
    static void ForbidAmmunition( MIL_AgentPion& callerAgent,  const PHY_DotationCategory* pDotationCategory );
    static void AllowAmmunition( MIL_AgentPion& callerAgent,  const PHY_DotationCategory* pDotationCategory );
    static void AllowAllAmmunitions( MIL_AgentPion& callerAgent );
    //@}
};

#endif // __DEC_FireFunctions_h_
