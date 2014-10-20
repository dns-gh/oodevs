// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "Color.h"
#include "RenderingContext_ABC.h"
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace svg;

namespace
{
    struct ColorDefinition
    {
        
        const char* name;
        unsigned char r, g, b;
    };

    // $$$$ ZEBRE 2007-11-19: vc80 iterator debugging...
    bool operator<( const std::string& lhs, const ColorDefinition& rhs )
    {
        return lhs < rhs.name;
    }
    bool operator<( const ColorDefinition& lhs, const std::string& rhs )
    {
        return lhs.name < rhs;
    }
    bool operator<( const ColorDefinition& lhs, const ColorDefinition& rhs )
    {
        return lhs < std::string( rhs.name );
    }

    static ColorDefinition colors[] = 
    {
        { "aliceblue", 240, 248, 255 },
        { "antiquewhite", 250, 235, 215 },
        { "aqua",  0, 255, 255 },
        { "aquamarine", 127, 255, 212 },
        { "azure", 240, 255, 255 },
        { "beige", 245, 245, 220 },
        { "bisque", 255, 228, 196 },
        { "black",  0, 0, 0 },
        { "blanchedalmond", 255, 235, 205 },
        { "blue",  0, 0, 255 },
        { "blueviolet", 138, 43, 226 },
        { "brown", 165, 42, 42 },
        { "burlywood", 222, 184, 135 },
        { "cadetblue",  95, 158, 160 },
        { "chartreuse", 127, 255, 0 },
        { "chocolate", 210, 105, 30 },
        { "coral", 255, 127, 80 },
        { "cornflowerblue", 100, 149, 237 },
        { "cornsilk", 255, 248, 220 },
        { "crimson", 220, 20, 60 },
        { "cyan",  0, 255, 255 },
        { "darkblue",  0, 0, 139 },
        { "darkcyan",  0, 139, 139 },
        { "darkgoldenrod", 184, 134, 11 },
        { "darkgray", 169, 169, 169 },
        { "darkgreen",  0, 100, 0 },
        { "darkgrey", 169, 169, 169 },
        { "darkkhaki", 189, 183, 107 },
        { "darkmagenta", 139, 0, 139 },
        { "darkolivegreen",  85, 107, 47 },
        { "darkorange", 255, 140, 0 },
        { "darkorchid", 153, 50, 204 },
        { "darkred", 139, 0, 0 },
        { "darksalmon", 233, 150, 122 },
        { "darkseagreen", 143, 188, 143 },
        { "darkslateblue",  72, 61, 139 },
        { "darkslategray",  47, 79, 79 },
        { "darkslategrey",  47, 79, 79 },
        { "darkturquoise",  0, 206, 209 },
        { "darkviolet", 148, 0, 211 },
        { "deeppink", 255, 20, 147 },
        { "deepskyblue",  0, 191, 255 },
        { "dimgray", 105, 105, 105 },
        { "dimgrey", 105, 105, 105 },
        { "dodgerblue",  30, 144, 255 },
        { "firebrick", 178, 34, 34 },
        { "floralwhite", 255, 250, 240 },
        { "forestgreen",  34, 139, 34 },
        { "fuchsia", 255, 0, 255 },
        { "gainsboro", 220, 220, 220 },
        { "ghostwhite", 248, 248, 255 },
        { "gold", 255, 215, 0 },
        { "goldenrod", 218, 165, 32 },
        { "gray", 128, 128, 128 },
        { "grey", 128, 128, 128 },
        { "green",  0, 128, 0 },
        { "greenyellow", 173, 255, 47 },
        { "honeydew", 240, 255, 240 },
        { "hotpink", 255, 105, 180 },
        { "indianred", 205, 92, 92 },
        { "indigo",  75, 0, 130 },
        { "ivory", 255, 255, 240 },
        { "khaki", 240, 230, 140 },
        { "lavender", 230, 230, 250 },
        { "lavenderblush", 255, 240, 245 },
        { "lawngreen", 124, 252, 0 },
        { "lemonchiffon", 255, 250, 205 },
        { "lightblue", 173, 216, 230 },
        { "lightcoral", 240, 128, 128 },
        { "lightcyan", 224, 255, 255 },
        { "lightgoldenrodyellow", 250, 250, 210 },
        { "lightgray", 211, 211, 211 },
        { "lightgreen", 144, 238, 144 },
        { "lightgrey", 211, 211, 211 },
        { "lightpink", 255, 182, 193 },
        { "lightsalmon", 255, 160, 122 },
        { "lightseagreen",  32, 178, 170 },
        { "lightskyblue", 135, 206, 250 },
        { "lightslategray", 119, 136, 153 },
        { "lightslategrey", 119, 136, 153 },
        { "lightsteelblue", 176, 196, 222 },
        { "lightyellow", 255, 255, 224 },
        { "lime",  0, 255, 0 },
        { "limegreen",  50, 205, 50 },
        { "linen", 250, 240, 230 },
        { "magenta", 255, 0, 255 },
        { "maroon", 128, 0, 0 },
        { "mediumaquamarine", 102, 205, 170 },
        { "mediumblue",  0, 0, 205 },
        { "mediumorchid", 186, 85, 211 },
        { "mediumpurple", 147, 112, 219 },
        { "mediumseagreen",  60, 179, 113 },
        { "mediumslateblue", 123, 104, 238 },
        { "mediumspringgreen",  0, 250, 154 },
        { "mediumturquoise",  72, 209, 204 },
        { "mediumvioletred", 199, 21, 133 },
        { "midnightblue",  25, 25, 112 },
        { "mintcream", 245, 255, 250 },
        { "mistyrose", 255, 228, 225 },
        { "moccasin", 255, 228, 181 },
        { "navajowhite", 255, 222, 173 },
        { "navy",  0, 0, 128 },
        { "oldlace", 253, 245, 230 },
        { "olive", 128, 128, 0 },
        { "olivedrab", 107, 142, 35 },
        { "orange", 255, 165, 0 },
        { "orangered", 255, 69, 0 },
        { "orchid", 218, 112, 214 },
        { "palegoldenrod", 238, 232, 170 },
        { "palegreen", 152, 251, 152 },
        { "paleturquoise", 175, 238, 238 },
        { "palevioletred", 219, 112, 147 },
        { "papayawhip", 255, 239, 213 },
        { "peachpuff", 255, 218, 185 },
        { "peru", 205, 133, 63 },
        { "pink", 255, 192, 203 },
        { "plum", 221, 160, 221 },
        { "powderblue", 176, 224, 230 },
        { "purple", 128, 0, 128 },
        { "red", 255, 0, 0 },
        { "rosybrown", 188, 143, 143 },
        { "royalblue",  65, 105, 225 },
        { "saddlebrown", 139, 69, 19 },
        { "salmon", 250, 128, 114 },
        { "sandybrown", 244, 164, 96 },
        { "seagreen",  46, 139, 87 },
        { "seashell", 255, 245, 238 },
        { "sienna", 160, 82, 45 },
        { "silver", 192, 192, 192 },
        { "skyblue", 135, 206, 235 },
        { "slateblue", 106, 90, 205 },
        { "slategray", 112, 128, 144 },
        { "slategrey", 112, 128, 144 },
        { "snow", 255, 250, 250 },
        { "springgreen",  0, 255, 127 },
        { "steelblue",  70, 130, 180 },
        { "tan", 210, 180, 140 },
        { "teal",  0, 128, 128 },
        { "thistle", 216, 191, 216 },
        { "tomato", 255, 99, 71 },
        { "turquoise",  64, 224, 208 },
        { "violet", 238, 130, 238 },
        { "wheat", 245, 222, 179 },
        { "white", 255, 255, 255 },
        { "whitesmoke", 245, 245, 245 },
        { "yellow", 255, 255, 0 },
        { "yellowgreen", 154, 205, 50 }
    };
    static const unsigned colorCount = sizeof( colors ) / sizeof( ColorDefinition );

