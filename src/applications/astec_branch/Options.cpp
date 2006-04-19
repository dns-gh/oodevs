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
#include "Observer_ABC.h"
#include "OptionVariant.h"
#include <algorithm>
#include "Settings.h"
#include "TristateOption.h"

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
void Options::Register( Observer_ABC& o )
{
    OptionsObserver_ABC* observer = dynamic_cast< OptionsObserver_ABC* >( &o );
    if( observer )
    {
        observers_.push_back( observer );
        for( CIT_Options it = options_.begin(); it != options_.end(); ++it )
            observer->OptionChanged( it->first, it->second );
    }
}

// -----------------------------------------------------------------------------
// Name: Options::Remove
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Options::Remove( Observer_ABC& o )
{
    OptionsObserver_ABC* observer = dynamic_cast< OptionsObserver_ABC* >( &o );
    T_Observers::iterator it = std::find( observers_.begin(), observers_.end(), observer );
    if( it != observers_.end() )
        observers_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: Options::Change
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Options::Change( const std::string& name, const OptionVariant& value )
{
    options_[ name ] = value;
    for( unsigned i = 0; i < observers_.size(); ++i )
        observers_.at( i )->OptionChanged( name, value );
}

// -----------------------------------------------------------------------------
// Name: Options::GetOption
// Created: AGE 2006-03-30
// -----------------------------------------------------------------------------
const OptionVariant& Options::GetOption( const std::string& name, const OptionVariant& defaultValue )
{
    if( options_.find( name ) == options_.end() )
        options_[ name ] = defaultValue;
    return options_[ name ];
}

// -----------------------------------------------------------------------------
// Name: Options::Load
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
template< typename T >
void Options::Load( Settings& settings, const std::string& name, T defaultValue )
{
    Change( name, OptionVariant( settings, name, defaultValue ) );
}

// -----------------------------------------------------------------------------
// Name: Options::Load
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
void Options::Load( Settings& settings )
{
    // $$$$ AGE 2006-04-19: Sauvegarder le type de l'option puis recharger la liste.
    Load( settings, "BigText", TristateOption::auto_ );
    Load( settings, "DebugPoints", TristateOption::auto_ );
    Load( settings, "DrawHoveredInfos", true );
    Load( settings, "DrawIdentifications", false );
    Load( settings, "DrawMessages", false );
    Load( settings, "DrawObjectsIcons", true );
    Load( settings, "DrawRCs", true );
    Load( settings, "DrawSubscribedRCsOnly", false );
    Load( settings, "DrawTraces", false );
    Load( settings, "FontSize", 10 );
    Load( settings, "GridSize", 10.0f );
    Load( settings, "LogisticLinks", TristateOption::auto_ );
    Load( settings, "MissingLogisticLinks", TristateOption::auto_ );
    Load( settings, "OldPaths", TristateOption::auto_ );
    Load( settings, "Paths", TristateOption::auto_ );
    Load( settings, "RealTimeLogistic", TristateOption::auto_ );
    Load( settings, "SmallText", TristateOption::auto_ );
    Load( settings, "TacticalLines", TristateOption::auto_ );
    Load( settings, "VisionCones", TristateOption::auto_ );
    Load( settings, "VisionLines", TristateOption::auto_ );
    Load( settings, "VisionSurfaces", TristateOption::auto_ );
}

// -----------------------------------------------------------------------------
// Name: Options::Save
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
void Options::Save( Settings& settings )
{
    for( CIT_Options it = options_.begin(); it != options_.end(); ++it )
        it->second.Save( settings, it->first );
}