// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "LocalWeather.h"
#include "Tools.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include <xeumeuleu/xml.h>

using namespace xml;

namespace
{
    unsigned int localCounter_ = 1;

    QTime MakeTime( const QString& str )
    {
        QStringList list = QStringList::split( QRegExp( "[hms]" ), str );
        return QTime( list.count() > 0 ? list[0].toInt() : 0
            , list.count() > 1 ? list[1].toInt() : 0
            , list.count() > 2 ? list[2].toInt() : 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: LocalWeather constructor
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
LocalWeather::LocalWeather( const kernel::CoordinateConverter_ABC& converter )
: converter_( converter )
, id_( localCounter_++ )
, name_( tools::translate( "LocalWeather", "Local weather %1" ).arg( id_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocalWeather constructor
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
LocalWeather::LocalWeather( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter )
: Weather( xis )
, converter_( converter )
, id_( localCounter_++ )
, name_( tools::translate( "LocalWeather", "Local weather %1" ).arg( id_ ) )
{
    
    std::string topLeft, bottomRight, startTime, endTime ;
    xis >> attribute( "start-time", startTime )
        >> attribute( "end-time", endTime );
    xis >> attribute( "top-left", topLeft )
        >> attribute( "bottom-right", bottomRight );
    topLeft_ = converter_.ConvertToXY( topLeft );
    bottomRight_ = converter_.ConvertToXY( bottomRight );
    {
        QString extended( startTime.c_str() );
        extended.insert( 13, ':' ); extended.insert( 11, ':' ); 
        extended.insert(  6, '-' ); extended.insert(  4, '-' );
        startTime_ = QDateTime::fromString( extended, Qt::ISODate );
    }
    {
        QString extended( endTime.c_str() );
        extended.insert( 13, ':' ); extended.insert( 11, ':' ); 
        extended.insert(  6, '-' ); extended.insert(  4, '-' );
        endTime_ = QDateTime::fromString( extended, Qt::ISODate );
    }

}

// -----------------------------------------------------------------------------
// Name: LocalWeather destructor
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
LocalWeather::~LocalWeather()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocalWeather::GetId
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
unsigned long LocalWeather::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: LocalWeather::GetName
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
QString LocalWeather::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: LocalWeather::Serialize
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void LocalWeather::Serialize( xml::xostream& xos ) const
{
    xos << attribute( "start-time", startTime_.toString( "yyyyMMddThhmmss" ).ascii() )
        << attribute( "end-time", endTime_.toString( "yyyyMMddThhmmss" ).ascii() );

    xos << attribute( "top-left", converter_.ConvertToMgrs( topLeft_ ) )
        << attribute( "bottom-right"  , converter_.ConvertToMgrs( bottomRight_ ) );
    Weather::Serialize( xos );
}

// -----------------------------------------------------------------------------
// Name: LocalWeather::SetPosition
// Created: SBO 2006-12-21
// -----------------------------------------------------------------------------
void LocalWeather::SetPosition( const geometry::Point2f& topLeft, const geometry::Point2f& bottomRight )
{
    topLeft_ = topLeft;
    bottomRight_ = bottomRight;
}

// -----------------------------------------------------------------------------
// Name: LocalWeather::GetTopLeft
// Created: SBO 2006-12-21
// -----------------------------------------------------------------------------
geometry::Point2f LocalWeather::GetTopLeft() const
{
    return topLeft_;
}

// -----------------------------------------------------------------------------
// Name: LocalWeather::GetBottomRight
// Created: SBO 2006-12-21
// -----------------------------------------------------------------------------
geometry::Point2f LocalWeather::GetBottomRight() const
{
    return bottomRight_;
}