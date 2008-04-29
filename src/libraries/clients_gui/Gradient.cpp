// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Gradient.h"
#include "xeumeuleu/xml.h"
#include "graphics/extensions.h"

using namespace xml;
using namespace gui;

namespace 
{
    struct Less {
        template< typename T >
        bool operator()( const T& lhs, const T& rhs ) {
            return lhs.first < rhs.first;
        }
    };
    
}

// -----------------------------------------------------------------------------
// Name: Gradient constructor
// Created: AGE 2007-07-02
// -----------------------------------------------------------------------------
Gradient::Gradient( xml::xistream& xis )
     : usedRatio_( 1 )
     , textureSize_( 0 )
{
    std::string name;
    xis >> attribute( "name", name )
            >> list( "color", *this, &Gradient::LoadValue );
    name_ = name.c_str();
}

// -----------------------------------------------------------------------------
// Name: Gradient constructor
// Created: AGE 2007-07-03
// -----------------------------------------------------------------------------
Gradient::Gradient()
    : name_( "" )
    , usedRatio_( 1 )
    , textureSize_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Gradient destructor
// Created: AGE 2007-07-02
// -----------------------------------------------------------------------------
Gradient::~Gradient()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Gradient::SetName
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void Gradient::SetName( const QString& name )
{
    name_ = name;
}

// -----------------------------------------------------------------------------
// Name: Gradient::GetName
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
QString Gradient::GetName() const
{
    return name_;
}

namespace
{
    unsigned TextureSize( unsigned i )
    {
        return i;
    }

    struct Color
    {
        Color( float a ) : r( 0 ), g( 0 ), b( 0 ), a( a ) {}
        float r, g, b, a;
    };

    Color MakeColor( const QColor& cFrom, const QColor& cEnd, Color result, float ratio ) 
    {
        result.r = ( 1.f - ratio ) * cFrom.red() / 255.f   + ratio * cEnd.red() / 255.f;
        result.g = ( 1.f - ratio ) * cFrom.green() / 255.f + ratio * cEnd.green() / 255.f;
        result.b = ( 1.f - ratio ) * cFrom.blue() / 255.f  + ratio * cEnd.blue() / 255.f;
        return result;
    }

    template< typename It >
    void FillGradient( It from, It end, const QColor& cFrom, const QColor& cEnd )
    {
        float increment = 1.f / float( end - from );
        float ratio = 0.f;
        while( from != end )
        {
            *from = MakeColor( cFrom, cEnd, *from, ratio );
            ratio +=increment;
            ++from;
        }
    }

    template< typename C1, typename C2 >
    void FillColors( C1& colors, const C2& with, float baseDistance )
    {
        unsigned colorIndex = 0;
        for( unsigned i = 0; i < with.size() - 1; ++i )
        {
            float distance = with[i+1].first - with[i].first;
            unsigned elements = unsigned( floor( distance / baseDistance + 0.4f ) );
            FillGradient( colors.begin() + colorIndex, colors.begin() + colorIndex + elements, with[i].second, with[i+1].second );
            colorIndex += elements;
        }
        colors.back() = MakeColor( with.back().second, with.back().second, colors.back(), 1 );
    }
}

// -----------------------------------------------------------------------------
// Name: Gradient::AddColor
// Created: AGE 2007-07-03
// -----------------------------------------------------------------------------
void Gradient::AddColor( float position, const QColor& color )
{
    colors_.push_back( T_Color( position, color ) );
}

// -----------------------------------------------------------------------------
// Name: Gradient::MakeGlTexture
// Created: AGE 2007-07-02
// -----------------------------------------------------------------------------
void Gradient::MakeGlTexture( float alpha )
{
    std::sort( colors_.begin(), colors_.end(), Less() );

    unsigned distance = FindBaseDistance();
    unsigned size = 1024 / distance + 1;
    textureSize_ = TextureSize( size );
    usedRatio_ = float( size ) / float( textureSize_ );

    std::vector< Color > colors;
    colors.resize( textureSize_, Color( alpha ) );
    FillColors( colors, colors_, distance / 1024.f );

    glTexImage1D( GL_TEXTURE_1D, 0, GL_RGBA, textureSize_, 0, GL_RGBA, GL_FLOAT, & colors.front() ) ;
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S , gl::GL_CLAMP_TO_EDGE );
}

// -----------------------------------------------------------------------------
// Name: Gradient::Length
// Created: AGE 2007-07-02
// -----------------------------------------------------------------------------
unsigned Gradient::Length() const
{
    return textureSize_;
}

// -----------------------------------------------------------------------------
// Name: Gradient::UsedRatio
// Created: AGE 2007-07-02
// -----------------------------------------------------------------------------
float Gradient::UsedRatio() const
{
    return usedRatio_;
}

// -----------------------------------------------------------------------------
// Name: Gradient::LoadValue
// Created: AGE 2007-07-02
// -----------------------------------------------------------------------------
void Gradient::LoadValue( xml::xistream& xis )
{
    float ratio;
    std::string colorName;
    xis >> attribute( "position", ratio )
        >> attribute( "color", colorName );
    QColor color( colorName.c_str() );
    if( color.isValid() )
        colors_.push_back( T_Color( ratio, color ) );
    else 
        throw std::runtime_error( "Invalid color '" + colorName + "'" );
}

namespace
{
    unsigned Pgcd( unsigned a, unsigned b )
    {
        if( !a )
            return 0;
        if( !b )
            return a;
        return Pgcd( b, a % b );
    }
}

// -----------------------------------------------------------------------------
// Name: Gradient::FindBaseDistance
// Created: AGE 2007-07-02
// -----------------------------------------------------------------------------
unsigned Gradient::FindBaseDistance() const
{
    unsigned pgcd = 1024;
    unsigned previous = 0;
    for( CIT_Colors it = colors_.begin(); it != colors_.end(); ++it )
    {
        unsigned current  = unsigned( it->first * 1024 );
        unsigned distance = current - previous;
        if( distance > 0 )
            pgcd = Pgcd( pgcd, distance );
        previous = current;
    }
    if( ! colors_.empty() )
    {
        unsigned current = 1024;
        unsigned distance = current - previous;
        if( distance > 0 )
            pgcd = Pgcd( pgcd, distance );
    }
    return pgcd;
}

// -----------------------------------------------------------------------------
// Name: Gradient::Save
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void Gradient::Save( xml::xostream& xos ) const
{
    xos << start( "gradient" )
            << attribute( "name", name_.ascii() );
    for( CIT_Colors it = colors_.begin(); it != colors_.end(); ++it )
        xos << start( "color" )
                << attribute( "position", it->first )
                << attribute( "color", it->second.name() )
            << end();
    xos << end();
}

// -----------------------------------------------------------------------------
// Name: Gradient::Accept
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void Gradient::Accept( GradientVisitor_ABC& visitor ) const
{
    for( CIT_Colors it = colors_.begin(); it != colors_.end(); ++it )
        visitor.Visit( it->first, it->second );
}

// -----------------------------------------------------------------------------
// Name: Gradient::operator=
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
Gradient& Gradient::operator=( const Gradient& rhs )
{
    colors_ = rhs.colors_;
    textureSize_ = rhs.textureSize_;
    usedRatio_ = rhs.usedRatio_;
    return *this;
}
