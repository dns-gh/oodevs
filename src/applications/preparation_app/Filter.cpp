// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "Filter.h"
#include "moc_Filter_ABC.cpp"

#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    std::string GetCurrentLanguage()
    {
        QSettings settings;
        settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Language", QTextCodec::locale() ).ascii();
    }
}

// -----------------------------------------------------------------------------
// Name: Filter constructor
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
Filter::Filter()
    : currentLanguage_( GetCurrentLanguage() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Filter destructor
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
Filter::~Filter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Filter::GetName
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
const std::string Filter::GetName() const
{
    T_Descriptions::const_iterator it = descriptions_.find( currentLanguage_ );
    if( it != descriptions_.end() )
        return it->second.first;
    else
    {
        it = descriptions_.find( "en" );
        if( it != descriptions_.end() )
            return it->second.first;
    }
    return "";
}

// -----------------------------------------------------------------------------
// Name: Filter::GetDescription
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
const std::string Filter::GetDescription() const
{
    T_Descriptions::const_iterator it = descriptions_.find( currentLanguage_ );
    if( it != descriptions_.end() )
        return it->second.second;
    else
    {
        it = descriptions_.find( "en" );
        if( it != descriptions_.end() )
            return it->second.second;
    }
    return tools::translate( "Filter", "No description available" ).ascii();
}

// -----------------------------------------------------------------------------
// Name: Filter::ReadDescriptions
// Created: ABR 2011-06-21
// -----------------------------------------------------------------------------
void Filter::ReadDescriptions( xml::xistream& xis )
{
    xis >> xml::start( "descriptions" )
            >> xml::list( "description", *this, &Filter::ReadDescription )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: Filter::ReadDescription
// Created: ABR 2011-06-17
// -----------------------------------------------------------------------------
void Filter::ReadDescription( xml::xistream& xis )
{
    assert( ( xis.has_attribute( "xml:lang" ) || xis.has_attribute( "lang" ) ) && xis.has_attribute( "name" ) );
    const std::string lang = xis.has_attribute( "xml:lang" ) ? xis.attribute< std::string >( "xml:lang" ) : xis.attribute< std::string >( "lang" );
    const std::string name = xis.attribute< std::string >( "name" );
    assert( !name.empty() && !lang.empty() );
    std::string description;
    xis >> description;
    descriptions_[ lang ] = std::make_pair( name, description );
}

// -----------------------------------------------------------------------------
// Name: Filter::NeedToReloadExercise
// Created: ABR 2011-06-24
// -----------------------------------------------------------------------------
bool Filter::NeedToReloadExercise() const
{
    return false;
}
