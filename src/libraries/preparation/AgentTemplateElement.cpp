// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AgentTemplateElement.h"
#include "AgentsModel.h"

#include "clients_gui/Tools.h"

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/CommandPostAttributes_ABC.h"
#include "clients_kernel/DictionaryExtensions.h"

#include "preparation/ColorController.h"

#include <xeumeuleu/xml.hpp>

#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: AgentTemplateElement constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
AgentTemplateElement::AgentTemplateElement( AgentsModel& agents, const kernel::Agent_ABC& agent )
    : agents_( agents )
    , type_  ( agent.GetType() )
    , cp_    ( tools::IsCommandPost( agent ) )
    , name_  ( agent.GetName() )
{
    const kernel::DictionaryExtensions& extensions = agent.Get< kernel::DictionaryExtensions >();
    if( extensions.IsEnabled() )
        extensions_ = extensions.GetExtensions();
    const kernel::Color_ABC& color = agent.Get< kernel::Color_ABC >();
    if( color.IsOverriden() )
        color_ = color.GetColor();
}

namespace
{
    const kernel::AgentType& ReadType( const tools::Resolver_ABC< kernel::AgentType, std::string >& types, xml::xistream& input )
    {
        return types.Get( input.attribute< std::string >( "agentType" ) );
    }

    void ReadName( xml::xistream& input, QString& name )
    {
        std::string str;
        input >> xml::optional >> xml::attribute( "name", str );
        name = str.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: AgentTemplateElement constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
AgentTemplateElement::AgentTemplateElement( AgentsModel& agents, const tools::Resolver_ABC< kernel::AgentType, std::string >& types, xml::xistream& input )
    : agents_( agents )
    , type_( ReadType( types, input ) )
    , cp_ ( false )
{
    ReadName( input, name_ );
    if( name_.isEmpty() )
        name_ = type_.GetLocalizedName().c_str();
    std::string strColor;
    input >> xml::attribute( "commandPost", cp_ )
          >> xml::optional >> xml::attribute( "color", strColor )
          >> xml::optional >> xml::start( "extensions" )
            >> xml::list( "entry", *this, &AgentTemplateElement::ReadExtension )
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
// Name: AgentTemplateElement destructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
AgentTemplateElement::~AgentTemplateElement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentTemplateElement::Instanciate
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
kernel::Entity_ABC* AgentTemplateElement::Instanciate( kernel::Entity_ABC& superior, const geometry::Point2f& center, ColorController& colorController )
{
    kernel::Automat_ABC* parent = dynamic_cast< kernel::Automat_ABC* >( &superior );
    if( !parent )
        return 0;
    kernel::Agent_ABC& result = agents_.CreateAgent( *parent, type_, center, cp_, name_ );
    if( !extensions_.empty() )
    {
        kernel::DictionaryExtensions& dico = result.Get< kernel::DictionaryExtensions >();
        dico.SetEnabled( true );
        for( std::map< std::string, std::string >::const_iterator it = extensions_.begin(); it != extensions_.end(); ++it )
            dico.SetValue( it->first, it->second );
    }
    if( color_ )
    {
        QColor color( color_->get< 0 >(), color_->get< 1 >(), color_->get< 2 >() );
        colorController.Add( result, color );
    }
    return &result;
}

// -----------------------------------------------------------------------------
// Name: AgentTemplateElement::Serialize
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
void AgentTemplateElement::Serialize( xml::xostream& output )
{
    output << xml::attribute( "type", "agent" )
           << xml::attribute( "agentType", type_.GetKeyName() )
           << xml::attribute( "commandPost", cp_ )
           << xml::attribute( "name", name_ );
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

// -----------------------------------------------------------------------------
// Name: AgentTemplateElement::ReadExtension
// Created: JSR 2012-03-12
// -----------------------------------------------------------------------------
void AgentTemplateElement::ReadExtension( xml::xistream& xis )
{
    extensions_[ xis.attribute< std::string >( "key") ] = xis.attribute< std::string >( "value");
}

// -----------------------------------------------------------------------------
// Name: AgentTemplateElement::IsCompatible
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
bool AgentTemplateElement::IsCompatible( const kernel::Entity_ABC& superior ) const
{
    return superior.GetTypeName() == kernel::Automat_ABC::typeName_;
}

// -----------------------------------------------------------------------------
// Name: AgentTemplateElement::GetName
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
QString AgentTemplateElement::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: AgentTemplateElement::Rename
// Created: JSR 2010-06-03
// -----------------------------------------------------------------------------
void AgentTemplateElement::Rename( const QString& name )
{
    name_ = name;
}
