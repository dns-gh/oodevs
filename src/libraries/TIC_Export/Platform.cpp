// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-18 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Export/Platform.cpp $
// $Author: Sbo $
// $Modtime: 19/07/05 16:36 $
// $Revision: 8 $
// $Workfile: Platform.cpp $
//
// *****************************************************************************

#include "TIC/Types.h"
#include "Platform.h"
#include "TIC/Entities/Platform.h"
#include "TIC/Entities/Natures/Platform/Device.h"
#include "TIC/Entities/Natures/Platform/DeviceCategory.h"
#include "TIC/Entities/Natures/Platform/DeviceSubCategory.h"

using namespace TICExport;


namespace TICExport
{

//-----------------------------------------------------------------------------
// Name: Platform::Platform
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
Platform::Platform()
: pPlatform_ ( 0 )
{
}

//-----------------------------------------------------------------------------
// Name: Platform::Platform
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
Platform::Platform( const TIC::Platform& platform )
: pPlatform_ ( &platform )
{
}

//-----------------------------------------------------------------------------
// Name: Platform::~Platform
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
Platform::~Platform()
{
}

//-----------------------------------------------------------------------------
// Name: Platform::GetId
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
unsigned int Platform::GetId() const
{
    assert( pPlatform_ );
    return pPlatform_->GetId();
}

//-----------------------------------------------------------------------------
// Name: Platform::GetTypeName
// Created: SBO 2005-05-19
//-----------------------------------------------------------------------------
const std::string& Platform::GetTypeName() const
{
    assert( pPlatform_ );
    return pPlatform_->GetTypeName();
}

//-----------------------------------------------------------------------------
// Name: Platform::GetTypeEMAT6
// Created: SBO 2005-06-02
//-----------------------------------------------------------------------------
const std::string& Platform::GetTypeEMAT6() const
{
    assert( pPlatform_ );
    return pPlatform_->GetEMAT6();
}

//-----------------------------------------------------------------------------
// Name: Platform::GetTypeEMAT8
// Created: SBO 2005-06-02
//-----------------------------------------------------------------------------
const std::string& Platform::GetTypeEMAT8() const
{
    assert( pPlatform_ );
    return pPlatform_->GetEMAT8();
}

//-----------------------------------------------------------------------------
// Name: Platform::GetTypeLFRIL
// Created: SBO 2005-06-02
//-----------------------------------------------------------------------------
const std::string& Platform::GetTypeLFRIL() const
{
    assert( pPlatform_ );
    return pPlatform_->GetLFRIL();
}

//-----------------------------------------------------------------------------
// Name: Platform::GetTypeNNO
// Created: SBO 2005-06-02
//-----------------------------------------------------------------------------
const std::string& Platform::GetTypeNNO() const
{
    assert( pPlatform_ );
    return pPlatform_->GetNNO();
}

//-----------------------------------------------------------------------------
// Name: Platform::GetTypeProtection
// Created: SBO 2005-06-27
//-----------------------------------------------------------------------------
const std::string& Platform::GetTypeProtection() const
{
    assert( pPlatform_ );
    return pPlatform_->GetProtection();
}

//-----------------------------------------------------------------------------
// Name: Platform::GetTypeVolume
// Created: SBO 2005-06-27
//-----------------------------------------------------------------------------
const std::string& Platform::GetTypeVolume() const
{
    assert( pPlatform_ );
    return pPlatform_->GetVolume();
}

//-----------------------------------------------------------------------------
// Name: Platform::GetDirection
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
int Platform::GetDirection() const
{
    assert( pPlatform_ );
    return pPlatform_->GetDirection();
}

//-----------------------------------------------------------------------------
// Name: Platform::GetSpeed
// Created: SBO 2005-05-18
//-----------------------------------------------------------------------------
unsigned int Platform::GetSpeed() const
{
    assert( pPlatform_ );
    return ( unsigned int )pPlatform_->GetSpeed();
}

//-----------------------------------------------------------------------------
// Name: Platform::GetLatitude
// Created: SBO 2005-06-06
//-----------------------------------------------------------------------------
double Platform::GetLatitude() const
{
    assert( pPlatform_ );
    return pPlatform_->GetPosition().GetLatitude();
}

//-----------------------------------------------------------------------------
// Name: Platform::GetLongitude
// Created: SBO 2005-06-06
//-----------------------------------------------------------------------------
double Platform::GetLongitude() const
{
    assert( pPlatform_ );
    return pPlatform_->GetPosition().GetLongitude();
}

//-----------------------------------------------------------------------------
// Name: Platform::GetHeight
// Created: SBO 2005-06-03
//-----------------------------------------------------------------------------
int Platform::GetHeight() const
{
    assert( pPlatform_ );
    return pPlatform_->GetHeight();
}

//-----------------------------------------------------------------------------
// Name: Platform::GetDevice
// Created: SBO 2005-06-30
//-----------------------------------------------------------------------------
const std::string& Platform::GetDevice() const
{
    assert( pPlatform_ );
    return pPlatform_->GetDevice().GetKey();
}

//-----------------------------------------------------------------------------
// Name: Platform::GetDeviceCategory
// Created: SBO 2005-06-30
//-----------------------------------------------------------------------------
const std::string& Platform::GetDeviceCategory() const
{
    assert( pPlatform_ );
    return pPlatform_->GetDeviceCategory().GetKey();
}

//-----------------------------------------------------------------------------
// Name: Platform::GetDeviceSubCategory
// Created: SBO 2005-07-04
//-----------------------------------------------------------------------------
const std::string& Platform::GetDeviceSubCategory() const
{
    assert( pPlatform_ );
    return pPlatform_->GetDeviceSubCategory().GetKey();
}

} // namespace TICExport