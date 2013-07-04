// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Wgs84DmsParser.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Wgs84DmsParser constructor
// Created: AME 2010-03-04
// -----------------------------------------------------------------------------
Wgs84DmsParser::Wgs84DmsParser( const kernel::CoordinateConverter_ABC& converter )
    : converter_( converter )
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
bool Wgs84DmsParser::Parse( const QString& content, geometry::Point2f& result, QStringList& hint ) const
{
    try
    {
        QStringList listValue = QStringList::split( ":", content );
        QString hintx;
        QString hinty;
        bool formatCoordX = FormatDmsCoordinate( listValue[ 0 ].stripWhiteSpace(), true, hintx );
        bool formatCoordY = FormatDmsCoordinate( listValue[ 1 ].stripWhiteSpace(), false, hinty );

        if( formatCoordX && formatCoordY )
        {
            result = converter_.ConvertFromGeoDms( hintx.toStdString(), hinty.toStdString() );
            hint.append( hintx );
            hint.append( hinty );
            return true;
        }
    }
    catch( ... )
    {
        //NOTHING
    }
    return false;
}
// -----------------------------------------------------------------------------
// Name: Wgs84DmsParser::FormatDmsCoordinate
// Created: AME 2010-03-11
// -----------------------------------------------------------------------------
bool Wgs84DmsParser::FormatDmsCoordinate( const QString& content, bool longitude, QString& hint ) const
{
    QString coordValue = content.stripWhiteSpace().upper();
    coordValue.replace( L'°', ' ' );
    coordValue.replace( L'\'', ' ' );
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
    const int value = longitude ? 180 : 90;

    int deg_lat = listParameters[ 0 ].toInt( &ok );
    if( !ok || deg_lat < -value || deg_lat > value )
        return false;
    int min_lat = listParameters[1].toInt( &ok );
    if( !ok || min_lat < 0 || min_lat >= 60 )
        return false;
    double sec_lat = listParameters[ 2 ].toDouble( &ok );
    if( !ok || sec_lat < 0 || sec_lat >= 60 )
        return false;

    QChar finalParameter;
    if( longitude )
        finalParameter = ( deg_lat < 0 ) ? 'W' : 'E';
    else
        finalParameter = ( deg_lat < 0 ) ? 'S' : 'N';

    static const QChar padding( '0' );
    hint = tools::translate( "Wgs84DmsParser", "%1° %2' %3.%4 %5" )
                       .arg( std::abs( deg_lat ), 2, 10, padding )
                       .arg( min_lat, 2, 10, padding )
                       .arg( static_cast< int >( sec_lat ), 2, 10, padding )
                       .arg( static_cast< int >( ( sec_lat - std::floor( sec_lat ) ) * 100 ), 2, 10, padding )
                       .arg( finalParameter );
    return true;
}

// -----------------------------------------------------------------------------
// Name: Wgs84DmsParser::GetNumberOfParameters
// Created: AME 2010-03-11
// -----------------------------------------------------------------------------
int Wgs84DmsParser::GetNumberOfParameters() const
{
    return 2;
}
