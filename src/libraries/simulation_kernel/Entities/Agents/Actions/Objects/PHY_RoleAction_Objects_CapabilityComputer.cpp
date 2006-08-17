// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Objects/PHY_RoleAction_Objects_CapabilityComputer.cpp $
// $Author: Nld $
// $Modtime: 26/10/04 17:03 $
// $Revision: 2 $
// $Workfile: PHY_RoleAction_Objects_CapabilityComputer.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_RoleAction_Objects_CapabilityComputer.h"

#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_CapabilityComputer constructor
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects_CapabilityComputer::PHY_RoleAction_Objects_CapabilityComputer( E_Operation nOperation, const MIL_RealObjectType& objectType )
    : nOperation_    ( nOperation )
    , objectType_    ( objectType )
    , bHasCapability_( false )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_CapabilityComputer destructor
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects_CapabilityComputer::~PHY_RoleAction_Objects_CapabilityComputer()
{

}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_CapabilityComputer::operator()
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_CapabilityComputer::operator() ( const PHY_ComposantePion& composante )
{
    if( bHasCapability_ )
        return;

    switch( nOperation_ )
    {
        case eConstruct: bHasCapability_ = composante.CanConstruct( objectType_ ); break;
        case eDestroy  : bHasCapability_ = composante.CanDestroy  ( objectType_ ); break;
        case eMine     : bHasCapability_ = composante.CanMine     ( objectType_ ); break;
        case eDemine   : bHasCapability_ = composante.CanDemine   ( objectType_ ); break;
        case eBypass   : bHasCapability_ = composante.CanBypass   ( objectType_, false ); break;
        case ePrepare  : bHasCapability_ = composante.CanConstruct( objectType_ ); break;
        default: assert( false );
    }
}
