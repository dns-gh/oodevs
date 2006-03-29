// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "SetGlOptions.h"
#include <qgl.h>

// -----------------------------------------------------------------------------
// Name: SetGlOptions constructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
SetGlOptions::SetGlOptions()
{
    QGLFormat format;
    format.setAlpha( true );
    format.setStencil( true );
    QGLFormat::setDefaultFormat( format );
    context_ = new QGLContext( QGLFormat::defaultFormat() );
}

// -----------------------------------------------------------------------------
// Name: SetGlOptions destructor
// Created: AGE 2006-03-16
// -----------------------------------------------------------------------------
SetGlOptions::~SetGlOptions()
{
    // NOTHING
}
