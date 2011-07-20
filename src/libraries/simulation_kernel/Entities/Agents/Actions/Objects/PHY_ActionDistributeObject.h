// *****************************************************************************
//
// $Mined: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionDistributeObject.h $
// $Author: Nld $
// $Modtime: 26/10/04 17:05 $
// $Revision: 2 $
// $Workfile: PHY_ActionDistributeObject.h $
//
// *****************************************************************************

#ifndef __PHY_ActionDistributeObject_h_
#define __PHY_ActionDistributeObject_h_

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_Population.h"

class PHY_RoleAction_Objects;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionDistributeObject
// Mined: JVT 2004-08-03
// =============================================================================
class PHY_ActionDistributeObject : public PHY_DecisionCallbackAction_ABC
{
public:
    //! @name Types
    //@{
    typedef MIL_AgentPion ActorType;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_ActionDistributeObject( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Object > pObject, double quantity );
    virtual ~PHY_ActionDistributeObject();
    //@}

    //! @name Operations
    //@{
    virtual void Execute();
    virtual void ExecuteSuspended();
    virtual void StopAction();
    //@}

private:
    //! @name Member data
    //@{
    PHY_RoleAction_Objects& role_;
    boost::shared_ptr< DEC_Knowledge_Object > pObject_;
    boost::shared_ptr< DEC_Knowledge_Population > pPeople_;
    double quantity_;
    //@}
};

#endif // __PHY_ActionDistributeObject_h_
