// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TemplateElement_ABC.h"
#include "ColorController.h"

#include "clients_kernel/Color_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/DictionaryExtensions.h"

#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

namespace
{
    boost::optional< boost::tuple< unsigned int, unsigned int, unsigned int > > GetColor( const kernel::Entity_ABC& entity )
    {
        boost::optional< boost::tuple< unsigned int, unsigned int, unsigned int > > result;
        const kernel::Color_ABC& color = entity.Get< kernel::Color_ABC >();
        if( color.IsOverriden() )
            result = color.GetColor();
        return result;
    }
    boost::optional< boost::tuple< unsigned int, unsigned int, unsigned int > > GetColor( xml::xistream& xis  )
    {
        boost::optional< boost::tuple< unsigned int, unsigned int, unsigned int > > result;
        const std::string strColor = xis.attribute( "color", "" );
        if( !strColor.empty() )
        {
            unsigned int color;
            std::stringstream ss( strColor );
            ss >> std::hex >> color;
            unsigned int red = ( color >> 16 ) & 0xff;
            unsigned int green = ( color >> 8 ) & 0xff;
            unsigned int blue = color & 0xff;
            result = boost::tuples::make_tuple( red, green, blue );
        }
        return result;
    }
    std::map< std::string, std::string > GetExtensions( const kernel::Entity_ABC& entity )
    {
        std::map< std::string, std::string > result;
        if( const kernel::DictionaryExtensions* extensions = entity.Retrieve< kernel::DictionaryExtensions >() )
            if( extensions->IsEnabled() )
                result = extensions->GetExtensions();
        return result;
    }
    std::map< std::string, std::string > GetExtensions( xml::xistream& xis )
    {
        std::map< std::string, std::string > result;
        xis >> xml::optional >> xml::start( "extensions" )
                >> xml::list( "entry", [&] ( xml::xistream& xis )
                {
                    result[ xis.attribute< std::string >( "key") ] = xis.attribute< std::string >( "value");
                } )
            >> xml::end;
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: TemplateElement_ABC constructor
// Created: ABR 2014-02-24
// -----------------------------------------------------------------------------
TemplateElement_ABC::TemplateElement_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TemplateElement_ABC constructor
// Created: ABR 2014-02-24
// -----------------------------------------------------------------------------
TemplateElement_ABC::TemplateElement_ABC( const kernel::Entity_ABC& entity )
    : name_( entity.GetName() )
    , color_( GetColor( entity ) )
    , extensions_( GetExtensions( entity ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TemplateElement_ABC constructor
// Created: ABR 2014-02-24
// -----------------------------------------------------------------------------
TemplateElement_ABC::TemplateElement_ABC( xml::xistream& xis )
    : name_( QString::fromStdString( xis.attribute( "name", "" ) ) )
    , color_( GetColor( xis ) )
    , extensions_( GetExtensions( xis ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TemplateElement_ABC destructor
// Created: ABR 2014-02-24
// -----------------------------------------------------------------------------
TemplateElement_ABC::~TemplateElement_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TemplateElement_ABC::GetName
// Created: ABR 2014-02-24
// -----------------------------------------------------------------------------
QString TemplateElement_ABC::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: TemplateElement_ABC::Rename
// Created: ABR 2014-02-24
// -----------------------------------------------------------------------------
void TemplateElement_ABC::Rename( const QString& name )
{
    name_ = name;
}

// -----------------------------------------------------------------------------
// Name: TemplateElement_ABC::SetColor
// Created: ABR 2014-02-25
// -----------------------------------------------------------------------------
void TemplateElement_ABC::SetColor( kernel::Entity_ABC& entity,
                                    ColorController& colorController )
{
    if( color_ )
    {
        QColor color( color_->get< 0 >(), color_->get< 1 >(), color_->get< 2 >() );
        colorController.Add( entity, color );
    }
}

// -----------------------------------------------------------------------------
// Name: TemplateElement_ABC::SetExtensions
// Created: ABR 2014-02-25
// -----------------------------------------------------------------------------
void TemplateElement_ABC::SetExtensions( kernel::Entity_ABC& entity )
{
    if( !extensions_.empty() )
    {
        kernel::DictionaryExtensions& dico = entity.Get< kernel::DictionaryExtensions >();
        dico.SetEnabled( true );
        for( auto it = extensions_.begin(); it != extensions_.end(); ++it )
            dico.SetValue( it->first, it->second );
    }
}

// -----------------------------------------------------------------------------
// Name: TemplateElement_ABC::Serialize
// Created: ABR 2014-02-24
// -----------------------------------------------------------------------------
void TemplateElement_ABC::Serialize( xml::xostream& output ) const
{
    output << xml::attribute( "name", name_.toStdString() );
    if( color_ )
    {
        kernel::Color_ABC::T_Color color = *color_;
        std::string strColor = QColor( color.get< 0 >(), color.get< 1 >(), color.get< 2 >() ).name().toStdString();
        boost::replace_all( strColor, "#", "0x" );
        output << xml::attribute( "color", strColor );
    }
    if( !extensions_.empty() )
    {
        output << xml::start( "extensions" );
        for( std::map< std::string, std::string >::const_iterator it = extensions_.begin(); it != extensions_.end(); ++it )
            output << xml::start( "entry" )
            << xml::attribute( "key", it->first )
            << xml::attribute( "value", it->second )
            << xml::end;
        output << xml::end;
    }
}
