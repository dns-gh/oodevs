// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Medical/PHY_RolePionLOG_Medical.inl $
// $Author: Jvt $
// $Modtime: 30/03/05 17:52 $
// $Revision: 4 $
// $Workfile: PHY_RolePionLOG_Medical.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetPion
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
inline
const MIL_AgentPionLOG_ABC& PHY_RolePionLOG_Medical::GetPion() const
{
    assert( pPion_ );
    return *pPion_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::GetWorkTime
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
inline
const PHY_LogWorkTime& PHY_RolePionLOG_Medical::GetWorkTime() const
{
    assert( pWorkTime_ );
    return *pWorkTime_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::EnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
inline
void PHY_RolePionLOG_Medical::EnableSystem()
{
    bSystemEnabled_ = true;
    bHasChanged_    = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::DisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
inline
void PHY_RolePionLOG_Medical::DisableSystem()
{
    bSystemEnabled_ = false;
    bHasChanged_    = true;
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::ReserveForSorting
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
void PHY_RolePionLOG_Medical::ReserveForSorting( const PHY_MedicalCollectionAmbulance& ambulance )
{
    assert( reservations_.find( &ambulance ) == reservations_.end() );
    reservations_.insert( &ambulance );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOG_Medical::CancelReservationForSorting
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
inline
void PHY_RolePionLOG_Medical::CancelReservationForSorting( const PHY_MedicalCollectionAmbulance& ambulance )
{
    int nOut = reservations_.erase( &ambulance );
    assert( nOut == 1 );
}