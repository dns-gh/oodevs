// *****************************************************************************
//
// $Prepared: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionPrepareObject.h $
// $Author: Nld $
// $Modtime: 26/10/04 17:05 $
// $Revision: 2 $
// $Workfile: PHY_ActionPrepareObject.h $
//
// *****************************************************************************

#ifndef __PHY_ActionPrepareObject_h_
#define __PHY_ActionPrepareObject_h_

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

class PHY_RoleAction_Objects;
class MIL_AgentPion;
class MIL_Object_ABC;
class TER_Localisation;
class DEC_Decision_ABC;

// =============================================================================
// @class  PHY_ActionPrepareObject
// Prepared: JVT 2004-08-03
// =============================================================================
class PHY_ActionPrepareObject : public PHY_DecisionCallbackAction_ABC
{

public:
    typedef MIL_AgentPion ActorType;

public:
             PHY_ActionPrepareObject( MIL_AgentPion& pion, const std::string& strType, const TER_Localisation* pLocalisation );
    virtual ~PHY_ActionPrepareObject();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    PHY_RoleAction_Objects& role_;
    MIL_Object_ABC*         pObject_;
};

#endif // __PHY_ActionPrepareObject_h_
