// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionParameterLimaList.h"
#include "ActionParameterLima.h"
#include "Tools.h"

// -----------------------------------------------------------------------------
// Name: ActionParameterLimaList constructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterLimaList::ActionParameterLimaList( const kernel::OrderParameter& parameter )
    : ActionParameter< QString >( parameter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimaList constructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterLimaList::ActionParameterLimaList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_LimasOrder& limas )
    : ActionParameter< QString >( parameter )
{
    for( unsigned int i = 0; i < limas.n; ++i )
        AddParameter( *new ActionParameterLima( tools::translate( "ActionParameter", "Lima" ), converter, limas.elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: ActionParameterLimaList destructor
// Created: SBO 2007-04-16
// -----------------------------------------------------------------------------
ActionParameterLimaList::~ActionParameterLimaList()
{
    // NOTHING
}
