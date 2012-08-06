#ifndef __PHY_ActionLendSpecificComposantes_h_
#define __PHY_ActionLendSpecificComposantes_h_

#include "PHY_ActionLendComposantes.h"

class PHY_ComposanteTypePion;

// =============================================================================
// @class  PHY_ActionLendSpecificComposantes
// Created: LDC 2012-08-03
// =============================================================================
class PHY_ActionLendSpecificComposantes : public PHY_ActionLendComposantes
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_ActionLendSpecificComposantes( MIL_AgentPion&, DEC_Decision_ABC* pStart, DEC_Decision_ABC* pAgent, PHY_ComposanteTypePion* type, unsigned int nbrToLend );
    virtual ~PHY_ActionLendSpecificComposantes();
    //@}
};

#endif // __PHY_ActionLendSpecificComposantes_h_
