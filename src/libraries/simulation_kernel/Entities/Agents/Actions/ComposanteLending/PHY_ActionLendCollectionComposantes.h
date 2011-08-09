// *****************************************************************************
//
// $Created: JVT 2005-05-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/ComposanteLending/PHY_ActionLendCollectionComposantes.h $
// $Author: Jvt $
// $Modtime: 12/05/05 15:21 $
// $Revision: 1 $
// $Workfile: PHY_ActionLendCollectionComposantes.h $
//
// *****************************************************************************

#ifndef __PHY_ActionLendCollectionComposantes_h_
#define __PHY_ActionLendCollectionComposantes_h_

#include "PHY_ActionLendComposantes.h"

// =============================================================================
// @class  PHY_ActionLendCollectionComposantes
// Created: JVT 2005-05-12
// =============================================================================
class PHY_ActionLendCollectionComposantes : public PHY_ActionLendComposantes
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_ActionLendCollectionComposantes( MIL_AgentPion&, DEC_Decision_ABC* pStart, DEC_Decision_ABC* pAgent, unsigned int nbrToLend );
    virtual ~PHY_ActionLendCollectionComposantes();
    //@}
};

#endif // __PHY_ActionLendCollectionComposantes_h_
