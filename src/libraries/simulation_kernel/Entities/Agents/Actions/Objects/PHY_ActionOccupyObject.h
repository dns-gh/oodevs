// *****************************************************************************
//
// $Demined: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionOccupyObject.h $
// $Author: Nld $
// $Modtime: 26/11/04 17:14 $
// $Revision: 1 $
// $Workfile: PHY_ActionOccupyObject.h $
//
// *****************************************************************************

#ifndef __PHY_ActionOccupyObject_h_
#define __PHY_ActionOccupyObject_h_

#include "Entities/Actions/PHY_Action_ABC.h"

class PHY_RoleAction_Objects;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionOccupyObject
// Demined: JVT 2004-08-03
// =============================================================================
class PHY_ActionOccupyObject : public PHY_Action_ABC
{
    
public:
    typedef MIL_AgentPion ActorType;

public:
             PHY_ActionOccupyObject( MIL_AgentPion& pion, DIA_Call_ABC& diaCall );
    virtual ~PHY_ActionOccupyObject();

    //! @name Operations
    //@{
    virtual void Execute         ();
    virtual void ExecuteSuspended();
    //@}

private:
    PHY_RoleAction_Objects& role_;
    uint                    nKnowledgeID_;    
};

#endif // __PHY_ActionOccupyObject_h_
