// *****************************************************************************
//
// $Mined: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_ActionExtractFromStockObject.h $
// $Author: Nld $
// $Modtime: 26/10/04 17:05 $
// $Revision: 2 $
// $Workfile: PHY_ActionExtractFromStockObject.h $
//
// *****************************************************************************

#ifndef __PHY_ActionExtractFromStockObject_h_
#define __PHY_ActionExtractFromStockObject_h_

#include "Entities/Actions/PHY_DecisionCallbackAction_ABC.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_Population.h"

class PHY_DotationCategory;
class PHY_RoleAction_Objects;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_ActionExtractFromStockObject
// Created: BCI 2011-02-08
// =============================================================================
class PHY_ActionExtractFromStockObject : public PHY_DecisionCallbackAction_ABC
{
public:
    //! @name Types
    //@{
    typedef MIL_AgentPion ActorType;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_ActionExtractFromStockObject( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Object > pObject, const std::vector< const PHY_DotationCategory* >& dotationTypes, double quantity );
    virtual ~PHY_ActionExtractFromStockObject();
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
    const std::vector< const PHY_DotationCategory* > dotationTypes_;
    PHY_RoleAction_Objects& role_;
    boost::shared_ptr< DEC_Knowledge_Object > pObject_;
    double quantity_;
    //@}
};

#endif // __PHY_ActionExtractFromStockObject_h_
