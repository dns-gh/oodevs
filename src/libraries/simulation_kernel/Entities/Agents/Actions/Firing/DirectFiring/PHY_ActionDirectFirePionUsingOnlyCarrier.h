// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionUsingOnlyCarrier.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_ActionDirectFirePionUsingOnlyCarrier.h $
//
// *****************************************************************************

#ifndef __PHY_ActionDirectFirePionUsingOnlyCarrier_h_
#define __PHY_ActionDirectFirePionUsingOnlyCarrier_h_

#include "PHY_ActionDirectFirePion_ABC.h"

// =============================================================================
// @class  PHY_ActionDirectFirePionUsingOnlyCarrier
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ActionDirectFirePionUsingOnlyCarrier : public PHY_ActionDirectFirePion_ABC
{
    MT_COPYNOTALLOWED( PHY_ActionDirectFirePionUsingOnlyCarrier )

public:
    //! @name Constructors/Destructor
    //@{
             PHY_ActionDirectFirePionUsingOnlyCarrier( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Agent > pEnemy, MT_Float percentage, int firingMode );
    virtual ~PHY_ActionDirectFirePionUsingOnlyCarrier();
    //@}
};
#endif // __PHY_ActionDirectFirePionUsingOnlyCarrier_h_
