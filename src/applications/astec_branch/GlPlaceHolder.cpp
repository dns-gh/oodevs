// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "GlPlaceHolder.h"
#include "splashscreen.cpp"

// -----------------------------------------------------------------------------
// Name: GlPlaceHolder constructor
// Created: AGE 2006-05-03
// -----------------------------------------------------------------------------
GlPlaceHolder::GlPlaceHolder( QWidget* parent )
    : QVBox( parent )
{
    QHBox* box = new QHBox( this );
    QLabel* label = new QLabel( box );
    box->setStretchFactor( label, 4212 );
    box->setSpacing( 4212 );
    label->setPixmap( QPixmap( qembed_findData( "astec.jpg" ) ) );
}

// -----------------------------------------------------------------------------
// Name: GlPlaceHolder destructor
// Created: AGE 2006-05-03
// -----------------------------------------------------------------------------
GlPlaceHolder::~GlPlaceHolder()
{
    // NOTHING
}
