// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UtmParser.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/ModelLoaded.h"
#include "clients_kernel/Controllers.h"
#include "tools/WorldParameters.h"
#pragma warning( push )
#pragma warning( disable : 4702 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UtmParser constructor
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
UtmParser::UtmParser( kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter )
    : controllers_( controllers )
    , converter_( converter )
    , numParameters_( 1 )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UtmParser destructor
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
UtmParser::~UtmParser()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UtmParser::Parse
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
bool UtmParser::Parse( QString content, geometry::Point2f& result, QStringList& hint ) const
{
    // "31 NEA 00000 00000"
    try
    {
        content.remove( ' ' );
        bool bOk = false;
        content.left( 2 ).toInt( &bOk );
        if( !bOk )
            content = zone_.c_str() + content;
        if( content.length() < 15 )
            content = Fill( content );
        result = converter_.ConvertToXY( std::string( content.ascii() ) );
        hint.append( content );
        return true;
    }
    catch( ... )
    {
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: UtmParser::Fill
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
QString UtmParser::Fill( QString value )
{
    int missing = 15 - int( value.length() );
    if( missing <= 0 || missing > 10 )
        return value;
    const int northMissing = missing / 2;
    QString north; north.fill( '0', northMissing );
    const int eastMissing  = missing - northMissing;
    QString east; east.fill( '0', eastMissing );
    value.insert( 10 - northMissing, north );
    value.insert( 15 - eastMissing, east );
    return value;
}

// -----------------------------------------------------------------------------
// Name: UtmParser::NotifyUpdated
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
void UtmParser::NotifyUpdated( const kernel::ModelLoaded& model )
{
    tools::WorldParameters parameters( model.config_ );
    if( ! parameters.utmZones_.empty() )
        zone_ = boost::lexical_cast< std::string, unsigned int >( parameters.utmZones_.front() );
}

// -----------------------------------------------------------------------------
// Name: UtmParser::GetNumberOfParameters
// Created: AME 2010-03-11
// -----------------------------------------------------------------------------
int UtmParser::GetNumberOfParameters() const
{
    return numParameters_;
}