// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "FeatureNameParser.h"
#include "TerrainFeatureSearcher.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: FeatureNameParser constructor
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
FeatureNameParser::FeatureNameParser()
    : searcher_( new TerrainFeatureSearcher() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FeatureNameParser destructor
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
FeatureNameParser::~FeatureNameParser()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FeatureNameParser::Parse
// Created: AGE 2008-05-29
// -----------------------------------------------------------------------------
bool FeatureNameParser::Parse( const QStringList& content, geometry::Point2f& result, QStringList& hint ) const
{
    try
    {
        if( content.size() != 1 )
            return false;
        const QString value = content[0];
        QString hintSearch;
        bool found = false;
        if( lastRequest_ == value )
            found =  searcher_->FindNext( result, hintSearch );
        else
            found =  searcher_->Search( value, result, hintSearch );
        hint.append( hintSearch );
        lastRequest_ = value;
        return found;
    }
    catch( ... )
    {
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: FeatureNameParser::GetDescriptor
// Created: BAX 2014-01-16
// -----------------------------------------------------------------------------
const LocationParserDescriptor& FeatureNameParser::GetDescriptor() const
{
    static const LocationParserDescriptor desc = {
        QStringList() << QString(),
        QList< int >() << INT_MAX,
    };
    return desc;
}

// -----------------------------------------------------------------------------
// Name: FeatureNameParser::Split
// Created: BAX 2014-01-16
// -----------------------------------------------------------------------------
QStringList FeatureNameParser::Split( const QString& input ) const
{
    return QStringList() << input;
}

// -----------------------------------------------------------------------------
// Name: FeatureNameParser::GetStringPosition
// Created: LGY 2014-01-22
// -----------------------------------------------------------------------------
std::string FeatureNameParser::GetStringPosition( const geometry::Point2f& ) const
{
    return lastRequest_.toStdString();
}

// -----------------------------------------------------------------------------
// Name: FeatureNameParser::Load
// Created: ABR 2014-03-04
// -----------------------------------------------------------------------------
void FeatureNameParser::Load( const tools::Path& graphicsDir )
{
    searcher_->Load( graphicsDir );
}
