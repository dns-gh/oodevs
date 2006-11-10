// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PropertiesTableDisplayer.h"
#include "preparation/KnowledgeGroupType.h"
#include "preparation/TeamKarma.h"

// -----------------------------------------------------------------------------
// Name: PropertiesTableDisplayer constructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
PropertiesTableDisplayer::PropertiesTableDisplayer()
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: PropertiesTableDisplayer destructor
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
PropertiesTableDisplayer::~PropertiesTableDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertiesTableDisplayer::Call
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
void PropertiesTableDisplayer::Call( const KnowledgeGroupType& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: PropertiesTableDisplayer::Call
// Created: SBO 2006-10-27
// -----------------------------------------------------------------------------
void PropertiesTableDisplayer::Call( const TeamKarma& value )
{
    AddToDisplay( value.GetName() );
}

// -----------------------------------------------------------------------------
// Name: PropertiesTableDisplayer::Call
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void PropertiesTableDisplayer::Call( const Enum_PopulationAttitude& value )
{
    AddToDisplay( value.ToString() );
}
