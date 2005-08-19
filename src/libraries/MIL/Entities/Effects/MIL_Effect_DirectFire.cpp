// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Effects/MIL_Effect_DirectFire.cpp $
// $Author: Nld $
// $Modtime: 30/03/05 11:33 $
// $Revision: 4 $
// $Workfile: MIL_Effect_DirectFire.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_Effect_DirectFire.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Actions/Firing/PHY_FireResults_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"

// -----------------------------------------------------------------------------
// Name: MIL_Effect_DirectFire constructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
MIL_Effect_DirectFire::MIL_Effect_DirectFire( const PHY_DotationCategory& dotationCategory, MIL_Agent_ABC& target, PHY_Composante_ABC& compTarget, PHY_FireResults_ABC& fireResult )
    : dotationCategory_( dotationCategory )
    , target_          ( target     )
    , compTarget_      ( compTarget )
    , fireResult_      ( fireResult )
{
    fireResult_.IncRef();
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_DirectFire destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
MIL_Effect_DirectFire::~MIL_Effect_DirectFire()
{
    fireResult_.DecRef();
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_DirectFire::Execute
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
bool MIL_Effect_DirectFire::Execute()
{
    target_.GetRole< PHY_RoleInterface_Composantes >().ApplyDirectFire( compTarget_, dotationCategory_, fireResult_ );
    delete this;
    return false; // Effect must be stopped
}
