// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "SVGFactory.h"
#include "SvgElement.h"
#include "Group.h"
#include "Path.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Polygon.h"
#include "PolyLine.h"
#include "Line.h"
#include "Ellipse.h"
#include "Definitions.h"
#include "Use.h"
#include "References.h"
#include "RenderingContext.h"
#include "Text.h"
#include "PropertyFactory.h"
#include "PropertyFactory_ABC.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: SVGFactory constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
SVGFactory::SVGFactory( TextRenderer& renderer )
    : renderer_( renderer )
    , factory_ ( new PropertyFactory() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SVGFactory destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
SVGFactory::~SVGFactory()
{
    // NOTHING
}

namespace
{
    struct DummyNode : public Container
    {
        DummyNode( xml::xistream& input, const SVGFactory& factory, References_ABC& references, const PropertyFactory& propertyFactory ) 
            : Container( input, factory, references, propertyFactory ) {};
        DummyNode( const DummyNode& rhs ) 
            : Container( rhs ) {};
        virtual Node& CompileInternal( RenderingContext_ABC& context, References_ABC& references ) const { return CompileChildren( *new DummyNode( *this ), context, references ); }
        virtual void DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const { DrawChildren( context, references ); }
    };
}

// -----------------------------------------------------------------------------
// Name: SVGFactory::Compile
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
svg::Node_ABC* SVGFactory::Compile( const std::string& filename, svg::References_ABC& references, float expectedPrecision ) const
{
    xml::xifstream input( filename );
    return Compile( input, references, expectedPrecision );
}

// -----------------------------------------------------------------------------
// Name: SVGFactory::Compile
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
svg::Node_ABC* SVGFactory::Compile( xml::xistream& input, svg::References_ABC& references, float expectedPrecision ) const
{
    References temporary;
    std::unique_ptr< svg::Node_ABC > base( Parse( input, temporary ) );
    RenderingContext context( expectedPrecision );
    return & base->Compile( context, references );
}

// -----------------------------------------------------------------------------
// Name: SVGFactory::Parse
// Created: AGE 2006-08-30
// -----------------------------------------------------------------------------
svg::Node_ABC* SVGFactory::Parse( const std::string& filename, svg::References_ABC& references ) const
{
    xml::xifstream input( filename );
    return Parse( input, references );
}

// -----------------------------------------------------------------------------
// Name: SVGFactory::Parse
// Created: AGE 2006-08-31
// -----------------------------------------------------------------------------
svg::Node_ABC* SVGFactory::Parse( xml::xistream& input, svg::References_ABC& references ) const
{
    return new DummyNode( input, *this, references, *factory_ );
}

// -----------------------------------------------------------------------------
// Name: SVGFactory::Create
// Created: AGE 2006-08-30
// -----------------------------------------------------------------------------
svg::Node_ABC* SVGFactory::Create( const std::string& nodeName, xml::xistream& input, svg::References_ABC& references ) const
{
    if( nodeName == "svg" )
        return new SvgElement( input, *this, references, *factory_ );
    if( nodeName == "g" )
        return new Group( input, *this, references, *factory_ );
    if( nodeName == "path" )
        return new Path( input, references, *factory_ );
    if( nodeName == "circle" )
        return new Circle( input, references, *factory_ );
    if( nodeName == "ellipse" )
        return new svg::Ellipse( input, references, *factory_ );
    if( nodeName == "line" )
        return new Line( input, references, *factory_ );
    if( nodeName == "polyline" )
        return new PolyLine( input, references, *factory_ );
    if( nodeName == "polygon" )
        return new svg::Polygon( input, references, *factory_ );
    if( nodeName == "rect" )
        return new svg::Rectangle( input, references, *factory_ );
    if( nodeName == "defs" )
        return new Definitions( input, *this, references, *factory_ );
    if( nodeName == "use" )
        return new Use( input, references, *factory_ );
    if( nodeName == "text" )
        return new Text( input, references, renderer_, *factory_ );
    return new DummyNode( input, *this, references, *factory_ );
}

// -----------------------------------------------------------------------------
// Name: SVGFactory::ChangePropertyFactory
// Created: ZEBRE 2007-05-24
// -----------------------------------------------------------------------------
void SVGFactory::ChangePropertyFactory( svg::RenderingContext_ABC::E_Properties property, std::unique_ptr< PropertyFactory_ABC > factory )
{
    factory_->ChangeFactory( property, std::move( factory ) );
}

// -----------------------------------------------------------------------------
// Name: SVGFactory::ChangePropertyFactory
// Created: ZEBRE 2007-05-24
// -----------------------------------------------------------------------------
void SVGFactory::ChangePropertyFactory( svg::RenderingContext_ABC::E_Properties property, const PropertyFactory_ABC& factory )
{
    factory_->ChangeFactory( property, factory );
}
