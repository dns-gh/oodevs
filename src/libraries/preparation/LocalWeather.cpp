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
LocalWeather::LocalWeather()
    : Weather()
    , id_( localCounter_++ )
    , name_( tools::translate( "LocalWeather", "Local weather %1" ).arg( id_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LocalWeather constructor
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
LocalWeather::LocalWeather( xml::xistream& xis )
    : Weather( xis )
    , id_( localCounter_++ )
    , name_( tools::translate( "LocalWeather", "Local weather %1" ).arg( id_ ) )
{
    xis >> attribute( "hautGauche", topLeft_ )
        >> attribute( "basDroit", bottomRight_ );
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
    xos << attribute( "hautGauche", topLeft_ )
        << attribute( "basDroite" , bottomRight_ );
    Weather::Serialize( xos );
}
