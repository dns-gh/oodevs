// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PreparationProfile.h"

// -----------------------------------------------------------------------------
// Name: PreparationProfile::GetProfile
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
PreparationProfile& PreparationProfile::GetProfile()
{
    static PreparationProfile profile;
    return profile;
}

// -----------------------------------------------------------------------------
// Name: PreparationProfile::GetLogin
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
const QString& PreparationProfile::GetLogin() const
{
    static const QString login( "Preparation" );
    return login;
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
// Name: PreparationProfile::IsPerceived
// Created: LDC 2013-07-02
// -----------------------------------------------------------------------------
bool PreparationProfile::IsPerceived( const kernel::Entity_ABC& ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: PreparationProfile::IsKnowledgeVisible
// Created: HBD 2010-08-03
// -----------------------------------------------------------------------------
bool PreparationProfile::IsKnowledgeVisible( const kernel::Knowledge_ABC& /*entity*/ ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: PreparationProfile::CanBeOrdered
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
bool PreparationProfile::CanBeOrdered( const kernel::Entity_ABC&, bool ) const
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

// -----------------------------------------------------------------------------
// Name: PreparationProfile::HasTimeControl
// Created: BAX 2014-01-30
// -----------------------------------------------------------------------------
bool PreparationProfile::HasTimeControl() const
{
    return true;
}

QString PreparationProfile::GetFilter() const
{
    return "";
}

void PreparationProfile::SetFilter( const kernel::Entity_ABC&, bool )
{
    // NOTHING
}

void PreparationProfile::SetFilter( const kernel::Profile_ABC&, bool )
{
    // NOTHING
}

void PreparationProfile::RemoveFilter( bool )
{
    // NOTHING
}

const kernel::Entity_ABC* PreparationProfile::GetFilteredEntity() const
{
    return 0;
}

const kernel::Profile_ABC* PreparationProfile::GetFilteredProfile() const
{
    return 0;
}

void PreparationProfile::SetFilter( const gui::GLOptions& /*options*/, bool /*update  = true*/ )
{
    // NOTHING
}
