// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Medical/PHY_MedicalSortingConsign.inl $
// $Author: Nld $
// $Modtime: 25/04/05 11:10 $
// $Revision: 2 $
// $Workfile: PHY_MedicalSortingConsign.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_MedicalSortingConsign::CouldNeedCollectionAmbulance
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
inline
bool PHY_MedicalSortingConsign::CouldNeedCollectionAmbulance() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalSortingConsign::IsATransportConsign
// Created: NLD 2005-04-25
// -----------------------------------------------------------------------------
inline
bool PHY_MedicalSortingConsign::IsATransportConsign() const
{
    return false;
}
