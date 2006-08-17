// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Medical/PHY_RolePion_Medical.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 3 $
// $Workfile: PHY_RolePion_Medical.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_RolePion_Medical.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePion_Medical, "PHY_RolePion_Medical" )

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical constructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Medical::PHY_RolePion_Medical( MT_RoleContainer& role )
    : PHY_RoleInterface_Medical( role )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RolePion_Medical::PHY_RolePion_Medical()
    : PHY_RoleInterface_Medical()
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Medical::~PHY_RolePion_Medical()
{

}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive > 
void PHY_RolePion_Medical::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Medical >( *this );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::Update
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RolePion_Medical::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::UpdateLogistic
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RolePion_Medical::UpdateLogistic( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::Clean
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_RolePion_Medical::Clean()
{
    // NOTHING
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::GetPosition
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_RolePion_Medical::GetPosition() const
{
    return GetRole< PHY_RolePion_Location >().GetPosition();
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::SendChangedState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePion_Medical::SendChangedState() const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::SendFullState
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void PHY_RolePion_Medical::SendFullState() const
{
    // NOTHING
}