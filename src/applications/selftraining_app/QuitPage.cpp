// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "QuitPage.h"
#include "moc_QuitPage.cpp"
#include <qapplication.h>

// -----------------------------------------------------------------------------
// Name: QuitPage constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
QuitPage::QuitPage( QWidgetStack* pages, Page_ABC& previous )
    : MenuPage( pages )
{
    AddLink( tr( "Yes" ), *this, SLOT( OnQuit() ) );
    AddLink( tr( "No" ) , previous );
}

// -----------------------------------------------------------------------------
// Name: QuitPage destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
QuitPage::~QuitPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: QuitPage::OnQuit
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void QuitPage::OnQuit()
{
    qApp->quit();
}
