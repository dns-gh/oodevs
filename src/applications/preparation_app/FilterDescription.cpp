// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "FilterDescription.h"
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
// Name: FilterDescription constructor
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
FilterDescription::FilterDescription()
    : currentLanguage_( ::GetCurrentLanguage() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FilterDescription constructor
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
FilterDescription::FilterDescription( xml::xistream& xis )
    : currentLanguage_( ::GetCurrentLanguage() )
{
    ReadDescriptions( xis );
}

// -----------------------------------------------------------------------------
// Name: FilterDescription destructor
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
FilterDescription::~FilterDescription()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FilterDescription::GetName
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
const std::string FilterDescription::GetName() const
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
// Name: FilterDescription::GetDescription
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
const std::string FilterDescription::GetDescription() const
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
    return tools::translate( "FilterDescription", "No description available" ).ascii();
}

// -----------------------------------------------------------------------------
// Name: FilterDescription::GetCurrentLanguage
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
const std::string FilterDescription::GetCurrentLanguage() const
{
    return currentLanguage_;
}

// -----------------------------------------------------------------------------
// Name: FilterDescription::ReadDescriptions
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
void FilterDescription::ReadDescriptions( xml::xistream& xis )
{
    xis >> xml::optional >> xml::start( "descriptions" )
            >> xml::list( "description", *this, &FilterDescription::ReadDescription )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: FilterDescription::ReadDescription
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
void FilterDescription::ReadDescription( xml::xistream& xis )
{
    assert( ( xis.has_attribute( "xml:lang" ) || xis.has_attribute( "lang" ) ) && xis.has_attribute( "name" ) );
    const std::string lang = xis.has_attribute( "xml:lang" ) ? xis.attribute< std::string >( "xml:lang" ) : xis.attribute< std::string >( "lang" );
    const std::string name = xis.attribute< std::string >( "name" );
    assert( !name.empty() && !lang.empty() );
    std::string description;
    xis >> xml::optional >> description;
    descriptions_[ lang ] = std::make_pair( name, description );
}
