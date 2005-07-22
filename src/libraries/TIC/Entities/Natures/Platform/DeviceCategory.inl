// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Platform/DeviceCategory.inl $
// $Author: Sbo $
// $Modtime: 30/06/05 14:42 $
// $Revision: 1 $
//
// *****************************************************************************

namespace TIC
{

// -----------------------------------------------------------------------------
// Name: DeviceCategory::Find
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
inline
const DeviceCategory* DeviceCategory::Find( const std::string& strName )
{
    CIT_DeviceCategoryMap it = deviceCategories_.find( strName );
    return it == deviceCategories_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: DeviceCategory::GetName
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
inline
const std::string& DeviceCategory::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: DeviceCategory::GetKey
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
inline
const std::string& DeviceCategory::GetKey() const
{
    return strKey_;
}

} // end namespace TIC