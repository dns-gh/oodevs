// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "actions_gui_pch.h"
#include "ParamLocation.h"

using namespace actions::gui;

// -----------------------------------------------------------------------------
// Name: ParamLocation constructor
// Created: ABR 2012-01-09
// -----------------------------------------------------------------------------
ParamLocation::ParamLocation( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter )
    : ParamLocationComposite( builder, parameter )
{
    SetShapeFilter( true, true, true, true, true );
}

// -----------------------------------------------------------------------------
// Name: ParamLocation destructor
// Created: ABR 2012-01-09
// -----------------------------------------------------------------------------
ParamLocation::~ParamLocation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ParamLocation::SetShapeFilter
// Created: ABR 2012-01-09
// -----------------------------------------------------------------------------
void ParamLocation::SetShapeFilter( bool point, bool line, bool polygon, bool circle, bool rectangle )
{
    Purge();
    if( circle )
        AddElement( "circle" );
    if( line )
        AddElement( "line" );
    if( point )
        AddElement( "point" );
    if( polygon )
        AddElement( "polygon" );
    if( rectangle )
        AddElement( "rectangle" );
    if( stack_ )
        InternalBuildInterface();
}
