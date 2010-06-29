// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "Profile.h"
#include <xeumeuleu/xml.h>

Profile Profile::Invalid;

// -----------------------------------------------------------------------------
// Name: Profile constructor
// Created: SBO 2010-02-19
// -----------------------------------------------------------------------------
Profile::Profile()
    : valid_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Profile constructor
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
Profile::Profile( xml::xistream& xis )
    : login_( xml::attribute< std::string >( xis, "name" ) )
    , supervisor_( false )
    , valid_( true )
{
    xis >> xml::optional() >> xml::attribute( "supervision", supervisor_ );
}

// -----------------------------------------------------------------------------
// Name: Profile destructor
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
Profile::~Profile()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Profile::GetLogin
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
QString Profile::GetLogin() const
{
    return login_.c_str();
}

// -----------------------------------------------------------------------------
// Name: Profile::IsSupervision
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
bool Profile::IsSupervision() const
{
    return supervisor_;
}

// -----------------------------------------------------------------------------
// Name: Profile::IsValid
// Created: SBO 2010-02-19
// -----------------------------------------------------------------------------
bool Profile::IsValid() const
{
    return valid_;
}
