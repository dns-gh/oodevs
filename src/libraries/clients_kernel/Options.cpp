// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Options.h"
#include "OptionsObserver_ABC.h"
#include "Observer_ABC.h"
#include "OptionVariant.h"
#include "TristateOption.h"
#include "FourStateOption.h"
#include "Settings_ABC.h"
#include <algorithm>
#include <qstringlist.h>

using namespace kernel;

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
void Options::Load( Settings_ABC& settings, const std::string& name, T defaultValue )
{
    Change( name, OptionVariant( settings, name, defaultValue ) );
}

// -----------------------------------------------------------------------------
// Name: Options::Load
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
void Options::Load( Settings_ABC& settings, const std::string& path /* = "" */)
{
    const std::string root = QString( path.c_str() ).endsWith( "/" ) ? path : path + "/";
    QStringList list = settings.EntryList( root.c_str() );
    for( QStringList::const_iterator it = list.begin(); it != list.end(); ++it )
    {
        const std::string typedName = (*it).ascii();
        if( ! typedName.empty() )
        {
            char type = typedName[0];
            std::string name = root + typedName.substr( 1 );
            name = name[0] == '/' ? name.substr( 1 ) : name;
            if( type == Settings_ABC::intPrefix )
                Load( settings, name, 1 );
            else if( type == Settings_ABC::boolPrefix )
                Load( settings, name, false );
            else if( type == Settings_ABC::floatPrefix )
                Load( settings, name, 1.f );
            else if( type == Settings_ABC::tristatePrefix )
                Load( settings, name, TristateOption::Auto() );
            else if( type == Settings_ABC::fourstatePrefix )
                Load( settings, name, FourStateOption::Selected() );
            else if( type == Settings_ABC::stringPrefix )
                Load( settings, name, QString( "" ) );
        }
    }
    list = settings.SubEntriesList( root.c_str() );
    for( QStringList::const_iterator it = list.begin(); it != list.end(); ++it )
        Load( settings, ( root + (*it).ascii() ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: Options::Save
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
void Options::Save( Settings_ABC& settings )
{
    for( CIT_Options it = options_.begin(); it != options_.end(); ++it )
        it->second.Save( settings, it->first );
}