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
FeatureNameParser::FeatureNameParser( kernel::Controllers& controllers )
    : searcher_( new TerrainFeatureSearcher( controllers ) )
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
bool FeatureNameParser::Parse( const QString& content, geometry::Point2f& result, QStringList& hint ) const
{
    QString hintSearch;
    bool found = false;
    if( lastRequest_ == content )
        found =  searcher_->FindNext( result, hintSearch );
    else
        found =  searcher_->Search( content, result, hintSearch );
    hint.append( hintSearch );
    lastRequest_ = content;
    return found;
}

// -----------------------------------------------------------------------------
// Name: FeatureNameParser::GetNumberOfParameters
// Created: AME 2010-03-11
// -----------------------------------------------------------------------------
int FeatureNameParser::GetNumberOfParameters() const
{
    return 1;
}
