// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "MgrsParser.h"
#include "clients_kernel/ModelLoaded.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "tools/ExerciseConfig.h"

using namespace gui;

namespace
{
    const int maxFieldSize = 5;
}

// -----------------------------------------------------------------------------
// Name: MgrsParser constructor
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
MgrsParser::MgrsParser( kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter )
    : controllers_( controllers )
    , converter_( converter )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: MgrsParser destructor
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
MgrsParser::~MgrsParser()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: MgrsParser::Parse
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
bool MgrsParser::Parse( const QStringList& content, geometry::Point2f& result, QStringList& hint, bool /*small*/ ) const
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
            hint[0] = (QString::fromStdString( zone_ ) + hint[0]).left( maxFieldSize );
        const int max = std::max( hint[1].size(), hint[2].size() );
        for( int i = 1; i < hint.size(); ++i )
            hint[i] = hint[i].append( QString( "0" ).repeated( max - hint[i].size() ) ).left( maxFieldSize );
        result = converter_.ConvertToXY( hint.join( "" ).toStdString() );
        return true;
    }
    catch( ... )
    {
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: MgrsParser::NotifyUpdated
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
void MgrsParser::NotifyUpdated( const kernel::ModelLoaded& model )
{
    const auto zone = model.config_.GetUtmZone();
    if( !zone.empty() )
        zone_ = zone;
}

// -----------------------------------------------------------------------------
// Name: MgrsParser::GetDescriptor
// Created: BAX 2014-01-16
// -----------------------------------------------------------------------------
const LocationParserDescriptor& MgrsParser::GetDescriptor() const
{
    static const LocationParserDescriptor desc = {
        QStringList() << QString() << QString() << QString(),
        QList< int >() << maxFieldSize << maxFieldSize << maxFieldSize,
    };
    return desc;
}

// -----------------------------------------------------------------------------
// Name: MgrsParser::Split
// Created: BAX 2014-01-16
// -----------------------------------------------------------------------------
QStringList MgrsParser::Split( const QString& input ) const
{
    auto cleaned = input;
    cleaned.remove( ' ' );
    const int left = std::max( 0, cleaned.size() - maxFieldSize ) / 2;
    return QStringList()
        << cleaned.left( maxFieldSize )
        << cleaned.mid( maxFieldSize, left )
        << cleaned.mid( maxFieldSize + left );
}

// -----------------------------------------------------------------------------
// Name: MgrsParser::GetStringPosition
// Created: LGY 2014-01-22
// -----------------------------------------------------------------------------
std::string MgrsParser::GetStringPosition( const geometry::Point2f& position ) const
{
    return converter_.ConvertToMgrs( position );
}
