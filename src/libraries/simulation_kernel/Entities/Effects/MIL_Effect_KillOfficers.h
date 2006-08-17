// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Effects/MIL_Effect_KillOfficers.h $
// $Author: Age $
// $Modtime: 29/11/04 14:56 $
// $Revision: 3 $
// $Workfile: MIL_Effect_KillOfficers.h $
//
// *****************************************************************************

#ifndef __MIL_Effect_KillOfficers_h_
#define __MIL_Effect_KillOfficers_h_

#include "MIL.h"

#include "MIL_Effect_ABC.h"

class PHY_Composante_ABC;
class MIL_Agent_ABC;

// =============================================================================
// @class  MIL_Effect_KillOfficers
// Created: SBO 2005-12-21
// =============================================================================
class MIL_Effect_KillOfficers : public MIL_Effect_ABC
{
    MT_COPYNOTALLOWED( MIL_Effect_KillOfficers )

public:
             MIL_Effect_KillOfficers( MIL_Agent_ABC& target );
    virtual ~MIL_Effect_KillOfficers();

    //! @name Operations
    //@{
    virtual bool Execute();
    //@}

private:
    MIL_Agent_ABC& target_;
};

#endif // __MIL_Effect_KillOfficers_h_
