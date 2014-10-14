// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Gradient.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Tools.h"
#include <graphics/extensions.h>
#include <xeumeuleu/xml.hpp>
#pragma warning( push )
#pragma warning( disable : 4702 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )
#include <boost/bind.hpp>

using namespace kernel;
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
// Created: AGE 2007-07-03
// -----------------------------------------------------------------------------
Gradient::Gradient( const QString& name /* = "" */ )
    : usedRatio_  ( 1 )
    , textureSize_( 0 )
{
    SetName( name );
}

// -----------------------------------------------------------------------------
// Name: Gradient constructor
// Created: AGE 2007-07-02
// -----------------------------------------------------------------------------
Gradient::Gradient( xml::xistream& xis )
    : usedRatio_  ( 1 )
    , textureSize_( 0 )
{
    std::string name;
    xis >> xml::attribute( "name", name )
        >> xml::list( "color", *this, &Gradient::LoadValue );
    SetName( QString::fromStdString( name ) );
}

// -----------------------------------------------------------------------------
// Name: Gradient constructor
// Created: SBO 2008-08-18
// -----------------------------------------------------------------------------
Gradient::Gradient( const QString& name, const QString& colors )
    : usedRatio_  ( 1 )
    , textureSize_( 0 )
{
    SetName( name );
    LoadValues( colors );
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
    displayName_ = tools::translate( "gradients", name );
}

// -----------------------------------------------------------------------------
// Name: Gradient::GetName
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
QString Gradient::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Gradient::GetDisplayName
// Created: ABR 2014-07-31
// -----------------------------------------------------------------------------
QString Gradient::GetDisplayName() const
{
    return displayName_;
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

    Color MakeColor( const QColor& cFrom, const QColor& cEnd, Color result, float ratio, float part )
    {
        result.r = ( ( 1.f - ratio ) * cFrom.red() + ratio * cEnd.red() ) / part;
        result.g = ( ( 1.f - ratio ) * cFrom.green() + ratio * cEnd.green() ) / part;
        result.b = ( ( 1.f - ratio ) * cFrom.blue() + ratio * cEnd.blue() ) / part;
        return result;
    }

    template< typename It >
    void FillGradient( It from, It end, const QColor& cFrom, const QColor& cEnd )
    {
        float increment = 1.f / float( std::distance( from, end ) );
        float ratio = 0.f;
        while( from != end )
        {
            *from = MakeColor( cFrom, cEnd, *from, ratio, 255.f );
            ratio += increment;
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
        colors.back() = MakeColor( with.back().second, with.back().second, colors.back(), 1, 255.f );
    }
}

namespace
{
    bool Check( std::pair< float, QColor >& color, float position )
    {
        return color.first == position;
    }
}

// -----------------------------------------------------------------------------
// Name: Gradient::AddColor
// Created: AGE 2007-07-03
// -----------------------------------------------------------------------------
void Gradient::AddColor( float position, const QColor& color )
{
    colors_.erase( std::remove_if( colors_.begin(), colors_.end(), boost::bind( &Check, _1, position ) ), colors_.end() );
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
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, gl::GL_CLAMP_TO_EDGE );
}

// -----------------------------------------------------------------------------
// Name: Gradient::Compute
// Created: LGY 2011-01-10
// -----------------------------------------------------------------------------
QColor Gradient::Compute( float density, float alpha, float min, float max )
{
    if( colors_.empty() )
        return QColor( 0, 255, 0 );
    std::sort( colors_.begin(), colors_.end(), Less() );
    T_Color from;
    T_Color end;
    float percent = density / ( max - min );
    float ratio;
    if( percent > colors_.back().first )
    {
        from = colors_.back();
        end = colors_.back();
        ratio = 1;
    }
    else
    {
        for( auto it = colors_.begin(); it != colors_.end(); ++it )
            if( it->first <= percent )
            {
                from = *it;
                end = ( ( it + 1 ) == colors_.end() ) ? *it : *( it + 1 );
            }
            ratio = ( percent - from.first ) / ( end.first - from.first );
    }
    Color result( alpha );
    result = MakeColor( from.second, end.second, result, ratio, 1.f );
    return QColor( static_cast< int >( result.r ), static_cast< int >( result.g ), static_cast< int >( result.b ) );
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
    xis >> xml::attribute( "position", ratio )
        >> xml::attribute( "color", colorName );
    QColor color( colorName.c_str() );
    if( color.isValid() )
        colors_.push_back( T_Color( ratio, color ) );
    else
        throw MASA_EXCEPTION( "Invalid color '" + colorName + "'" );
}

// -----------------------------------------------------------------------------
// Name: Gradient::LoadValues
// Created: SBO 2008-08-18
// -----------------------------------------------------------------------------
void Gradient::LoadValues( const QString& values )
{
    QStringList colors = QStringList::split( ";", values );
    T_Colors result;
    for( QStringList::const_iterator it = colors.begin(); it != colors.end(); ++it )
    {
        const QStringList item = QStringList::split( ",", *it );
        if( item.size() == 2 )
        {
            const float ratio = boost::lexical_cast< float >( item[0].toStdString() );
            const QColor color( item[1] );
            result.push_back( T_Color( ratio, color ) );
        }
    }
    if( !result.empty() )
        std::swap( colors_, result );
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
    for( auto it = colors_.begin(); it != colors_.end(); ++it )
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
void Gradient::Save( kernel::OptionsController& options, const std::string& group ) const
{
    options.Change( group + name_.toStdString(), GetValues() );
}

// -----------------------------------------------------------------------------
// Name: Gradient::Accept
// Created: SBO 2007-07-03
// -----------------------------------------------------------------------------
void Gradient::Accept( GradientVisitor_ABC& visitor ) const
{
    for( auto it = colors_.begin(); it != colors_.end(); ++it )
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

// -----------------------------------------------------------------------------
// Name: Gradient::GetValues
// Created: ABR 2014-07-28
// -----------------------------------------------------------------------------
QString Gradient::GetValues() const
{
    QStringList colors;
    for( auto it = colors_.begin(); it != colors_.end(); ++it )
        colors.append( QString::number( it->first ) + "," + it->second.name() );
    return colors.join( ";" );
}
