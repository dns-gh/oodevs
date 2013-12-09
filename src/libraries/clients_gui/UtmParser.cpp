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
#include "clients_kernel/ModelLoaded.h"
#include "clients_kernel/Controllers.h"
#include "tools/ExerciseConfig.h"
#include <boost/lexical_cast.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UtmParser constructor
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
UtmParser::UtmParser( kernel::Controllers& controllers, const T_Converter& converter )
    : controllers_( controllers )
    , converter_( converter )
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
bool UtmParser::Parse( const QString& content, geometry::Point2f& result, QStringList& hint ) const
{
    // "31 NEA 00000 00000"
    try
    {
        QString strContent = content;
        strContent.remove( ' ' );
        bool bOk = false;
        strContent.left( 2 ).toInt( &bOk );
        if( !bOk )
            strContent = zone_.c_str() + strContent;
        if( strContent.length() < 15 )
            strContent = Fill( strContent );
        result = converter_( strContent.toStdString() );
        hint.append( strContent );
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
    int missing = 15 - value.length();
    if( missing <= 0 || missing > 10 )
        return value;
    const int northMissing = missing / 2;
    QString north;
    north.fill( '0', northMissing );
    const int eastMissing  = missing - northMissing;
    QString east;
    east.fill( '0', eastMissing );
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
    const std::vector< unsigned char >& utmZones = model.config_.GetUtmZones();
    if( ! utmZones.empty() )
        zone_ = boost::lexical_cast< std::string, unsigned int >( utmZones.front() );
}

// -----------------------------------------------------------------------------
// Name: UtmParser::GetNumberOfParameters
// Created: AME 2010-03-11
// -----------------------------------------------------------------------------
int UtmParser::GetNumberOfParameters() const
{
    return 1;
}
