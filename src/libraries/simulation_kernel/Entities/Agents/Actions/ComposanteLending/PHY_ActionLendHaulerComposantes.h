// *****************************************************************************
//
// $Created: JVT 2005-05-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/ComposanteLending/PHY_ActionLendHaulerComposantes.h $
// $Author: Jvt $
// $Modtime: 12/05/05 15:21 $
// $Revision: 1 $
// $Workfile: PHY_ActionLendHaulerComposantes.h $
//
// *****************************************************************************

#ifndef __PHY_ActionLendHaulerComposantes_h_
#define __PHY_ActionLendHaulerComposantes_h_

#include "PHY_ActionLendComposantes.h"

// =============================================================================
// @class  PHY_ActionLendHaulerComposantes
// Created: JVT 2005-05-12
// =============================================================================
class PHY_ActionLendHaulerComposantes : public PHY_ActionLendComposantes
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_ActionLendHaulerComposantes( MIL_AgentPion&, DEC_Decision_ABC* pStart, DEC_Decision_ABC* pAgent, unsigned int nbrToLend );
    virtual ~PHY_ActionLendHaulerComposantes();
    //@}
};

#endif // __PHY_ActionLendHaulerComposantes_h_
