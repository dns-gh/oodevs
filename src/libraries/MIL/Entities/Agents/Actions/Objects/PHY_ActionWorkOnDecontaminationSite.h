// *****************************************************************************
//
// $Demined: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionWorkOnDecontaminationSite.h $
// $Author: Nld $
// $Modtime: 2/11/04 18:48 $
// $Revision: 1 $
// $Workfile: PHY_ActionWorkOnDecontaminationSite.h $
//
// *****************************************************************************

#ifndef __PHY_ActionWorkOnDecontaminationSite_h_
#define __PHY_ActionWorkOnDecontaminationSite_h_

#include "MIL.h"

#include "Entities/Agents/Actions/PHY_Action_ABC.h"

class PHY_RoleAction_Objects;
class MIL_RealObject_ABC;

// =============================================================================
// @class  PHY_ActionWorkOnDecontaminationSite
// Demined: JVT 2004-08-03
// =============================================================================
class PHY_ActionWorkOnDecontaminationSite : public PHY_Action_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionWorkOnDecontaminationSite )

public:
    PHY_ActionWorkOnDecontaminationSite( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionWorkOnDecontaminationSite();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    PHY_RoleAction_Objects& role_;
    uint                    nKnowledgeID_;    
};

#include "PHY_ActionWorkOnDecontaminationSite.inl"

#endif // __PHY_ActionWorkOnDecontaminationSite_h_
