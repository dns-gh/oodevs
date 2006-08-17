// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Roles/Logistic/Medical/PHY_MedicalCollectionAmbulance.inl $
// $Author: Nld $
// $Modtime: 11/01/05 17:57 $
// $Revision: 1 $
// $Workfile: PHY_MedicalCollectionAmbulance.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::GetNbrHumans
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
uint PHY_MedicalCollectionAmbulance::GetNbrHumans() const
{
    return consigns_.size();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionAmbulance::IsAnEmergency
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
inline
bool PHY_MedicalCollectionAmbulance::IsAnEmergency() const
{
    return bEmergencyAmbulance_;
}
