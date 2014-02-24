// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AutomatTemplateElement.h"
#include "AgentsModel.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/SymbolHierarchy_ABC.h"
#include "clients_gui/EntityType.h"
#include "preparation/ColorController.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
AutomatTemplateElement::AutomatTemplateElement( AgentsModel& agents, const kernel::Automat_ABC& automat )
    : agents_( agents )
    , type_  ( automat.Get< gui::EntityType< kernel::AutomatType > >().GetType() )
    , name_  ( automat.GetName() )
{
    const kernel::DictionaryExtensions& extensions = automat.Get< kernel::DictionaryExtensions >();
    if( extensions.IsEnabled() )
        extensions_ = extensions.GetExtensions();
    const kernel::Color_ABC& color = automat.Get< kernel::Color_ABC >();
    if( color.IsOverriden() )
        color_ = color.GetColor();
    const kernel::SymbolHierarchy_ABC& symbol = automat.Get< kernel::SymbolHierarchy_ABC >();
    if( symbol.IsOverriden() )
        symbol_ = symbol.GetValue();
}

namespace
{
    const kernel::AutomatType& ReadType( const tools::Resolver_ABC< kernel::AutomatType, std::string >& types, xml::xistream& input )
    {
        return types.Get( input.attribute< std::string >( "automatType" ) );
    }

    void ReadName( xml::xistream& input, QString& name )
    {
        std::string str;
        input >> xml::optional >> xml::attribute( "name", str );
        name = str.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
AutomatTemplateElement::AutomatTemplateElement( AgentsModel& agents, const kernel::AutomatType& type, xml::xistream& input )
    : agents_( agents )
    , type_  ( type )
{
    ReadName( input, name_ );
    if( name_.isEmpty() )
        name_ = type_.GetName().c_str();
    std::string strColor;
    input >> xml::optional >> xml::attribute( "color", strColor )
          >> xml::optional >> xml::attribute( "symbol", symbol_ )
          >> xml::optional >> xml::start( "extensions" )
            >> xml::list( "entry", *this, &AutomatTemplateElement::ReadExtension )
          >> xml::end;
    if( !strColor.empty() )
    {
        unsigned int color;
        std::stringstream ss( strColor );
        ss >> std::hex >> color;
        unsigned int red = ( color >> 16 ) & 0xff;
        unsigned int green = ( color >> 8 ) & 0xff;
        unsigned int blue = color & 0xff;
        color_ = boost::tuples::make_tuple( red, green, blue );
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement destructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
AutomatTemplateElement::~AutomatTemplateElement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement::Instanciate
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
kernel::Entity_ABC* AutomatTemplateElement::Instanciate( kernel::Entity_ABC& superior, const geometry::Point2f&, ColorController& colorController )
{
    if( superior.GetTypeName() == kernel::Formation_ABC::typeName_ )
    {
        kernel::Automat_ABC& automat = agents_.CreateAutomat( superior, type_, name_ );
        if( !extensions_.empty() )
        {
            kernel::DictionaryExtensions& dico = automat.Get< kernel::DictionaryExtensions >();
            dico.SetEnabled( true );
            for( std::map< std::string, std::string >::const_iterator it = extensions_.begin(); it != extensions_.end(); ++it )
                dico.SetValue( it->first, it->second );
        }
        if( color_ )
        {
            QColor color( color_->get< 0 >(), color_->get< 1 >(), color_->get< 2 >() );
            colorController.Add( automat, color );
        }
        if( !symbol_.empty() )
        {
            kernel::SymbolHierarchy_ABC& symbol = automat.Get< kernel::SymbolHierarchy_ABC >();
            symbol.OverrideValue( symbol_ );
        }
        return &automat;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement::Serialize
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
void AutomatTemplateElement::Serialize( xml::xostream& output )
{
    output << xml::attribute( "type", "automat" )
           << xml::attribute( "automatType", type_.GetName() )
           << xml::attribute( "name", name_ );
    if( color_ )
    {
        kernel::Color_ABC::T_Color color = *color_;
        std::string strColor = QColor( color.get< 0 >(), color.get< 1 >(), color.get< 2 >() ).name().toStdString();
        boost::replace_all( strColor, "#", "0x" );
        output << xml::attribute( "color", strColor );
    }
    if( !symbol_.empty() )
        output << xml::attribute( "symbol", symbol_ );
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

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement::ReadExtension
// Created: JSR 2012-03-12
// -----------------------------------------------------------------------------
void AutomatTemplateElement::ReadExtension( xml::xistream& xis )
{
    extensions_[ xis.attribute< std::string >( "key") ] = xis.attribute< std::string >( "value");
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement::IsCompatible
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
bool AutomatTemplateElement::IsCompatible( const kernel::Entity_ABC& superior ) const
{
    return superior.GetTypeName() == kernel::Formation_ABC::typeName_;
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement::GetName
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
QString AutomatTemplateElement::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: AutomatTemplateElement::Rename
// Created: JSR 2010-06-03
// -----------------------------------------------------------------------------
void AutomatTemplateElement::Rename( const QString& name )
{
    name_ = name;
}
