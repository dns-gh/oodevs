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
    : QWidget( parent )
{
    setStyleSheet( "background-color: #000000" );
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
}

// -----------------------------------------------------------------------------
// Name: GlPlaceHolder destructor
// Created: AGE 2006-05-03
// -----------------------------------------------------------------------------
GlPlaceHolder::~GlPlaceHolder()
{
    // NOTHING
}
