// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/NBC/PHY_RolePion_NBC.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: PHY_RolePion_NBC.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_UnitCanBeContaminated::IsWearingNbcProtectionSuit
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_NBC::IsWearingNbcProtectionSuit() const
{
    return bNbcProtectionSuitWorn_;   
}

// -----------------------------------------------------------------------------
// Name: PHY_UnitCanBeContaminated::IsContaminated
// Created: NLD 2004-05-05
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_NBC::IsContaminated() const
{
    return !nbcAgentTypesContaminating_.empty();    
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::WearProtectionSuit
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_NBC::WearNbcProtectionSuit()
{
    if( bNbcProtectionSuitWorn_ )
        return;
    bNbcProtectionSuitWorn_ = true;      
    bHasChanged_            = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::RemoveProtectionSuit
// Created: NLD 2004-04-30
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_NBC::RemoveNbcProtectionSuit()
{
    if( !bNbcProtectionSuitWorn_ )
        return;
    bNbcProtectionSuitWorn_ = false;    
    bHasChanged_            = true;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Update
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_NBC::Update( bool /*bIsDead*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::Clean
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_NBC::Clean()
{
    bHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_NBC::HasChanged
// Created: NLD 2004-09-22
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_NBC::HasChanged() const
{
    return bHasChanged_;
}
