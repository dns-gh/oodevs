// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "UserProfileFactory.h"
#include "UserProfile.h"

// -----------------------------------------------------------------------------
// Name: UserProfileFactory constructor
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
UserProfileFactory::UserProfileFactory( kernel::Controller& controller, Publisher_ABC& publisher )
    : controller_( controller )
    , publisher_( publisher )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfileFactory destructor
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
UserProfileFactory::~UserProfileFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfileFactory::Create
// Created: JSR 2014-06-06
// -----------------------------------------------------------------------------
kernel::UserProfile_ABC* UserProfileFactory::Create( xml::xistream& /*xis*/ ) const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: UserProfileFactory::Create
// Created: JSR 2014-06-06
// -----------------------------------------------------------------------------
kernel::UserProfile_ABC* UserProfileFactory::Create( const QString& /*name*/ ) const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: UserProfileFactory::Create
// Created: SBO 2007-01-19
// -----------------------------------------------------------------------------
kernel::UserProfile_ABC* UserProfileFactory::Create( const sword::ProfileCreation& message ) const
{
    return new UserProfile( message, controller_, publisher_ );
}

// -----------------------------------------------------------------------------
// Name: UserProfileFactory::Create
// Created: JSR 2014-06-06
// -----------------------------------------------------------------------------
kernel::UserProfile_ABC* UserProfileFactory::Create( kernel::UserProfile_ABC& profile ) const
{
    return new UserProfile( static_cast< UserProfile& >( profile ) );
}

// -----------------------------------------------------------------------------
// Name: UserProfileFactory::Create
// Created: JSR 2014-06-06
// -----------------------------------------------------------------------------
kernel::UserProfile_ABC* UserProfileFactory::Create() const
{
    return new UserProfile( controller_, publisher_ );
}
