// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UtmParser.h"
#include "clients_kernel/CoordinateConverter_ABC.h"

using namespace gui;

namespace
{
    const int gridZoneSize = 3;
    const int eastingSize = 6;
    const int northingSize = 7;
}

UtmParser::UtmParser( const kernel::CoordinateConverter_ABC& converter, const std::string& code )
    : converter_( converter )
    , code_( code )
{
    // NOTHING
}

UtmParser::~UtmParser()
{
    // NOTHING
}

bool UtmParser::Parse( const QStringList& content, geometry::Point2f& result, QStringList& hint, bool /*small*/ ) const
{
    // "31N 000000 0000000"
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
            return false;
        hint[1] = hint[1].append( QString( "0" ).repeated( eastingSize - hint[1].size() ) );
        hint[2] = hint[2].append( QString( "0" ).repeated( northingSize - hint[2].size() ) );
        result = converter_.ConvertFromUtm( hint.join( "" ).toStdString(), code_ );
        return true;
    }
    catch( ... )
    {
        return false;
    }
}

const LocationParserDescriptor& UtmParser::GetDescriptor() const
{
    static const LocationParserDescriptor desc = {
        QStringList() << QString() << QString() << QString(),
        QList< int >() << gridZoneSize << eastingSize << northingSize,
    };
    return desc;
}

QStringList UtmParser::Split( const QString& input ) const
{
    auto cleaned = input;
    cleaned.remove( ' ' );
    return QStringList()
        << cleaned.left( gridZoneSize )
        << cleaned.mid( gridZoneSize, eastingSize )
        << cleaned.right( northingSize );
}

std::string UtmParser::GetStringPosition( const geometry::Point2f& position ) const
{
    return converter_.ConvertToUtm( position, code_ );
}
