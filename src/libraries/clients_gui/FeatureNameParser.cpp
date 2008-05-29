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
bool FeatureNameParser::Parse( QString content, geometry::Point2f& result, QString& hint ) const
{
    const bool next = lastRequest_ == content;
    lastRequest_ = content;
    if( next )
        return searcher_->FindNext( result, hint );
    else 
        return searcher_->Search( content, result, hint );
}
