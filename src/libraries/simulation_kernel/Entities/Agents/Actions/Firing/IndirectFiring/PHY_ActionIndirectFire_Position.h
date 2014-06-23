// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/IndirectFiring/PHY_ActionIndirectFire_Position.h $
// $Author: Jvt $
// $Modtime: 2/05/05 16:32 $
// $Revision: 2 $
// $Workfile: PHY_ActionIndirectFire_Position.h $
//
// *****************************************************************************

#ifndef __PHY_ActionIndirectFire_Position_h_
#define __PHY_ActionIndirectFire_Position_h_

#include "PHY_ActionIndirectFire_ABC.h"

class DEC_Decision_ABC;
class MIL_Effect_IndirectFire;

// =============================================================================
// @class  PHY_ActionIndirectFire_Position
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionIndirectFire_Position : public PHY_ActionIndirectFire_ABC
{
public:
             PHY_ActionIndirectFire_Position( MIL_Agent_ABC& pion, const PHY_DotationCategory* pDotationCategory, float rNbInterventionType, const MT_Vector2D* pTargetPosition );
             PHY_ActionIndirectFire_Position( MIL_Agent_ABC& pion, const PHY_DotationCategory* pDotationCategory, float rNbInterventionType, const MT_Vector2D* pTargetPosition, DEC_Decision_ABC* requester );
    virtual ~PHY_ActionIndirectFire_Position();

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

#endif // __PHY_ActionIndirectFire_Position_h_
