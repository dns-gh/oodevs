// *****************************************************************************
//
// $Mined: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionSupplyObject.h $
// $Author: Nld $
// $Modtime: 26/10/04 17:05 $
// $Revision: 2 $
// $Workfile: PHY_ActionSupplyObject.h $
//
// *****************************************************************************

#ifndef __PHY_ActionSupplyObject_h_
#define __PHY_ActionSupplyObject_h_

#include "MIL.h"

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"

class PHY_RoleAction_Objects;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionSupplyObject
// Mined: JVT 2004-08-03
// =============================================================================
class PHY_ActionSupplyObject : public PHY_DecisionCallbackAction_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionSupplyObject )

public:
    typedef MIL_AgentPion ActorType;

public:
    PHY_ActionSupplyObject( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    virtual ~PHY_ActionSupplyObject();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    PHY_RoleAction_Objects& role_;
    boost::shared_ptr< DEC_Knowledge_Object > object_;    
};

#endif // __PHY_ActionSupplyObject_h_
