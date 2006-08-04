// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_MedicalResourcesAlarms::IsEvacuationResourcesLevelReached
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
inline
bool PHY_MedicalResourcesAlarms::IsEvacuationResourcesLevelReached( MT_Float rPreviousRatio, MT_Float rCurrentRatio )
{
    return IsLevelReached( evacuationResourcesLevels_, rPreviousRatio, rCurrentRatio );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalResourcesAlarms::IsCollectionResourcesLevelReached
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
inline
bool PHY_MedicalResourcesAlarms::IsCollectionResourcesLevelReached( MT_Float rPreviousRatio, MT_Float rCurrentRatio )
{
    return IsLevelReached( collectionResourcesLevels_, rPreviousRatio, rCurrentRatio );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalResourcesAlarms::IsDoctorResourcesLevelReached
// Created: NLD 2006-08-02
// -----------------------------------------------------------------------------
inline
bool PHY_MedicalResourcesAlarms::IsDoctorResourcesLevelReached( MT_Float rPreviousRatio, MT_Float rCurrentRatio )
{
    return IsLevelReached( doctorsResourcesLevels_, rPreviousRatio, rCurrentRatio );
}
