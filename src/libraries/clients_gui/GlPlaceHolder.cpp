// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GlPlaceHolder.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GlPlaceHolder constructor
// Created: AGE 2006-05-03
// -----------------------------------------------------------------------------
GlPlaceHolder::GlPlaceHolder( QWidget* parent )
    : Q3VBox( parent )
{
    setStyleSheet("Q3VBox { background-color: #000000; }");
    setBackgroundRole( QPalette::Window );
    setFrameStyle( QFrame::StyledPanel + QFrame::Raised );
}

// -----------------------------------------------------------------------------
// Name: GlPlaceHolder destructor
// Created: AGE 2006-05-03
// -----------------------------------------------------------------------------
GlPlaceHolder::~GlPlaceHolder()
{
    // NOTHING
}
