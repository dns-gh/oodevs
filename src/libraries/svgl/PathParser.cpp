// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "PathParser.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: PathParser constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
PathParser::PathParser( const std::string& data )
    : base_( data )
    , data_( data )
    , crap_( false )
{

}

// -----------------------------------------------------------------------------
// Name: PathParser destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
PathParser::~PathParser()
{

}

// -----------------------------------------------------------------------------
// Name: PathParser::SubParser
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
PathParser PathParser::SubParser()
{
    std::istringstream::pos_type pos = data_.tellg();
    std::istringstream::pos_type bad( std::_BADOFF );
    if( pos != bad )
        return PathParser( base_.substr( static_cast< size_t >( pos )) );
    return PathParser( "" );
}

// -----------------------------------------------------------------------------
// Name: PathParser::IsCoordinate
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
bool PathParser::IsCoordinate()
{
    PathParser sub( SubParser() );
    sub.GetCoordinate();
    return ! sub.crap_;
}

// -----------------------------------------------------------------------------
// Name: PathParser::IsFloat
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
bool PathParser::IsFloat()
{
    PathParser sub( SubParser() );
    sub.GetFloat();
    return ! sub.crap_;
}

// -----------------------------------------------------------------------------
// Name: PathParser::GetCommand
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
char PathParser::GetCommand()
{
    SkipSeparators();
    const char result = char( data_.get() );
    SkipSeparators();
    return result;
}

// -----------------------------------------------------------------------------
// Name: PathParser::GetCoordinate
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
geometry::Point PathParser::GetCoordinate()
{
    const float x = GetFloat();
    const float y = GetFloat();
    return geometry::Point( x, y );
}

// -----------------------------------------------------------------------------
// Name: PathParser::GetFloat
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
float PathParser::GetFloat()
{
    float result;
    data_ >> result;
    crap_ = data_.fail();
    SkipComma();
    return result;
}

// -----------------------------------------------------------------------------
// Name: PathParser::GetFlag
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
bool PathParser::GetFlag()
{
	SkipSeparators();
    char result = char( data_.get() );
    crap_ = data_.fail();
    SkipComma();
    return result != '0';
}

// -----------------------------------------------------------------------------
// Name: PathParser::SkipSeparators
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
void PathParser::SkipSeparators()
{
    while( IsSeparator( data_.peek() ) )
        data_.ignore();
}

// -----------------------------------------------------------------------------
// Name: PathParser::IsSeparator
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
bool PathParser::IsSeparator( int value )
{
    return value == ' ' || value == '\t' || value == '\n';
}

// -----------------------------------------------------------------------------
// Name: PathParser::SkipComma
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
void PathParser::SkipComma()
{
    SkipSeparators();
    while( data_.peek() == ',' )
        data_.ignore();
}

// -----------------------------------------------------------------------------
// Name: PathParser::Eof
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
bool PathParser::Eof()
{
    return data_.peek() == std::char_traits< char >::eof();
}
