// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "GamingListItemDisplayer.h"
#include "gaming/AgentKnowledge_ABC.h"
#include "gaming/PopulationKnowledge_ABC.h"
#include "gaming/ObjectKnowledge_ABC.h"

// -----------------------------------------------------------------------------
// Name: GamingListItemDisplayer constructor
// Created: SBO 2007-03-28
// -----------------------------------------------------------------------------
GamingListItemDisplayer::GamingListItemDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GamingListItemDisplayer destructor
// Created: SBO 2007-03-28
// -----------------------------------------------------------------------------
GamingListItemDisplayer::~GamingListItemDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GamingListItemDisplayer::Call
// Created: SBO 2007-03-28
// -----------------------------------------------------------------------------
void GamingListItemDisplayer::Call( const AgentKnowledge_ABC& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: GamingListItemDisplayer::Call
// Created: SBO 2007-03-28
// -----------------------------------------------------------------------------
void GamingListItemDisplayer::Call( const PopulationKnowledge_ABC& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: GamingListItemDisplayer::Call
// Created: SBO 2007-03-28
// -----------------------------------------------------------------------------
void GamingListItemDisplayer::Call( const ObjectKnowledge_ABC& value )
{
    AddToDisplay( value.GetName() );
}
