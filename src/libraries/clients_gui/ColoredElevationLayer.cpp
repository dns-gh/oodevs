// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ColoredElevationLayer.h"
#include "xeumeuleu/xml.h"
#include "clients_kernel/DetectionMap.h"
#include "boost/static_assert.hpp"

using namespace xml;
using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ColoredElevationLayer constructor
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
ColoredElevationLayer::ColoredElevationLayer( const DetectionMap& elevation )
    : ElevationLayer( elevation.GetMap() )
{
    xifstream xis(  "preferences.xml" ); // $$$$ AGE 2006-04-19: 
    xis >> start( "preferences" )
            >> start( "elevations" )
            >> list( "elevation", *this, &ColoredElevationLayer::ReadElevation );
}

// -----------------------------------------------------------------------------
// Name: ColoredElevationLayer destructor
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
ColoredElevationLayer::~ColoredElevationLayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ColoredElevationLayer::ReadElevation
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
void ColoredElevationLayer::ReadElevation( xml::xistream& xis )
{
    std::string color;
    float percentage;
    xis >> content( "percentage", percentage )
        >> content( "color", color );
    colors_.push_back( T_Color( percentage * 0.01f, QColor( color.c_str() ) ) );
}

namespace
{
    union color_t {
        unsigned int rgba;
        unsigned char c[4];
    };
    BOOST_STATIC_ASSERT( sizeof( color_t ) == 4 );
}

// -----------------------------------------------------------------------------
// Name: ColoredElevationLayer::SelectColor
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
void ColoredElevationLayer::SelectColor( short e, float /*slope*/, short maxElevation, unsigned char* color )
{
    // $$$$ AGE 2006-05-12: ce truc rame. Trouver mieux
    const float elevation = float( e ) / float( maxElevation );
    if( elevation <= colors_.front().first )
        WriteColor( colors_.front().second, color );
    else if( elevation >= colors_.back().first )
        WriteColor( colors_.back().second, color );
    else
    {
        for( CIT_Colors it = colors_.begin() + 1; it != colors_.end(); ++it )
        {
            if( elevation <= it->first )
            {
                const float in1 = (it-1)->first;
                const float in2 = it->first;
                const float ratio2 = ( elevation - in1 ) / ( in2 - in1 );
                const float ratio1 = 1.f - ratio2;
                color_t color1, color2;
                color1.rgba = (it-1)->second.rgb();
                color2.rgba = it->second.rgb();
                *color     = (unsigned char)( color1.c[0] * ratio1 + color2.c[0] * ratio2 );
                *(color+1) = (unsigned char)( color1.c[1] * ratio1 + color2.c[1] * ratio2 );
                *(color+2) = (unsigned char)( color1.c[2] * ratio1 + color2.c[2] * ratio2 );
                return;
            };
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ColoredElevationLayer::WriteColor
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
inline
void ColoredElevationLayer::WriteColor( const QColor& color, unsigned char* where )
{
    color_t c;
    c.rgba = color.rgb();
    *where     = c.c[0];
    *(where+1) = c.c[1];
    *(where+2) = c.c[2];
}
