// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Platform/Device.inl $
// $Author: Sbo $
// $Modtime: 30/06/05 14:35 $
// $Revision: 1 $
//
// *****************************************************************************

namespace TIC
{

// -----------------------------------------------------------------------------
// Name: Device::Find
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
inline
const Device* Device::Find( const std::string& strName )
{
    CIT_DeviceMap it = devices_.find( strName );
    return it == devices_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: Device::GetName
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
inline
const std::string& Device::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: Device::GetKey
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
inline
const std::string& Device::GetKey() const
{
    return strKey_;
}

} // end namespace TIC