// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-17 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Types/PlatformType.inl $
// $Author: Sbo $
// $Modtime: 13/07/05 15:07 $
// $Revision: 7 $
// $Workfile: PlatformType.inl $
//
// *****************************************************************************

namespace TIC
{

// -----------------------------------------------------------------------------
// Name: PlatformType::Find
// Created: SBO 2005-05-26
// -----------------------------------------------------------------------------
inline
const PlatformType* PlatformType::Find( const std::string& strName )
{
    CIT_PlatformTypeMap it = platformTypes_.find( strName );
    return it == platformTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PlatformType::Find
// Created: SBO 2005-05-26
// -----------------------------------------------------------------------------
inline
const PlatformType* PlatformType::Find( ASN1T_TypePion asnId )
{
    for( CIT_PlatformTypeMap it = platformTypes_.begin(); it != platformTypes_.end(); ++it )
        if( ( *it ).second->GetId() == asnId )
            return ( *it ).second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PlatformType::GetName
// Created: SBO 2005-05-26
// -----------------------------------------------------------------------------
inline
const std::string& PlatformType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PlatformType::GetId
// Created: SBO 2005-05-26
// -----------------------------------------------------------------------------
inline
uint PlatformType::GetId() const
{
    return nId_;
}

// -----------------------------------------------------------------------------
// Name: PlatformType::GetMaxSpeed
// Created: SBO 2005-06-21
// -----------------------------------------------------------------------------
inline
T_Speed PlatformType::GetMaxSpeed() const
{
    return nMaxSpeed_;
}

// -----------------------------------------------------------------------------
// Name: PlatformType::GetEMAT6
// Created: SBO 2005-06-02
// -----------------------------------------------------------------------------
inline
const std::string& PlatformType::GetEMAT6() const
{
    return strEMAT6_;
}

// -----------------------------------------------------------------------------
// Name: PlatformType::GetEMAT8
// Created: SBO 2005-06-02
// -----------------------------------------------------------------------------
inline
const std::string& PlatformType::GetEMAT8() const
{
    return strEMAT8_;
}

// -----------------------------------------------------------------------------
// Name: PlatformType::GetLFRIL
// Created: SBO 2005-06-02
// -----------------------------------------------------------------------------
inline
const std::string& PlatformType::GetLFRIL() const
{
    return strLFRIL_;
}

// -----------------------------------------------------------------------------
// Name: PlatformType::GetNNO
// Created: SBO 2005-06-02
// -----------------------------------------------------------------------------
inline
const std::string& PlatformType::GetNNO() const
{
    return strNNO_;
}

// -----------------------------------------------------------------------------
// Name: PlatformType::GetProtection
// Created: SBO 2005-06-27
// -----------------------------------------------------------------------------
inline
const std::string& PlatformType::GetProtection() const
{
    return strProtection_;
}

// -----------------------------------------------------------------------------
// Name: PlatformType::GetVolume
// Created: SBO 2005-06-27
// -----------------------------------------------------------------------------
inline
const std::string& PlatformType::GetVolume() const
{
    return strVolume_;
}


// -----------------------------------------------------------------------------
// Name: PlatformType::CanBeDisaggregated
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
inline
bool PlatformType::CanBeDisaggregated() const
{
    return bCanBeDisaggregated_;
}


// -----------------------------------------------------------------------------
// Name: PlatformType::GetDevice
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
inline
const Device& PlatformType::GetDevice() const
{
    assert( pDevice_ );
    return *pDevice_;
}

// -----------------------------------------------------------------------------
// Name: PlatformType::GetDeviceCategory
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
inline
const DeviceCategory& PlatformType::GetDeviceCategory() const
{
    assert( pDeviceCategory_ );
    return *pDeviceCategory_;
}

// -----------------------------------------------------------------------------
// Name: PlatformType::GetDeviceSubCategory
// Created: SBO 2005-07-04
// -----------------------------------------------------------------------------
inline
const DeviceSubCategory& PlatformType::GetDeviceSubCategory() const
{
    assert( pDeviceSubCategory_ );
    return *pDeviceSubCategory_;
}

} // end namespace TIC