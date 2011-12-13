// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ExerciseSettings.h"
#include "Tools.h"
#include "tools/Loader_ABC.h"
#include "tools/SchemaWriter_ABC.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// =============================================================================
// Setting
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Setting constructor
// Created: ABR 2011-12-09
// -----------------------------------------------------------------------------
ExerciseSettings::Setting::Setting( xml::xistream& xis )
    : name_( xis.attribute< std::string >( "name", "invalid" ) )
    , type_( xis.attribute< std::string >( "type", "invalid" ) )
{
    if( type_ == "bool" )
        value_ = xis.attribute< bool >( "value" );
    else if( type_ == "string" )
        value_ = xis.attribute< std::string >( "value" );
    else if( type_ == "int" )
        value_ = xis.attribute< int >( "value" );
    else if( type_ == "unsigned int" )
        value_ = xis.attribute< unsigned int >( "value" );
    else if( type_ == "long" )
        value_ = xis.attribute< long >( "value" );
    else if( type_ == "unsigned long" )
        value_ = xis.attribute< unsigned long >( "value" );
    else if( type_ == "float" )
        value_ = xis.attribute< float >( "value" );
    else if( type_ == "double" )
        value_ = xis.attribute< double >( "value" );
}

// -----------------------------------------------------------------------------
// Name: ExerciseSettings::~Setting
// Created: ABR 2011-12-09
// -----------------------------------------------------------------------------
ExerciseSettings::Setting::~Setting()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExerciseSettings::Serialize
// Created: ABR 2011-12-09
// -----------------------------------------------------------------------------
void ExerciseSettings::Setting::Serialize( xml::xofstream& xos ) const
{
    xos << xml::start( "setting" )
        << xml::attribute( "name", name_ )
        << xml::attribute( "type", type_ );

    if( type_ == "bool" )
        xos << xml::attribute( "value",  boost::get< bool >( value_ ) );
    else if( type_ == "string" )
        xos << xml::attribute( "value",  boost::get< std::string >( value_ ) );
    else if( type_ == "int" )
        xos << xml::attribute( "value",  boost::get< int >( value_ ) );
    else if( type_ == "unsigned int" )
        xos << xml::attribute( "value",  boost::get< unsigned int >( value_ ) );
    else if( type_ == "long" )
        xos << xml::attribute( "value",  boost::get< long >( value_ ) );
    else if( type_ == "unsigned long" )
        xos << xml::attribute( "value",  boost::get< unsigned long >( value_ ) );
    else if( type_ == "float" )
        xos << xml::attribute( "value",  boost::get< float >( value_ ) );
    else if( type_ == "double" )
        xos << xml::attribute( "value",  boost::get< double >( value_ ) );
    xos << xml::end; // settings
}

// =============================================================================
// ExerciseSettings
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ExerciseSettings constructor
// Created: ABR 2011-12-09
// -----------------------------------------------------------------------------
ExerciseSettings::ExerciseSettings()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExerciseSettings destructor
// Created: ABR 2011-12-09
// -----------------------------------------------------------------------------
ExerciseSettings::~ExerciseSettings()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: ExerciseSettings::Purge
// Created: ABR 2011-12-09
// -----------------------------------------------------------------------------
void ExerciseSettings::Purge()
{
    for( IT_Settings it = settings_.begin(); it != settings_.end(); ++it )
    {
        delete it->second;
        it->second = 0;
    }
    settings_.clear();
}

// -----------------------------------------------------------------------------
// Name: ExerciseSettings::Load
// Created: ABR 2011-12-12
// -----------------------------------------------------------------------------
void ExerciseSettings::Load( xml::xistream& xis )
{
    xis >> xml::start( "settings" )
            >> xml::list( "setting", *this, &ExerciseSettings::ReadSetting )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ExerciseSettings::ReadSetting
// Created: ABR 2011-12-09
// -----------------------------------------------------------------------------
void ExerciseSettings::ReadSetting( xml::xistream& xis )
{
    std::string name = xis.attribute< std::string >( "name", "" );
    assert( !name.empty() );
    settings_[ name ] = new Setting( xis );
}

// -----------------------------------------------------------------------------
// Name: ExerciseSettings::Serialize
// Created: ABR 2011-12-09
// -----------------------------------------------------------------------------
void ExerciseSettings::Serialize( const std::string& filename, const tools::SchemaWriter_ABC& schemaWriter ) const
{
    xml::xofstream xos( filename );
    xos << xml::start( "settings" );
    schemaWriter.WriteExerciseSchema( xos, "settings" );
    for( CIT_Settings it = settings_.begin(); it != settings_.end(); ++it )
        it->second->Serialize( xos );
    xos << xml::end;
}
