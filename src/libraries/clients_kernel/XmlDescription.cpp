// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "XmlDescription.h"

#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

//namespace
//{
//    std::string GetCurrentLanguage()
//    {
//        QSettings settings;
//        settings.setPath( "MASA Group", tools::translate( "Application", "SWORD" ) );
//        return settings.readEntry( "/Common/Language", QTextCodec::locale() ).ascii();
//    }
//}
//
// -----------------------------------------------------------------------------
// Name: XmlDescription constructor
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
XmlDescription::XmlDescription( const std::string& currentLanguage )
    : currentLanguage_( currentLanguage )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: XmlDescription constructor
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
XmlDescription::XmlDescription( xml::xistream& xis, const std::string& currentLanguage )
    : currentLanguage_( currentLanguage )
{
    ReadDescriptions( xis );
}

// -----------------------------------------------------------------------------
// Name: XmlDescription destructor
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
XmlDescription::~XmlDescription()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: XmlDescription::SetCurrentLanguage
// Created: ABR 2011-11-10
// -----------------------------------------------------------------------------
void XmlDescription::SetCurrentLanguage( const std::string& currentLanguage )
{
    currentLanguage_ = currentLanguage;
}

// -----------------------------------------------------------------------------
// Name: XmlDescription::GetName
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
const std::string XmlDescription::GetName() const
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
// Name: XmlDescription::GetDescription
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
const std::string XmlDescription::GetDescription() const
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
    return tools::translate( "XmlDescription", "No description available" ).ascii();
}

// -----------------------------------------------------------------------------
// Name: XmlDescription::GetCurrentLanguage
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
const std::string XmlDescription::GetCurrentLanguage() const
{
    return currentLanguage_;
}

// -----------------------------------------------------------------------------
// Name: XmlDescription::ReadDescriptions
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
void XmlDescription::ReadDescriptions( xml::xistream& xis )
{
    xis >> xml::optional >> xml::start( "descriptions" )
        >> xml::list( "description", *this, &XmlDescription::ReadDescription )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: XmlDescription::ReadDescription
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
void XmlDescription::ReadDescription( xml::xistream& xis )
{
    assert( ( xis.has_attribute( "xml:lang" ) || xis.has_attribute( "lang" ) ) && xis.has_attribute( "name" ) );
    const std::string lang = xis.has_attribute( "xml:lang" ) ? xis.attribute< std::string >( "xml:lang" ) : xis.attribute< std::string >( "lang" );
    const std::string name = xis.attribute< std::string >( "name" );
    assert( !name.empty() && !lang.empty() );
    std::string description;
    xis >> xml::optional >> description;
    descriptions_[ lang ] = std::make_pair( name, description );
}
