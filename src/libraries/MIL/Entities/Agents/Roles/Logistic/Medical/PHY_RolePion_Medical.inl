// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Medical/PHY_RolePion_Medical.inl $
// $Author: Nld $
// $Modtime: 27/04/05 17:11 $
// $Revision: 3 $
// $Workfile: PHY_RolePion_Medical.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::HandleHumanEvacuatedByThirdParty
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
inline
PHY_MedicalHumanState* PHY_RolePion_Medical::HandleHumanEvacuatedByThirdParty( MIL_AgentPion& /*pion*/, PHY_Human& /*human*/ )
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::HandleHumanForEvacuation
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
inline
PHY_MedicalHumanState* PHY_RolePion_Medical::HandleHumanForEvacuation( MIL_AgentPion& /*pion*/, PHY_Human& /*human*/ )
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::GetAvailabilityScoreForEvacuation
// Created: NLD 2006-03-29
// -----------------------------------------------------------------------------
inline
int PHY_RolePion_Medical::GetAvailabilityScoreForEvacuation() const
{
    return std::numeric_limits< int >::min();
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::HandleHumanForCollection
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Medical::HandleHumanForCollection( PHY_MedicalHumanState& /*humanState*/ )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::GetAvailabilityScoreForCollection
// Created: NLD 2006-03-29
// -----------------------------------------------------------------------------
inline
int PHY_RolePion_Medical::GetAvailabilityScoreForCollection() const
{
    return std::numeric_limits< int >::min();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::HandleHumanForSorting
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Medical::HandleHumanForSorting( const PHY_MedicalCollectionAmbulance& /*ambulance*/, PHY_MedicalHumanState& /*humanState*/ )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::GetAvailabilityScoreForSorting
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
int PHY_RolePion_Medical::GetAvailabilityScoreForSorting( const PHY_MedicalCollectionAmbulance& /*ambulance*/ ) const
{
    return std::numeric_limits< int >::min();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::ReserveForSorting
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Medical::ReserveForSorting( const PHY_MedicalCollectionAmbulance& /*ambulance*/ )
{    
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::CancelReservationForSorting
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Medical::CancelReservationForSorting( const PHY_MedicalCollectionAmbulance& /*ambulance*/ )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::HandleHumanForHealing
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Medical::HandleHumanForHealing( PHY_MedicalHumanState& /*humanState*/ )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::GetAvailabilityScoreForHealing
// Created: NLD 2005-11-18
// -----------------------------------------------------------------------------
inline
int PHY_RolePion_Medical::GetAvailabilityScoreForHealing( PHY_MedicalHumanState& /*humanState*/ )
{
    return std::numeric_limits< int >::min();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::EnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Medical::EnableSystem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::DisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Medical::DisableSystem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::ChangePriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Medical::ChangePriorities( const T_MedicalPriorityVector& /*priorities*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::ChangePriorities
// Created: NLD 2005-04-27
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Medical::ChangePriorities( const T_AutomateVector& /*priorities*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::ChangeWorkTime
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
inline
void PHY_RolePion_Medical::ChangeWorkTime( const PHY_LogWorkTime& /*workTime*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::CanCollectionAmbulanceGo
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
inline
bool PHY_RolePion_Medical::CanCollectionAmbulanceGo( const PHY_MedicalCollectionAmbulance& /*ambulance*/ ) const
{
    return true;
}
