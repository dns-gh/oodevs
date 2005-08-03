// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Platform.inl $
// $Author: Sbo $
// $Modtime: 21/07/05 15:06 $
// $Revision: 15 $
// $Workfile: Platform.inl $
//
// *****************************************************************************

#include "Entities/Types/PawnPlatform.h"

namespace TIC {

//-----------------------------------------------------------------------------
// Name: Platform::GetId
// Created: SBO 2005-05-11
//-----------------------------------------------------------------------------
inline
T_EntityId Platform::GetId() const
{
	return nId_;
}

//-----------------------------------------------------------------------------
// Name: Platform::GetTypeName
// Created: SBO 2005-05-27
//-----------------------------------------------------------------------------
inline
const std::string& Platform::GetTypeName() const
{
    assert( pType_ );
    return pType_->GetType().GetName();
}

//-----------------------------------------------------------------------------
// Name: Platform::GetEMAT6
// Created: SBO 2005-06-02
//-----------------------------------------------------------------------------
inline
const std::string& Platform::GetEMAT6() const
{
    assert( pType_ );
    return pType_->GetType().GetEMAT6();
}

//-----------------------------------------------------------------------------
// Name: Platform::GetEMAT8
// Created: SBO 2005-06-02
//-----------------------------------------------------------------------------
inline
const std::string& Platform::GetEMAT8() const
{
    assert( pType_ );
    return pType_->GetType().GetEMAT8();
}

//-----------------------------------------------------------------------------
// Name: Platform::GetLFRIL
// Created: SBO 2005-06-02
//-----------------------------------------------------------------------------
inline
const std::string& Platform::GetLFRIL() const
{
    assert( pType_ );
    return pType_->GetType().GetLFRIL();
}

//-----------------------------------------------------------------------------
// Name: Platform::GetNNO
// Created: SBO 2005-06-02
//-----------------------------------------------------------------------------
inline
const std::string& Platform::GetNNO() const
{
    assert( pType_ );
    return pType_->GetType().GetNNO();
}

//-----------------------------------------------------------------------------
// Name: Platform::GetProtection
// Created: SBO 2005-06-27
//-----------------------------------------------------------------------------
inline
const std::string& Platform::GetProtection() const
{
    assert( pType_ );
    return pType_->GetType().GetProtection();
}

//-----------------------------------------------------------------------------
// Name: Platform::GetVolume
// Created: SBO 2005-06-27
//-----------------------------------------------------------------------------
inline
const std::string& Platform::GetVolume() const
{
    assert( pType_ );
    return pType_->GetType().GetVolume();
}

//-----------------------------------------------------------------------------
// Name: Platform::IsEmbarquable
// Created: SBO 2005-06-23
//-----------------------------------------------------------------------------
inline
bool Platform::IsEmbarquable() const
{
    assert( pType_ );
    return pType_->IsEmbarquable();
}
// -----------------------------------------------------------------------------
// Name: Platform::GetDevice
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
inline
const Device& Platform::GetDevice() const
{
    assert( pType_ );
    return pType_->GetType().GetDevice();
}

// -----------------------------------------------------------------------------
// Name: Platform::GetDeviceCategory
// Created: SBO 2005-06-30
// -----------------------------------------------------------------------------
inline
const DeviceCategory& Platform::GetDeviceCategory() const
{
    assert( pType_ );
    return pType_->GetType().GetDeviceCategory();
}

// -----------------------------------------------------------------------------
// Name: Platform::GetDeviceSubCategory
// Created: SBO 2005-07-04
// -----------------------------------------------------------------------------
inline
const DeviceSubCategory& Platform::GetDeviceSubCategory() const
{
    assert( pType_ );
    return pType_->GetType().GetDeviceSubCategory();
}

} // end namespace TIC