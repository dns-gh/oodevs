// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Platform/DeviceCategory.cpp $
// $Author: Sbo $
// $Modtime: 13/07/05 15:05 $
// $Revision: 3 $
// $Workfile: DeviceCategory.cpp $
//
// *****************************************************************************

#include "TIC_Pch.h"

#include "DeviceCategory.h"

using namespace TIC;

DeviceCategory::T_DeviceCategoryMap DeviceCategory::deviceCategories_;
const DeviceCategory                DeviceCategory::notDefined( "None", eNone, "NONE" );

// =============================================================================
// MANAGER
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DeviceCategory::Initialize
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
void DeviceCategory::Initialize()
{
    deviceCategories_[ notDefined.GetName() ] = &notDefined;
}

// -----------------------------------------------------------------------------
// Name: DeviceCategory::Terminate
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
void DeviceCategory::Terminate()
{
    for( CIT_DeviceCategoryMap it = deviceCategories_.begin(); it != deviceCategories_.end(); ++it )
        if( it->second != &notDefined )
            delete it->second;
    deviceCategories_.clear();
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DeviceCategory constructor
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
DeviceCategory::DeviceCategory( const std::string& strName, E_Type nType, const std::string& strKey )
    : strName_ ( strName )
    , nType_   ( nType   )
    , strKey_  ( strKey  )
{
}

// -----------------------------------------------------------------------------
// Name: DeviceCategory destructor
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
DeviceCategory::~DeviceCategory()
{
}