// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "PropertyFactory.h"
#include "PropertyFactory_ABC.h"
#include "PropertyGroup.h"
#include "ColorFactory.h"
#include "PaintFactory.h"
#include "FillRuleFactory.h"
#include "OpacityFactory.h"
#include "LengthFactory.h"
#include "DashArrayFactory.h"
#include "FontFamilyFactory.h"
#include "FontWeightFactory.h"
#include "FontSizeFactory.h"
#include "TextAnchorFactory.h"
#include "xeumeuleu/xml.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: PropertyFactory constructor
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
PropertyFactory::PropertyFactory()
{
    propertyTypes_.reserve( RenderingContext_ABC::nbrProperty );
    AddType( "color",            RenderingContext_ABC::color,           new ColorFactory() );
    AddType( "fill",             RenderingContext_ABC::fill,            new PaintFactory() );
    AddType( "stroke",           RenderingContext_ABC::stroke,          new PaintFactory() );
    AddType( "fill-rule",        RenderingContext_ABC::fillRule,        new FillRuleFactory() );
    AddType( "fill-opacity",     RenderingContext_ABC::fillOpacity,     new OpacityFactory() );
    AddType( "stroke-opacity",   RenderingContext_ABC::strokeOpacity,   new OpacityFactory() );
    AddType( "stroke-width",     RenderingContext_ABC::strokeWidth,     new LengthFactory() );
    AddType( "stroke-dasharray", RenderingContext_ABC::strokeDasharray, new DashArrayFactory() );
    AddType( "font-family",      RenderingContext_ABC::fontFamily,      new FontFamilyFactory() );
    AddType( "font-weight",      RenderingContext_ABC::fontWeight,      new FontWeightFactory() );
    AddType( "font-size",        RenderingContext_ABC::fontSize,        new FontSizeFactory() );
    AddType( "text-anchor",      RenderingContext_ABC::textAnchor,      new TextAnchorFactory() );
}

// -----------------------------------------------------------------------------
// Name: PropertyFactory destructor
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
PropertyFactory::~PropertyFactory()
{
    for( CIT_PropertyTypes it = propertyTypes_.begin(); it != propertyTypes_.end(); ++it )
        delete it->factory_;
}

// -----------------------------------------------------------------------------
// Name: PropertyFactory::ChangeFactory
// Created: ZEBRE 2007-05-24
// -----------------------------------------------------------------------------
void PropertyFactory::ChangeFactory( RenderingContext_ABC::E_Properties type, std::unique_ptr< PropertyFactory_ABC > factory )
{
    for( IT_PropertyTypes it = propertyTypes_.begin(); it != propertyTypes_.end(); ++it )
        if( it->type_ == type )
        {
            delete it->factory_;
            it->factory_ = factory.release();
            return;
        }
}

namespace
{
    class FactoryWrapper : public PropertyFactory_ABC
    {
    public:
        FactoryWrapper( const PropertyFactory_ABC& forward )
            : forward_( &forward ) {}
        virtual Property_ABC* CreateProperty( const std::string& content ) const
        {
            return forward_->CreateProperty( content );
        }
    private:
         const PropertyFactory_ABC* forward_;
    };
}

// -----------------------------------------------------------------------------
// Name: PropertyFactory::ChangeFactory
// Created: AGE 2007-10-31
// -----------------------------------------------------------------------------
void PropertyFactory::ChangeFactory( RenderingContext_ABC::E_Properties type, const PropertyFactory_ABC& factory )
{
    ChangeFactory( type, std::unique_ptr< PropertyFactory_ABC >( new FactoryWrapper( factory ) ) );
}

// -----------------------------------------------------------------------------
// Name: PropertyFactory::AddType
// Created: AGE 2006-09-11
// -----------------------------------------------------------------------------
void PropertyFactory::AddType( const std::string& name, RenderingContext_ABC::E_Properties type, const PropertyFactory_ABC* factory )
{
    propertyTypes_.push_back( PropertyType() );
    propertyTypes_.back().name_    = name;
    propertyTypes_.back().type_    = type;
    propertyTypes_.back().factory_ = factory;
}

namespace
{
    void split( const std::string& value, const std::string& separator, std::vector< std::string >& where )
    {
        std::string::size_type pos = value.find_first_of( separator );
        if( pos != std::string::npos )
        {
            const std::string head = value.substr( 0, pos );
            const std::string tail = value.substr( pos + 1, value.size() - pos - 1 );
            where.push_back( head );
            split( tail, separator, where );
        }
        else
            where.push_back( value );
    }
}

// -----------------------------------------------------------------------------
// Name: PropertyFactory::CreateFromCss
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
void PropertyFactory::CreateFromCss( PropertyGroup& group, xml::xistream& input ) const
{
    CreateProperties( group, xml::attribute( input, "style", std::string() ) );
}

// -----------------------------------------------------------------------------
// Name: PropertyFactory::CreateProperties
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
void PropertyFactory::CreateProperties( PropertyGroup& group, const std::string& css ) const
{
    std::vector< std::string > properties;
    split( css, ";", properties );
    for( std::vector< std::string >::const_iterator it = properties.begin(); it != properties.end(); ++it )
    {
        const std::string& property = *it;
        std::vector< std::string > keyvalue;
        split( property, ":", keyvalue );
        if( keyvalue.size() == 2 )
        {
            const std::string& key   = keyvalue.front();
            const std::string& value = keyvalue.back();
            for( CIT_PropertyTypes it = propertyTypes_.begin(); it != propertyTypes_.end(); ++it )
            {
                if( it->name_ == key )
                {
                    Property_ABC* property = it->factory_->CreateProperty( value );
                    if( property )
                        group.Add( it->type_, *property );
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PropertyFactory::CreateProperties
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
void PropertyFactory::CreateProperties( PropertyGroup& group, xml::xistream& input ) const
{
    CreateFromCss( group, input );

    for( CIT_PropertyTypes it = propertyTypes_.begin(); it != propertyTypes_.end(); ++it )
    {
        const std::string& propertyName = it->name_;
        std::string content;
        input >> xml::optional() >> xml::attribute( propertyName, content );
        Property_ABC* property = it->factory_->CreateProperty( content );
        if( property )
            group.Add( it->type_, *property  );
    }
}
