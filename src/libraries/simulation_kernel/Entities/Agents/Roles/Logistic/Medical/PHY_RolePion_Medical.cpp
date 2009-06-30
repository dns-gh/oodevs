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

#include "simulation_kernel_pch.h"
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RolePion_Medical::PHY_RolePion_Medical()
    : PHY_RoleInterface_Medical()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical destructor
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
PHY_RolePion_Medical::~PHY_RolePion_Medical()
{
    // NOTHING
}

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

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::GetPosition
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_RolePion_Medical::GetPosition() const
{
    return GetRole< PHY_RolePion_Location >().GetPosition();
}

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

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::HandleHumanEvacuatedByThirdParty
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
PHY_MedicalHumanState* PHY_RolePion_Medical::HandleHumanEvacuatedByThirdParty( MIL_AgentPion& /*pion*/, PHY_Human& /*human*/ )
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::HandleHumanForEvacuation
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
PHY_MedicalHumanState* PHY_RolePion_Medical::HandleHumanForEvacuation( MIL_AgentPion& /*pion*/, PHY_Human& /*human*/ )
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::GetAvailabilityScoreForEvacuation
// Created: NLD 2006-03-29
// -----------------------------------------------------------------------------
int PHY_RolePion_Medical::GetAvailabilityScoreForEvacuation( const PHY_Human& /*human*/ ) const
{
    return std::numeric_limits< int >::min();
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::HandleHumanForCollection
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
bool PHY_RolePion_Medical::HandleHumanForCollection( PHY_MedicalHumanState& /*humanState*/ )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::GetAvailabilityScoreForCollection
// Created: NLD 2006-03-29
// -----------------------------------------------------------------------------
int PHY_RolePion_Medical::GetAvailabilityScoreForCollection( const PHY_MedicalHumanState& /*humanState*/ ) const
{
    return std::numeric_limits< int >::min();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::HandleHumanForSorting
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Medical::HandleHumanForSorting( const PHY_MedicalCollectionAmbulance& /*ambulance*/, PHY_MedicalHumanState& /*humanState*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::GetAvailabilityScoreForSorting
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
int PHY_RolePion_Medical::GetAvailabilityScoreForSorting( const PHY_MedicalCollectionAmbulance& /*ambulance*/ ) const
{
    return std::numeric_limits< int >::min();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::ReserveForSorting
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
void PHY_RolePion_Medical::ReserveForSorting( const PHY_MedicalCollectionAmbulance& /*ambulance*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::CancelReservationForSorting
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_RolePion_Medical::CancelReservationForSorting( const PHY_MedicalCollectionAmbulance& /*ambulance*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::HandleHumanForHealing
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
bool PHY_RolePion_Medical::HandleHumanForHealing( PHY_MedicalHumanState& /*humanState*/ )
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::GetAvailabilityScoreForHealing
// Created: NLD 2005-11-18
// -----------------------------------------------------------------------------
int PHY_RolePion_Medical::GetAvailabilityScoreForHealing( const PHY_MedicalHumanState& /*humanState*/ )
{
    return std::numeric_limits< int >::min();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::EnableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePion_Medical::EnableSystem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::DisableSystem
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePion_Medical::DisableSystem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::EnableHealingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void PHY_RolePion_Medical::EnableHealingFunction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::DisableHealingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void PHY_RolePion_Medical::DisableHealingFunction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::EnableSortingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void PHY_RolePion_Medical::EnableSortingFunction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::DisableSortingFunction
// Created: NLD 2006-08-01
// -----------------------------------------------------------------------------
void PHY_RolePion_Medical::DisableSortingFunction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::ChangePriorities
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_RolePion_Medical::ChangePriorities( const T_MedicalPriorityVector& /*priorities*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::ChangePriorities
// Created: NLD 2005-04-27
// -----------------------------------------------------------------------------
void PHY_RolePion_Medical::ChangePriorities( const T_AutomateVector& /*priorities*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePion_Medical::CanCollectionAmbulanceGo
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
bool PHY_RolePion_Medical::CanCollectionAmbulanceGo( const PHY_MedicalCollectionAmbulance& /*ambulance*/ ) const
{
    return true;
}
