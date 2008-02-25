// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "MenuPage.h"
#include "MenuButton.h"
#include <qlayout.h>

// -----------------------------------------------------------------------------
// Name: MenuPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
MenuPage::MenuPage( QWidgetStack* pages )
    : Page_ABC( pages )
{
    layout()->setAlignment( Qt::AlignCenter );
    setSpacing( 5 );
}

// -----------------------------------------------------------------------------
// Name: MenuPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
MenuPage::~MenuPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MenuPage::AddLink
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void MenuPage::AddLink( const QString& title, Page_ABC& page, const char* slot /*= 0*/ )
{
    QPushButton* button = new MenuButton( title, this );
    connect( button, SIGNAL( clicked() ), &page, slot ? slot : SLOT( show() ) );
}
