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
#include "xeumeuleu/xml.h"

using namespace xml;

namespace
{
    unsigned int localCounter_ = 1;
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
    std::string topLeft, bottomRight;
    xis >> attribute( "hautGauche", topLeft )
        >> attribute( "basDroit", bottomRight );
    topLeft_ = converter_.ConvertToXY( topLeft );
    bottomRight_ = converter_.ConvertToXY( bottomRight );
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
    xos << attribute( "hautGauche", converter_.ConvertToMgrs( topLeft_ ) )
        << attribute( "basDroit"  , converter_.ConvertToMgrs( bottomRight_ ) );
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
