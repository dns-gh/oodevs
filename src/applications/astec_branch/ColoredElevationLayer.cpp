// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ColoredElevationLayer.h"
#include "xeumeuleu/xml.h"
#include "DetectionMap.h"
using namespace xml;

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
                >> list( "elevation", *this, ReadElevation );
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

// -----------------------------------------------------------------------------
// Name: ColoredElevationLayer::SelectColor
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
void ColoredElevationLayer::SelectColor( short e, float /*slope*/, short maxElevation, unsigned char* color )
{
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
                const QColor& color1 = (it-1)->second;
                const QColor& color2 = it->second;
                *color     = (unsigned char)( color1.red()   * ratio1 + color2.red()   * ratio2 );
                *(color+1) = (unsigned char)( color1.green() * ratio1 + color2.green() * ratio2 );
                *(color+2) = (unsigned char)( color1.blue()  * ratio1 + color2.blue()  * ratio2 );
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
    *where     = (unsigned char)( color.red() );
    *(where+1) = (unsigned char)( color.green() );
    *(where+2) = (unsigned char)( color.blue() );
}
