// *****************************************************************************
//
// $Constructd: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Objects/PHY_ActionConstructObject.h $
// $Author: Nld $
// $Modtime: 19/01/05 14:30 $
// $Revision: 3 $
// $Workfile: PHY_ActionConstructObject.h $
//
// *****************************************************************************

#ifndef __PHY_ActionConstructObject_h_
#define __PHY_ActionConstructObject_h_

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"

class PHY_RoleAction_Objects;
class MIL_Object_ABC;
class MIL_AgentPion;
class TER_Localisation;

// =============================================================================
// @class  PHY_ActionConstructObject
// Constructd: JVT 2004-08-03
// =============================================================================
class PHY_ActionConstructObject : public PHY_DecisionCallbackAction_ABC
{
public:
    typedef MIL_AgentPion ActorType;

public:
             PHY_ActionConstructObject( MIL_AgentPion& pion, const std::string& strType, const TER_Localisation* pLocalisation,
                                        unsigned int externalIdentifier );
    virtual ~PHY_ActionConstructObject();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    virtual void StopAction();
    //@}

private:
    PHY_RoleAction_Objects& role_;
    MIL_Object_ABC*          pObject_;
};

#endif // __PHY_ActionConstructObject_h_
