// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Options.h"
#include "OptionsObserver_ABC.h"
#include "OptionVariant.h"
#include <algorithm>

// -----------------------------------------------------------------------------
// Name: Options constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Options::Options()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: Options destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Options::~Options()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Options::Register
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Options::Register( OptionsObserver_ABC& observer )
{
    observers_.push_back( &observer );
    for( CIT_Options it = options_.begin(); it != options_.end(); ++it )
        observer.OptionChanged( it->first, it->second );
}

// -----------------------------------------------------------------------------
// Name: Options::Remove
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Options::Remove( OptionsObserver_ABC& observer )
{
    observers_.erase( std::find( observers_.begin(), observers_.end(), &observer ) );
}

// -----------------------------------------------------------------------------
// Name: Options::Change
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Options::Change( const std::string& name, const OptionVariant& value )
{
    options_[ name ] = value;
    for( CIT_Observers it = observers_.begin(); it != observers_.end(); ++it )
        (*it)->OptionChanged( name, value );
}
