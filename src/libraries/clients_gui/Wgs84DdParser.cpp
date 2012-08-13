// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "clients_gui_pch.h"
#include "Wgs84DdParser.h"
#include "clients_kernel/CoordinateConverter_ABC.h"

using namespace gui;
// -----------------------------------------------------------------------------
// Name: Wgs84DdParser constructor
// Created: AME 2010-03-04
// -----------------------------------------------------------------------------
Wgs84DdParser::Wgs84DdParser( const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
    , numParameters_( 2 )
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: Wgs84DdParser destructor
// Created: AME 2010-03-04
// -----------------------------------------------------------------------------
Wgs84DdParser::~Wgs84DdParser()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Wgs84DdParser::Parse
// Created: AME 2010-03-04
// -----------------------------------------------------------------------------
bool Wgs84DdParser::Parse( QString content, geometry::Point2f& result, QStringList& hint ) const
{
    QStringList listValue = QStringList::split( ":", content );
    QString hintX = listValue[ 0 ].stripWhiteSpace();
    QString hintY = listValue[ 1 ].stripWhiteSpace();

    float x, y;
    std::stringstream strX( hintX.toStdString() );
    strX >> x ;
    if( ! strX )
        return false;
    std::stringstream strY( hintY.toStdString() );
    strY >> y ;
    if( ! strY )
        return false;
    hint.append( hintX );
    hint.append( hintY );

    geometry::Point2d point( x, y );
    result = converter_.ConvertFromGeo( point );
    return true;
}

// -----------------------------------------------------------------------------
// Name: Wgs84DdParser::GetNumberOfParameters
// Created: AME 2010-03-11
// -----------------------------------------------------------------------------
int Wgs84DdParser::GetNumberOfParameters() const
{
    return numParameters_;
}