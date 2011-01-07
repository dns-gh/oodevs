// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ElevationPainter.h"
#include "clients_kernel/DetectionMap.h"
#include <boost/lexical_cast.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ElevationPainter constructor
// Created: LGY 2010-01-06
// -----------------------------------------------------------------------------
ElevationPainter::ElevationPainter( const kernel::DetectionMap& elevation )
    : elevation_( elevation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ElevationPainter destructor
// Created: LGY 2010-01-06
// -----------------------------------------------------------------------------
ElevationPainter::~ElevationPainter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ElevationPainter::Draw
// Created: LGY 2011-01-06
// -----------------------------------------------------------------------------
void ElevationPainter::Draw( QPainter& painter, unsigned int pourcentage, int x, int y ) const
{
    QFont font( "Normal", 8, QFont::Light );
    painter.setFont( font );
    const unsigned int elevation = static_cast< unsigned int >( Compute( pourcentage ) );
    int position = pourcentage > 50 ? x - 5 : x;
    painter.drawText( position, y, boost::lexical_cast< std::string >( elevation ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: ElevationComputer::Compute
// Created: LGY 2010-01-06
// -----------------------------------------------------------------------------
float ElevationPainter::Compute( unsigned int pourcentage ) const
{
    return elevation_.MaximumElevation() * pourcentage / 100.f;
}
