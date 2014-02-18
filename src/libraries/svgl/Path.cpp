// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "Path.h"
#include "PathCommand_ABC.h"
#include "MoveTo.h"
#include "ClosePath.h"
#include "LineTo.h"
#include "PathParser.h"
#include "PolyCurveTo.h"
#include "HorizontalTo.h"
#include "VerticalTo.h"
#include "PolyEllipticalArc.h"
#include "PathContext.h"
#include "RenderingContext_ABC.h"
#include "DrawerTesselator.h"
#include "CompiledPath.h"
#include "Paint_ABC.h"
#include "RenderingContext_ABC.h"
#include "FillRule.h"
#include "Length.h"
#include "DashArray.h"

using namespace xml;
using namespace svg;
using namespace path;

// -----------------------------------------------------------------------------
// Name: Path constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
Path::Path( xml::xistream& input, References_ABC& references, const PropertyFactory& factory )
    : Node( input, references, factory )
{
    std::string data;
    input >> xml::attribute( "d", data );;
    Parse( data );
}

// -----------------------------------------------------------------------------
// Name: Path constructor
// Created: AGE 2006-08-16
// -----------------------------------------------------------------------------
Path::Path( xml::xistream& input, References_ABC& references, const PropertyFactory& factory, const std::string& data )
    : Node( input, references, factory )
{
    Parse( data );
}

// -----------------------------------------------------------------------------
// Name: Path destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
Path::~Path()
{
    for( CIT_PathCommands it = commands_.begin(); it != commands_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: Path::DrawInternal
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
void Path::DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const
{
    if( context.SetupFill( references ) )
    {
        const unsigned fillType = context.GetProperty< FillRule >( RenderingContext_ABC::fillRule ).GetType();
        DrawerTesselator tesselator( fillType );
        for( CIT_PathCommands it = commands_.begin(); it != commands_.end(); ++it )
            (*it)->DrawArea( tesselator, context.ExpectedPrecision() );
        tesselator.Commit();
    }
    if( context.SetupStroke( references ) )
    {
        for( CIT_PathCommands it = commands_.begin(); it != commands_.end(); ++it )
            (*it)->DrawLine( context.ExpectedPrecision() );
    }
}

// -----------------------------------------------------------------------------
// Name: Path::CompileInternal
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
Node& Path::CompileInternal( RenderingContext_ABC& context, References_ABC& ) const
{
    const unsigned fillType = context.GetProperty< FillRule >( RenderingContext_ABC::fillRule ).GetType();
    CompiledPath& result = *new CompiledPath( *this, fillType );
    for( CIT_PathCommands it = commands_.begin(); it != commands_.end(); ++it )
        (*it)->DrawArea( result, context.ExpectedPrecision() );
    result.Commit();

    return result;
}

// -----------------------------------------------------------------------------
// Name: Path::CreateCommand
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
PathCommand_ABC* Path::CreateCommand( PathParser& data, PathContext& context )
{
    char command = data.GetCommand();
    const bool isRelative = command >= 'a' && command <= 'z';
    context.SetRelative( isRelative );
    command = char( ::tolower( command ) );
    if( command == 'm' )
        return new MoveTo( data, context );
    if( command == 'z' )
        return new ClosePath( data, context );
    if( command == 'l' )
        return new svg::path::LineTo( data, context );
    if( command == 'c' || command == 's' || command == 'q' || command == 't' )
        return new PolyCurveTo( data, context, command );
    if( command == 'h' )
        return new HorizontalTo( data, context );
    if( command == 'v' )
        return new VerticalTo( data, context );
    if( command == 'a' )
        return new PolyEllipticalArc( data, context );
    throw std::runtime_error( std::string( "unknown command '" ) + command + "'" );
}

// -----------------------------------------------------------------------------
// Name: Path::Parse
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
void Path::Parse( const std::string& data )
{
    PathParser parser( data );
    PathContext context;
    while( ! parser.Eof() )
        commands_.push_back( CreateCommand( parser, context ) );
}