    ColorDefinition FindColor( std::string name )
    {
        std::for_each( name.begin(), name.end(), &tolower );
        ColorDefinition* color = std::lower_bound( colors, colors + colorCount, name );
        if( color != colors + colorCount && color->name == name )
            return *color;
        const ColorDefinition empty = { 0, 0, 0, 0 };
        return empty;
    }
}

// -----------------------------------------------------------------------------
// Name: Color constructor
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
Color::Color( const std::string& name )
    : r_( 0 ), g_( 0 ), b_( 0 )
    , context_( 0 )
    , pickingMode_( false )
{
    if( name.find( "rgb" ) != std::string::npos )
    {
        std::stringstream str( RemoveNonNumeric( name ) );
        str >> r_ >> g_ >> b_;
        if( name.find( '%' ) != std::string::npos )
        {
            r_ /= 100.f; g_ /= 100.f; b_ /= 100.f;
        }
        else
        {
            r_ /= 255.f; g_ /= 255.f; b_ /= 255.f;
        }
    }
    else
    {
        const std::string::size_type p = name.find( "#" );
        if( p != std::string::npos )
        {
            const std::string hex = name.substr( p + 1, 6 );
            std::stringstream str( hex );
            long value = 0;
            str >> std::hex >> value;
            r_ = ( ( value >> 16 ) & 0xFF ) / 255.f;
            g_ = ( ( value >> 8  ) & 0xFF ) / 255.f;
            b_ = ( ( value       ) & 0xFF ) / 255.f;
        }
        else
        {
            ColorDefinition c = FindColor( name );
            r_ = c.r / 255.f;
            g_ = c.g / 255.f;
            b_ = c.b / 255.f;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Color constructor
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
Color::Color( const Color& rhs )
    : r_( rhs.r_ )
    , g_( rhs.g_ )
    , b_( rhs.b_ )
    , context_( rhs.context_ )
    , pickingMode_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Color destructor
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
Color::~Color()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Color::Set
// Created: AGE 2006-10-25
// -----------------------------------------------------------------------------
void Color::Set( float r, float g, float b )
{
    r_ = r; g_ = g; b_ = b;
}

// -----------------------------------------------------------------------------
// Name: Color::Setup
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
bool Color::Setup( References_ABC&, float opacity ) const
{
    if( !pickingMode_ )
        glColor4f( r_, g_, b_, opacity );
    return true;
}

// -----------------------------------------------------------------------------
// Name: Color::ReachTop
// Created: LGY 2013-03-06
// -----------------------------------------------------------------------------
void Color::ReachTop( const RenderingContext_ABC& context )
{
    pickingMode_ = context.IsPickingMode();
    context_ = &context;
}

// -----------------------------------------------------------------------------
// Name: Color::LeaveTop
// Created: LGY 2013-03-06
// -----------------------------------------------------------------------------
void Color::LeaveTop( const RenderingContext_ABC& /*context*/ )
{
    pickingMode_ = false;
    context_ = 0;
}

// -----------------------------------------------------------------------------
// Name: Color::RemoveNonNumeric
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
std::string Color::RemoveNonNumeric( const std::string& in )
{
    std::string result( in );
    for( unsigned i = 0; i < result.size(); ++i )
    {
        char& c = result[i];
        if( ( c < '0' || c > '9' ) && c != '.' )
            c = ' ';
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: Color::Clone
// Created: AGE 2006-08-15
// -----------------------------------------------------------------------------
Color& Color::Clone() const
{
    return *new Color( *this );
}
