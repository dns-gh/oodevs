// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PreparationProfile.h"

// -----------------------------------------------------------------------------
// Name: PreparationProfile::GetProfile
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
const kernel::Profile_ABC& PreparationProfile::GetProfile()
{
    static PreparationProfile profile;
    return profile;
}

// -----------------------------------------------------------------------------
// Name: PreparationProfile::IsVisible
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
bool PreparationProfile::IsVisible( const kernel::Entity_ABC& ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: PreparationProfile::CanBeOrdered
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
bool PreparationProfile::CanBeOrdered( const kernel::Entity_ABC& ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: PreparationProfile::CanDoMagic
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
bool PreparationProfile::CanDoMagic( const kernel::Entity_ABC& ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: PreparationProfile::IsSupervision
// Created: SBO 2007-06-18
// -----------------------------------------------------------------------------
bool PreparationProfile::IsSupervision() const
{
    return true;
}
