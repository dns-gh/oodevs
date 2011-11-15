// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Effects/MIL_Effect_DirectFirePion.cpp $
// $Author: Nld $
// $Modtime: 30/03/05 11:33 $
// $Revision: 4 $
// $Workfile: MIL_Effect_DirectFirePion.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Effect_DirectFirePion.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Actions/PHY_FireResults_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_Effect_DirectFirePion constructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
MIL_Effect_DirectFirePion::MIL_Effect_DirectFirePion( const PHY_DotationCategory& dotationCategory, MIL_Agent_ABC& target, PHY_Composante_ABC& compTarget, PHY_FireResults_ABC& fireResult )
    : dotationCategory_( dotationCategory )
    , target_          ( target     )
    , compTarget_      ( compTarget )
    , fireResult_      ( fireResult )
{
    fireResult_.IncRef();
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_DirectFirePion destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
MIL_Effect_DirectFirePion::~MIL_Effect_DirectFirePion()
{
    fireResult_.DecRef();
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_DirectFirePion::Execute
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
bool MIL_Effect_DirectFirePion::Execute()
{
    target_.GetRole< PHY_RoleInterface_Composantes >().ApplyDirectFire( compTarget_, dotationCategory_, fireResult_ );
    delete this;
    return false; // Effect must be stopped
}
