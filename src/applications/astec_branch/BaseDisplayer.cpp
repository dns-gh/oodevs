// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "BaseDisplayer.h"
#include "Agent.h"
#include "DotationType.h"
#include "Team.h"
#include "EquipmentType.h"
#include "Object_ABC.h"
#include "Population.h"
#include "PopulationConcentration.h"
#include "CoordinateConverter.h"

// -----------------------------------------------------------------------------
// Name: BaseDisplayer constructor
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
BaseDisplayer::BaseDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer destructor
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
BaseDisplayer::~BaseDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const bool& value )
{
    AddToDisplay( value ? qApp->tr( "Oui" ) : qApp->tr( "Non" ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const Agent& value )
{
    AddToDisplay( QString( value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const DotationType& value )
{
    AddToDisplay( QString( value.GetCategory().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const Team& value )
{
    AddToDisplay( QString( value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const EquipmentType& value )
{
    AddToDisplay( QString( value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const Object_ABC& value )
{
    AddToDisplay( QString( value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const Population& value )
{
    AddToDisplay( QString( value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: BaseDisplayer::Call
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void BaseDisplayer::Call( const PopulationConcentration& value )
{
    AddToDisplay( QString( value.GetName().c_str() ) );
}