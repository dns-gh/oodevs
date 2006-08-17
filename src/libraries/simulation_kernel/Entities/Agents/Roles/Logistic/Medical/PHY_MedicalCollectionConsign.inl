// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Medical/PHY_MedicalCollectionConsign.inl $
// $Author: Nld $
// $Modtime: 25/04/05 11:09 $
// $Revision: 2 $
// $Workfile: PHY_MedicalCollectionConsign.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::CouldNeedCollectionAmbulance
// Created: NLD 2005-01-13
// -----------------------------------------------------------------------------
inline
bool PHY_MedicalCollectionConsign::CouldNeedCollectionAmbulance() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalCollectionConsign::IsATransportConsign
// Created: NLD 2005-04-25
// -----------------------------------------------------------------------------
inline
bool PHY_MedicalCollectionConsign::IsATransportConsign() const
{
    return true;
}
