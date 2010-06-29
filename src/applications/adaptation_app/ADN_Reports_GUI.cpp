// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Reports_GUI.h"

// -----------------------------------------------------------------------------
// Name: ADN_Reports_GUI constructor
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
ADN_Reports_GUI::ADN_Reports_GUI( ADN_Reports_Data& data )
    : ADN_GUI_ABC( "ADN_Reports_GUI" )
    , data_      ( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_GUI destructor
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
ADN_Reports_GUI::~ADN_Reports_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Reports_GUI::Build
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
void ADN_Reports_GUI::Build()
{
    assert( pMainWidget_ == 0 );
    pMainWidget_ = new QWidget( 0, "Reports main widget" );
}
