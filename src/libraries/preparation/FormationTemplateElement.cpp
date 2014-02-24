// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "FormationTemplateElement.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/SymbolHierarchy_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "FormationModel.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "preparation/ColorController.h"
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: FormationTemplateElement constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
FormationTemplateElement::FormationTemplateElement( FormationModel& formations, const kernel::Formation_ABC& formation )
    : formations_( formations )
    , levelId_   ( static_cast< unsigned int >( formation.GetLevel() ) )
    , name_      ( formation.GetName() )
{
    const kernel::DictionaryExtensions& extensions = formation.Get< kernel::DictionaryExtensions >();
    if( extensions.IsEnabled() )
        extensions_ = extensions.GetExtensions();
    const kernel::Color_ABC& color = formation.Get< kernel::Color_ABC >();
    if( color.IsOverriden() )
        color_ = color.GetColor();
    const kernel::SymbolHierarchy_ABC& symbol = formation.Get< kernel::SymbolHierarchy_ABC >();
    if( symbol.IsOverriden() )
        symbol_ = symbol.GetValue();
}

// -----------------------------------------------------------------------------
// Name: FormationTemplateElement constructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
FormationTemplateElement::FormationTemplateElement( FormationModel& formations, xml::xistream& input )
    : formations_( formations )
    , levelId_( 0 )
{
    std::string name;
    std::string strColor;
    input >> xml::attribute( "level", levelId_ )
          >> xml::attribute( "name", name )
          >> xml::optional >> xml::attribute( "color", strColor )
          >> xml::optional >> xml::attribute( "symbol", symbol_ )
          >> xml::optional >> xml::start( "extensions" )
            >> xml::list( "entry", *this, &FormationTemplateElement::ReadExtension )
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
    name_ = name.c_str();
}

// -----------------------------------------------------------------------------
// Name: FormationTemplateElement destructor
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
FormationTemplateElement::~FormationTemplateElement()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationTemplateElement::Instanciate
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
kernel::Entity_ABC* FormationTemplateElement::Instanciate( kernel::Entity_ABC& superior, const geometry::Point2f&, ColorController& colorController )
{
    kernel::Formation_ABC* formation = formations_.Create( superior, static_cast< E_NatureLevel >( levelId_ ), name_ );
    if( formation )
    {
        if( !extensions_.empty() )
        {
            kernel::DictionaryExtensions& dico = formation->Get< kernel::DictionaryExtensions >();
            dico.SetEnabled( true );
            for( std::map< std::string, std::string >::const_iterator it = extensions_.begin(); it != extensions_.end(); ++it )
                dico.SetValue( it->first, it->second );
        }
        if( color_ )
        {
            QColor color( color_->get< 0 >(), color_->get< 1 >(), color_->get< 2 >() );
            colorController.Add( *formation, color );
        }
        if( !symbol_.empty() )
        {
            kernel::SymbolHierarchy_ABC& symbol = formation->Get< kernel::SymbolHierarchy_ABC >();
            symbol.OverrideValue( symbol_ );
        }
    }
    return formation;
}

// -----------------------------------------------------------------------------
// Name: FormationTemplateElement::Serialize
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
void FormationTemplateElement::Serialize( xml::xostream& output )
{
    output << xml::attribute( "type", "formation" )
           << xml::attribute( "level", levelId_ );
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
// Name: FormationTemplateElement::ReadExtension
// Created: JSR 2012-03-12
// -----------------------------------------------------------------------------
void FormationTemplateElement::ReadExtension( xml::xistream& xis )
{
    extensions_[ xis.attribute< std::string >( "key") ] = xis.attribute< std::string >( "value");
}

// -----------------------------------------------------------------------------
// Name: FormationTemplateElement::IsCompatible
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
bool FormationTemplateElement::IsCompatible( const kernel::Entity_ABC& superior ) const
{
    const kernel::Formation_ABC* formation = dynamic_cast< const kernel::Formation_ABC* >( &superior );
    return formation && static_cast< unsigned int >( formation->GetLevel() ) > levelId_ ||
           superior.GetTypeName() == kernel::Team_ABC::typeName_;
}

// -----------------------------------------------------------------------------
// Name: FormationTemplateElement::GetName
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
QString FormationTemplateElement::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: FormationTemplateElement::Rename
// Created: JSR 2010-06-03
// -----------------------------------------------------------------------------
void FormationTemplateElement::Rename( const QString& name )
{
    name_ = name;
}
