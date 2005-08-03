// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Platform/DeviceSubCategory.inl $
// $Author: Sbo $
// $Modtime: 4/07/05 10:19 $
// $Revision: 1 $
//
// *****************************************************************************

namespace TIC
{

// -----------------------------------------------------------------------------
// Name: DeviceSubCategory::Find
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
inline
const DeviceSubCategory* DeviceSubCategory::Find( const std::string& strName )
{
    CIT_DeviceSubCategoryMap it = deviceSubCategories_.find( strName );
    return it == deviceSubCategories_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: DeviceSubCategory::GetName
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
inline
const std::string& DeviceSubCategory::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: DeviceSubCategory::GetKey
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
inline
const std::string& DeviceSubCategory::GetKey() const
{
    return strKey_;
}

} // end namespace TIC