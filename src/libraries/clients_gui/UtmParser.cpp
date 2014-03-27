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

namespace
{
    static const int MaxFieldSize = 5;
}

// -----------------------------------------------------------------------------
// Name: UtmParser constructor
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
UtmParser::UtmParser( kernel::Controllers& controllers, const T_Converter& converter,
                      const T_StringConverter& stringConverter )
    : controllers_( controllers )
    , converter_( converter )
    , stringConverter_( stringConverter )
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
bool UtmParser::Parse( const QStringList& content, geometry::Point2f& result, QStringList& hint, bool /*small*/ ) const
{
    // "31 NEA 00000 00000"
    try
    {
        if( content.size() > 3 )
            return false;
        hint = content;
        while( hint.size() < 3 )
            hint << QString();
        for( auto it = hint.begin(); it != hint.end(); ++it )
            it->remove( ' ' );
        bool ok = false;
        hint[0].left( 2 ).toInt( &ok );
        if( !ok )
            hint[0] = (QString::fromStdString( zone_ ) + hint[0]).left( MaxFieldSize );
        const int max = std::max( hint[1].size(), hint[2].size() );
        for( int i = 1; i < hint.size(); ++i )
            hint[i] = hint[i].append( QString( "0" ).repeated( max - hint[i].size() ) ).left( MaxFieldSize );
        result = converter_( hint.join( "" ).toStdString() );
        return true;
    }
    catch( ... )
    {
        return false;
    }
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
// Name: UtmParser::GetDescriptor
// Created: BAX 2014-01-16
// -----------------------------------------------------------------------------
const LocationParserDescriptor& UtmParser::GetDescriptor() const
{
    static const LocationParserDescriptor desc = {
        QStringList() << QString() << QString() << QString(),
        QList< int >() << MaxFieldSize << MaxFieldSize << MaxFieldSize,
    };
    return desc;
}

// -----------------------------------------------------------------------------
// Name: UtmParser::Split
// Created: BAX 2014-01-16
// -----------------------------------------------------------------------------
QStringList UtmParser::Split( const QString& input ) const
{
    int left = std::max( 0, input.size() - MaxFieldSize ) / 2;
    return QStringList()
        << input.left( MaxFieldSize )
        << input.mid( MaxFieldSize, left )
        << input.mid( MaxFieldSize + left );
}

// -----------------------------------------------------------------------------
// Name: UtmParser::GetStringPosition
// Created: LGY 2014-01-22
// -----------------------------------------------------------------------------
std::string UtmParser::GetStringPosition( const geometry::Point2f& position ) const
{
    return stringConverter_( position );
}
