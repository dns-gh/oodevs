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
#include "clients_kernel/Tools.h"

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
bool Wgs84DmsParser::Parse( const QStringList& content, geometry::Point2f& result, QStringList& hint ) const
{
    try
    {
        if( content.size() != 2 )
            return false;
        QString hintx;
        QString hinty;
        bool formatCoordX = FormatDmsCoordinate( content[ 1 ].stripWhiteSpace(), true, hintx );
        bool formatCoordY = FormatDmsCoordinate( content[ 0 ].stripWhiteSpace(), false, hinty );

        if( formatCoordX && formatCoordY )
        {
            result = converter_.ConvertFromGeoDms( hintx.toStdString(), hinty.toStdString() );
            // Lat
            hint.append( hinty );
            // Long
            hint.append( hintx );
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
// Name: Wgs84DmsParser::GetDescriptor
// Created: BAX 2014-01-16
// -----------------------------------------------------------------------------
const LocationParserDescriptor& Wgs84DmsParser::GetDescriptor() const
{
    static const LocationParserDescriptor desc = {
        QStringList()
            << tools::translate( "gui::LocationEditorBox", "Lat" )
            << tools::translate( "gui::LocationEditorBox", "Long" ),
        QList< int >() << INT_MAX << INT_MAX,
    };
    return desc;
}

// -----------------------------------------------------------------------------
// Name: Wgs84DmsParser::Split
// Created: BAX 2014-01-16
// -----------------------------------------------------------------------------
QStringList Wgs84DmsParser::Split( const QString& input ) const
{
    auto list = input.split( ":" );
    std::swap( list[0], list[1] );
    return list;
}

// -----------------------------------------------------------------------------
// Name: Wgs84DmsParser::GetStringPosition
// Created: LGY 2014-01-22
// -----------------------------------------------------------------------------
std::string Wgs84DmsParser::GetStringPosition( const geometry::Point2f& position ) const
{
    return converter_.GetStringPosition( position, eCoordinateSystem_Wgs84Dms );
}
