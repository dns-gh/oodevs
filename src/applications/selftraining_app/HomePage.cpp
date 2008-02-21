// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "HomePage.h"
#include "SelfTrainingPage.h"
//#include "MultiplayerPage.h"
//#include "OptionsPage.h"
#include "QuitPage.h"

// -----------------------------------------------------------------------------
// Name: HomePage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
HomePage::HomePage( QWidgetStack* pages, const tools::GeneralConfig& config )
    : MenuPage( pages )
{
    AddLink( tr( "Self Training" ), *new SelfTrainingPage( pages, *this, config ) );
    AddLink( tr( "Multiplayer" ),   *new SelfTrainingPage( pages, *this, config ) );
    AddLink( tr( "Options" ),       *new SelfTrainingPage( pages, *this, config ) );
    AddLink( tr( "Quit" ),          *new QuitPage        ( pages, *this ) );
}

// -----------------------------------------------------------------------------
// Name: HomePage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
HomePage::~HomePage()
{
    // NOTHING
}
