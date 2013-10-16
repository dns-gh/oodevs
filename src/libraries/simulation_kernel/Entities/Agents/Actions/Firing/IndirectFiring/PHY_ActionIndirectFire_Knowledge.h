// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/IndirectFiring/PHY_ActionIndirectFire_Knowledge.h $
// $Author: Jvt $
// $Modtime: 2/05/05 16:37 $
// $Revision: 2 $
// $Workfile: PHY_ActionIndirectFire_Knowledge.h $
//
// *****************************************************************************

#ifndef __PHY_ActionIndirectFire_Knowledge_h_
#define __PHY_ActionIndirectFire_Knowledge_h_

#include "PHY_ActionIndirectFire_ABC.h"

class DEC_Knowledge_Agent;
class MIL_Effect_IndirectFire;
class MIL_EffectManager;

// =============================================================================
// @class  PHY_ActionIndirectFire_Knowledge
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionIndirectFire_Knowledge : public PHY_ActionIndirectFire_ABC
{
public:
        PHY_ActionIndirectFire_Knowledge( MIL_AgentPion& pion, const PHY_DotationCategory* pDotationCategory, float rNbInterventionType, unsigned int nTargetKnowledgeID );
        PHY_ActionIndirectFire_Knowledge( MIL_AgentPion& pion, const PHY_DotationCategory* pDotationCategory, float rNbInterventionType, boost::shared_ptr< DEC_Knowledge_Agent > targetKnowledge );
    virtual ~PHY_ActionIndirectFire_Knowledge();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    virtual void StopAction();
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MIL_Effect_IndirectFire* > T_Effects;
    typedef T_Effects::const_iterator             CIT_Effects;
    //@}

private:
    T_Effects effects_;
};

#endif // __PHY_ActionIndirectFire_Knowledge_h_
