// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "GlTools_ABC.h"
#include "Controllers.h"
#include "Options.h"
#include "TristateOption.h"

// -----------------------------------------------------------------------------
// Name: GlTools_ABC constructor
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
GlTools_ABC::GlTools_ABC( Controllers& controllers )
    : controllers_( controllers )
    , selected_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GlTools_ABC destructor
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
GlTools_ABC::~GlTools_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GlTools_ABC::Select
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
void GlTools_ABC::Select( bool dummy )
{
    selected_ = dummy;
}
    
// -----------------------------------------------------------------------------
// Name: GlTools_ABC::ShouldDisplay
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
bool GlTools_ABC::ShouldDisplay( const std::string& name ) const
{
    return ShouldDisplay( name, selected_ );
}

// -----------------------------------------------------------------------------
// Name: GlTools_ABC::ShouldDisplay
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
bool GlTools_ABC::ShouldDisplay( const std::string& name, bool autoCondition ) const
{
    return controllers_.options_.GetOption( name, TristateOption::auto_ )
        .To< TristateOption >().IsSet( autoCondition );
}
