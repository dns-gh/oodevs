// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_DirectFireResults.h $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 3 $
// $Workfile: PHY_DirectFireResults.h $
//
// *****************************************************************************

#ifndef __PHY_DirectFireResults_h_
#define __PHY_DirectFireResults_h_

#include "MIL.h"

#include "Entities/Agents/Actions/Firing/PHY_FireResults_ABC.h"

class MIL_Agent_ABC;
class MIL_AgentPion;

// =============================================================================
// @class  PHY_DirectFireResults
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DirectFireResults : public PHY_FireResults_ABC
{
    MT_COPYNOTALLOWED( PHY_DirectFireResults )

public:
    PHY_DirectFireResults( const MIL_AgentPion& firer, const MIL_Agent_ABC& target );
    virtual ~PHY_DirectFireResults();

private:
    const uint            nID_;
    const MIL_AgentPion& firer_;
    const MIL_Agent_ABC& target_;
};

#include "PHY_DirectFireResults.inl"

#endif // __PHY_DirectFireResults_h_
