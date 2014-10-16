// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "CursorStrategy.h"

#include "GLView_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: CursorStrategy constructor
// Created: SBO 2008-02-12
// -----------------------------------------------------------------------------
CursorStrategy::CursorStrategy( GLView_ABC& tools )
    : tools_          ( tools )
    , selectedTool_   ( Qt::ArrowCursor )
    , selectedContext_( Qt::ArrowCursor )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CursorStrategy destructor
// Created: SBO 2008-02-12
// -----------------------------------------------------------------------------
CursorStrategy::~CursorStrategy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CursorStrategy::SelectTool
// Created: SBO 2008-02-12
// -----------------------------------------------------------------------------
void CursorStrategy::SelectTool( const QCursor& cursor, bool selected )
{
    selectedTool_ = selected ? cursor : QCursor( Qt::ArrowCursor );
    tools_.SetCurrentCursor( selectedTool_ );
}

// -----------------------------------------------------------------------------
// Name: CursorStrategy::SelectContext
// Created: SBO 2008-02-12
// -----------------------------------------------------------------------------
void CursorStrategy::SelectContext( const QCursor& cursor, bool selected )
{
    selectedContext_ = selected ? cursor : selectedTool_;
    tools_.SetCurrentCursor( selectedContext_ );
}
