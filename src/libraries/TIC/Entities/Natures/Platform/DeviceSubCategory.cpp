// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Platform/DeviceSubCategory.cpp $
// $Author: Sbo $
// $Modtime: 13/07/05 15:06 $
// $Revision: 2 $
// $Workfile: DeviceSubCategory.cpp $
//
// *****************************************************************************

#include "TIC_pch.h"

#include "DeviceSubCategory.h"

using namespace TIC;

DeviceSubCategory::T_DeviceSubCategoryMap DeviceSubCategory::deviceSubCategories_;
const DeviceSubCategory                DeviceSubCategory::notDefined( "None", eNone, "NONE" );

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DeviceSubCategory::Initialize
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
void DeviceSubCategory::Initialize()
{
    deviceSubCategories_[ notDefined.GetName() ] = &notDefined;
}

// -----------------------------------------------------------------------------
// Name: DeviceSubCategory::Terminate
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
void DeviceSubCategory::Terminate()
{
    for( CIT_DeviceSubCategoryMap it = deviceSubCategories_.begin(); it != deviceSubCategories_.end(); ++it )
        if( it->second != &notDefined )
            delete it->second;
    deviceSubCategories_.clear();
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DeviceSubCategory constructor
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
DeviceSubCategory::DeviceSubCategory( const std::string& strName, E_Type nType, const std::string& strKey )
    : strName_ ( strName )
    , nType_   ( nType   )
    , strKey_  ( strKey  )
{
}

// -----------------------------------------------------------------------------
// Name: DeviceSubCategory destructor
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
DeviceSubCategory::~DeviceSubCategory()
{
}