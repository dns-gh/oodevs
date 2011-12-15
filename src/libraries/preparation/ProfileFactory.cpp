// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ProfileFactory.h"
#include "UserProfile.h"
#include "Model.h"

// -----------------------------------------------------------------------------
// Name: ProfileFactory constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
ProfileFactory::ProfileFactory( kernel::Controller& controller, const Model& model, const kernel::ExtensionTypes& extensions )
    : controller_( controller )
    , model_     ( model )
    , extensions_( extensions )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfileFactory destructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
ProfileFactory::~ProfileFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfileFactory::Create
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfile* ProfileFactory::Create( xml::xistream& xis ) const
{
    return new UserProfile( xis, controller_, model_, extensions_ );
}

// -----------------------------------------------------------------------------
// Name: ProfileFactory::Create
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfile* ProfileFactory::Create( const QString& name ) const
{
    return new UserProfile( name, controller_, model_, extensions_ );
}

// -----------------------------------------------------------------------------
// Name: ProfileFactory::Create
// Created: LGY 2011-12-12
// -----------------------------------------------------------------------------
UserProfile* ProfileFactory::Create( const QString& name, const std::string& role ) const
{
    return new UserProfile( name, role, controller_, model_, extensions_ );
}
