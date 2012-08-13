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
#include "Wgs84DmsParser.h"
#include "clients_kernel/CoordinateConverter_ABC.h"

using namespace gui;
// -----------------------------------------------------------------------------
// Name: Wgs84DmsParser constructor
// Created: AME 2010-03-04
// -----------------------------------------------------------------------------
Wgs84DmsParser::Wgs84DmsParser( const kernel::CoordinateConverter_ABC& converter )
: converter_( converter )
, numParameters_( 2 )
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: Wgs84DmsParser destructor
// Created: AME 2010-03-04
// -----------------------------------------------------------------------------
Wgs84DmsParser::~Wgs84DmsParser()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: Wgs84DmsParser::Parse
// Created: AME 2010-03-04
// -----------------------------------------------------------------------------
bool Wgs84DmsParser::Parse( QString content, geometry::Point2f& result, QStringList& hint ) const
{
    try
    {
        QStringList listValue = QStringList::split( ":", content );
        bool formatCoordX, formatCoordY = false;
        QString hintx, hinty;
        formatCoordX = FormatDmsCoordinate( listValue[ 0 ].stripWhiteSpace(), true, hintx );
        if( formatCoordX )
        formatCoordY = FormatDmsCoordinate( listValue[ 1 ].stripWhiteSpace(), false, hinty );

        if( formatCoordX && formatCoordY )
        {
            result = converter_.ConvertFromGeoDms( hintx.toStdString(), hinty.toStdString() );
            hint.append( hintx );
            hint.append( hinty );
            return true;
        }
        return false;
    }
    catch( ... )
    {
        return false;
    }
    return true;
}
// -----------------------------------------------------------------------------
// Name: Wgs84DmsParser::FormatDmsCoordinate
// Created: AME 2010-03-11
// -----------------------------------------------------------------------------
bool Wgs84DmsParser::FormatDmsCoordinate( const QString content, bool longitude, QString& hint ) const
{
    QString coordValue = content.stripWhiteSpace().upper();
    coordValue.replace('°',' ');
    coordValue.replace('\'',' ');
    if( coordValue.find( "S" ) >= 0 || coordValue.find( "W" ) >= 0 )
      coordValue = "-" + coordValue;
    coordValue.replace( 'N', ' ' );
    coordValue.replace( 'S', ' ' );
    coordValue.replace( 'E', ' ' );
    coordValue.replace( 'W', ' ' );

    QStringList listParameters = QStringList::split( " ", coordValue );
    if( listParameters.count() != 3 )
      return false;

    bool ok = true;
    int value;
    if( longitude )  value = 180; else value = 90;

    int deg_lat = listParameters[0].toInt( &ok );
    if(!ok || deg_lat < -value || deg_lat > value ) return false;
    int min_lat = listParameters[1].toInt( &ok );
    if(!ok || min_lat < 0 || min_lat >= 60) return false;
    double sec_lat = listParameters[2].toDouble( &ok );
    if(!ok || sec_lat < 0 || sec_lat >= 60) return false;

    const char* finalParameter;
    if( longitude )
      if( deg_lat < 0 )
        finalParameter = "W";
      else
        finalParameter = "E";
    else
      if( deg_lat < 0 )
        finalParameter = "S";
      else
        finalParameter = "N";

    hint.sprintf( "%02d° %02d' %02d.%02d %s", abs(deg_lat), min_lat, (int)sec_lat, (int)(sec_lat - floor(sec_lat) )*100, finalParameter ) ;
    return true;
}

// -----------------------------------------------------------------------------
// Name: Wgs84DmsParser::GetNumberOfParameters
// Created: AME 2010-03-11
// -----------------------------------------------------------------------------
int Wgs84DmsParser::GetNumberOfParameters() const
{
    return numParameters_;
}