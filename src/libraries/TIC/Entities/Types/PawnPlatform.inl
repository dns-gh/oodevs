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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Types/PawnPlatform.inl $
// $Author: Sbo $
// $Modtime: 24/06/05 17:19 $
// $Revision: 1 $
// $Workfile: PawnPlatform.inl $
//
// *****************************************************************************

#include "PlatformType.h"

namespace TIC
{


// -----------------------------------------------------------------------------
// Name: PawnPlatform::GetType
// Created: SBO 2005-06-24
// -----------------------------------------------------------------------------
inline
const PlatformType& PawnPlatform::GetType() const
{
    assert( pType_ );
    return *pType_;
}

// -----------------------------------------------------------------------------
// Name: PawnPlatform::IsEmbarquable
// Created: SBO 2005-06-24
// -----------------------------------------------------------------------------
inline
bool PawnPlatform::IsEmbarquable() const
{
    return bIsEmbarquable_;
}

} // end namespace TIC